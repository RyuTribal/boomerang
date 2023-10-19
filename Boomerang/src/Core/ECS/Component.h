#pragma once

namespace Boomerang {

    class Component {
    public:
        virtual ~Component() = default;
        virtual std::string ToString()
        {
            // Just a default implementation so as not to force
            // the user to implement this
            return "";
        };
    };

}