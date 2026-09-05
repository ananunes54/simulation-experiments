#ifndef MATH5D_INCLUDED_H
#define MATH5D_INCLUDED_H

#include <glm/glm.hpp>

class Vec5
{
    private:
        float m_data[5];

    public:
        Vec5();
        Vec5(float s);
        Vec5(float r0, float r1, float r2, float r3, float r4);

        float& operator[](unsigned int i);
        const float& operator[](unsigned int i) const;
        Vec5 operator*(float scalar) const;
        bool operator==(const Vec5& other) const;
        bool operator!=(const Vec5& other) const;

        const float* value_ptr();
        glm::vec4 truncate() const;
};

class Mat5
{
    private:
        Vec5 m_columns[5];

    public:
        Mat5(float diagonal = 1.0f);
        Mat5(const Vec5& c0, const Vec5& c1, const Vec5& c2, const Vec5& c3, const Vec5& c4);

        Vec5& operator[](unsigned int i);
        const Vec5& operator[](unsigned int i) const;
        Mat5 operator*(float scalar) const;
        Vec5 operator*(const Vec5& vec) const;
        Mat5 operator*(const Mat5& other) const;
        bool operator==(const Mat5& other) const;
        bool operator!=(const Mat5& other) const;

        const float* value_ptr();
        Mat5 exp() const;
        glm::mat4 truncate() const;
};

namespace math5 {
    bool compare(const Vec5& v1, const Vec5& v2, float error);
    bool compare(const Mat5& m1, const Mat5& m2, float error);
}

#endif
