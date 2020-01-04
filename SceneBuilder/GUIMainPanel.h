#ifndef GUI_MAIN_PANEL_H
#define GUI_MAIN_PANEL_H

#include <SDL2/SDL.h>
#include "imgui/imgui.h"
#include "CameraFreeLook.h"

namespace GUI {
	void begin(SDL_Window* window) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
	};

	void render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	};

	void mainPanel(CameraFreeLook &cameraFreeLook) {
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		ImGui::SetNextWindowContentSize(ImVec2(300, 900));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
		ImGui::Begin("Information", NULL, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SetWindowPos(ImVec2(0, 0), true);
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Text("Camera");
		ImGui::Text("Position %.3f, %.3f, %.3f", cameraFreeLook.getCameraPosition().x, cameraFreeLook.getCameraPosition().y, cameraFreeLook.getCameraPosition().z);
		ImGui::Text("Yaw %.3f", cameraFreeLook.yaw);
		ImGui::Text("Pitch %.3f", cameraFreeLook.pitch);
		ImGui::PopStyleVar();
		ImGui::End();
	};
}

#endif