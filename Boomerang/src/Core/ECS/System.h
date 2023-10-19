#pragma once
#include <vector>

#include "Entity.h"

namespace Boomerang {
    class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void Update() = 0;
    };
    template<typename ReturnType, typename... Args>
    class System : public ISystem {
    public:
        virtual ReturnType UpdateWithArgs(Args... args) = 0;

        void Update() override {
            // This function will remain empty because the actual update
            // will be done using UpdateWithArgs.

            /*
             * This might seem like a hack
             * but in order to provide the opportunity to create a simple
             * system later that isn't a scoring system
             * with any amount of arguments, this is necessary
             *
             * This is called type erasure
             */
        }
    };

}
