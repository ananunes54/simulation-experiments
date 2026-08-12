#include <cstdlib>

#include <math5d.h>
#include <matrix_exponential.h>

Vec5::Vec5()
{
    for (auto i = 0; i < 5; i++)
        m_data[i] = 0.0f;
}

Vec5::Vec5(float s)
{
    for (auto i = 0; i < 5; i++)
        m_data[i] = s;
}

Vec5::Vec5(float r0, float r1, float r2, float r3, float r4)
{
    m_data[0] = r0;
    m_data[1] = r1;
    m_data[2] = r2;
    m_data[3] = r3;
    m_data[4] = r4;
}

const float* Vec5::value_ptr()
{
    return &m_data[0];
}


float& Vec5::operator[](unsigned int i)
{
    return m_data[i];
}

const float& Vec5::operator[](unsigned int i) const
{
    return m_data[i];
}

Vec5 Vec5::operator*(float scalar) const
{
    return Vec5(m_data[0] * scalar, m_data[1] * scalar, m_data[2] * scalar, m_data[3] * scalar, m_data[4] * scalar);
}


Mat5::Mat5(float diagonal)
{
    for(int col = 0; col < 5; ++col)
    {
        for(int row = 0; row < 5; ++row)
        {
            if (col == row)
            {
                m_columns[col][row] = diagonal;
            }
            
            else
            {
                m_columns[col][row] = 0.0f;
            }
        }
    }
}

Mat5::Mat5(const Vec5& c0, const Vec5& c1, const Vec5& c2, const Vec5& c3, const Vec5& c4)
{
    m_columns[0] = c0;
    m_columns[1] = c1;
    m_columns[2] = c2;
    m_columns[3] = c3;
    m_columns[4] = c4;
}

Vec5& Mat5::operator[](unsigned int i)
{
    return m_columns[i];
}

const Vec5& Mat5::operator[](unsigned int i) const
{
    return m_columns[i];
}

const float* Mat5::value_ptr()
{
    return &m_columns[0][0];
}

Mat5 Mat5::operator*(float scalar) const
{
    return Mat5(m_columns[0] * scalar, m_columns[1] * scalar, m_columns[2] * scalar, m_columns[3] * scalar, m_columns[4] * scalar);
}

Vec5 Mat5::operator*(const Vec5& vec) const
{
    Vec5 result;
    for (auto j = 0; j < 5; j++)
    {
        for (auto i = 0; i < 5; i++)
        {
            result[i] += m_columns[j][i] * vec[j];
        }
    }

    return result;
}

Mat5 Mat5::operator*(const Mat5& other) const
{
    return Mat5((*this) * other.m_columns[0],
                (*this) * other.m_columns[1],
                (*this) * other.m_columns[2],
                (*this) * other.m_columns[3],
                (*this) * other.m_columns[4]);    
}

Mat5 Mat5::exp() const
{
    double buffer[25];
    for (auto j = 0; j < 5; j++)
    {
        for (auto i = 0; i < 5; i++)
        {
            buffer[j*5 + i] = static_cast<double>(m_columns[j][i]);
        }
    }

    double* expPtr = r8mat_expm1(5, buffer);
    Mat5 result;
    for (auto j = 0; j < 5; j++)
    {
        for (auto i = 0; i < 5; i++)
        {
            result[j][i] = static_cast<float>(expPtr[j*5 + i]);
        }
    }

    free(expPtr);
    return result;
}
