#include "Net/Client.h"

#include "Core.h"
#include "Util/Log.h"


namespace Boomerang
{
	Message Client::WaitForServer()
	{
        char buffer[BUFFER_SIZE];

        while (true)
        {
            int bytes_received = ReceiveData(buffer, sizeof(buffer));
            if (bytes_received > 0)
            {
                return MessageParser::DecodeMessage(buffer);
            }
        }
	}
    void Client::HandleInput()
    {
        std::string answer = MESSAGE_AND_WAIT("Answer: ");
        Message message;
        message.message = answer;
        message.status = Status::WAITING;

        std::string encoded_message = MessageParser::EncodeMessage(message);

        if(SendData(encoded_message))
        {
            CLIENT_INFO("Message sent!");
        }
        else
        {
            CLIENT_ERROR("Message could not send!");
        }
    }
}
