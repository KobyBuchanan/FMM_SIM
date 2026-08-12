#include <imgui.h>
#include <imgui-SFML.h>
#include "hud.hpp"
#include "utils/debug_tools.hpp"
#include <iostream>



void MyGui::RenderGui(sf::RenderWindow& window, const Quadtree& tree, sf::FloatRect worldBounds) {
    //local variables
    float f = 0.f;
    static bool render_quadtree = false;
    //menus
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
            wall_collision_toggle();
        }
        if (ImGui::MenuItem("DRAW_QUADTREE_TOGGLE")) {
            render_quadtree = !render_quadtree;
        }

        ImGui::EndMenu(); 
    }

    ImGui::EndMainMenuBar();
    //render over windo not gui
    if (render_quadtree) {
        drawQuadtree(window, tree, tree.root, worldBounds);
    }
}

