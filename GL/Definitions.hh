#pragma once

#include <GL/glew.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

namespace Animate::GL {

    /**
     * 2d vector.
     */
    struct Vector2 {
        GLfloat x,y;

        Vector2(GLfloat x=0., GLfloat y=0.) : x(x), y(y) {}
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

        /**
         * Normalise.
         */
        Vector3 normalise()
        {
            GLfloat u = sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) );
            return Vector3(
                x/u,
                y/u,
                z/u
            );
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
            return Vector3(x + that.x, y + that.y, x+ that.z);
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
         * Negative assignment.
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

    /**
     * 4d vector.
     */
    struct Vector4 {
        GLfloat x,y,z,w;

        Vector4(GLfloat x=0., GLfloat y=0., GLfloat z=0., GLfloat w=1.) : x(x), y(y), z(z), w(w) {}

        /**
         * Return a raw c array.
         **/
        GLfloat *get_raw_data() {
            GLfloat *data = (GLfloat *)malloc(sizeof(GLfloat)*4);

            data[0] = x;
            data[1] = y;
            data[2] = z;
            data[3] = w;

            return data;
        }

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
    };
    //Aliases
    typedef Vector4 Colour;


    /**
     * 4x4 matrix.
     */
    struct Matrix44 {
        Vector4 r1;
        Vector4 r2;
        Vector4 r3;
        Vector4 r4;

        Matrix44(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4) : r1(r1), r2(r2), r3(r3), r4(r4) {}

        /**
         * Return a raw c array.
         **/
        GLfloat *get_raw_data() {
            GLfloat *data = (GLfloat *)malloc(sizeof(GLfloat)*16);

            GLfloat *r1_data = r1.get_raw_data();
            GLfloat *r2_data = r2.get_raw_data();
            GLfloat *r3_data = r3.get_raw_data();
            GLfloat *r4_data = r4.get_raw_data();

            std::memcpy(&data[0],  r1_data, sizeof(GLfloat)*4);
            std::memcpy(&data[4],  r2_data, sizeof(GLfloat)*4);
            std::memcpy(&data[8],  r3_data, sizeof(GLfloat)*4);
            std::memcpy(&data[12], r4_data, sizeof(GLfloat)*4);

            std::free(r1_data);
            std::free(r2_data);
            std::free(r3_data);
            std::free(r4_data);

            return data;
        }

        /**
         * Matrix transposition.
         **/
        Matrix44 transpose()
        {
            return Matrix44(
                Vector4(r1.x, r2.x, r3.x, r4.x),
                Vector4(r1.y, r2.y, r3.y, r4.y),
                Vector4(r1.z, r2.z, r3.z, r4.z),
                Vector4(r1.w, r2.w, r3.w, r4.w)
            );
        }

        /**
         * Matrix multiplication.
         **/
        Matrix44 operator*(Matrix44 that)
        {
            Matrix44 b = that.transpose();
            return Matrix44 (
                Vector4(r1.dot(b.r1), r1.dot(b.r2), r1.dot(b.r3), r1.dot(b.r4)),
                Vector4(r2.dot(b.r1), r2.dot(b.r2), r2.dot(b.r3), r2.dot(b.r4)),
                Vector4(r3.dot(b.r1), r3.dot(b.r2), r3.dot(b.r3), r3.dot(b.r4)),
                Vector4(r4.dot(b.r1), r4.dot(b.r2), r4.dot(b.r3), r4.dot(b.r4))
            );
        }
    };
}
