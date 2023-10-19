#pragma once
#include <functional>
#include <memory>

#include "Australian/AustralianDeck.h"
#include "Australian/AustralianVariant.h"
#include "Core/GameVariant.h"
#include "Core/Deck.h"

namespace Boomerang
{
#define REGISTER_GAME_VARIANT(variant_name, variant_class, deck_class) \
    GameVariantFactory::RegisterGameVariant(variant_name, []() -> std::pair<std::unique_ptr<GameVariant>, std::unique_ptr<Deck>> { \
        return { std::make_unique<variant_class>(), std::make_unique<deck_class>() }; \
    });

    class GameVariantFactory {
    public:
        using FactoryFunction = std::function<std::pair<std::unique_ptr<GameVariant>, std::unique_ptr<Deck>>()>;

        static void RegisterGameVariant(const std::string& variant_name, FactoryFunction func);
        static std::pair<std::unique_ptr<GameVariant>, std::unique_ptr<Deck>> CreateVariantAndDeck(const std::string& variant_name);
        static std::unordered_map<std::string, FactoryFunction>& GetAllVariants() { return factories; }

        /*
         *  ADD OTHER GAME VARIANTS HERE!!!!
         *  This system allows for
         *  various variants to be combined with different
         *  decks if you feel lazy and not a want to
         *  create your own deck
         */ 
        static void RegisterAllGameVariants() {
            REGISTER_GAME_VARIANT("Australian", AustralianVariant, AustralianDeck)
        }

    private:
        static std::unordered_map<std::string, FactoryFunction> factories;
    };
}

