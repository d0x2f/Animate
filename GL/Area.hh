#pragma once

#include <gtkmm.h>

namespace Animate
{
    class Context;

    namespace GL
    {
        class Area : public Gtk::GLArea
        {
            public:
                Area(Context *context);
                ~Area();

            protected:
                //Slots
                void on_realize();

            private:
                Context *context;
        };
    }
}
