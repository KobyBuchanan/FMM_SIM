#include <imgui.h>
#include <imgui-SFML.h>
#include "hud.hpp"
#include "utils/debug_tools.hpp"
#include <iostream>



void MyGui::RenderGui(sf::RenderWindow& window,std::vector<Object>& objects, const Quadtree& tree, sf::FloatRect worldBounds, float frameTime) {
    //local variables
    float f = 0.f;
    static bool render_quadtree = false;
    static bool render_confirmation_popUp = false;
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
        if (ImGui::MenuItem("PARTICLE_COLLISION_TOGGLE")) {
            particle_collision_toggle();
        }
        if (ImGui::MenuItem("DRAW_QUADTREE_TOGGLE")) {
            render_quadtree = !render_quadtree;
        }
    
        if(ImGui::MenuItem("CLEAR_OBJECTS")) {
            render_confirmation_popUp = true;
        }

        ImGui::Separator();
        if (ImGui::BeginMenu("Gravity Slider")) {
            ImGui::SliderFloat("Value", &G, 0.0f, 10000.0f);
            ImGui::EndMenu();
        }
        
        ImGui::EndMenu(); 
    }

    //FPS Counter
    ImGui::Text("FPS: %.1f", 1.f / frameTime);

    ImGui::EndMainMenuBar();

    //render over window not gui
    if (render_quadtree) {
        drawQuadtree(window, tree, tree.root, worldBounds);
    }
    if (render_confirmation_popUp) {
        ImGui::OpenPopup("Confirm Deletion");
        render_confirmation_popUp = false;
    }
    
    //pop up logic
    if(ImGui::BeginPopupModal("Confirm Deletion", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete this file?\nThis action cannot be undone.\n\n");
        if (ImGui::Button("Yes, Delete", ImVec2(120, 0))) {
            clearObjects(objects);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}




