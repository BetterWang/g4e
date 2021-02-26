#include <vector>
#include <string>

#include <CLI11.hpp>

#include <StringHelpers.hh>
#include <LogLevels.hh>
#include "ArgumentProcessor.hh"


UserArguments InputProcessor::Process(int argc, char **argv)
{
    UserArguments result;                                          // This function result
    CLI::App app{mAppDescription};

    bool optShowGui = false;
    int optThreads = 1;
    std::string optVerbose("info");
    std::string optOutputName("g4e_output");
    std::vector<std::string> optAllFiles;
    app.add_flag("-g,--gui", optShowGui, "Shows Geant4 GUI");
    app.add_option("-o,--output", optOutputName, "Base name for Output files");
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

    //
    // Input files (macros and data files)
    result.AllFileNames = optAllFiles;
    ProcessFileNames(result);                               // Separate file names as macro / data files
    if(result.MacroFileNames.empty()) {
        // TODO interactive terminal mode
        InputProcessor::PrintNoMacroHelp();
        exit(0);
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

    // Output file name:
    result.OutputBaseName = optOutputName;

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

void InputProcessor::ProcessFileNames(UserArguments &result) {
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

void InputProcessor::ProcessHomePath(UserArguments &result, const char *homeCstr) {
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

void InputProcessor::ProcessMacroPath(UserArguments &result, const char *macroPathCstr) {
    result.IsSetMacroPath = macroPathCstr != nullptr;
    result.MacroPath = macroPathCstr? macroPathCstr: "";

    // Add JLeic detector to default Macro Path
    std::vector<std::string> paths;
    if(result.IsSetMacroPath) {
        result.MacroPath += ":"+result.ResourcePath+"/reference_detector";
    } else {
        result.MacroPath = result.ResourcePath+"/reference_detector";
    }
    fmt::print("ENV:G4E_MACRO_PATH:  is-set={}, value='{}'",  result.IsSetMacroPath, result.MacroPath );

}

std::string InputProcessor::mAppDescription =
        "g4e - Geant 4 Electron Ion Collider - is a full simulation part of ESCalate framework.";

void InputProcessor::PrintNoMacroHelp() {
    fmt::print(mAppDescription + "\n\n"
        "There is no macro files provided. Please look at "
        "$G4E_HOME/examples folder macros examples or how to run g4e from python\n"
        "You can get more help here: \n"
        "https://g4e.readthedocs.io/\n"
        "https://geant4.web.cern.ch/\n\n");
}