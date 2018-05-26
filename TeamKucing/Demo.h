#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO2, VAO2, EBO2, texture2, VBO3, VAO3, EBO3, texture3;
	GLuint VBOKucingB, VAOKucingB, EBOKucingB, textureKucingB;
	GLuint VBOKucingP, VAOKucingP, EBOKucingP, textureKucingP;
	GLuint VBOKucingE, VAOKucingE, EBOKucingE, textureKucingE;
	GLuint VBOKucingW, VAOKucingW, EBOKucingW, textureKucingW;
	GLuint VBORumput, VAORumput, EBORumput, textureRumput;
	GLuint VAOWiskas, VBOWiskas, EBOWiskas, textureWiskas;
	GLuint VAOTembokRumah, VBOTembokRumah, EBOTembokRumah, textureTembokRumah;
	GLuint VAOAtapRumah, VBOAtapRumah, EBOAtapRumah, textureAtapRumah;
	GLuint VAOKotakMakan, VBOKotakMakan, EBOKotakMakan, textureKotakRumah;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);

	void BuildKucingB(), BuildKucingP(), BuildKucingE(), BuildKucingW();
	void DrawKucingB(), DrawKucingP(), DrawKucingE(), DrawKucingW();
	void BuildRumput();
	void DrawRumput();
	void BuildTembokRumah();
	void DrawTembokRumah();
	void BuildAtapRumah();
	void DrawAtapRumah();
	void BuildKotakMakan();
	void DrawKotakMakan();
	void BuildWiskas();
	void DrawWiskas();
	void MoveCamera(float speed);
	void MoveCat(char i, float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
	bool lockCamera;
	float x, y, z, OBJECT_SPEED;
	float angle = 0;
};

