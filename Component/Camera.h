#pragma once
#include "pch.h"
#include "../Component/MonoBehaviour.h"
#include "../CoreModule/MathStructs.h"
#include <imgui/imgui.h>

namespace KritiaEngine {
	class Camera : public Behaviour{
	public:
		Camera(GameObject* gameObject);
		Matrix4x4 GetViewMatrix();
		Vector3 GetPosition();
		/// <summary>
		/// fov in y direction in degree
		/// </summary>
		float Fovy;
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
		float Speed;
		float MouseSensitivity;

		float Yaw;
		float Pitch;
		void EditorMove();
		void EditorProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
		void EditorProcessMouseScroll(float yoffset);
		// calculates the front vector from the Camera's (updated) Euler Angles
		void EditorUpdateCameraVectors();
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual void OnInspector() override;
		virtual std::string GetInspectorLabel() override;
		Vector3 Position;
		Vector3 Up;
		Vector3 Right;
		Vector3 Forward;
		Vector3 Target;
		Vector3 WorldUp;
		std::string inspectorLabel = "Camera";
	};


}


