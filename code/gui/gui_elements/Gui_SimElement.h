#ifndef ST_GUI_SIMELEMENT_H
#define ST_GUI_SIMELEMENT_H

#include "ext/include/imgui/imgui.h"
#include "ext/include/aMath/a_math.h"

#include "Utils/properties.h"

// Custom Textbox
struct Funcs
{
    static int MyResizeCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            ImVector<char>* my_str = (ImVector<char>*)data->UserData;
            IM_ASSERT(my_str->begin() == data->Buf);
            my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
            data->Buf = my_str->begin();
        }
        return 0;
    }

    // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
    // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
    static bool CustomInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
    }
};

class Gui_SimElement
{
private:

protected:
    bool is_visible = true;
    std::string name;

    Property<aMath::Vec3> position = aMath::Vec3();
    Property<aMath::Vec4> color = aMath::Vec4(1.0, 1.0, 1.0, 1.0);

protected:
    void render_float_input_2(const char* title, bool show_xy, Property<aMath::Vec2> &value);
    void render_float_input_3(const char* title, bool show_xyz, Property<aMath::Vec3> &value);
    void render_float_input_4(const char* title, bool show_xyzw, Property<aMath::Vec4> &value);

public:

    virtual void update(float deltaTime) = 0;
    virtual void renderGui() = 0;
    virtual void render() = 0;

    bool should_delete() {return !is_visible;};

    std::string get_name() {return name;};

};

#endif