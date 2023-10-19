#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Boomerang
{
	std::shared_ptr<spdlog::logger> Log::s_ServerLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_GameLogger;
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_ServerLogger = spdlog::stdout_color_mt("SERVER");
		s_ServerLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_GameLogger = spdlog::stdout_color_mt("BOOMERANG");
		s_GameLogger->set_level(spdlog::level::trace);
	}
}