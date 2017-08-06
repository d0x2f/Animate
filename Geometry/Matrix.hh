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
            static Matrix frustum(float left, float right, float bottom, float top, float near, float far);
            static Matrix orthographic(float left, float right, float bottom, float top, float near, float far);
    };
}
