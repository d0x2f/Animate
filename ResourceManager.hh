#pragma once

#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>

namespace Animate
{
    class ResourceManager
    {
        public:
            ResourceManager();
            ~ResourceManager();

            void load_from_directory(std::string directory);
            std::vector<GLbyte> get_resource(std::string name);
            std::string get_resource_as_string(std::string name);

        private:
            std::map< std::string, std::vector<GLbyte> > data_store;
    };
}
