#ifndef G4E_LOGGING_HH
#define G4E_LOGGING_HH

#include <spdlog/sinks/ostream_sink.h>

#include "LogLevels.hh"

namespace Logging {
    void InitializeSpdLog();                  /// Initialize spdlog
    void SetGlobalLevel(LogLevels level);     /// Sets the global log level
}

void Logging::SetGlobalLevel(LogLevels level)
{
    switch ((LogLevels::Values) level) {
        case LogLevels::FATAL:
            spdlog::set_level(spdlog::level::critical);
            break;
        case LogLevels::ERROR:
            spdlog::set_level(spdlog::level::err);
            break;
        case LogLevels::WARN:
            spdlog::set_level(spdlog::level::warn);
            break;
        case LogLevels::INFO:
            spdlog::set_level(spdlog::level::info);
            break;
        case LogLevels::DEBUG:
            spdlog::set_level(spdlog::level::debug);
            break;
        case LogLevels::TRACE:
            spdlog::set_level(spdlog::level::trace);
            break;
        case LogLevels::OFF:
            spdlog::set_level(spdlog::level::off);
            break;
        default:
            G4Exception("Logging::SetGlobalLevel()", "g4e", FatalException, "Case not implemented");
    }

    spdlog::debug("Log level is set to: {}", level.ToString());
}

void Logging::InitializeSpdLog()
{
    spdlog::info("Initializing g4e, parsing arguments...");
    //auto osink = std::make_shared<spdlog::sinks::ostream_sink_mt> (G4cout);
    //spdlog::default_logger()->sinks().clear();
    //spdlog::default_logger()->sinks().push_back(osink);
    spdlog::info("Initialized G4E sink");
    spdlog::set_level(spdlog::level::debug);
}


#endif //G4E_LOGGING_HH
