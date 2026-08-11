#include <math5d.h>

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
