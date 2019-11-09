#pragma once

#include <cmath>
#include <algorithm>

#include "raylib.h"
#include "raymath.h"

namespace blender_like_camera {
	inline Vector3 operator+(Vector3 a, Vector3 b) {
		return Vector3Add(a, b);
	}
	inline Vector3 operator-(Vector3 a, Vector3 b) {
		return Vector3Subtract(a, b);
	}
	inline Vector3 operator*(Vector3 v, float s) {
		return Vector3Multiply(v, s);
	}
	inline Vector3 operator*(float s, Vector3 v) {
		return Vector3Multiply(v, s);
	}

	inline void UpdateCamera(
		Camera *camera,
		float wheel_sensitivity = 0.2f,
		float zoom_mouse_sensitivity = 0.002f,
		float rotate_sensitivity = 0.005f,
		float shift_sensitivity = 0.0007f
	) {
		Vector2 mousePositionDelta = { 0.0f, 0.0f };
		{
			Vector2 mousePosition = GetMousePosition();
			static Vector2 previousMousePosition = { 0.0f, 0.0f };
			mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
			mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
			previousMousePosition = mousePosition;
		}

		bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

		int deltaWheel = GetMouseWheelMove();
		if (deltaWheel != 0) {
			auto target = camera->target;
			auto position = camera->position;

			float d = Vector3Distance(target, position);
			d -= d * deltaWheel * wheel_sensitivity;
			d = std::max(0.1f, d);

			auto dir = Vector3Normalize(position - target);
			position = target + dir * d;
			camera->position = position;
		}
		else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			if (mousePositionDelta.y != 0.0f) {
				auto target = camera->target;
				auto position = camera->position;

				float d = Vector3Distance(target, position);
				d -= d * mousePositionDelta.y * zoom_mouse_sensitivity;
				d = std::max(0.1f, d);

				auto dir = Vector3Normalize(position - target);
				position = target + dir * d;
				camera->position = position;
			}
		}

		if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && shift == false) {
			if (mousePositionDelta.x != 0.0f) {
				auto target = camera->target;
				auto position = camera->position;
				auto up = camera->up;
				auto S = position - target;

				auto q = QuaternionFromAxisAngle(Vector3{ 0, 1, 0 }, -rotate_sensitivity * mousePositionDelta.x);

				S = Vector3RotateByQuaternion(S, q);
				up = Vector3RotateByQuaternion(up, q);

				camera->position = target + S;
				camera->up = up;
			}

			if (mousePositionDelta.y != 0.0f) {
				auto target = camera->target;
				auto position = camera->position;

				auto foward = position - target;

				auto up = camera->up;
				if (Vector3Length(up) <= std::numeric_limits<float>::epsilon()) {
					up = Vector3{ 0, 1, 0 };
				}
				auto right = Vector3Normalize(Vector3CrossProduct(foward, up));
				up = Vector3CrossProduct(right, foward);
				up = Vector3Normalize(up);

				auto S = position - target;
				auto q = QuaternionFromAxisAngle(right, rotate_sensitivity * mousePositionDelta.y);
				S = Vector3RotateByQuaternion(S, q);
				camera->position = target + S;
				camera->up = up;
			}
		}

		if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && shift) {
			auto target = camera->target;
			auto position = camera->position;

			auto foward = position - target;

			auto up = camera->up;
			if (Vector3Length(up) <= std::numeric_limits<float>::epsilon()) {
				up = Vector3{ 0, 1, 0 };
			}
			auto right = Vector3Normalize(Vector3CrossProduct(foward, up));
			up = Vector3CrossProduct(right, foward);
			up = Vector3Normalize(up);

			float d = Vector3Distance(target, position);

			auto move =
				d * up * shift_sensitivity * mousePositionDelta.y +
				d * right * shift_sensitivity * mousePositionDelta.x;

			target = target + move;
			position = position + move;

			camera->target = target;
			camera->position = position;
		}
	}
}