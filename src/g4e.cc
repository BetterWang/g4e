//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//



#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"

#include <argparse.hh>

#include "StringHelpers.hh"

#include "design_main/jleic/JLeicDetectorConstruction.hh"
#include "JLeicPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "JLeicRunAction.hh"
#include "JLeicEventAction.hh"
#include "JLeicSteppingAction.hh"
#include "JLeicSteppingVerbose.hh"
#include "JLeicTrackingAction.hh"

#include <G4MTRunManager.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <Randomize.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

//-- physics processes --
#include <FTFP_BERT.hh>
#include <QGSP_BIC.hh>



/// Program Configuration provided by arguments or environment variables
/// This class provides all needed dynamic information about flags and
/// environment variables provided by users
struct ProgramArguments
{
    bool ShowGui = false;
    int ThreadsCount = 1;
    std::vector<std::string> MacroFileNames;
    std::vector<std::string> SourceFileNames;       /// The list of input files
    std::string SourceGenerator;            /// The generator to use
    bool IsSetSources = false;              /// true - users provided source

    std::string MacroPath;      /// G4E_MACRO_PATH
    bool IsSetMacroPath;        /// true if macro path was defined in environment variables
    std::string HomePath;       /// G4E_HOME
    bool IsSetHomePath;         /// true if G4E_HOME was determined
    std::string ResourcePath;   /// Path to resources directory
};


/// Processes both program arguments and environment variables to build the right ProgramArguments
ProgramArguments ParseArgEnv(int argc, char **argv) {
    ArgumentParser parser("g4e - Geant 4 Electron Ion COllider");
    parser.add_argument("-g", "--gui", "Shows Geant4 GUI", false);
    parser.add_argument("-t", "--threads", "Number of threads. Single threaded mode if 0 or 1", false);
    parser.add_argument("-j", "--jobs", "(alias to -t flag) Number of threads. Single threaded mode if 0 or 1", false);
    parser.add_argument("-s", "--source",
                       "Source files to process. "
                       "Should start with generator name. Like --source=beagle:/path/to/file.txt:path/to/another.txt", false);
    parser.add_argument("--files", "input files", false);

    // Now parse the arguments
    try {
        parser.parse(argc, argv);
    } catch (const ArgumentParser::ArgumentNotFound& ex) {
        std::cout << ex.what() << std::endl;
        throw;
    }

    // User wants just help
    if(parser.is_help()) {
        fmt::print("Detector simulation tool for EIC\n");
        exit(1);
    }

    ProgramArguments result;        // This function result

    // Open GUI arguments:
    result.ShowGui = parser.get<bool>("g");
    fmt::print("ARG:ShowGui = {}\n", result.ShowGui);

    // Number of threads
    if(parser.exists("t")) {
        result.ThreadsCount = parser.get<int>("t");
    }
    fmt::print("ARG:ThreadsCount = {}\n", result.ThreadsCount);


    // Macro files:
    result.MacroFileNames = parser.getv<std::string>("");

    // Print file names if apply
    if(!result.MacroFileNames.empty()) {
        fmt::print("ARG:Macro files:\n");
        for(const auto& fileName: result.MacroFileNames) {
            fmt::print("   {}\n", fileName);
        }
    }

    // G4E_HOME
    const char* homeCstr = std::getenv("G4E_HOME");
    result.IsSetHomePath = homeCstr != nullptr;
    result.HomePath = homeCstr ? homeCstr : "";
    fmt::print("ENV:G4E_HOME: is-set={}, value='{}'\n", result.IsSetHomePath, result.HomePath);

    // WARN user if G4E_HOME is not set
    if(!result.IsSetHomePath) {
        spdlog::warn("Environment variable 'G4E_HOME' is not set (!!!). G4E will try to find resources in your current dir (and fail, most probably)");
    } else {
        result.ResourcePath = result.HomePath + "/resources";
    }

    // G4E_MACRO_PATH
    const char* macroPathCstr = std::getenv("G4E_MACRO_PATH");
    result.IsSetMacroPath = macroPathCstr != nullptr;
    result.MacroPath = macroPathCstr? macroPathCstr: "";

    // Add JLeic detector to default Macro Path
    std::vector<std::string> paths;
    if(result.IsSetMacroPath) {
        result.MacroPath += ":"+result.ResourcePath+"/jleic";
    } else {
        result.MacroPath = result.ResourcePath+"/jleic";
    }
    fmt::print("ENV:G4E_MACRO_PATH:  is-set={}, value='{}'",  result.IsSetMacroPath, result.MacroPath );

    // SOURCES
    if(parser.exists("s")) {
        auto rawSources = parser.get<std::string>("s");

        // Check we have something
        if(rawSources.empty()) {
            fmt::print("ERROR. Flag --source aka -s is provided but the value is not set\n");
            parser.print_help();
            exit(1);
        }

        auto tokens = g4e::Split(rawSources, ":");


        // The first should go a generator:
        result.SourceGenerator = tokens[0];
        fmt::print("ARG:SourceGenerator = {}", result.SourceGenerator);

        // Then there should be input files (or at least one!)
        if(tokens.size()<2){
            fmt::print("ERROR. No source file provided. See '--source' flag usage\n");
            parser.print_help();
            exit(1);
        }

        // If we are here, the sources are OK
        result.IsSetSources = true;
        for(size_t i=1; i < tokens.size(); i++ ) {
            result.SourceFileNames.push_back(tokens[i]);
        }
    }


    return result;
}


int main(int argc, char **argv)
{
    using namespace fmt;

    spdlog::info("Initializing g4e, parsing arguments...");
    auto osink = std::make_shared<spdlog::sinks::ostream_sink_mt> (G4cout);
    spdlog::default_logger()->sinks().clear();
    spdlog::default_logger()->sinks().push_back(osink);
    spdlog::info("Initialized G4E sink");
    spdlog::set_level(spdlog::level::debug);


    ProgramArguments args = ParseArgEnv(argc, argv);

    //choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    //my Verbose output class
    G4VSteppingVerbose::SetInstance(new JLeicSteppingVerbose);

    // Construct the default run manager
    G4RunManager * runManager;
    if(args.ThreadsCount > 1) {
        // Multi-threaded run manager
        auto mtRunManager = new G4MTRunManager;
        mtRunManager->SetNumberOfThreads(args.ThreadsCount);
        runManager = mtRunManager;
    } else {
        // Single threaded mode
        runManager = new G4RunManager;
    }

    auto detector = new JLeicDetectorConstruction();

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new JLeicPhysicsList(detector));

    // RUN action
    auto runAction = new JLeicRunAction(detector);
    runManager->SetUserAction(runAction);

    // Primary Generator
    auto pgAction = new PrimaryGeneratorAction();
    runManager->SetUserAction(pgAction);

    // Event action
    auto eventAction = new JLeicEventAction(runAction);
    runManager->SetUserAction(eventAction);

    // Stepping action
    auto steppingAction = new JLeicSteppingAction(detector, eventAction, runAction);
    runManager->SetUserAction(steppingAction);

    // Tracking action
    auto trackingAction = new JLeicTrackingAction();
    runManager->SetUserAction(trackingAction);

    // Vis manager?
    G4VisExecutive* visManager = nullptr;
    G4UIExecutive* uiExec = nullptr;

    // We show GUI if user didn't provided any macros of if he has --gui/-g flag
    if(args.MacroFileNames.empty() || args.ShowGui) {
        args.ShowGui = true;
        visManager = new G4VisExecutive;
        visManager->Initialize();
        uiExec = new G4UIExecutive(argc, argv);
    }

    // auto *ui = new G4UIExecutive(argc, argv);
    G4UImanager *ui = G4UImanager::GetUIpointer();

    // set macro path from environment if it is set
    ui->ApplyCommand(format("/control/macroPath {}", args.MacroPath));

    // We have some user defined file
    if (args.MacroFileNames.empty()) {
        // We don't have any macros set.
        // Now use jleic if no GUI or jleicvis for GUI
        std::string defaultMacro = args.ShowGui? "jleicvis.mac":"jleic.mac";
        args.MacroFileNames.push_back(defaultMacro);
    }


    // Execute all macros
    fmt::print("Executing macro files:");
    for(const auto& fileName: args.MacroFileNames) {
        std::string command = "/control/execute " + fileName;
        fmt::print("   {}", command);
        ui->ApplyCommand(command);
    }

    // We start visual mode if no files provided or if --gui flag is given
    if(args.ShowGui && uiExec) {
        uiExec->SessionStart();
    }

    return 0;
}
