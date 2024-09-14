#include "Widgets.h"

#include <imgui_internal.h>

#include "../Renderer/Command.h"

namespace Insight
{
    static const Widgets::ComboOption<i32> s_CubeFaceOptions[] = {
        {0, "+X"},
        {1, "-X"},
        {2, "+Y"},
        {3, "-Y"},
        {4, "+Z"},
        {5, "-Z"},
    };

    static constexpr u32 s_CubeFaceOptionCount = sizeof(s_CubeFaceOptions) / sizeof(Widgets::ComboOption<
        i32>);

    static const Widgets::ComboOption<i32> s_MipOptions[] = {
        {0, "0"},
        {1, "1"},
        {2, "2"},
        {3, "3"},
        {4, "4"},
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
        {9, "9"},
        {10, "10"},
        {11, "11"},
        {12, "12"},
        {13, "13"},
        {14, "14"},
        {15, "15"},
        {16, "16"},
        {17, "17"},
        {18, "18"},
        {19, "19"},
        {20, "20"},
        {21, "21"},
        {22, "22"},
        {23, "23"},
        {24, "24"},
        {25, "25"},
        {26, "26"},
        {27, "27"},
        {28, "28"},
        {29, "29"},
        {30, "30"},
        {31, "31"},
    };

    static constexpr u32 s_MipOptionCount = sizeof(s_MipOptions) / sizeof(Widgets::ComboOption<
        i32>);

    void Widgets::SetNextWindowClass(const string& windowClassName, bool tabBar)
    {
        ImGuiWindowClass windowClass;
        FillWindowClass(windowClassName, &windowClass);
        if (!tabBar)
        {
            windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;
        }

        ImGui::SetNextWindowClass(&windowClass);
    }

    void Widgets::FillWindowClass(const string& windowClassName, ImGuiWindowClass* outWindowClass)
    {
        outWindowClass->ClassId = ImGui::GetID(windowClassName.c_str());
        outWindowClass->DockingAllowUnclassed = false;
    }

    bool Widgets::BeginWindow(const string& name, bool* pOpen, ImGuiWindowFlags flags)
    {
        return ImGui::Begin(name.c_str(), pOpen, flags);
    }

    void Widgets::EndWindow()
    {
        ImGui::End();
    }

    void Widgets::InputText(const string& name, string& buffer, const ImGuiInputTextFlags flags, const float width)
    {
        ImGui::PushItemWidth(width);

        static char text[64];
        strcpy(text, buffer.c_str());

        if (ImGui::InputText(name.c_str(), text, IM_ARRAYSIZE(text), flags))
        {
            buffer = text;
        }
    }

    void Widgets::DragFloat(const std::string& string, f32* p, const f32 f32, const Insight::f32 min,
                            const Insight::f32 max)
    {
        ImGui::DragFloat(string.c_str(), p, f32, min, max);
    }

    void Widgets::DragFloat2(const std::string& string, f32* p, const f32 f32, const Insight::f32 min,
                             const Insight::f32 max)
    {
        ImGui::DragFloat2(string.c_str(), p, f32, min, max);
    }

    void Widgets::DragFloat3(const std::string& string, f32* p, const f32 f32, const Insight::f32 min,
                             const Insight::f32 max)
    {
        ImGui::DragFloat3(string.c_str(), p, f32, min, max);
    }

    void Widgets::DragFloat4(const std::string& string, f32* p, const f32 f32, const Insight::f32 min,
                             const Insight::f32 max)
    {
        ImGui::DragFloat4(string.c_str(), p, f32, min, max);
    }

    void Widgets::Image(const Ref<Renderer::Image>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
                        const ImVec4& tint_col, const ImVec4& border_col)
    {
        Renderer::Command::SubmitResource(image);
        ImGui::Image(image->GetImTextureId(), size, uv0, uv1, tint_col, border_col);
    }

    void Widgets::Cubemap(const Ref<Renderer::Image>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
                          const ImVec4& tint_col, const ImVec4& border_col)
    {
        Renderer::Command::SubmitResource(image);

        ImGuiStorage* storage = ImGui::GetStateStorage();
        ImGuiID faceId = ImGui::GetID("CubeFace");
        i32 face = storage->GetInt(faceId);

        if (Combo("Face", &face, s_CubeFaceOptions, s_CubeFaceOptionCount))
        {
            storage->SetInt(faceId, face);
        }

        ImGui::Image(image->GetCubemapFaceImTextureId(face), size, uv0, uv1, tint_col, border_col);
    }

    void Widgets::CubemapMips(const Ref<Renderer::Image>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
                          const ImVec4& tint_col, const ImVec4& border_col)
    {
        Renderer::Command::SubmitResource(image);

        ImGuiStorage* storage = ImGui::GetStateStorage();
        ImGuiID faceId = ImGui::GetID("CubeFace");
        i32 face = storage->GetInt(faceId);
        ImGuiID mipId = ImGui::GetID("CubeMip");
        i32 mip = storage->GetInt(mipId);

        if (Combo("Face", &face, s_CubeFaceOptions, s_CubeFaceOptionCount))
        {
            storage->SetInt(faceId, face);
        }

        if (Combo("Mip", &mip, s_MipOptions, image->GetMipLevels()))
        {
            storage->SetInt(mipId, mip);
        }

        ImGui::Image(image->GetCubemapFaceMipImTextureId(face, mip), size, uv0, uv1, tint_col, border_col);
    }

    void Widgets::ImageCentered(const Ref<Renderer::Image>& image, const ImVec2& center, const ImVec2& size,
                                const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
    {
        Renderer::Command::SubmitResource(image);
        const ImVec2 position = {center.x - size.x / 2, center.y - size.y / 2};
        ImGui::SetCursorPos(position);
        ImGui::Image(image->GetImTextureId(), size, uv0, uv1, tint_col, border_col);
    }

    void Widgets::ImageCenteredScreenPos(const Ref<Renderer::Image>& image, const ImVec2& center, const ImVec2& size,
                                         const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col,
                                         const ImVec4& border_col)
    {
        Renderer::Command::SubmitResource(image);
        const ImVec2 position = {center.x - size.x / 2, center.y - size.y / 2};
        ImGui::SetCursorScreenPos(position);
        ImGui::Image(image->GetImTextureId(), size, uv0, uv1, tint_col, border_col);
    }

    bool Widgets::Button(const string& label, const ImVec2& size)
    {
        return ImGui::Button(label.c_str(), size);
    }

    bool Widgets::SmallButton(const string& label, const ImVec2& size)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
        bool clicked = ImGui::ButtonEx(label.c_str(), size, ImGuiButtonFlags_AlignTextBaseLine);
        ImGui::PopStyleVar();
        return clicked;
    }

    bool Widgets::SmallToggleButton(const string& label, const bool toggled, const ImVec2& size)
    {
        bool clicked = false;

        if (toggled)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            clicked = SmallButton(label, size);
            ImGui::PopStyleColor();
        }
        else
        {
            clicked = SmallButton(label, size);
        }

        return clicked;
    }

    bool Widgets::BeginPropertyTree(const string& name, ImVec2 size)
    {
        bool open = ImGui::BeginTable(name.c_str(), 2,
                                      ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable |
                                      ImGuiTableFlags_ScrollY, size);

        if (open)
        {
            ImGui::TableSetupColumn("Object");
            ImGui::TableSetupColumn("Contents");
            // ImGui::TableHeadersRow();
        }

        return open;
    }

    void Widgets::EndPropertyTree()
    {
        ImGui::EndTable();
    }

    bool Widgets::ExpandingPropertyTreeLabel(const string& label)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        return ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    }

    bool Widgets::PropertyTreeLabel(const string& label, const bool selected)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        if (selected)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::TreeNodeEx(label.c_str(), flags);

        return ImGui::IsItemClicked();
    }

    void Widgets::PropertyTreeText(const string& label, const string& text)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        // ImGui::SetNextItemWidth(-FLT_MIN);

        ImGui::Text("%s", text.c_str());
    }

    void Widgets::PropertyTreeDragFloat(const string& label, f32* p, const f32 x, const f32 min, const f32 max)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);

        DragFloat("##" + label, p, x, min, max);
    }

    void Widgets::PropertyTreeDragFloat2(const string& label, f32* p, const f32 x, const f32 min, const f32 max)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);

        DragFloat2("##" + label, p, x, min, max);
    }

    void Widgets::PropertyTreeDragFloat3(const string& label, f32* p, const f32 x, const f32 min, const f32 max)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);

        DragFloat3("##" + label, p, x, min, max);
    }

    void Widgets::PropertyTreeDragFloat4(const string& label, f32* p, f32 x, f32 min, f32 max)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);

        DragFloat4("##" + label, p, x, min, max);
    }

    void Widgets::PropertyTreeColor(const string& label, vec3* color)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);

        ImGui::ColorEdit3(("##" + label).c_str(), &color->x);
    }

    bool Widgets::PropertyTreeU32(const string& label, u32& value, const u32 step, const u32 min, const u32 max)
    {
        PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        bool changed = false;

        if (ImGui::InputScalar(label.c_str(), ImGuiDataType_U32, &value, &step, &step, "%d"))
        {
            changed = true;
        }

        value = std::clamp(value, min, max);

        return changed;
    }
}
