#pragma once

#include <imgui.h>

#include "../AssetDescriptor.h"
#include "../Renderer/Image.h"
#include "../Insight.h"
#include "../Math.h"
#include "../Uuid.h"


namespace Insight
{
    class Texture;

    class Widgets
    {
    public:
        template <typename T>
        struct ComboOption
        {
            T Value;
            string Label;
        };

        static void SetNextWindowClass(const string& windowClassName, bool tabBar);
        static void FillWindowClass(const string& windowClassName, ImGuiWindowClass* outWindowClass);
        static bool BeginWindow(const string& name, bool* pOpen = nullptr, ImGuiWindowFlags flags = 0);
        static void EndWindow();

        static void InputText(const string& name, string& string, ImGuiInputTextFlags flags = 0,
                              float width = -FLT_MIN);
        static void DragFloat(const std::string& string, f32* p, f32 f32, Insight::f32 min = -FLT_MAX,
                              Insight::f32 max = FLT_MAX);
        static void DragFloat2(const std::string& string, f32* p, f32 f32, Insight::f32 min = -FLT_MAX,
                               Insight::f32 max = FLT_MAX);
        static void DragFloat3(const std::string& string, f32* p, f32 f32, Insight::f32 min = -FLT_MAX,
                               Insight::f32 max = FLT_MAX);
        static void DragFloat4(const std::string& string, f32* p, f32 f32, Insight::f32 min = -FLT_MAX,
                               Insight::f32 max = FLT_MAX);

        static void Image(const Ref<Renderer::Image>& image, const ImVec2& size, const ImVec2& uv0 = {0, 0},
                          const ImVec2& uv1 = {1, 1}, const ImVec4& tint_col = {1, 1, 1, 1},
                          const ImVec4& border_col = {0, 0, 0, 0});
        static void Cubemap(const Ref<Renderer::Image>& image, const ImVec2& size, const ImVec2& uv0 = {0, 0},
                            const ImVec2& uv1 = {1, 1}, const ImVec4& tint_col = {1, 1, 1, 1},
                            const ImVec4& border_col = {0, 0, 0, 0});
        static void CubemapMips(const Ref<Renderer::Image>& image, const ImVec2& size, const ImVec2& uv0 = {0, 0},
                            const ImVec2& uv1 = {1, 1}, const ImVec4& tint_col = {1, 1, 1, 1},
                            const ImVec4& border_col = {0, 0, 0, 0});
        static void ImageCentered(const Ref<Renderer::Image>& image, const ImVec2& center, const ImVec2& size,
                                  const ImVec2& uv0 = {0, 0}, const ImVec2& uv1 = {1, 1},
                                  const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0});
        static void ImageCenteredScreenPos(const Ref<Renderer::Image>& image, const ImVec2& center, const ImVec2& size,
                                           const ImVec2& uv0 = {0, 0}, const ImVec2& uv1 = {1, 1},
                                           const ImVec4& tint_col = {1, 1, 1, 1},
                                           const ImVec4& border_col = {0, 0, 0, 0});

        static bool Button(const string& label, const ImVec2& size = {0, 0});
        static bool SmallButton(const string& label, const ImVec2& size = {0, 0});
        static bool SmallToggleButton(const string& label, bool toggled, const ImVec2& size = {0, 0});

        static bool BeginPropertyTree(const string& name, ImVec2 size = {0, 0});
        static void EndPropertyTree();

        static bool ExpandingPropertyTreeLabel(const string& label);
        static bool PropertyTreeLabel(const string& label, bool selected = false);

        static void PropertyTreeText(const string& label, const string& text);
        static void PropertyTreeDragFloat(const string& label, f32* p, f32 x, f32 min, f32 max);
        static void PropertyTreeDragFloat2(const string& label, f32* p, f32 x, f32 min, f32 max);
        static void PropertyTreeDragFloat3(const string& label, f32* p, f32 x, f32 min, f32 max);
        static void PropertyTreeDragFloat4(const string& label, f32* p, f32 x, f32 min, f32 max);

        static void PropertyTreeColor(const string& label, vec3* color);

        static bool PropertyTreeU32(const string& label, u32& value, u32 step = 1, u32 min = 0, u32 max = UINT32_MAX);

        template <typename T>
        static bool Combo(const string& label, T* value, const ComboOption<T>* options, u32 optionCount)
        {
            bool wasSelected = false;

            u32 selectedIndex = 0;
            for (u32 i = 0; i < optionCount; i++)
            {
                if (*value == options[i].Value)
                {
                    selectedIndex = i;
                    break;
                }
            }

            if (ImGui::BeginCombo(("combo##" + label).c_str(), options[selectedIndex].Label.c_str()))
            {
                for (u32 i = 0; i < optionCount; i++)
                {
                    const bool isSelected = *value == options[i].Value;

                    if (ImGui::Selectable(options[i].Label.c_str(), isSelected))
                    {
                        *value = options[i].Value;
                        wasSelected = true;
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            return wasSelected;
        }

        template <typename T>
        static bool PropertyTreeCombo(const string& label, T& value, const ComboOption<T>* options, u32 optionCount)
        {
            PropertyTreeLabel(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            return Combo<T>(label, &value, options, optionCount);
        }
    };
}
