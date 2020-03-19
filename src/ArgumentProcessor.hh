#ifndef G4E_ARGUMENTPROCESSOR_HH
#define G4E_ARGUMENTPROCESSOR_HH

#include <vector>
#include <string>

#include <CLI11.hpp>

#include <StringHelpers.hh>
#include <LogLevels.hh>

/** \file
 *  ArgumentProcessor manage users inputs on program start.
 *  By the arguments one means:
 *      1. Program arguments
 *      2. Environment variables
 *      3. Pipe (when implemented)
 *
 *  InputArguments is the result of work of ArgumentProcessor.
 *  InputArguments represents a set of ready to use parameters:
 *      1. Configure initialization
 *      2. Provide running aspects (verbosity, input, output)
 *      3. Info, what was set by user (macro PATH env, macros to run, input files (if given by command line)
 */


/// Program Configuration provided by arguments or environment variables
/// This class provides all needed dynamic information about flags and
/// environment variables provided by users
struct UserArguments
{
    bool ShowGui = false;
    int ThreadsCount = 1;

    std::vector<std::string> AllFileNames;          /// List of all arguments which are not flags=values (macro file names, input file names, etc.)
    std::vector<std::string> MacroFileNames;        /// List of file names with .mac extension
    std::vector<std::string> SourceFileNames;       /// The list of all other input files
    std::string SourceGenerator;                    /// The generator to use

    std::string MacroPath = "";              /// G4E_MACRO_PATH
    bool IsSetMacroPath = false;             /// true if macro path was defined in environment variables

    std::string HomePath = "";               /// G4E_HOME
    bool IsSetHomePath = false;              /// true if G4E_HOME was determined

    std::string ResourcePath;                /// Path to resources directory

    std::string OutputBaseName;              /// Desired output file(s) name (without suffix and extension)

    LogLevels LogLevel = LogLevels::INFO;    /// Controlled by -v. --verbosity
};



class InputProcessor
{
public:

    static LogLevels ProcessVerbosity(const std::string& str);

    static UserArguments Process(int argc, char **argv);

    /// Prints help if no macro files are provided
    static void PrintNoMacroHelp();

private:


    static void ProcessFileNames(UserArguments& result);

    static void ProcessHomePath(UserArguments& result, const char *homeCstr);

    static void ProcessMacroPath(UserArguments& result, const char *macroPathCstr);

/// Processes both program arguments and environment variables to build the right ProgramArguments
    static std::string mAppDescription;
};




#endif //G4E_ARGUMENTPROCESSOR_HH
