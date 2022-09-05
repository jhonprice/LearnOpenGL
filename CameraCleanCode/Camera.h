#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
//GLM ͷ�ļ�
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//<<�����������>>
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//<<���Ĭ�ϲ���>>
#pragma region CameraDefaultInfo
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;
#pragma endregion CameraDefaultInfo




class Camera {
public:
//<<�����Ϣ>>
#pragma region CameraInfo
	//�����λ���뷽��
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Up;
	glm::vec3 Right;
	//�����ٶ�
	float MovementSpeed = SPEED;
	float MouseSensitivity = SENSITIVITY;
	//���ŷ����
	//float lastX = width / 2, lastY = height / 2;
	float Yaw = YAW;
	float Pitch = PITCH;
	//fov
	float Zoom = FOV;
#pragma endregion CameraInfo


//<<������캯��>>
#pragma region buildCamera
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)):Pos(position) {
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(FOV)
	{
		Pos = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Pos, Pos + Front, Up);
	}
#pragma endregion buildCamera

	//<<���Ƽ���>>
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Pos += Front * velocity;
		if (direction == BACKWARD)
			Pos -= Front * velocity;
		if (direction == LEFT)
			Pos -= Right * velocity;
		if (direction == RIGHT)
			Pos += Right * velocity;
		if (direction == UP)
			Pos += WorldUp * velocity;
			
		if (direction == DOWN)
			Pos -= WorldUp * velocity;

	}
	//<<�������>>
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		//ȷ��λ��
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// ���Ʒ�Χ
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		//ͨ���µ�ŷ�����ع��������ϵ
		updateCameraVectors();
	}
	//<<���ƹ���>>
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
private:
	//<<ͨ��ŷ���Ǽ����������ϵ>>
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));  
		Up = glm::normalize(glm::cross(Right, Front));
	}

};


class CameraUtils {
public:
	Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f) };
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	//���ŷ����
	float lastX, lastY;
	int width, height;
	bool firstMouse = true;

	CameraUtils(int width, int height) :lastX(width / 2), lastY(height / 2), width(width), height(height) {}

	void wsad(GLFWwindow* window) {
		float cameraSpeed = 2.5f * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
	}
	void mouseMove(double xpos, double ypos) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		//ȷ��λ��
		float xoffset = xpos - lastX;

		//����y�������Ǵӵ׵�����
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	void mouseRoll(double yoffset) {
		camera.ProcessMouseScroll(yoffset);
	}
	void deltaUpdate(float currentFrame) {
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	glm::mat4 getPerpective() {
		auto fovRadians = glm::radians(camera.Zoom);
		auto aspect = width * 1.f / height;
		auto nearPlane = 0.1f, farPlane = 100.0f;
		glm::mat4 projection;
		projection = glm::perspective(fovRadians, aspect, nearPlane, farPlane);
		return projection;
	}


};