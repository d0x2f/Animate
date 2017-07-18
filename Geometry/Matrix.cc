#include <iostream>
#include <iomanip>

#include "Matrix.hh"

using namespace Animate::Geometry;

/**
 * Constructor.
 */
Matrix::Matrix(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4) : r1(r1), r2(r2), r3(r3), r4(r4)
{}

/**
* Return a raw c array.
**/
GLfloat *Matrix::get_raw_data() {
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
Matrix Matrix::transpose()
{
    return Matrix(
        Vector4(r1.x, r2.x, r3.x, r4.x),
        Vector4(r1.y, r2.y, r3.y, r4.y),
        Vector4(r1.z, r2.z, r3.z, r4.z),
        Vector4(r1.w, r2.w, r3.w, r4.w)
    );
}

/**
 * Apply a translation transform.
 *
 * @param delta The movement vector
 *
 * @return A new matrix with the transform applied.
 */
Matrix Matrix::translate(Vector3 delta)
{
    return Matrix(
        Vector4(1.,0.,0., delta.x),
        Vector4(0.,1.,0., delta.y),
        Vector4(0.,0.,1., delta.z),
        Vector4(0.,0.,0., 1.)
    ) * (*this);
}

/**
 * Apply a scaling transform.
 *
 * @param factor A vector representing the x, y and z scaling factors.
 *
 * @return A new matrix with the transform applied.
 */
Matrix Matrix::scale(Vector2 factor)
{
    return Matrix(
        Vector4(factor.x),
        Vector4(0., factor.y),
        Vector4(0.,0., 1.),
        Vector4(0.,0.,0., 1.)
    ) * (*this);
}

/**
 * Apply a scaling transform.
 *
 * @param factor A vector representing the x, y and z scaling factors.
 *
 * @return A new matrix with the transform applied.
 */
Matrix Matrix::scale(Vector3 factor)
{
    return Matrix(
        Vector4(factor.x),
        Vector4(0., factor.y),
        Vector4(0.,0., factor.z),
        Vector4(0.,0.,0., 1.)
    ) * (*this);
}

/**
 * Apply a rotation transform.
 *
 * @param factor A vector representing the x, y and z rotation values.
 *
 * @return A new matrix with the transform applied.
 */
Matrix Matrix::rotate(Vector3 rotation)
{
    const GLfloat& x = rotation.x;
    const GLfloat& y = rotation.y;
    const GLfloat& z = -rotation.z;

    return Matrix(      //Z
        Vector4(cos(z), -sin(z), 0, 0),
        Vector4(sin(z), cos(z), 0, 0),
        Vector4(0, 0, 1, 0),
        Vector4(0, 0, 0, 1)
    ) * Matrix(         //Y
        Vector4(cos(y), 0, sin(y), 0),
        Vector4(0, 1, 0, 0),
        Vector4(-sin(y), 0, cos(y), 0),
        Vector4(0, 0, 0, 1)
    ) * Matrix(         //X
        Vector4(1, 0, 0, 0),
        Vector4(0, cos(x), -sin(x), 0),
        Vector4(0, sin(x), cos(x), 0),
        Vector4(0, 0, 0, 1)
    ) * (*this);
}

/**
* Matrix multiplication.
**/
Matrix Matrix::operator*(Matrix b)
{
    b = b.transpose();
    return Matrix (
        Vector4(r1.dot(b.r1), r1.dot(b.r2), r1.dot(b.r3), r1.dot(b.r4)),
        Vector4(r2.dot(b.r1), r2.dot(b.r2), r2.dot(b.r3), r2.dot(b.r4)),
        Vector4(r3.dot(b.r1), r3.dot(b.r2), r3.dot(b.r3), r3.dot(b.r4)),
        Vector4(r4.dot(b.r1), r4.dot(b.r2), r4.dot(b.r3), r4.dot(b.r4))
    );
}

/**
* Matrix subtraction.
**/
Matrix Matrix::operator-(Matrix b)
{
    return Matrix (
        r1 - b.r1,
        r2 - b.r2,
        r3 - b.r3,
        r4 - b.r4
    );
}

/**
 * Print this matrix to cout.
 */
void Matrix::print()
{
    std::cout << std::setw(3) << r1.x << ' ';
    std::cout << std::setw(3) << r1.y << ' ';
    std::cout << std::setw(3) << r1.z << ' ';
    std::cout << std::setw(3) << r1.w << std::endl;

    std::cout << std::setw(3) << r2.x << ' ';
    std::cout << std::setw(3) << r2.y << ' ';
    std::cout << std::setw(3) << r2.z << ' ';
    std::cout << std::setw(3) << r2.w << std::endl;

    std::cout << std::setw(3) << r3.x << ' ';
    std::cout << std::setw(3) << r3.y << ' ';
    std::cout << std::setw(3) << r3.z << ' ';
    std::cout << std::setw(3) << r3.w << std::endl;

    std::cout << std::setw(3) << r4.x << ' ';
    std::cout << std::setw(3) << r4.y << ' ';
    std::cout << std::setw(3) << r4.z << ' ';
    std::cout << std::setw(3) << r4.w << std::endl << std::endl;
}

/*
 * Return the identity matrix.
 *
 * @return The identity matrix.
 */
Matrix Matrix::identity()
{
    return Matrix(
        Vector4(1.),
        Vector4(0., 1.),
        Vector4(0., 0., 1.),
        Vector4(0., 0., 0., 1.)
    );
}

/**
 * Calculate a view matrix.
 *
 * @param eye       The position of the camera.
 * @param center    The point to look at.
 * @param up        The up direction.
 *
 * @return The matrix.
 */
Matrix Matrix::look_at(Vector3 eye, Vector3 center, Vector3 up)
{
    Vector3 f = (center - eye).normalise();

    Vector3 s = f.cross(up);

    Vector3 u = s.normalise().cross(f);

    Matrix view_matrix(
        Vector4( s.x,  s.y,  s.z, 0.),
        Vector4( u.x,  u.y,  u.z, 0.),
        Vector4(-f.x, -f.y, -f.z, 0.),
        Vector4(   0.,  0.,   0., 1.)
    );

    view_matrix = Matrix(
        Vector4( 1., 0., 0., -eye.x),
        Vector4( 0., 1., 0., -eye.y),
        Vector4( 0., 0., 1., -eye.z),
        Vector4( 0., 0., 0., 1.)
    ) * view_matrix;

    return view_matrix;
}

/**
 * Calculate an frustum projection matrix.
 *
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 *
 * @return The matrix.
 */
Matrix Matrix::frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
    return Matrix(
        Vector4(2*near / (right - left), 0., (right+left)/(right-left), 0.),
        Vector4(0., 2*near / (top - bottom), (top+bottom)/(top-bottom), 0.),
        Vector4(0., 0., -((far+near)/(far-near)), -((2*far*near)/(far-near))),
        Vector4(0., 0., -1, 0.)
    );
}

/**
 * Calculate an orthographic projection matrix.
 *
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 *
 * @return The matrix.
 */
Matrix Matrix::orthographic(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
    return Matrix(
        Vector4(2 / (right - left), 0., 0., -(right+left)/(right-left)),
        Vector4(0., 2 / (top - bottom), 0., -(top+bottom)/(top-bottom)),
        Vector4(0., 0., -2/(far-near), -((far+near)/(far-near))),
        Vector4(0., 0., 0, 1.)
    );
}
