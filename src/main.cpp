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
#include "raylib.h"

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_VSYNC_HINT);
	
	InitWindow(screenWidth, screenHeight, "");

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = Vector3 { 10.0f, 10.0f, 10.0f };  // Camera position
	camera.target = Vector3 { 0, 0, 0};      // Camera looking at point
	camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FREE);

	// SetTargetFPS(60); 

	// Main game loop
	while (!WindowShouldClose()) 
	{
		UpdateCamera(&camera);

		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D(camera);

		DrawGrid(10, 1.0f);

		DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, 1, 1, 1, WHITE);
		DrawCubeWires(Vector3{ 0.0f, 0.0f, 0.0f }, 1, 1, 1, BLACK);

		EndMode3D();

		DrawFPS(10, 10);

		EndDrawing();
	}

    CloseWindow();

	return 0;
}