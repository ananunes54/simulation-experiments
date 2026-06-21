#include <material.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Material::setFloat(std::string& name, float value)
{
    m_uniforms[name] = value;
}

void Material::setGlmMat3(std::string& name, glm::mat3 value)
{
    m_uniforms[name] = value;
}

void Material::setGlmMat4(std::string& name, glm::mat4 value)
{
    m_uniforms[name] = value;
}

void Material::setGlmVec3(std::string& name, glm::vec3 value)
{
    m_uniforms[name] = value;
}

void Material::setGlmVec4(std::string& name, glm::vec4 value)
{
    m_uniforms[name] = value;
}

void Material::bind()
{
    unsigned int shaderID = m_shader.getID();
    glUseProgram(shaderID);

    for (const auto& [name, value] : m_uniforms)
    {
        int uLocation = glGetUniformLocation(shaderID, name.c_str());
        switch (value.index()) {
            case 0:
                {
                    glUniform1i(uLocation, std::get<0>(value));
                    break;
                }
            case 1:
                {
                    glUniform1f(uLocation, std::get<1>(value));
                    break;
                }
            case 2:
                {
                    glm::vec3 val = std::get<2>(value);
                    glUniform3fv(uLocation, 1, glm::value_ptr(val));
                    break;
                }
            case 3:
                {
                    glm::vec4 val = std::get<3>(value);
                    glUniform4fv(uLocation, 1, glm::value_ptr(val));
                    break;
                }
            case 4:
                {
                    glm::mat3 val = std::get<4>(value);
                    glUniformMatrix3fv(uLocation, 1, GL_FALSE, glm::value_ptr(val));
                    break;
                }
            case 5:
                {
                    glm::mat4 val = std::get<5>(value);
                    glUniformMatrix4fv(uLocation, 1, GL_FALSE, glm::value_ptr(val));
                    GLenum err = glGetError();
                    if (err != GL_NO_ERROR)
                        std::cout << "erro " << err << " em material.bind, " << name << std::endl;
                    break;
                }
        }
    }
}

void Material::unbind()
{
    glUseProgram(0);
}

