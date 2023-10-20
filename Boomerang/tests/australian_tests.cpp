#include "Boomerang.h"
#include "RuleSets/VariantFactory.h"
#include <gtest/gtest.h>

#include "RuleSets/Australian/GameConfig/Components.h"
#include "RuleSets/Australian/ScoringSystems/ThrowCatch.h"
#include "RuleSets/Australian/ScoringSystems/TouristSite.h"

class GameTest : public ::testing::Test {
public:
	std::unique_ptr<Boomerang::Deck> test_deck;
    std::unique_ptr<Boomerang::GameVariant> test_variant;
    GameTest()
    {
        
    }
protected:

    void SetUp() override {
        Boomerang::GameVariantFactory::RegisterAllGameVariants();
        auto [game_variant, deck] = Boomerang::GameVariantFactory::CreateVariantAndDeck("Australian");
        test_deck = std::move(deck);
        test_deck->BuildDeck();
        test_variant = std::move(game_variant);
        test_variant->BuildRules();
    }
    void TearDown() override {
        test_deck = nullptr;
        test_variant = nullptr;
    }
};

TEST_F(GameTest, DealCards) {
    
    Boomerang::Boomerang game{ test_deck.get(), test_variant.get()};
    for (auto &player : game.variant()->GetPlayers()) {
        EXPECT_EQ(player.second->GetHand()->size(), 7); // each player should have 7 cards
    }
}

TEST_F(GameTest, TooFewPlayers)
{
    test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    ASSERT_THROW(
        test_variant->CheckRulesValidity(),
        std::runtime_error
    );
}

TEST_F(GameTest, TooManyPlayers)
{
    for (int i = 0; i < 5; i++) {
        test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    }
    ASSERT_THROW(
        test_variant->CheckRulesValidity(),
        std::runtime_error
    );
}

TEST_F(GameTest, EnoughPlayers)
{
	test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    test_variant->CheckRulesValidity();

}

TEST_F(GameTest, FullDeck)
{
    EXPECT_EQ(test_deck->size(), 28);
}

TEST_F(GameTest, ValidateTouristSiteOnCards) {
    auto &regionSiteMap = test_variant->GetGameConfig()->GetValueOfComponent<Boomerang::RegionSiteMapComponent>();

    for (const auto& card : test_deck->deck()) {
        auto touristSiteComponent = card->GetComponent<Boomerang::RegionComponent>();
        EXPECT_NE(touristSiteComponent, nullptr); 

        auto letterComponent = card->GetComponent<Boomerang::SiteComponent>();
        EXPECT_NE(letterComponent, nullptr); 

        EXPECT_TRUE(regionSiteMap.find(touristSiteComponent->value) != regionSiteMap.end()) << "Region " << touristSiteComponent->value << " not found in RegionSiteMap.";

        std::string validLetters = regionSiteMap[touristSiteComponent->value];

        EXPECT_NE(validLetters.find(letterComponent->value), std::string::npos) << "Letter " << letterComponent->value << " is not valid for region " << touristSiteComponent->value << " This was the map: " << validLetters;
    }
}

TEST_F(GameTest, ValidNumberForScore) {
    for (auto& card : test_deck->deck()) { 
        auto numberComponent = card->GetComponent<Boomerang::NumberComponent>();
        ASSERT_NE(numberComponent, nullptr) << "Card lacks a NumberComponent";

        ASSERT_NE(&(numberComponent->value), nullptr) << "NumberComponent's value is a nullptr";
    }
}

TEST_F(GameTest, CardHasAtLeastTwoSymbols) {
    for (auto& card : test_deck->deck()) {
        int symbolCount = 0;

        // Count symbols in Collections
        auto collectionsComponent = card->GetValueOfComponent<Boomerang::CollectionsComponent>();
        if (!collectionsComponent.empty() &&
            (collectionsComponent == "Leaves" ||
                collectionsComponent == "Wildflowers" ||
                collectionsComponent == "Shells" ||
                collectionsComponent == "Souvenirs")) {
            symbolCount++;
        }

        // Count symbols in Animals
        auto animalsComponent = card->GetValueOfComponent<Boomerang::AnimalsComponent>();
        if (!animalsComponent.empty() &&
            (animalsComponent == "Kangaroos" ||
                animalsComponent == "Emus" ||
                animalsComponent == "Wombats" ||
                animalsComponent == "Koalas" ||
                animalsComponent == "Platypuses")) {
            symbolCount++;
        }

        // Count symbols in Activities
        auto activitiesComponent = card->GetValueOfComponent<Boomerang::ActivitiesComponent>();
        if (!activitiesComponent.empty() &&
            (activitiesComponent == "Swimming" ||
                activitiesComponent == "Bushwalking" ||
                activitiesComponent == "Indigenous Culture" ||
                activitiesComponent == "Sightseeing")) {
            symbolCount++;
        }

        ASSERT_GE(symbolCount, 2) << "Card has fewer than two symbols: " << symbolCount << " Card details: " + card->ToString();
    }
}


TEST_F(GameTest, ShuffleDeck) {
    std::vector<int> originalOrder;
    for (const auto& card : test_deck->deck()) {
        originalOrder.push_back(card->id());
    }

    test_deck->ShuffleDeck();

    bool isSameOrder = true;
    size_t idx = 0;
    for (const auto& card : test_deck->deck()) {
        if (originalOrder[idx++] != card->id()) {
            isSameOrder = false;
            break;
        }
    }

    ASSERT_FALSE(isSameOrder) << "Deck seems not to be shuffled";

    std::vector<int> shuffledOrder;
    for (const auto& card : test_deck->deck()) {
        shuffledOrder.push_back(card->id());
    }

    ASSERT_EQ(originalOrder.size(), shuffledOrder.size()) << "Some cards are missing or duplicated after shuffling";
    std::sort(originalOrder.begin(), originalOrder.end());
    std::sort(shuffledOrder.begin(), shuffledOrder.end());
    ASSERT_EQ(originalOrder, shuffledOrder) << "Some cards are missing or duplicated after shuffling";
}

TEST_F(GameTest, CheckThrowCardAfterDeal) {
    test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    test_variant->AddPlayer(Boomerang::PlayerTypes::BOT);

    Boomerang::Boomerang game{ test_deck.get(), test_variant.get() };
    game.DealCards();
    

    for (const auto& playerPair : game.variant()->GetPlayers()) {
        const auto& player = playerPair.second;
        std::tuple<int, int> answer = player->ChooseCard(*player->GetHand(), true);
        player->AddToDraft(test_deck->GetCardById(std::get<1>(answer)));
        EXPECT_EQ(player->GetDraft()->size(), 1) << "Player " << player->getID() << " has incorrect draft size after deal!";
    }
}

TEST_F(GameTest, ThrowCatchScore)
{
    /*
     *This ensures the first and last card is always
     * ["The Bungle Bungles", 'A', "Western Australia", 1, "Leaves", "", "Indigenous Culture"]
     * ["Nitmiluk National Park", 'G', "Northern Territory", 4, "Shells", "Platypuses", ""]
    */ 
    test_deck->ResetDeck(); 
	
    std::vector<Boomerang::Card*> fake_draft{};
    for(int i = 0; i < 7; i++)
    {
        fake_draft.push_back(test_deck->TakeCard());
    }

    EXPECT_EQ(Boomerang::ThrowCatchScoringSystem().UpdateWithArgs(fake_draft, 1, *test_variant->GetGameConfig(), test_variant->GetPlayers()), 3);
    
}

TEST(GameTouristSites, CheckValidTouristSiteLetterOnCard)
{
    /*
     * This ensures the following card sequence for both players:
     
     
        ("The Bungle Bungles", 'A', "Western Australia", 1, "Leaves", "", "Indigenous Culture");
        ("The Pinnacles", 'B', "Western Australia", 1, "", "Kangaroos", "Sightseeing");
        ("Margaret River", 'C', "Western Australia", 1, "Shells", "Kangaroos", "");
        ("Kalbarri National Park", 'D', "Western Australia", 1, "Wildflowers", "", "Bushwalking");
        ("Uluru", 'E', "Northern Territory", 4, "", "Emus", "Indigenous Culture");
        ("Kakadu National Park", 'F', "Northern Territory", 4, "", "Wombats", "Sightseeing");
        ("Nitmiluk National Park", 'G', "Northern Territory", 4, "Shells", "Platypuses", "");

        10pts
        
        ("King's Canyon", 'H', "Northern Territory", 4, "", "Koalas", "Swimming");
        ("The Great Barrier Reef", 'I', "Queensland", 6, "Wildflowers", "", "Sightseeing");
        ("The Whitsundays", 'J', "Queensland", 6, "", "Kangaroos", "Indigenous Culture");
        ("Daintree Rainforest", 'K', "Queensland", 6, "Souvenirs", "", "Bushwalking");
        ("Surfers Paradise", 'L', "Queensland", 6, "Wildflowers", "", "Swimming");
        ("Barossa Valley", 'M', "South Australia", 3, "", "Koalas", "Bushwalking");
        ("Lake Eyre", 'N', "South Australia", 3, "", "Emus", "Swimming");

        10pts
    */



    Boomerang::GameVariantFactory::RegisterAllGameVariants();
    std::shared_ptr<Boomerang::AustralianDeck> shared_deck = std::make_shared<Boomerang::AustralianDeck>();
    std::shared_ptr<Boomerang::AustralianVariant> shared_variant = std::make_shared<Boomerang::AustralianVariant>();
    shared_deck->BuildDeck();

    shared_deck->ResetDeck();

    int player_id_1 = shared_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    std::vector<Boomerang::Card*> fake_draft_1{};
    for (int i = 0; i < 7; i++)
    {
        fake_draft_1.push_back(shared_deck->TakeCard());
    }
    int player_id_2 = shared_variant->AddPlayer(Boomerang::PlayerTypes::BOT);
    std::vector<Boomerang::Card*> fake_draft_2{};
    for (int i = 0; i < 7; i++)
    {

        fake_draft_2.push_back(shared_deck->TakeCard());
    }

    shared_variant->BuildRules();

    EXPECT_EQ(Boomerang::TouristSiteScoringSystem().UpdateWithArgs(fake_draft_1, player_id_1, *shared_variant->GetGameConfig(), shared_variant->GetPlayers()), 10) << "Cards: " << shared_variant->GetPlayers()[player_id_1]->DraftToString(false);
    EXPECT_EQ(Boomerang::TouristSiteScoringSystem().UpdateWithArgs(fake_draft_2, player_id_2, *shared_variant->GetGameConfig(), shared_variant->GetPlayers()), 10) << "Cards: " << shared_variant->GetPlayers()[player_id_2]->DraftToString(false);
}

// You get the point, the tests are relatively the same

