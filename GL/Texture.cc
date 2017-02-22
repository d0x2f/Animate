#include <gtkmm.h>
#include <iostream>

#include "Texture.hh"
#include "../Utilities.hh"

using namespace Animate::GL;

Texture::~Texture()
{
    glDeleteTextures(1, &this->texture_id);
}

void Texture::load_from_resource(std::string resource_id)
{
    std::cout << "Loading texture: " << resource_id << std::endl;

    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_resource(resource_id);

    if (!pixbuf) {
        g_assert_not_reached();
    }

    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        pixbuf->get_width(),
        pixbuf->get_height(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixbuf->get_pixels()
    );

    /*gluBuild2DMipmaps(
        GL_TEXTURE_2D,
        GL_RGB,
        image.get_width(),
        image.get_height(),
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixbuf->get_pixels()
    );*/

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::use()
{
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

GLuint Texture::get_id()
{
    return this->texture_id;
}
