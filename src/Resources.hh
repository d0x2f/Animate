#pragma once

#include <vector>
#include <map>

namespace Animate
{
    class Resources
    {
        public:
            static void initialise();
            static std::map<std::string, std::vector<uint8_t> > resources;
    };
}