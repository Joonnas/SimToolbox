#ifndef ANDROMEDA_MATH_H
#define ANDROMEDA_MATH_H

#include <math.h>
#include <iostream>

namespace aMath
{

    static const float PI = 3.1415;
    static const float TAU = 6.283;

    struct Vec2
    {
        float x;
        float y;
        Vec2(float x, float y) : x(x), y(y) {}
        Vec2()
        {
            x = 0.0f;
            y = 0.0f;
        }
        float length() { return sqrt(x*x + y*y); }
        void negate()
        {
            x = -x;
            y = -y;
        }
        void scale(float magnitude) {
            float old_mag = length();
            if (old_mag == 0.0) return;
            float factor = magnitude / old_mag;
            x *= factor;
            y *= factor;
        }
    };
    inline Vec2 operator+(const Vec2& a, const Vec2& b) { return { a.x + b.x, a.y + b.y }; }
    inline Vec2 operator+(const Vec2& a, float b) { return { a.x + b, a.y + b }; }
    inline Vec2 operator+(float a, const Vec2& b) { return { a + b.x, a + b.x }; }
    inline Vec2 operator-(const Vec2& a, const Vec2& b) { return { a.x - b.x, a.y - b.y }; }
    inline Vec2 operator-(const Vec2& a, float b) { return { a.x - b, a.y - b }; }
    inline Vec2 operator-(float a, const Vec2& b) { return { a - b.x, a - b.y }; }
    inline Vec2 operator*(const Vec2& a, const Vec2& b) { return { a.x * b.x, a.y * b.y }; }
    inline Vec2 operator*(const Vec2& a, float b) { return { a.x * b, a.y * b }; }
    inline Vec2 operator*(float a, const Vec2& b) { return { a * b.x, a * b.y }; }
    inline Vec2 operator/(const Vec2& a, const Vec2& b) { return { a.x / b.x, a.y / b.y }; }
    inline Vec2 operator/(const Vec2& a, float b) { return { a.x / b, a.y / b }; }
    inline Vec2 operator/(float a, const Vec2& b) { return { a / b.x, a / b.y }; }
    inline bool operator!=(const Vec2& a, const Vec2& b) { return !(a.x == b.x && a.y == b.y); }

    struct Vec3
    {
        float x;
        float y;
        float z;
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vec3()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
        float length_squared() {
            float a = x*x + y*y + z*z;
            return (a > 0) ? a : -a; 
        }
        float length() { return sqrt(length_squared()); }
        void negate()
        {
            x = -x;
            y = -y;
            z = -z;
        }
        void scale(float magnitude) {
            float old_mag = length();
            if (old_mag == 0.0) return;
            float factor = magnitude / old_mag;
            x *= factor;
            y *= factor;
            z *= factor;
        }
        void normalize() {scale(1.0);}
    };
    inline Vec3 operator+(const Vec3& a, const Vec3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
    inline Vec3 operator+(const Vec3& a, float b) { return { a.x + b, a.y + b, a.z + b }; }
    inline Vec3 operator+(float a, const Vec3& b) { return { a + b.x, a + b.y, a + b.z }; }
    inline Vec3 operator-(const Vec3& a, const Vec3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
    inline Vec3 operator-(const Vec3& a, float b) { return { a.x - b, a.y - b, a.z - b }; }
    inline Vec3 operator-(float a, const Vec3& b) { return { a - b.x, a - b.y, a - b.z }; }
    inline Vec3 operator*(const Vec3& a, const Vec3& b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
    inline Vec3 operator*(const Vec3& a, float b) { return { a.x * b, a.y * b, a.z * b }; }
    inline Vec3 operator*(float a, const Vec3& b) { return { a * b.x, a * b.y, a * b.z }; }
    inline Vec3 operator/(const Vec3& a, const Vec3& b) { return { a.x / b.x, a.y / b.y, a.z / b.y }; }
    inline Vec3 operator/(const Vec3& a, float b) { return { a.x / b, a.y / b, a.z / b }; }
    inline Vec3 operator/(float a, const Vec3& b) { return { a / b.x, a / b.y, a / b.z }; }
    inline bool operator!=(const Vec3& a, const Vec3& b) { return !(a.x == b.x && a.y == b.y && a.z == b.z); }


    struct Vec4
    {
        float x;
        float y;
        float z;
        float w;
        Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vec4()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            w = 0.0f;
        }
        float length() { return sqrt(x*x + y*y + z*z + w*w); }
        void negate()
        {
            x = -x;
            y = -y;
            z = -z;
            w = -w;
        }
    };
    inline Vec4 operator+(const Vec4& a, const Vec4& b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
    inline Vec4 operator+(const Vec4& a, float b) { return { a.x + b, a.y + b, a.z + b, a.w + b }; }
    inline Vec4 operator+(float a, const Vec4& b) { return { a + b.x, a + b.y, a + b.z, a + b.w }; }
    inline Vec4 operator-(const Vec4& a, const Vec4& b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
    inline Vec4 operator-(const Vec4& a, float b) { return { a.x - b, a.y - b, a.z - b, a.w - b }; }
    inline Vec4 operator-(float a, const Vec4& b) { return { a - b.x, a - b.y, a - b.z, a - b.w }; }
    inline Vec4 operator*(const Vec4& a, const Vec4& b) { return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }
    inline Vec4 operator*(const Vec4& a, float b) { return { a.x * b, a.y * b, a.z * b, a.w * b }; }
    inline Vec4 operator*(float a, const Vec4& b) { return { a * b.x, a * b.y, a * b.z, a * b.w }; }
    inline Vec4 operator/(const Vec4& a, const Vec4& b) { return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }
    inline Vec4 operator/(const Vec4& a, float b) { return { a.x / b, a.y / b, a.z / b, a.w / b }; }
    inline Vec4 operator/(float a, const Vec4& b) { return { a / b.x, a / b.y, a / b.z, a / b.w }; }
    inline bool operator!=(const Vec4& a, const Vec4& b) { return !(a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }


    struct Mat3
    {
        static const int translateX = 2;
        static const int translateY = 5;
        static const int scaleX = 0;
        static const int scaleY = 4;
        static const int rotX1 = 0;
        static const int rotX2 = 1;
        static const int rotY1 = 3;
        static const int rotY2 = 4;
        float values[3 * 3] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f };
        Mat3() {}
        Mat3(float values[3 * 3]) : values() {}
        void translate(const Vec2& a)
        {
            values[translateX] = a.x;
            values[translateY] = a.y;
        }
        void scale(const Vec2& a)
        {
            values[scaleX] = a.x;
            values[scaleY] = a.y;
        }
        void rotate(float angle)
        {
            values[rotX1] = cos(angle);
            values[rotX2] = -sin(angle);
            values[rotY1] = sin(angle);
            values[rotY2] = cos(angle);
        }
    };

    struct Mat4
    {
        static const int translateX = 3;
        static const int translateY = 7;
        static const int translateZ = 11;
        static const int scaleX = 0;
        static const int scaleY = 5;
        static const int scaleZ = 10;

        static const int rot00 = 0;
        static const int rot01 = 1;
        static const int rot02 = 2;
        static const int rot10 = 4;
        static const int rot11 = 5;
        static const int rot12 = 6;
        static const int rot20 = 8;
        static const int rot21 = 9;
        static const int rot22 = 10;

        float values[4 * 4] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f };
        Mat4() {}
        Mat4(float values[4 * 4]) : values() {}
        void translate(const Vec3& a)
        {
            values[translateX] += a.x;
            values[translateY] += a.y;
            values[translateZ] += a.z;
        }
        void scale(const Vec3& a)
        {
            values[scaleX] = a.x;
            values[scaleY] = a.y;
            values[scaleZ] = a.z;
        }
        void rotateX(float angle) {
            values[rot11] = cos(angle);
            values[rot12] = -sin(angle);
            values[rot21] = sin(angle);
            values[rot22] = cos(angle);
        }
        void rotateY(float angle) {
            values[rot00] = cos(angle);
            values[rot02] = sin(angle);
            values[rot20] = -sin(angle);
            values[rot22] = cos(angle);
        }
        void rotateZ(float angle) {
            values[rot00] = cos(angle);
            values[rot01] = -sin(angle);
            values[rot10] = sin(angle);
            values[rot11] = cos(angle);
        }
        void printValues()
        {
            for (int i = 0; i < sizeof(values) / sizeof(float); i++)
            {
                std::cout << values[i] << std::endl;
            }
        }
    };

    typedef Vec2 Point2;
    typedef Vec3 Point3;

    typedef Vec3 Color_RGB;
    typedef Vec4 Color_RGBA;

    /*
     *
     * Math Functions
     *
     */

    void identity(Mat3& matrix);
    void identity(Mat4& matrix);

    // Vec2 operations
    Vec2 add(const Vec2& a, const Vec2& b);
    Vec2 subtract(const Vec2& a, const Vec2& b);
    Vec2 mul(const Vec2& a, const Vec2& b);
    Vec2 mul(float a, const Vec2& b);
    float dot(const Vec2& a, const Vec2& b);

    // Vec3 operations
    Vec3 add(const Vec3& a, const Vec3& b);
    Vec3 subtract(const Vec3& a, const Vec3& b);
    Vec3 mul(const Vec3& a, const Vec3& b);
    Vec3 mul(float a, const Vec3& b);
    Vec3 cross(const Vec3& a, const Vec3& b);
    float dot(const Vec3& a, const Vec3& b);

    // Vec4 operations
    Vec4 add(const Vec4& a, const Vec4& b);
    Vec4 subtract(const Vec4& a, const Vec4& b);
    Vec4 mul(const Vec4& a, const Vec4& b);
    Vec4 mul(float a, const Vec4& b);
    float dot(const Vec4& a, const Vec4& b);

    // Matrix * Vector and Vector * Matrix
    Vec3 mul(const Mat3& m, const Vec3& v);
    Vec3 mul(const Vec3& v, const Mat3& m);
    Vec4 mul(const Mat4& m, const Vec4& v);
    Vec4 mul(const Vec4& v, const Mat4& m);

    // Skalar * Matrix
    Mat3 mul(float a, const Mat3& m);
    Mat4 mul(float a, const Mat4& m);

    // Matrix * Matrix
    Mat3 mul(const Mat3& a, const Mat3& b);
    Mat4 mul(const Mat4& a, const Mat4& b);

    // Matrix addition and subtraction
    Mat3 add(const Mat3& a, const Mat3& b);
    Mat3 subtract(const Mat3& a, const Mat3& b);
    Mat4 add(const Mat4& a, const Mat4& b);
    Mat4 subtract(const Mat4& a, const Mat4& b);

    Vec4 multiply(const Mat4& a, const Vec4& b);
    Mat4 multiply(const Mat4& a, const Mat4& b);

    // Transformation matrices
    // Translation
    Mat4 translate(float x, float y, float z);
    Mat4 translate(aMath::Vec3& pos);

    // Scale
    Mat4 scale(float x, float y, float z);
    Mat4 scale(aMath::Vec3& s);

    //Rotate
    Mat4 rotateX(float angle);
    Mat4 rotateY(float angle);
    Mat4 rotateZ(float angle);

    // Orthographic matrix
    Mat4 orthographic(float left, float right, float top, float bottom, float near, float far);

    // TODO
    /*

    Translate

    Perspective Matrix
    Rotate Matrix
    Move Matrix

    */

   /*
   ROTATION
   */

    // Vec2 rotate(const Vec2& a, float angle);

    /*
    BEZIER CURVES
    */

    // 2D Bezier curves

    // Point2 calculate2DBezier_linear(Point2 p1, Point2 p2, float t) {
    //     Point2 res = { 0.0f, 0.0f };
    //     if (t >= 0.0f && t <= 1.0f) res = ((1.0 - t) * p1) + (t * p2);
    //     return res;
    // }

    // Point2 calculate2DBezier_quadratic(Point2 p1, Point2 p2, Point2 p3, float t) {
    //     Point2 res = { 0.0f, 0.0f };
    //     if (t >= 0.0f && t <= 1.0f)	res = p2 + ((1.0 - t) * (1.0 - t) * (p1 - p2)) + (t * t * (p3 - p2));
    //     return res;
    // }

    // Point2 calculate2DBezier_cubic(Point2 p1, Point2 p2, Point2 p3, Point2 p4, float t) {
    //     Point2 res = { 0.0f, 0.0f };
    //     if (t >= 0.0f && t <= 1.0f)	res = ((1.0 - t) * (1.0 - t) * (1.0 - t) * p1) + (3.0 * (1.0 - t) * (1.0 - t) * t * p2) + (3.0 * (1.0 - t) * t * t * p3) + (t * t * t * p4);
    //     return res;
    // }

    // // 3D Bezier curves
    
    // Point3 calculate3DBezier_linear(Point3 p1, Point3 p2, float t) {
    //     Point3 res = { 0.0f, 0.0f, 0.0f };
    //     if (t >= 0.0f && t <= 1.0f) res = ((1.0 - t) * p1) + (t * p2);
    //     return res;
    // }

    // Point3 calculate3DBezier_quadratic(Point3 p1, Point3 p2, Point3 p3, float t) {
    //     Point3 res = { 0.0f, 0.0f, 0.0f };
    //     if (t >= 0.0f && t <= 1.0f)	res = p2 + ((1.0 - t) * (1.0 - t) * (p1 - p2)) + (t * t * (p3 - p2));
    //     return res;
    // }

    // Point3 calculate3DBezier_cubic(Point3 p1, Point3 p2, Point3 p3, Point3 p4, float t) {
    //     Point3 res = { 0.0f, 0.0f, 0.0f };
    //     if (t >= 0.0f && t <= 1.0f)	res = ((1.0 - t) * (1.0 - t) * (1.0 - t) * p1) + (3.0 * (1.0 - t) * (1.0 - t) * t * p2) + (3.0 * (1.0 - t) * t * t * p3) + (t * t * t * p4);
    //     return res;
    // }

    // TODO
    /*
    
    Add Error Handling (if t is outside the bounds)

    */

}

#endif