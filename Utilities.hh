#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

namespace Animate {

    class Utilities {
        public:
            static std::vector<GLbyte> read_file(std::string filename);
    };

}
