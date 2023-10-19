#pragma once
#include "Components.h"
#include "Core/Card/Card.h"
#include "Core/ECS/Entity.h"

namespace Boomerang
{
	class Player : public Entity
	{
	public:
		Player(int id) : _id(id)
		{
			AddComponent<DraftComponent>();
			AddComponent<HandComponent>();
			AddComponent<RoundScoreComponent>();
		}
		int getID() const { return _id; }

		std::vector<Card*>* GetHand() { return GetValueOfComponent<HandComponent>().get(); }
		std::unique_ptr<std::vector<Card*>>& GetHandPtr() { return GetValueOfComponent<HandComponent>(); }

		std::vector<Card*>* GetDraft() { return &GetValueOfComponent<DraftComponent>(); }

		std::unique_ptr<std::vector<Card*>> MoveHand() { return std::move(GetHandPtr()); }

		void SetHand(std::unique_ptr<std::vector<Card*>> new_hand) { GetHandPtr() = std::move(new_hand); }

		std::vector<int>* GetScoreTable() { return &GetValueOfComponent<RoundScoreComponent>(); }

		void AddRoundScore(int score) { GetScoreTable()->push_back(score); }

		int GetScore(int round) { return (*GetScoreTable())[round - 1]; }

		int GetTotalScore()
		{
			int score = 0;
			for (int round_score : *GetScoreTable())
			{
				score += round_score;
			}

			return score;
		}

		void AddToHand(Card* card) { GetHandPtr()->emplace_back(card); }

		void AddToDraft(Card* card)
		{
			auto it = std::find(GetHandPtr()->begin(), GetHandPtr()->end(), card);
			if (it != GetHandPtr()->end()) {
				int index = std::distance(GetHandPtr()->begin(), it);
				GetDraft()->emplace_back(PopFromHand(index));
			}
		}

		Card* PopFromHand(int index)
		{
			if (index >= 0 && index < GetHandPtr()->size()) {
				Card* card = GetHandPtr()->at(index);
				GetHandPtr()->erase(GetHandPtr()->begin() + index);
				return card;
			}
			return nullptr;
		}
		void ClearDraft() { GetDraft()->clear(); }

		std::string GetHandString()
		{
			std::string hand_string = "";
			for (Card* card : *GetHandPtr())
			{
				hand_string += card->ToString() + "\n";
			}
			return hand_string;
		}

		bool ExistsInHand(int id)
		{
			for (auto& card : *GetHandPtr())
			{
				if (card->id() == id)
				{
					return true;
				}
			}
			return false;
		}

		bool ExistsInHand(int id, std::vector<Card*>& cards)
		{
			for (auto& card : cards)
			{
				if (card->id() == id)
				{
					return true;
				}
			}
			return false;
		}

		virtual std::tuple<int, int> ChooseCard(std::vector<Card*>& cards, bool automatic_choice) = 0;

		std::string type_name() override { return "Player"; }

		std::string DraftToString(bool ignore_throw)
		{
			std::string draft_string = "";
			bool first = true;
			for(Card* card : *GetDraft())
			{
				if(first && ignore_throw)
				{
					draft_string += "----------------------------------------\n";
					first = false;
				}
				else
				{
					draft_string += card->ToString()+ "\n";
				}
			}
			return draft_string;
		}

	private:
		int _id;
	};
}
