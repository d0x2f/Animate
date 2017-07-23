#pragma once

#include "Definitions.hh"

namespace Animate::Geometry
{
    /**
     * 4x4 matrix.
     */
    class Matrix
    {
        public:
            Vector4 r1;
            Vector4 r2;
            Vector4 r3;
            Vector4 r4;

            Matrix(Vector4 r1 = Vector4(), Vector4 r2 = Vector4(), Vector4 r3 = Vector4(), Vector4 r4 = Vector4());

            GLfloat *get_raw_data();

            Matrix transpose();
            Matrix translate(Vector3 delta);
            Matrix scale(Vector2 factor);
            Matrix scale(Vector3 factor);
            Matrix rotate(Vector3 rotation);

            Matrix operator*(Matrix b);
            Matrix operator-(Matrix b);

            void print();

            static Matrix identity();
            static Matrix look_at(Vector3 eye, Vector3 center, Vector3 up = Vector3(0.,1.));
            static Matrix frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
            static Matrix orthographic(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
    };
}
