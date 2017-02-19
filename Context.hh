#pragma once

#include <memory>

#include "ResourceManager.hh"
#include "GL/Area.hh"

namespace Animate {

    class Context {
        public:
            void set_gl_area(GL::Area *gl_area);

            ResourceManager *get_resource_manager();
            GL::Area *get_gl_area();

        private:
            std::unique_ptr<ResourceManager> resource_manager;
            std::unique_ptr<GL::Area> gl_area;
    };

}
