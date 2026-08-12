#include <imgui.h>
#include <imgui-SFML.h>
#include "hud.hpp"
#include "constants.hpp"
#include <iostream>

float f = 0.f;

void MyGui::RenderGui() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Menu")) {
        if (ImGui::MenuItem("New")) {}
        if (ImGui::MenuItem("Open", "Ctrl+O")) {}
        if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        if (ImGui::MenuItem("Save As..")) {}
        ImGui::Separator();
        if (ImGui::BeginMenu("Options")) {
            ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {}
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("debug tools")) {
        if (ImGui::MenuItem("WALL_COLLISION_TOGGLE")) {
        ENABLE_WALL_COLLISION = !ENABLE_WALL_COLLISION;
        }
        ImGui::EndMenu(); 
    }

    ImGui::EndMainMenuBar();

}