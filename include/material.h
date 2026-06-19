#ifndef MATERIAL_INCLUDED_H
#define MATERIAL_INCLUDED_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <variant>

#include <shaders.h>

using UniformType = std::variant<int, float, glm::vec3, glm::vec4, glm::mat3, glm::mat4>;
class Material
{
    private:
        Shader& m_shader;
        std::unordered_map<std::string, UniformType> m_uniforms;

    public:
        Material(Shader& shader) : m_shader(shader) {};
        void setFloat(std::string& name, float value);
        void setGlmMat3(std::string& name, glm::mat3 value);
        void setGlmMat4(std::string& name, glm::mat4 value);
        void setGlmVec3(std::string& name, glm::vec3 value);
        void setGlmVec4(std::string& name, glm::vec4 value);

        void bind();
        void unbind();
};

#endif
