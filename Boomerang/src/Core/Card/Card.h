#pragma once

#include <string>

#include "Components.h"
#include "Core/ECS/Entity.h"

namespace Boomerang
{
	
	class Card : public Entity
	{
	public:
		Card(int id) : m_ID{ id }
		{
			
		}

		int id() { return m_ID; }

		std::string ToString() override
		{
			std::string card_info = "[" + std::to_string(id()) + "]: { ";

			for (const auto& comp : GetComponents()) {
				card_info += comp.second->ToString() + ", ";
			}

			// Remove the trailing comma and space
			if (card_info.size() > 2) {
				card_info.pop_back();
				card_info.pop_back();
			}

			card_info += " }";

			return card_info;
		}
		
	private:
		int m_ID;
	};
}
