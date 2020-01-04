#ifndef GUI_MAIN_PANEL_H
#define GUI_MAIN_PANEL_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "CameraFreeLook.h"
#include "Light.h"
#include "STLight.h"
#include "STMaterial.h"

namespace GUI {
	void init(SDL_Window* &window, SDL_GLContext &context) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void begin(SDL_Window* &window) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
	};

	void render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	};

	void mainPanel(CameraFreeLook &cameraFreeLook, Light& light, STLight &stLight, STMaterial &stMaterial) {
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

		ImGui::Separator();
		ImGui::Text("Light Position");
		ImGui::SliderFloat("Light X", &light.position.x, -32.0f, 32.0f);
		ImGui::SliderFloat("Light Y", &light.position.y, -32.0f, 32.0f);
		ImGui::SliderFloat("Light Z", &light.position.z, -32.0f, 32.0f);

		ImGui::Separator();
		ImGui::Text("Shininess");
		ImGui::SliderFloat("Amount", &stMaterial.shininess, 0.0f, 64.0f);

		ImGui::Separator();
		ImGui::Text("Material Specular");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SliderFloat("MatSpec X", &stMaterial.specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat("MatSpec Y", &stMaterial.specular.y, 0.0f, 1.0f);
		ImGui::SliderFloat("MatSpec Z", &stMaterial.specular.z, 0.0f, 1.0f);

		ImGui::Separator();
		ImGui::Text("Light Specular");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SliderFloat("LightSpec X", &stLight.specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat("LightSpec Y", &stLight.specular.y, 0.0f, 1.0f);
		ImGui::SliderFloat("LightSpec Z", &stLight.specular.z, 0.0f, 1.0f);

		ImGui::PopStyleVar();
		ImGui::End();
	};

	void shutdown() {
		ImGui_ImplSDL2_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}
}

#endif