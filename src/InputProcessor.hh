#ifndef G4E_INPUTPROCESSOR_HH
#define G4E_INPUTPROCESSOR_HH


#include <vector>
#include <string>

#include <CLI11.hpp>

#include <StringHelpers.hh>
#include <LogLevels.hh>

/** \file
 *  InputProcessor manage users inputs on program start.
 *  By the inputs one means:
 *      1. Program arguments
 *      2. Environment variables
 *      3. Pipe (if ever one would like to implement)
 *
 *  InputArguments is the result of work of InputProcessor.
 *  InputArguments represents a set of ready to use parameters:
 *      1. Configure initialization
 *      2. Provide running aspects (verbosity, input, output)
 *      3. Info, what was set by user (macro PATH env, macros to run, input files (if given by command line)
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

    LogLevels LogLevel;              /// Controlled by -v. --verbosity

};



class InputProcessor
{
public:

    static LogLevels ProcessVerbosity(const std::string& str);

    static InputArguments Process(int argc, char **argv);

private:


    static void ProcessFileNames(InputArguments& result);

    static void ProcessHomePath(InputArguments& result, const char *homeCstr);

    static void ProcessMacroPath(InputArguments& result, const char *macroPathCstr);

/// Processes both program arguments and environment variables to build the right ProgramArguments


};



InputArguments InputProcessor::Process(int argc, char **argv)
{
    InputArguments result;                                          // This function result
    CLI::App app{"g4e - Geant 4 Electron Ion Collider"};

    bool optShowGui = false;
    int optThreads = 1;
    std::string optVerbose("info");
    std::vector<std::string> optAllFiles;
    app.add_flag("-g,--gui", optShowGui, "Shows Geant4 GUI");
    app.add_option("-t,-j,--threads,--jobs", optThreads, "Number of threads. Single threaded mode if 0 or 1", 1);
    app.add_option("-v,--verbose", optVerbose,
                   "Verbosity 0-5 or: off fatal error warn info debug trace. '-v' (no val) means 'debug'. Can be set with /g4r/logLevel", "info");
    app.add_option("files", optAllFiles, "Input files. Macros (.mac) or generator files");

    // Parse everything
    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        app.exit(e);
        throw;
    }

    // verbosity
    result.LogLevel = ProcessVerbosity(optVerbose);
    fmt::print("ARG:LogLevel = {}\n", result.LogLevel.ToString());

    //
    // Open GUI?:
    result.ShowGui = optShowGui;
    fmt::print("ARG:ShowGui = {}\n", result.ShowGui);

    //
    // Number of threads
    result.ThreadsCount = optThreads;
    fmt::print("ARG:ThreadsCount = {}\n", result.ThreadsCount);

    //
    // Input files (macros and data files)
    result.AllFileNames = optAllFiles;
    ProcessFileNames(result);                               // Separate file names as macro / data files

    // G4E_HOME
    const char* homeCstr = std::getenv("G4E_HOME");
    ProcessHomePath(result, homeCstr);

    // G4E_MACRO_PATH
    const char* macroPathCstr = std::getenv("G4E_MACRO_PATH");
    ProcessMacroPath(result, macroPathCstr);

    return result;
}

LogLevels InputProcessor::ProcessVerbosity(const std::string &str) {
    if(str.empty()) return LogLevels::DEBUG;    // empty means user put -v, he probably wants something more verbose!
    else if (g4e::ToLowerCopy(str) == "0") return LogLevels::OFF;
    else if (g4e::ToLowerCopy(str) == "1") return LogLevels::FATAL;
    else if (g4e::ToLowerCopy(str) == "2") return LogLevels::ERROR;
    else if (g4e::ToLowerCopy(str) == "3") return LogLevels::WARN;
    else if (g4e::ToLowerCopy(str) == "4") return LogLevels::INFO;
    else if (g4e::ToLowerCopy(str) == "5") return LogLevels::DEBUG;
    else if (g4e::ToLowerCopy(str) == "6") return LogLevels::TRACE;

    // it is not 0 - 6, so try to parse it (like there are 'debug' or 'info')
    return LogLevels::Parse(str);
}

void InputProcessor::ProcessFileNames(InputArguments &result) {
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

void InputProcessor::ProcessHomePath(InputArguments &result, const char *homeCstr) {
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

void InputProcessor::ProcessMacroPath(InputArguments &result, const char *macroPathCstr) {
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


#endif //G4E_INPUTPROCESSOR_HH
