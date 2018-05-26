#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);
	


	lockCamera = false;
	x = 0, y = 0, z = 0;
	OBJECT_SPEED = 0.01f;

	BuildKucingB();
	BuildKucingP();
	BuildKucingE();
	BuildKucingW();
	BuildRumput();
	BuildAtapRumah();
	BuildTembokRumah();
	BuildWiskas();
	BuildKotakMakan();
	BuildKotakMakan();
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOKucingB);
	glDeleteBuffers(1, &VBOKucingB);
	glDeleteBuffers(1, &EBOKucingB);

	glDeleteVertexArrays(1, &VAOKucingP);
	glDeleteBuffers(1, &VBOKucingP);
	glDeleteBuffers(1, &EBOKucingP);

	glDeleteVertexArrays(1, &VAOKucingE);
	glDeleteBuffers(1, &VBOKucingE);
	glDeleteBuffers(1, &EBOKucingE);

	glDeleteVertexArrays(1, &VAOKucingW);
	glDeleteBuffers(1, &VBOKucingW);
	glDeleteBuffers(1, &EBOKucingW);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		
		Init();

	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		
		if (lockCamera)
		{
			MoveCamera(CAMERA_SPEED);
		}
		MoveCat('z', OBJECT_SPEED);

	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		
		if (lockCamera)
		{
			MoveCamera(-CAMERA_SPEED);
		}
		MoveCat('z', -OBJECT_SPEED);
		
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		
		if (lockCamera)
		{
			MoveCamera(CAMERA_SPEED);
		}
		MoveCat('x', OBJECT_SPEED);
		
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		
		if (lockCamera)
		{
			MoveCamera(-CAMERA_SPEED);
		}
		MoveCat('x', -OBJECT_SPEED);
		
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (!lockCamera)
		{
			MoveCamera(CAMERA_SPEED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (!lockCamera)
		{
			MoveCamera(-CAMERA_SPEED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (!lockCamera)
		{
			StrafeCamera(-CAMERA_SPEED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (!lockCamera)
		{
			StrafeCamera(CAMERA_SPEED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		//kodingan buat lock object
		if (lockCamera)
		{
			lockCamera = false;
		}
		else {
			lockCamera = true;
		}
	}

	if (!lockCamera)
	{
		// update camera rotation
		// ----------------------
		double mouseX, mouseY;
		double midX = screenWidth / 2;
		double midY = screenHeight / 2;
		float angleY = 0.0f;
		float angleZ = 0.0f;

		// Get mouse position
		glfwGetCursorPos(window, &mouseX, &mouseY);
		if ((mouseX == midX) && (mouseY == midY)) {
			return;
		}

		// Set mouse position
		glfwSetCursorPos(window, midX, midY);

		// Get the direction from the mouse cursor, set a resonable maneuvering speed
		angleY = (float)((midX - mouseX)) / 1000;
		angleZ = (float)((midY - mouseY)) / 1000;

		// The higher the value is the faster the camera looks around.
		viewCamY += angleZ * 2;

		// limit the rotation around the x-axis
		if ((viewCamY - posCamY) > 8) {
			viewCamY = posCamY + 8;
		}
		if ((viewCamY - posCamY) < -8) {
			viewCamY = posCamY - 8;
		}
		RotateCamera(-angleY);
	}
		
}

void Demo::Update(double deltaTime) {
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.26f, 0.89f, 0.89f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	
	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//// Pass perspective projection matrix
	//glm::mat4 projectionDiam = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	//GLint projLocDiam = glGetUniformLocation(this->shaderProgram, "projectionDiam");
	//glUniformMatrix4fv(projLocDiam, 1, GL_FALSE, glm::value_ptr(projectionDiam));

	//// LookAt camera (position, target/direction, up)
	//glm::mat4 viewDiam = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	//GLint viewLocDiam = glGetUniformLocation(this->shaderProgram, "viewDiam");
	//glUniformMatrix4fv(viewLocDiam, 1, GL_FALSE, glm::value_ptr(viewDiam));

	// set lighting attribute
	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 1, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 2, 3);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	DrawKucingB();
	DrawKucingE();
	DrawKucingP();
	DrawKucingW();
	DrawAtapRumah();
	DrawTembokRumah();
	DrawKotakMakan();
	DrawWiskas();
	DrawRumput();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildKucingB() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKucingB);
	glBindTexture(GL_TEXTURE_2D, textureKucingB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("catexture.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//badan depan
		x-0.6, y+2, z+0.8 , 0.5 , 0.5, //0
		x-1.2, y+0  , z+0.8 , 0 , 0, //1
		x-0   , y+0  , z+0.8 , 1 , 0, //2
		//badan belakang
		x-0.6, y+2, z+0.1 , 0.5 , 0.5, //3
		x-1.2, y+0  , z+0.1 , 0 , 0, //4
		x-0   , y+0  , z+0.1 , 1 , 0, //5
	};

	unsigned int indices[] = {
		0, 1, 2, //badanDepan
		3, 4, 5, //badanBelakang
		5, 3, 2, 2, 0, 3, //badanKanan
		3, 4, 0, 0, 4, 1, //badanKiri
		1, 2, 4, 4, 2, 5, //badanBawah
	};

	glGenVertexArrays(1, &VAOKucingB);
	glGenBuffers(1, &VBOKucingB);
	glGenBuffers(1, &EBOKucingB);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOKucingB);

	glBindBuffer(GL_ARRAY_BUFFER, VBOKucingB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKucingB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKucingB()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKucingB);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOKucingB); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, 0, z));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);


	glDrawElements(GL_TRIANGLES, 25, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKucingP() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKucingP);
	glBindTexture(GL_TEXTURE_2D, textureKucingP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("catexture.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//wajahDepan
		x - 1.2, y + 2   , z + 0.9, 0, 0, //0
		x - 1.2, y + 1.4 , z + 0.9, 0, 0, //1
		x - 0   , y + 1.4 , z + 0.9, 1, 1, //2
		x - 0   , y + 2  , z + 0.9, 1 ,0,   //3

		//wajahBelakang
		x - 1.2, y + 2   , z + 0, 0, 1, //4
		x - 1.2, y + 1.4 , z + 0, 0, 0, //5
		x - 0   , y + 1.4 , z + 0, 1, 0, //6
		x - 0   , y + 2  , z + 0, 1 ,0,  //7

		//dagu
		x - 0.6, y + 1.2, z + 0.9,1,0, //8
		x - 0.6, y + 1.2, z + 0, 0.5, 0.5, //9

		//telingaKiri
		x - 0.9  , y + 2 , z + 0.9,0,0, //10
		x - 1 , y + 2.3 , z + 0.9,0,0, //11
		x - 0.9  , y + 2 , z + 0.5,0,0, //12
		x - 1 , y + 2.3 , z + 0.5,0,0, //13
		x - 1.2 , y + 2 , z + 0.5,0,0, //14

		//telingaKanan
		x - 0.3 , y + 2   , z + 0.9,0,0, //15
		x - 0.2 , y + 2.3 , z + 0.9,0,0, //16
		x - 0.3 , y + 2   , z + 0.5,0,0, //17
		x - 0.2 , y + 2.3 , z + 0.5,0,0,  //18
		x - 0   , y + 2   , z + 0.5,0,0, //19
	};

	unsigned int indices[] = {
		//0,3,1,1,3,2, //Wajahdepan
		4,5,7,7,5,6, //WajahBelakang
		0,1,5,0,5,4, //WajahKiri
		7,6,3,3,6,2, //WajahKanan
		8,1,2, 9,5,6, //Dagu
		6,9,2,2,9,8, //DaguKanan
		5,9,1,1,9,8, //DaguKanan
		0,4,3,3,4,7, //Rambut
		0,11,10,14,12,13, //TelingaKiri
		3,15,16,17,18,19, //TelingaKanan
		19,16,3,3,19,18,
		18,17,16,16,17,15, //belakangTelingaKanan
		13,12,11,11,12,10,
		14,13,0,0,13,11, //
	};

	glGenVertexArrays(1, &VAOKucingP);
	glGenBuffers(1, &VBOKucingP);
	glGenBuffers(1, &EBOKucingP);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOKucingP);

	glBindBuffer(GL_ARRAY_BUFFER, VBOKucingP);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKucingP);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKucingP()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKucingP);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(textureKucingP); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, 0, z));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);

	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKucingE() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKucingE);
	glBindTexture(GL_TEXTURE_2D, textureKucingE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("badan.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//ekor
		x - 0.4 , y + 0 , z + 0.2 , 1,1, //0
		x - 0.8 , y + 0 , z + 0.2 , 1,0, //1
		x - 0.4 , y + 0 , z - 0.05 , 0,0, //2
		x - 0.8 , y + 0 , z - 0.05 , 0,0, //3
		x - 0.6 , y + 2, z - 0.3, 0,1, //4
	};

	unsigned int indices[] = {
		1, 0, 3, 3, 0, 2,
		2, 3, 4, 4, 3, 1,
		1, 0, 4, 4, 0, 2, //ekorFull
	};

	glGenVertexArrays(1, &VAOKucingE);
	glGenBuffers(1, &VBOKucingE);
	glGenBuffers(1, &EBOKucingE);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOKucingE);

	glBindBuffer(GL_ARRAY_BUFFER, VBOKucingE);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKucingE);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKucingE()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKucingE);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOKucingE); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, 0, z));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);

	glDrawElements(GL_TRIANGLES, 16, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKucingW() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKucingW);
	glBindTexture(GL_TEXTURE_2D, textureKucingW);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("catface.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		//wajah
		x - 1.2, y + 2   , z + 0.9, 0, 1, //0
		x - 1.2, y + 1.4 , z + 0.9, 0, 0, //1
		x - 0   , y + 1.4 , z + 0.9, 1, 0, //2
		x - 0   , y + 2  , z + 0.9, 1 ,1,   //3
	};

	unsigned int indices[] = {
			0,1,3,3,1,2//Wajah
		};

	glGenVertexArrays(1, &VAOKucingW);
	glGenBuffers(1, &VBOKucingW);
	glGenBuffers(1, &EBOKucingW);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOKucingW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOKucingW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKucingW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKucingW()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKucingW);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOKucingW); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, 0, z));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);

	glDrawElements(GL_TRIANGLES, 10, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildRumput()
{
	// Load and create a texture 
	glGenTextures(1, &textureRumput);
	glBindTexture(GL_TEXTURE_2D, textureRumput);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("grass.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-25.0, -0.5, -25.0,  0,  0,
		 25.0, -0.5, -25.0, 25,  0,
		 25.0, -0.5,  25.0, 25, 25,
		-25.0, -0.5,  25.0,  0, 25,
		
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAORumput);
	glGenBuffers(1, &VBORumput);
	glGenBuffers(1, &EBORumput);

	glBindVertexArray(VAORumput);

	glBindBuffer(GL_ARRAY_BUFFER, VBORumput);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORumput);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawRumput()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureRumput);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAORumput); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	//nyoba lesson 5
	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//akhir nyoba

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTembokRumah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureTembokRumah);
	glBindTexture(GL_TEXTURE_2D, textureTembokRumah);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wall.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		//TembokKiri
		x - 2.6, y + 0   , z - 0.2, 0, 0, //0
		x - 2, y + 0 , z - 0.2, 1, 0, //1
		x - 2   , y + 2.4 , z - 0.2, 1, 1, //2
		x - 2.6   , y + 2.4  , z - 0.2, 0 ,1,   //3
		x - 2.6   , y + 2.4  , z - 5, 1 ,1,   //4
		x - 2.6   , y + 0  , z - 5, 1 ,0,   //5
		x - 2   , y + 0  , z - 5, 0 ,0,   //6
		x - 2   , y + 2.4  , z - 5, 0 ,1,   //7

		//TembokKanan
		x + 0.8, y + 0   , z - 0.2, 0, 0, //8
		x + 1.4, y + 0 , z - 0.2, 1, 0, //9
		x + 0.8, y + 2.4 , z - 0.2, 0, 1, //10
		x + 1.4, y + 2.4  , z - 0.2, 1 ,1,   //11
		x + 1.4, y + 2.4  , z - 5, 0 ,1,   //12
		x + 1.4, y + 0  , z - 5, 0 ,0,   //13
		x + 0.8 , y + 0  , z - 5, 1 ,0,   //14
		x + 0.8   , y + 2.4  , z - 5, 1 ,1   //15
	};

	unsigned int indices[] = {
		//TembokKiri
		0,1,3,3,1,2,
		2,1,7,7,1,6,
		6,5,7,7,5,4,
		4,3,5,5,3,0,

		//TembokKanan
		8,9,10,10,9,11,
		11,9,12,12,9,13,
		13,14,12,12,14,15,
		15,10,14,14,10,8,

		//TembokBelakang
		7,15,6,6,15,14

	};

	glGenVertexArrays(1, &VAOTembokRumah);
	glGenBuffers(1, &VBOTembokRumah);
	glGenBuffers(1, &EBOTembokRumah);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOTembokRumah);

	glBindBuffer(GL_ARRAY_BUFFER, VBOTembokRumah);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTembokRumah);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembokRumah()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTembokRumah);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOTembokRumah); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildAtapRumah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureAtapRumah);
	glBindTexture(GL_TEXTURE_2D, textureAtapRumah);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("atap2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		//Atap
		x - 2.9, y + 2.4   , z - 0.2, 0, 0, //0
		x + 1.7, y + 2.4, z - 0.2, 1, 0, //1
		x - 0.6   , y + 3.6 , z - 2.4, 0.5, 1, //2
		x - 2.9   , y + 2.4  , z - 5, 1 ,0,   //3
		x + 1.7   , y + 2.4  , z - 5, 0 ,0,   //4
	};

	unsigned int indices[] = {
		//Atap
		0,1,2,
		2,1,4,
		4,2,3,
		3,2,0
	};

	glGenVertexArrays(1, &VAOAtapRumah);
	glGenBuffers(1, &VBOAtapRumah);
	glGenBuffers(1, &EBOAtapRumah);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOAtapRumah);

	glBindBuffer(GL_ARRAY_BUFFER, VBOAtapRumah);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOAtapRumah);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawAtapRumah()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtapRumah);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOAtapRumah); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.25f);


	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKotakMakan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKotakRumah);
	glBindTexture(GL_TEXTURE_2D, textureKotakRumah);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("bowl.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		//KotakMakanBelakang
		x + 1.0, y + 0   , z + 0.5, 0, 0, //0
		x + 2.2, y + 0   , z + 0.5, 1, 0, //1
		x + 1.0, y + 0.4 , z + 0.5, 0, 1, //2
		x + 2.2, y + 0.4 , z + 0.5, 1 ,1, //3
		x + 2.2, y + 0.4 , z + 0.3, 0 ,1, //4
		x + 2.2, y + 0   , z + 0.3, 0, 0, //5
		x + 1.0, y + 0   , z + 0.3, 1, 0, //6
		x + 1.0, y + 0.4 , z + 0.3, 1 ,1, //7
		x + 2.2, y + 0   , z + 1.5, 1, 0, //8
		x + 2.4, y + 0   , z + 1.5, 0, 0, //9
		x + 2.2, y + 0.4 , z + 1.5, 1, 1, //10
		x + 2.4, y + 0.4 , z + 1.5, 0 ,1, //11
		x + 2.4, y + 0.4 , z + 0.3, 1 ,1, //12
		x + 2.4, y + 0   , z + 0.3, 1, 0, //13
		x + 0.8, y + 0   , z + 1.5, 1, 0, //14
		x + 1.0, y + 0   , z + 1.5, 0, 0, //15
		x + 0.8, y + 0.4 , z + 1.5, 1, 1, //16
		x + 1.0, y + 0.4 , z + 1.5, 0, 1, //17
		x + 0.8, y + 0.4 , z + 0.3, 0, 1, //18
		x + 0.8, y + 0   , z + 0.3, 0, 0, //19
		x + 1.0, y + 0.4 , z + 1.3, 1, 1, //20
		x + 2.2, y + 0.4 , z + 1.3, 0, 1, //21
		x + 2.2, y + 0   , z + 1.3, 0, 0, //22
		x + 1.0, y + 0   , z + 1.3, 1, 0 //23
	};

	unsigned int indices[] = {
		//KubusBelakang
		0,1,2,2,1,3,
		3,1,4,4,1,5,
		5,6,4,4,6,7,
		7,6,2,2,6,0,
		2,3,7,7,3,4,

		//KubusKanan
		8,9,10,10,9,11,
		11,9,12,12,9,13,
		13,5,12,12,5,4,
		4,5,10,10,5,8,
		10,11,4,4,11,12,

		//KubusKiri
		14,15,16,16,15,17,
		17,15,7,7,15,6,
		6,19,7,7,19,18,
		18,19,16,16,19,14,
		16,17,18,18,17,7,

		//KubusDepan
		15,8,17,17,8,10,
		10,8,21,21,8,22,
		22,23,21,21,23,20,
		20,23,17,17,23,15,
		17,10,20,20,10,21,

		//AlasBawah
		1,0,22,22,0,23
	};

	glGenVertexArrays(1, &VAOKotakMakan);
	glGenBuffers(1, &VBOKotakMakan);
	glGenBuffers(1, &EBOKotakMakan);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOKotakMakan);

	glBindBuffer(GL_ARRAY_BUFFER, VBOKotakMakan);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKotakMakan);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKotakMakan()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKotakRumah);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOKotakMakan); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 1.0f, 1.0f, 1.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 1.0f, 1.0f, 1.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 1.00f);


	glDrawElements(GL_TRIANGLES, 200, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildWiskas() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("muka.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		//WiskasDepan
		x + 2.0, y + 0   , z + 0.1, 0, 0, //0
		x + 3.2, y + 0, z + 0.1, 1, 0, //1
		x + 3.2   , y + 1.5 , z + 0.1, 1, 1, //2
		x + 2.0   , y + 1.5  , z + 0.1,0 ,1,   //3

		//WiskasBelakang
		x + 2.0, y + 0   , z - 0.6, 1, 0, //4
		x + 3.2, y + 0, z - 0.6, 0, 0, //5
		x + 3.2   , y + 1.5 , z - 0.6, 0, 1, //6
		x + 2.0   , y + 1.5  , z - 0.6,1 ,1,   //7

		//WiskasAtas
		x + 2.0   , y + 1.5  , z + 0.1,0 ,0,   //8
		x + 3.2   , y + 1.5 , z + 0.1, 1, 0, //9
		x + 3.2   , y + 1.5 , z - 0.6, 1, 1, //10
		x + 2.0   , y + 1.5  , z - 0.6,0 ,1   //11


	};

	unsigned int indices[] = {
		0,1,3,3,1,2,
		2,6,1,1,6,5,
		5,4,6,6,4,7,
		7,3,4,4,3,0,
		8,9,10,10,9,11

	};

	glGenVertexArrays(1, &VAOWiskas);
	glGenBuffers(1, &VBOWiskas);
	glGenBuffers(1, &EBOWiskas);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOWiskas);

	glBindBuffer(GL_ARRAY_BUFFER, VBOWiskas);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWiskas);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawWiskas()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOWiskas); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	GLint ambientMatLoc = glGetUniformLocation(this->shaderProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 1.0f, 1.0f, 1.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->shaderProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 1.0f, 1.0f, 1.0f);
	GLint specularMatLoc = glGetUniformLocation(this->shaderProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.75f);


	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 4.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 2.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 2.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.0011f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Demo::MoveCamera(float speed)
{
	float cx = viewCamX - posCamX;
	float cz = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + cx * speed;
	posCamZ = posCamZ + cz * speed;
	viewCamX = viewCamX + cx * speed;
	viewCamZ = viewCamZ + cz * speed;
}

void Demo::MoveCat(char i, float speed)
{
	// forward positive cameraspeed and backward negative -cameraspeed.
	if (i == 'x')
	{
		x += speed;
	}
	else {
		z += speed;
	}
	
}

void Demo::StrafeCamera(float speed)
{
	float cx = viewCamX - posCamX;
	float cz = viewCamZ - posCamZ;
	float orthoX = -cz;
	float orthoZ = cx;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float cx = viewCamX - posCamX;
	float cz = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * cx + glm::cos(speed) * cz);
	viewCamX = (float)(posCamX + glm::cos(speed) * cx - glm::sin(speed) * cz);
}


int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Kucing Bahagia", 1080, 720, false, true);
}