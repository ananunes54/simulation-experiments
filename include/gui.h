#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H

#include <string>
#include <glm/glm.hpp>

namespace UIWidget
{
    bool drawVec3Control(const char* label, glm::vec3& target, const glm::vec2& minAndMax, const char* precision, const glm::vec3& resetValues);

    bool drawFloatControl(const char* label, float& target, const glm::vec2& minAndMax, const char* precision, float resetValue);

    bool drawButton(const char* label);

    bool drawDualButton(const char* labelTrue, const char* labelFalse, bool value);
};

#endif
