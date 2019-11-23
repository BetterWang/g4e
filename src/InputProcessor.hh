#ifndef G4E_INPUTPROCESSOR_HH
#define G4E_INPUTPROCESSOR_HH

#include <vector>
#include <string>

/** \file
 *  InputProcessor manage users inputs on program start:
 *      1. Program arguments
 *      2. Environment variables
 *      3. Pipe (if ever one would like to implement)
 *
 *  InputArguments is the result of work of InputProcessor.
 *  InputArguments represents a set of ready to use parameters:
 *      1. Configure initialization
 *      2. Provide running aspects (verbosity, input, output)
 *      3. Info, what was set by user
 *
 */


/// Program Configuration provided by arguments or environment variables
/// This class provides all needed dynamic information about flags and
/// environment variables provided by users
struct InputArguments
{
    bool ShowGui = false;
    int ThreadsCount = 1;
    std::vector<std::string> MacroFileNames;
    std::vector<std::string> SourceFileNames;       /// The list of input files
    std::string SourceGenerator;                    /// The generator to use
    bool IsSetSources = false;                      /// true - users provided source

    std::string MacroPath = "";      /// G4E_MACRO_PATH
    bool IsSetMacroPath = false;     /// true if macro path was defined in environment variables

    std::string HomePath = "";       /// G4E_HOME
    bool IsSetHomePath = false;      /// true if G4E_HOME was determined

    std::string ResourcePath;        /// Path to resources directory
};



class InputProcessor
{

/// Processes both program arguments and environment variables to build the right ProgramArguments
    ProgramArguments Parse(int argc, char **argv) {
        ArgumentParser parser("g4e - Geant 4 Electron Ion COllider");
        parser.add_argument("-g", "--gui", "Shows Geant4 GUI", false);
        parser.add_argument("-t", "--threads", "Number of threads. Single threaded mode if 0 or 1", false);
        parser.add_argument("-j", "--jobs", "(alias to -t flag) Number of threads. Single threaded mode if 0 or 1", false);
        parser.add_argument("-s", "--source", "Source files to process. "
                                              "Should start with generator name. "
                                              "Like --source=beagle:/path/to/file.txt:path/to/another.txt", false);
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

};


#endif //G4E_INPUTPROCESSOR_HH
