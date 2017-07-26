#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

namespace Animate::Geometry {

    /**
     * 2d vector.
     */
    struct Vector2 {
        GLfloat x,y;

        Vector2(GLfloat x=0., GLfloat y=0.) : x(x), y(y) {}

        /**
         * Normalise.
         */
        Vector2 normalise()
        {
            GLfloat u = sqrt( pow(x, 2) + pow(y, 2) );

            //Avoid division by zero
            if ( u == 0) {
                u = 1;
            }

            return Vector2(
                x/u,
                y/u
            );
        }

        /**
         * Dot product.
         */
        GLfloat dot(Vector2 that)
        {
            return  x * that.x +
                    y * that.y;
        }

        /**
         * Addition.
         */
        Vector2 operator+(Vector2 that)
        {
            return Vector2(
                x + that.x,
                y + that.y
            );
        }

        /**
         * Subtraction.
         */
        Vector2 operator-(Vector2 that)
        {
            return Vector2(
                x - that.x,
                y - that.y
            );
        }

        /**
         * Negation.
         */
        Vector2 operator*(GLfloat factor)
        {
            return Vector2(
                x * factor,
                y * factor
            );
        }
    };
    //Aliases
    typedef Vector2 Position;
    typedef Vector2 Size;

    /**
     * 3d vector.
     */
    struct Vector3 {
        GLfloat x,y,z;

        Vector3(GLfloat x=0., GLfloat y=0., GLfloat z=0.) : x(x), y(y), z(z) {}

        Vector2 xy()
        {
            return Vector2(x, y);
        }

        /**
         * Normalise.
         */
        Vector3 normalise()
        {
            GLfloat u = sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) );

            //Avoid division by zero
            if ( u == 0) {
                u = 1;
            }

            return Vector3(
                x/u,
                y/u,
                z/u
            );
        }

        bool is_out_of_bounds(Vector2 bottom_left, Vector2 top_right)
        {
            if (this->x < bottom_left.x)
                return true;

            if (this->x > top_right.x)
                return true;

            if (this->y < bottom_left.y)
                return true;

            if (this->y > top_right.y)
                return true;

            return false;
        }

        /**
         * Dot product.
         */
        GLfloat dot(Vector3 that)
        {
            return  x * that.x +
                    y * that.y +
                    z * that.z;
        }

        /**
         * Cross product.
         */
        Vector3 cross(Vector3 that)
        {
            return Vector3(
                y * that.z - z * that.y,
                z * that.x - x * that.z,
                x * that.y - y * that.x
            );
        }

        /**
         * Addition.
         */
        Vector3 operator+(Vector3 that)
        {
            return Vector3(
                x + that.x,
                y + that.y,
                z + that.z
            );
        }

        /**
         * Negation.
         */
        Vector3 operator-(Vector3 that)
        {
            return Vector3(
                x - that.x,
                y - that.y,
                z - that.z
            );
        }

        /**
         * Negation.
         */
        Vector3 operator*(GLfloat factor)
        {
            return Vector3(
                x * factor,
                y * factor,
                z * factor
            );
        }

        /**
         * Addition assignment.
         */
        void operator+=(Vector3 that)
        {
            x += that.x;
            y += that.y;
            z += that.z;
        }

        /**
         * Subtraction assignment.
         */
        void operator-=(Vector3 that)
        {
            x -= that.x;
            y -= that.y;
            z -= that.z;
        }
    };
    //Aliases
    typedef Vector3 Point;
    typedef Vector3 Scale;

    /**
     * 4d vector.
     */
    struct Vector4 {
        GLfloat x,y,z,w;

        Vector4(GLfloat x=0., GLfloat y=0., GLfloat z=0., GLfloat w=0.) : x(x), y(y), z(z), w(w) {}

        /**
         * Dot product.
         */
        GLfloat dot(Vector4 that)
        {
            return  x * that.x +
                    y * that.y +
                    z * that.z +
                    w * that.w;
        }

        /**
        * Vector subtraction
        **/
        Vector4 operator-(Vector4 b)
        {
            return Vector4 (
                x - b.x,
                y - b.y,
                z - b.z,
                w - b.w
            );
        }
    };
    //Aliases
    typedef Vector4 Colour;
}
