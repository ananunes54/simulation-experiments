#include <material.h>
#include <glm/gtc/type_ptr.hpp>
#include <utils.h>

void Material::setFloat(const char* name, float value)
{
    m_uniforms[name] = value;
}

void Material::setGlmMat3(const char* name, glm::mat3 value)
{
    m_uniforms[name] = value;
}

void Material::setGlmMat4(const char* name, glm::mat4 value)
{
    m_uniforms[name] = value;
}

void Material::setGlmVec3(const char* name, glm::vec3 value)
{
    m_uniforms[name] = value;
}

void Material::setGlmVec4(const char* name, glm::vec4 value)
{
    m_uniforms[name] = value;
}

void Material::bind()
{
    unsigned int shaderID = m_shader.getID();
    GLCall(glUseProgram(shaderID));

    for (const auto& [name, value] : m_uniforms)
    {
        int uLocation = glGetUniformLocation(shaderID, name);
        switch (value.index()) {
            case 0:
                {
                    GLCall(glUniform1i(uLocation, std::get<0>(value)));
                    break;
                }
            case 1:
                {
                    GLCall(glUniform1f(uLocation, std::get<1>(value)));
                    break;
                }
            case 2:
                {
                    glm::vec3 val = std::get<2>(value);
                    GLCall(glUniform3fv(uLocation, 1, glm::value_ptr(val)));
                    break;
                }
            case 3:
                {
                    glm::vec4 val = std::get<3>(value);
                    GLCall(glUniform4fv(uLocation, 1, glm::value_ptr(val)));
                    break;
                }
            case 4:
                {
                    glm::mat3 val = std::get<4>(value);
                    GLCall(glUniformMatrix3fv(uLocation, 1, GL_FALSE, glm::value_ptr(val)));
                    break;
                }
            case 5:
                {
                    glm::mat4 val = std::get<5>(value);
                    GLCall(glUniformMatrix4fv(uLocation, 1, GL_FALSE, glm::value_ptr(val)));
                    break;
                }
        }
    }
}

void Material::unbind()
{
    GLCall(glUseProgram(0));
}

