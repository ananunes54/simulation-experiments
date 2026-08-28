#include <gui.h>
#include <imgui.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

bool UIWidget::drawVec3Control(const char* label, glm::vec3& target, const glm::vec2& minAndMax, const char* precision, const glm::vec3& resetValues)
{
    bool wasAltered = false;

    std::string resetLabel("Reset##");
    resetLabel.append(label);

    if (ImGui::Button(resetLabel.c_str()))
    {
        wasAltered = true;
        target = resetValues;
    }

    ImGui::SameLine();

    if (ImGui::SliderFloat3(label, glm::value_ptr(target), minAndMax[0], minAndMax[1], precision))
    {
        wasAltered = true;
    }

    return wasAltered;
}


bool UIWidget::drawFloatControl(const char* label, float& target, const glm::vec2& minAndMax, const char* precision, float resetValue)
{
    bool wasAltered = false;

    std::string resetLabel("Reset##");
    resetLabel.append(label);

    if (ImGui::Button(resetLabel.c_str()))
    {
        wasAltered = true;
        target = resetValue;
    }

    ImGui::SameLine();

    if (ImGui::SliderFloat(label, &target, minAndMax[0], minAndMax[1], precision))
    {
        wasAltered = true;
    }

    return wasAltered;
}

bool UIWidget::drawButton(const char* label)
{
    bool wasAltered = false;

    if (ImGui::Button(label))
    {
        wasAltered = true;
    }

    return wasAltered;
}


bool UIWidget::drawDualButton(const char* labelTrue, const char* labelFalse, bool value)
{
    bool wasAltered = false;

    if (ImGui::Button(value ? labelTrue : labelFalse))
    {
        wasAltered = true;
    }

    return wasAltered;
}
