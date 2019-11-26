#ifndef G4E_INPUTPROCESSOR_HH
#define G4E_INPUTPROCESSOR_HH


#include <vector>
#include <string>

#include <argparse.hh>

#include <StringHelpers.hh>

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

    std::vector<std::string> AllFileNames;          /// List of all arguments which are not flags=values (macro file names, input file names, etc.)
    std::vector<std::string> MacroFileNames;        /// List of file names with .mac extension
    std::vector<std::string> SourceFileNames;       /// The list of all other input files
    std::string SourceGenerator;                    /// The generator to use

    std::string MacroPath = "";      /// G4E_MACRO_PATH
    bool IsSetMacroPath = false;     /// true if macro path was defined in environment variables

    std::string HomePath = "";       /// G4E_HOME
    bool IsSetHomePath = false;      /// true if G4E_HOME was determined

    std::string ResourcePath;        /// Path to resources directory
};



class InputProcessor
{
public:

    static InputArguments Process(int argc, char **argv) {
        ArgumentParser parser("g4e - Geant 4 Electron Ion Collider");
        parser.add_argument("-g", "--gui", "Shows Geant4 GUI", false);
        parser.add_argument("-t", "--threads", "Number of threads. Single threaded mode if 0 or 1", false);
        parser.add_argument("-j", "--jobs", "(alias to -t and --threads flags)", false);
        parser.add_argument("files", "Input files. Macros (.mac) or generator files", false);

        InputArguments result;                                          // This function result
        try {
            parser.parse(argc, argv);                                   // Now parse the arguments
        } catch (const ArgumentParser::ArgumentNotFound& ex) {
            std::cout << ex.what() << std::endl;
            throw;
        }

        // User wants just help
        if(parser.is_help()) {
            fmt::print("Detector simulation tool for EIC\n");
            exit(1);
        }

        //
        // Open GUI?:
        result.ShowGui = parser.get<bool>("g");
        fmt::print("ARG:ShowGui = {}\n", result.ShowGui);

        //
        // Number of threads
        if(parser.exists("t")) {
            result.ThreadsCount = parser.get<int>("t");
        }
        if(parser.exists("j")) {                                // -j is the common alias to -t
            result.ThreadsCount = parser.get<int>("j");
        }
        fmt::print("ARG:ThreadsCount = {}\n", result.ThreadsCount);

        //
        // Input files (macros and data files)
        result.AllFileNames = parser.getv<std::string>("");
        ProcessFileNames(result);                               // Separate file names as macro / data files

        // G4E_HOME
        const char* homeCstr = std::getenv("G4E_HOME");
        ProcessHomePath(result, homeCstr);

        // G4E_MACRO_PATH
        const char* macroPathCstr = std::getenv("G4E_MACRO_PATH");
        ProcessMacroPath(result, macroPathCstr);

        return result;
    }

private:


    static void ProcessFileNames(InputArguments& result)
    {
        // Separate filenames to Mac and other files
        for(const auto& name: result.AllFileNames) {
            if(g4e::EndsWith(name, ".mac")) {
                result.MacroFileNames.push_back(name);
            } else {
                result.SourceFileNames.push_back(name);
            }
        }

        // Print file names if apply
        if(!result.MacroFileNames.empty()) {
            fmt::print("ARG:Macro files:\n");
            for(const auto& fileName: result.MacroFileNames) {
                fmt::print("   {}\n", fileName);
            }
        }

        // Source file names:
        if(!result.MacroFileNames.empty()) {
            fmt::print("ARG:Source files:\n");
            for(const auto& fileName: result.SourceFileNames) {
                fmt::print("   {}\n", fileName);
            }
        }

    }

    static void ProcessHomePath(InputArguments& result, const char *homeCstr)
    {
        result.IsSetHomePath = homeCstr != nullptr;
        result.HomePath = homeCstr ? homeCstr : "";
        fmt::print("ENV:G4E_HOME: is-set={}, value='{}'\n", result.IsSetHomePath, result.HomePath);

        // WARN user if G4E_HOME is not set
        if(!result.IsSetHomePath) {
            spdlog::warn("Environment variable 'G4E_HOME' is not set (!!!). G4E will try to find resources in your current dir (and fail, most probably)");
        } else {
            result.ResourcePath = result.HomePath + "/resources";
        }
    }

    static void ProcessMacroPath(InputArguments& result, const char *macroPathCstr)
    {
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

    }

/// Processes both program arguments and environment variables to build the right ProgramArguments


};


#endif //G4E_INPUTPROCESSOR_HH
