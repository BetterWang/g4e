#ifndef G4E_LOGLEVELS_HH
#define G4E_LOGLEVELS_HH

#include <G4String.hh>

#include <G4Exception.hh>

#include <spdlog/spdlog.h>

#include <StringHelpers.hh>

/**
 * This is an enum with some syntax sugar:
 * Now you can write:
 *
 *   - LogLevels f = LogLevels::WARN;
 *   - LogLevels f("Warn");
 *   - logLevel.ToString();
 *   - And the compiler will prevent things like:  LogLevels f = 1;  // Compile time error.
 */
class LogLevels
{
public:


    enum Values
    {
        OFF = 0,
        FATAL,      /// Before immediate crash
        ERROR,      /// The data after this shouldn't be trusted
        WARN,       /// It is better to be reviewed by experts if appears
        INFO,       /// Some important messages to users. Not too verbose
        DEBUG,      /// Messages needed for development
        TRACE,      /// Tons* of debugging output!
    };

    LogLevels() = default;

    constexpr LogLevels(Values val) : value(val) { }

    // Convert from a string
    explicit LogLevels(const G4String& str): value(Parse(str)) { }

    static LogLevels::Values Parse(const G4String& str);

    G4String ToString();


    explicit operator Values() const { return value; }  // Allow switch and comparisons.
                                                       // note: Putting constexpr here causes
                                                       // clang to stop warning on incomplete
                                                       // case handling.
    explicit operator bool() = delete;        // Prevent usage: if(fruit)

    //constexpr operator==(LogLevels a) const { return value == a.value; }
    //constexpr operator!=(LogLevels a) const { return value != a.value; }

private:
    Values value;
};

LogLevels::Values LogLevels::Parse(const G4String &str)
{
    if(str.empty() || g4e::ToLowerCopy(str) == "off") return Values::OFF;
    else if (g4e::ToLowerCopy(str) == "fatal") return FATAL;
    else if (g4e::ToLowerCopy(str) == "error") return ERROR;
    else if (g4e::ToLowerCopy(str) == "warn") return WARN;
    else if (g4e::ToLowerCopy(str) == "warning") return WARN;
    else if (g4e::ToLowerCopy(str) == "info") return INFO;
    else if (g4e::ToLowerCopy(str) == "debug") return DEBUG;
    else if (g4e::ToLowerCopy(str) == "trace") return TRACE;
    else if (g4e::ToLowerCopy(str) == "verbose") return TRACE;

    // Wrong word!
    auto message = fmt::format("Can't parse this log level: '{}'", str);
    G4Exception("LogLevels::LogLevels(const G4String& str)", "g4e", FatalException, message.c_str());
}

G4String LogLevels::ToString()
{
    switch (value) {
        case FATAL: return "fatal";
        case ERROR: return "error";
        case WARN:  return "warn";
        case INFO:  return "info";
        case DEBUG: return "debug";
        case TRACE: return "trace";
        case OFF:   return "off";
    }
    G4Exception("LogLevels::ToString()", "g4e", FatalException, "Case not implemented");
    return "unknown";
}


#endif //G4E_LOGLEVELS_HH

// * Ton - It is derived from the tun, the term applied to a cask of the largest capacity.
// This could contain a volume between 175 and 213 imperial gallons (210 and 256 US gal; 800 and 970 l),
// which could weigh around 2,000 pounds (910 kg) and occupy some 60 cubic feet (1.7 m3) of space.