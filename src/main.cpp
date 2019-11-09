/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera mode
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <cmath>
#include <algorithm>
#include <vector>
#include <cctype>

#include "raylib.h"
#include "blenderlikecamera.hpp"
#include "imguiintegration.hpp"

inline Color GetColorFromRGB(float r, float g, float b, float a) {
	return {
		(unsigned char)(Clamp(r * 256.0f, 0, 255)),
		(unsigned char)(Clamp(g * 256.0f, 0, 255)),
		(unsigned char)(Clamp(b * 256.0f, 0, 255)),
		(unsigned char)(Clamp(a * 256.0f, 0, 255))
	};
}

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "");

	SetTraceLogLevel(LOG_WARNING);

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = Vector3 { 10.0f, 10.0f, 10.0f };  // Camera position
	camera.target = Vector3 { 0, 0, 0};                 // Camera looking at point
	camera.up = Vector3 { 0.0f, 1.0f, 0.0f };           // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FREE);

	// SetTargetFPS(10); 
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2;
	io.IniFilename = nullptr;

	imgui_integration::Initialize();

	// Image
	Texture2D lenna = LoadTexture("../resources/lenna.png");

	Image whitenoise;
	whitenoise.data = (unsigned char *)RL_MALLOC(128 * 128 * sizeof(Color));
	{
		Color *data = (Color *)whitenoise.data;
		for (int i = 0; i < 128 * 128; ++i) {
			data[i].r = GetRandomValue(0, 255);
			data[i].g = GetRandomValue(0, 255);
			data[i].b = GetRandomValue(0, 255);
			data[i].a = 255;
		}
	}
	whitenoise.width = 128;
	whitenoise.height = 128;
	whitenoise.mipmaps = 1;
	whitenoise.format = UNCOMPRESSED_R8G8B8A8;
	Texture2D noise = LoadTextureFromImage(whitenoise);
	UnloadImage(whitenoise);

	// Main game loop
	while (!WindowShouldClose()) 
	{
		if (imgui_integration::IsImGuiUsingMouse() == false) {
			blender_like_camera::UpdateCamera(&camera);
		}

		static float cube_w = 1.0f;
		static float cube_h = 1.0f;
		static float cube_d = 1.0f;
		static float cube_color[] = {1, 1, 1};

		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D(camera);

		DrawGrid(10, 1.0f);
		
		DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, cube_w, cube_h, cube_d, GetColorFromRGB(cube_color[0], cube_color[1], cube_color[2], 1.0f));
		DrawCubeWires(Vector3{ 0.0f, 0.0f, 0.0f }, cube_w, cube_h, cube_d, BLACK);

		EndMode3D();

		DrawFPS(10, 10);

		imgui_integration::BeginImGui();

		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::SetNextWindowSize({ 500, 800 }, ImGuiCond_Once);
		ImGui::Begin("Cube");

		ImGui::Text("fps = %d", GetFPS());
		ImGui::SliderFloat("cube w", &cube_w, 0.0f, 5.0f);
		ImGui::SliderFloat("cube h", &cube_h, 0.0f, 5.0f);
		ImGui::SliderFloat("cube d", &cube_d, 0.0f, 5.0f);

		ImGui::ColorPicker3("cube color", cube_color);

		ImGui::End();

		ImGui::Begin("Image");
		ImGui::Image((ImTextureID)&noise, ImVec2 { (float)noise.width, (float)noise.height });
		ImGui::Image((ImTextureID)&lenna, ImVec2{ (float)lenna.width, (float)lenna.height });
		ImGui::End();

		imgui_integration::EndImGui();

		EndDrawing();
	}

    CloseWindow();

	return 0;
}