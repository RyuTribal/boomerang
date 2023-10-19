#include "VariantFactory.h"

namespace Boomerang {

    std::unordered_map<std::string, GameVariantFactory::FactoryFunction> GameVariantFactory::factories;

    void GameVariantFactory::RegisterGameVariant(const std::string& variant_name, FactoryFunction func) {
        factories[variant_name] = func;
    }

    std::pair<std::unique_ptr<GameVariant>, std::unique_ptr<Deck>> GameVariantFactory::CreateVariantAndDeck(const std::string& variant_name) {
        auto it = factories.find(variant_name);
        if (it != factories.end()) {
            return it->second();
        }
        return { nullptr, nullptr };
    }

}