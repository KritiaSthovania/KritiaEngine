#pragma once
#include "pch.h"
#include "../Component/MonoBehaviour.h"
#include "../CoreModule/MathStructs.h"

namespace KritiaEngine {
	class Camera : public Behaviour{
	public:
		Camera(GameObject* gameObject);
		Matrix4x4 GetViewMatrix();
		Vector3 GetPosition();
		float Speed;
		float MouseSensitivity;
		float Zoom;
		float Yaw;
		float Pitch;
		// Current Rendering Camera
		static std::shared_ptr<Camera> current;
		static std::shared_ptr<Camera> editorCamera;

		// Editor-Only functions

        /// <summary>
        /// only used in Editor to create an editor camera
        /// </summary>
		Camera();
		/// <summary>
		/// Only used in Editor to update the editor camera
		/// </summary>
		void EditorCameraUpdate();

	private:
		// Editor-Only funcions
		void Move();
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
		void ProcessMouseScroll(float yoffset);
		// calculates the front vector from the Camera's (updated) Euler Angles
		void UpdateCameraVectors();
		Vector3 Position;
		Vector3 Up;
		Vector3 Right;
		Vector3 Forward;
		Vector3 Target;
		Vector3 WorldUp;
	};


}


