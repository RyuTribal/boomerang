#pragma once
#include <string>

#include "Util/Log.h"

/*
 * How to encode:
 * encoding will be done as follows:
 * status: #s
 * message #m
 */

namespace Boomerang
{
	enum Status
	{
		WAITING,
		CLIENTS_TURN,
        END,
        NONE
	};

	struct Message
	{
		Status status;
		std::string message;
	};

	class MessageParser
	{
	public:
        static Message DecodeMessage(const std::string& buffer)
        {
            Message msg;

            size_t StatusPos = buffer.find("#s");
            size_t MessagePos = buffer.find("#m");

            if (StatusPos == std::string::npos && MessagePos == std::string::npos)
            {
                msg.message = buffer;
                msg.status = NONE;
                return msg;
            }

            if (StatusPos != std::string::npos && MessagePos != std::string::npos)
            {
                std::string statusStr = buffer.substr(StatusPos + 2, MessagePos - StatusPos - 2);
                if (statusStr == "WAITING")
                {
                    msg.status = WAITING;
                }
                else if (statusStr == "CLIENTS_TURN")
                {
                    msg.status = CLIENTS_TURN;
                }
                else if(statusStr == "END")
                {
                    msg.status = END;
                }

                msg.message = buffer.substr(MessagePos + 2);
            }

            return msg;
        }

        static std::string EncodeMessage(const Message& msg)
        {
            std::string buffer;

            switch (msg.status)
            {
            case WAITING:
                buffer += "#sWAITING";
                break;
            case CLIENTS_TURN:
                buffer += "#sCLIENTS_TURN";
                break;
            case END:
                buffer += "#sEND";
                break;
            }

            buffer += "#m" + msg.message;

            return buffer;
        }
    };
}
