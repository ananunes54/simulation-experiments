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
        std::unordered_map<const char*, UniformType> m_uniforms;

    public:
        Material(Shader& shader) : m_shader(shader) {};
        void setFloat(const char* name, float value);
        void setGlmMat3(const char* name, glm::mat3 value);
        void setGlmMat4(const char* name, glm::mat4 value);
        void setGlmVec3(const char* name, glm::vec3 value);
        void setGlmVec4(const char* name, glm::vec4 value);

        void bind();
        void unbind();
};

#endif
