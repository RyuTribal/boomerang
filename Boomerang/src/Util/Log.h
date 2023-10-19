#pragma once

#include <iostream>
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


namespace Boomerang
{
	enum ErrorType
	{
		BOOMERANG,
		SERVER,
		CLIENT
	};

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetServerLogger() { return s_ServerLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetGameLogger() { return s_GameLogger; }

		inline static std::string MessageAndWaitForInput(const std::string message, const std::string default)
		{
			std::cout << message << std::endl;
			std::string response;
			std::getline(std::cin, response);

			if(response.empty())
			{
				response = default;
			}
			return response;
		}

		inline static int ToInt(const std::string message)
		{
			int number;
			try {
				number = std::stoi(message);
			}
			catch (const std::exception& e) {
				number = NULL;
			}
			return number;
		}

		inline static void ThrowRuntimeError(ErrorType type, std::string message)
		{
			switch(type)
			{
			case BOOMERANG: 
				throw std::runtime_error("[GAME_ERROR]: " + message);
			case SERVER:
				throw std::runtime_error("[SERVER_ERROR]: " + message);
			case CLIENT:
				throw std::runtime_error("[CLIENT_ERROR]: " + message);
			}
		}

		inline static ErrorType ExtractErrorType(std::string message)
		{
			if (message.find("[SERVER_ERROR]") != std::string::npos)
			{
				return SERVER;
			}
			else if(message.find("[CLIENT_ERROR]") != std::string::npos)
			{
				return CLIENT;
			}
			else
			{
				return BOOMERANG;
			}
		}

	private:
		static std::shared_ptr<spdlog::logger> s_ServerLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_GameLogger;
	};
}

// Server log macros
#define SERVER_FATAL(...) ::Boomerang::Log::GetServerLogger()->critical(__VA_ARGS__)
#define SERVER_ERROR(...) ::Boomerang::Log::GetServerLogger()->error(__VA_ARGS__)
#define SERVER_WARN(...) ::Boomerang::Log::GetServerLogger()->warn(__VA_ARGS__)
#define SERVER_INFO(...) ::Boomerang::Log::GetServerLogger()->info(__VA_ARGS__)
#define SERVER_TRACE(...) ::Boomerang::Log::GetServerLogger()->trace(__VA_ARGS__)

// Client log macros
#define CLIENT_FATAL(...) ::Boomerang::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CLIENT_ERROR(...) ::Boomerang::Log::GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_WARN(...) ::Boomerang::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_INFO(...) ::Boomerang::Log::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_TRACE(...) ::Boomerang::Log::GetClientLogger()->trace(__VA_ARGS__)

// Game log macros
#define GAME_FATAL(...) ::Boomerang::Log::GetGameLogger()->critical(__VA_ARGS__)
#define GAME_ERROR(...) ::Boomerang::Log::GetGameLogger()->error(__VA_ARGS__)
#define GAME_WARN(...) ::Boomerang::Log::GetGameLogger()->warn(__VA_ARGS__)
#define GAME_INFO(...) ::Boomerang::Log::GetGameLogger()->info(__VA_ARGS__)
#define GAME_TRACE(...) ::Boomerang::Log::GetGameLogger()->trace(__VA_ARGS__)

// Generic macros
#define MESSAGE_AND_WAIT(message) ::Boomerang::Log::MessageAndWaitForInput(message, "")
#define MESSAGE_AND_WAIT_WITH_DEFAULT(message, default) ::Boomerang::Log::MessageAndWaitForInput(message, default)
#define MESSAGE_TO_INT(message) ::Boomerang::Log::ToInt(message)
#define HANDLE_ERROR(error_origin, message) ::Boomerang::Log::ThrowRuntimeError(error_origin, message)
#define ERROR_TYPE(message) ::Boomerang::Log::ExtractErrorType(std::string(message));