#pragma once
#include "Cards/Components.h"
#include "Core/Deck.h"

namespace Boomerang
{

	class AustralianDeck : public Deck
	{
	public:
        void BuildDeck() override
        {
            AddCard(next_card_id++, "The Bungle Bungles", 'A', "Western Australia", 1, "Leaves", "", "Indigenous Culture");
            AddCard(next_card_id++, "The Pinnacles", 'B', "Western Australia", 1, "", "Kangaroos", "Sightseeing");
            AddCard(next_card_id++, "Margaret River", 'C', "Western Australia", 1, "Shells", "Kangaroos", "");
            AddCard(next_card_id++, "Kalbarri National Park", 'D', "Western Australia", 1, "Wildflowers", "", "Bushwalking");
            AddCard(next_card_id++, "Uluru", 'E', "Northern Territory", 4, "", "Emus", "Indigenous Culture");
            AddCard(next_card_id++, "Kakadu National Park", 'F', "Northern Territory", 4, "", "Wombats", "Sightseeing");
            AddCard(next_card_id++, "Nitmiluk National Park", 'G', "Northern Territory", 4, "Shells", "Platypuses", "");
            AddCard(next_card_id++, "King's Canyon", 'H', "Northern Territory", 4, "", "Koalas", "Swimming");
            AddCard(next_card_id++, "The Great Barrier Reef", 'I', "Queensland", 6, "Wildflowers", "", "Sightseeing");
            AddCard(next_card_id++, "The Whitsundays", 'J', "Queensland", 6, "", "Kangaroos", "Indigenous Culture");
            AddCard(next_card_id++, "Daintree Rainforest", 'K', "Queensland", 6, "Souvenirs", "", "Bushwalking");
            AddCard(next_card_id++, "Surfers Paradise", 'L', "Queensland", 6, "Wildflowers", "", "Swimming");
            AddCard(next_card_id++, "Barossa Valley", 'M', "South Australia", 3, "", "Koalas", "Bushwalking");
            AddCard(next_card_id++, "Lake Eyre", 'N', "South Australia", 3, "", "Emus", "Swimming");
            AddCard(next_card_id++, "Kangaroo Island", 'O', "South Australia", 3, "", "Kangaroos", "Bushwalking");
            AddCard(next_card_id++, "Mount Gambier", 'P', "South Australia", 3, "Wildflowers", "", "Sightseeing");
            AddCard(next_card_id++, "Blue Mountains", 'Q', "New South Whales", 5, "", "Wombats", "Indigenous Culture");
            AddCard(next_card_id++, "Sydney Harbour", 'R', "New South Whales", 5, "", "Emus", "Sightseeing");
            AddCard(next_card_id++, "Bondi Beach", 'S', "New South Whales", 5, "", "Wombats", "Swimming");
            AddCard(next_card_id++, "Hunter Valley", 'T', "New South Whales", 5, "", "Emus", "Bushwalking");
            AddCard(next_card_id++, "Melbourne", 'U', "Victoria", 2, "", "Wombats", "Bushwalking");
            AddCard(next_card_id++, "The MCG", 'V', "Victoria", 2, "Leaves", "", "Indigenous Culture");
            AddCard(next_card_id++, "Twelve Apostles", 'W', "Victoria", 2, "Shells", "", "Swimming");
            AddCard(next_card_id++, "Royal Exhibition Building", 'X', "Victoria", 2, "Leaves", "Platypuses", "");
            AddCard(next_card_id++, "Salamanca Markets", 'Y', "Tasmania", 7, "Leaves", "Emus", "");
            AddCard(next_card_id++, "Mount Wellington", 'Z', "Tasmania", 7, "", "Koalas", "Sightseeing");
            AddCard(next_card_id++, "Port Arthur", '*', "Tasmania", 7, "Leaves", "", "Indigenous Culture");
            AddCard(next_card_id++, "Richmond", '-', "Tasmania", 7, "", "Kangaroos", "Swimming");
        }

		void AddCard(int id, std::string name, char site, std::string region, int number, std::string collections, std::string animals, std::string activities)
		{
			GenerateCard(id, 
                MakePair<NameComponent>(name), 
                MakePair<SiteComponent>(site),
				MakePair<RegionComponent>(region), 
                MakePair<NumberComponent>(number), 
				MakePair<CollectionsComponent>(collections), 
                MakePair<AnimalsComponent>(animals), 
				MakePair<ActivitiesComponent>(activities));
		}
	};
}
