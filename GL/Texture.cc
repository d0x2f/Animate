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
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_resource(resource_id);

    if (!pixbuf) {
        g_assert_not_reached();
    }

    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glTexStorage2D(
        GL_TEXTURE_2D,
        10,
        GL_RGBA8,
        pixbuf->get_width(),
        pixbuf->get_height()
    );
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        pixbuf->get_width(),
        pixbuf->get_height(),
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixbuf->get_pixels()
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Loaded texture: " << resource_id << " (" << this->texture_id << ")" << std::endl;
}

void Texture::use()
{
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

GLuint Texture::get_id()
{
    return this->texture_id;
}
