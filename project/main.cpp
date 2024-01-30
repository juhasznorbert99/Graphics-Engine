//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include <time.h>
#include <windows.h>
#include <time.h>
#include <iostream>
#include "SkyBox.hpp"

int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

std::vector<const GLchar*> faces;
std::vector<const GLchar*> night;
glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightPos;
GLuint lightPosLoc;
glm::vec4 felinarPos;
GLuint felinarPosLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

GLuint textureID;

//MINE
//GLuint shadowMapFBO;
//GLuint depthMapTexture;

//END MINE
gps::Camera myCamera(
	glm::vec3(0.0f, 2.0f, 5.5f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 1.5f;

bool pressedKeys[1024];
float angleY = 0.0f;
float yaw = 0.0f;
float pitch = 0.0f;
GLfloat lightAngle;



GLint darkness = 0;
GLint lightning = 0;
GLuint darknessLoc;
GLuint lightningLoc;
GLuint lightningLoc2;
GLuint darknessLoc2;


typedef struct sphere {
	glm::vec3 center;
	float radius;
}Sphere;
gps::Model3D nanosuit;
gps::Model3D nanosuit2;
gps::Model3D spaceship;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D frunzaModel;
gps::Model3D darkMagicianModel;
gps::Model3D park;
gps::Model3D scene;
gps::Model3D rainDrop;
gps::Model3D felinar;
gps::Model3D felinarBec;
gps::Model3D felinarStalp;
gps::Model3D Dorugoramon;
gps::Model3D Tsuchidarumon;
gps::Model3D RizeGreymon;
gps::Model3D Courage;
gps::Model3D CourageMetal;
gps::Model3D Dodomon;
gps::Model3D DigimonLogo;
gps::Model3D MetalGarurumon;
gps::Model3D wanyamon;
gps::Model3D jyarimon;
gps::Model3D Swing;
gps::Model3D Sand;
gps::Model3D Horse;
gps::Model3D fireBall;
gps::Model3D ChaosBlackWarGreymon;
gps::Model3D natura;
Sphere darkMagicianSphere;
Sphere felinarS;
Sphere ryzerGreymonS;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;
gps::Shader frunzaShader;
gps::Shader felinarShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;


gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

bool showDepthMap;
bool raining = false;
GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}
Sphere computeSphere(glm::vec3 objectInstance, float height) {
	Sphere s;
	s.radius = height / 2;
	s.center = glm::vec3(objectInstance.x, (objectInstance.y * 2 + height) / 2, objectInstance.z);
	return s;
}
float computeHeight(gps::Model3D m) {
	std::vector<gps::Mesh> m1 = m.getMashes();
	float maxx = -9999.f;
	float minn = 9999.f;
	for (int i = 0; i < m1.size(); i++) {
		for (int j = 0; j < m1[i].vertices.size(); j++) {
			glm::vec3 current = m1[i].vertices[j].Position;
			if (current.y > maxx) {
				maxx = current.y;
			}
			else if (current.y < minn) {
				minn = current.y;
			}
		}
	}
	return (maxx - minn);
}
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}
bool begin = true;
float xpos0, ypos0, deltax, deltay;

float notAllowY = 0.0f;

int play = 0;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	/*
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		return;
	}
	*/
	//myCamera.rotate(0.01f * ypos, 0.01f * xpos
	if (play == 0) {
		if (begin) {
			xpos0 = xpos;
			ypos0 = ypos;
			begin = false;
		}
		else {
			deltax = xpos - xpos0;
			deltay = ypos - ypos0;
			if (notAllowY + deltay < 1400.0f && notAllowY + deltay >-1400) {

				notAllowY += deltay;
				myCamera.rotate(-0.001f * deltay, 0.001f * deltax);
				//myCustomShader.useShaderProgram();
				////std::cout << myCamera.getCameraTarget().x<< " " << myCamera.getCameraTarget().y << std::endl;
				//lightPos = glm::vec4(myCamera.getCameraTarget().x, myCamera.getCameraTarget().y, 20.f, 0.f);
				////lightPos = glm::vec3(-25.7307f, 19.454f, 50.4833f);
				//lightPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
				//glUniform4fv(lightPosLoc, 1, glm::value_ptr(lightPos));	
			}
			//std::cout << "->>>>>>>>>>>>>>>>>>>" << deltax << std::endl;
			xpos0 = xpos;
			ypos0 = ypos;
		}
	}
	
}
int counterRain = 0;
void processMovement()
{
	if (glfwGetKey(glWindow, GLFW_KEY_Z)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_X)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.f);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_C)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(5.f);
	}
	//if (pressedKeys[GLFW_KEY_Q]) {
	//	angleY -= 1.0f;
	//}

	//if (pressedKeys[GLFW_KEY_E]) {
	//	angleY += 1.0f;
	//}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_LEFT]) {
		yaw = -0.05f;
		myCamera.rotate(pitch, yaw);
	}

	if (pressedKeys[GLFW_KEY_RIGHT]) {
		yaw = 0.05f;
		myCamera.rotate(pitch, yaw);
	}

	if (pressedKeys[GLFW_KEY_B]) {
		darkness = 1;
		myCustomShader.useShaderProgram();
		glUniform1i(darknessLoc, darkness);
		frunzaShader.useShaderProgram();
		glUniform1i(darknessLoc2, darkness);
		mySkyBox.Load(night);
	}
	if (pressedKeys[GLFW_KEY_N]) {
		darkness = 0;
		myCustomShader.useShaderProgram();
		glUniform1i(darknessLoc, darkness);
		frunzaShader.useShaderProgram();
		glUniform1i(darknessLoc2, darkness);
		mySkyBox.Load(faces);
	}
	if (pressedKeys[GLFW_KEY_T]) {

		lightning = 1;
		myCustomShader.useShaderProgram();
		glUniform1i(lightningLoc, 1);
		frunzaShader.useShaderProgram();
		glUniform1i(lightningLoc2, 1);
	}
	if (pressedKeys[GLFW_KEY_W]) {
		std::cout << myCamera.getPositionVec().x << " " << myCamera.getPositionVec().y << " " << myCamera.getPositionVec().z << std::endl;
		std::cout << ryzerGreymonS.center.x << " " << ryzerGreymonS.center.y << " " << ryzerGreymonS.center.z << " " << ryzerGreymonS.radius << std::endl << std::endl;

		glm::vec3 auxCameraPosition = myCamera.getPositionVec();
		glm::vec3 auxCameraFrontDirection = myCamera.getCameraFrontDirection();

		auxCameraPosition += auxCameraFrontDirection * cameraSpeed;
		float distance = sqrt((auxCameraPosition.x - felinarS.center.x) * (auxCameraPosition.x - felinarS.center.x) + (auxCameraPosition.y - felinarS.center.y) * (auxCameraPosition.y - felinarS.center.y) + (auxCameraPosition.z - felinarS.center.z) * (auxCameraPosition.z - felinarS.center.z));
		float greyDistance = sqrt((auxCameraPosition.x - ryzerGreymonS.center.x) * (auxCameraPosition.x - ryzerGreymonS.center.x) + (auxCameraPosition.y - ryzerGreymonS.center.y) * (auxCameraPosition.y - ryzerGreymonS.center.y) + (auxCameraPosition.z - ryzerGreymonS.center.z) * (auxCameraPosition.z - ryzerGreymonS.center.z));
		if (distance > felinarS.radius && greyDistance > ryzerGreymonS.radius)
			myCamera.move(gps::MOVE_FORWARD, cameraSpeed);

	}

	if (pressedKeys[GLFW_KEY_S]) {
		std::cout << myCamera.getPositionVec().x << " " << myCamera.getPositionVec().y << " " << myCamera.getPositionVec().z << std::endl;
		std::cout << felinarS.center.x << " " << felinarS.center.y << " " << felinarS.center.z << " " << felinarS.radius << std::endl << std::endl;
		glm::vec3 auxCameraPosition = myCamera.getPositionVec();
		glm::vec3 auxCameraFrontDirection = myCamera.getCameraFrontDirection();

		auxCameraPosition -= auxCameraFrontDirection * cameraSpeed;
		float distance = sqrt((auxCameraPosition.x - felinarS.center.x) * (auxCameraPosition.x - felinarS.center.x) + (auxCameraPosition.y - felinarS.center.y) * (auxCameraPosition.y - felinarS.center.y) + (auxCameraPosition.z - felinarS.center.z) * (auxCameraPosition.z - felinarS.center.z));
		float greyDistance = sqrt((auxCameraPosition.x - ryzerGreymonS.center.x) * (auxCameraPosition.x - ryzerGreymonS.center.x) + (auxCameraPosition.y - ryzerGreymonS.center.y) * (auxCameraPosition.y - ryzerGreymonS.center.y) + (auxCameraPosition.z - ryzerGreymonS.center.z) * (auxCameraPosition.z - ryzerGreymonS.center.z));
		if (distance > felinarS.radius && greyDistance > ryzerGreymonS.radius)
			myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		std::cout << myCamera.getPositionVec().x << " " << myCamera.getPositionVec().y << " " << myCamera.getPositionVec().z << std::endl;
		std::cout << felinarS.center.x << " " << felinarS.center.y << " " << felinarS.center.z << " " << felinarS.radius << std::endl << std::endl;
		glm::vec3 auxCameraPosition = myCamera.getPositionVec();
		glm::vec3 auxCameraFrontDirection = myCamera.getCameraFrontDirection();

		auxCameraPosition -= auxCameraFrontDirection * cameraSpeed;
		float distance = sqrt((auxCameraPosition.x - felinarS.center.x) * (auxCameraPosition.x - felinarS.center.x) + (auxCameraPosition.y - felinarS.center.y) * (auxCameraPosition.y - felinarS.center.y) + (auxCameraPosition.z - felinarS.center.z) * (auxCameraPosition.z - felinarS.center.z));
		float greyDistance = sqrt((auxCameraPosition.x - ryzerGreymonS.center.x) * (auxCameraPosition.x - ryzerGreymonS.center.x) + (auxCameraPosition.y - ryzerGreymonS.center.y) * (auxCameraPosition.y - ryzerGreymonS.center.y) + (auxCameraPosition.z - ryzerGreymonS.center.z) * (auxCameraPosition.z - ryzerGreymonS.center.z));
		if (distance > felinarS.radius && greyDistance > ryzerGreymonS.radius)
			myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		std::cout << myCamera.getPositionVec().x << " " << myCamera.getPositionVec().y << " " << myCamera.getPositionVec().z << std::endl;
		std::cout << felinarS.center.x << " " << felinarS.center.y << " " << felinarS.center.z << " " << felinarS.radius << std::endl << std::endl;
		glm::vec3 auxCameraPosition = myCamera.getPositionVec();
		glm::vec3 auxCameraFrontDirection = myCamera.getCameraFrontDirection();

		auxCameraPosition += auxCameraFrontDirection * cameraSpeed;
		float distance = sqrt((auxCameraPosition.x - felinarS.center.x) * (auxCameraPosition.x - felinarS.center.x) + (auxCameraPosition.y - felinarS.center.y) * (auxCameraPosition.y - felinarS.center.y) + (auxCameraPosition.z - felinarS.center.z) * (auxCameraPosition.z - felinarS.center.z));
		float greyDistance = sqrt((auxCameraPosition.x - ryzerGreymonS.center.x) * (auxCameraPosition.x - ryzerGreymonS.center.x) + (auxCameraPosition.y - ryzerGreymonS.center.y) * (auxCameraPosition.y - ryzerGreymonS.center.y) + (auxCameraPosition.z - ryzerGreymonS.center.z) * (auxCameraPosition.z - ryzerGreymonS.center.z));
		if (distance > felinarS.radius && greyDistance > ryzerGreymonS.radius)
			myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_R]) {
		counterRain++;
		Sleep(1000);
		if (counterRain % 2 == 1) {

			raining = true;
		}
		else {
			raining = false;
		}
	}
	if (pressedKeys[GLFW_KEY_UP]) {
		myCamera.move(gps::MOVE_UP, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {
		myCamera.move(gps::MOVE_DOWN, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_O]) {

	}
	
	if (pressedKeys[GLFW_KEY_P]) {
		Sleep(1000);
		myCamera.setCamera(glm::vec3(0.0f, 2.0f, 5.5f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		play = 1;
	}

}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {

	nanosuit.LoadModel("objects/nanosuit/nanosuit.obj");
	nanosuit2.LoadModel("objects/nanosuit/nanosuit.obj");
	ground.LoadModel("objects/ground/ground.obj");
	lightCube.LoadModel("objects/cube/cube.obj");
	screenQuad.LoadModel("objects/quad/quad.obj");
	spaceship.LoadModel("objects/spaceship/Intergalactic_Spaceship-(Wavefront).obj");
	frunzaModel.LoadModel("resources/leaf.obj");
	scene.LoadModel("objects/park/mapa.obj");
	rainDrop.LoadModel("objects/raindrop/rainDrop.obj");
	felinar.LoadModel("objects/felinar/felinar.obj");
	felinarBec.LoadModel("objects/felinar/felinarBec.obj");
	felinarStalp.LoadModel("objects/felinar/felinarStalp.obj");
	natura.LoadModel("objects/park/natura.obj");
	Dorugoramon.LoadModel("objects/Dorugoramon/Dorugoramon.obj");
	Tsuchidarumon.LoadModel("objects/Tsuchidarumon/Tsuchidarumon.obj");
	RizeGreymon.LoadModel("objects/RizeGreymon/RizeGreymon.obj");
	Courage.LoadModel("objects/Courage/Courage.obj");
	CourageMetal.LoadModel("objects/Courage/CourageMetal.obj");
	Dodomon.LoadModel("objects/Dodomon/Dodomon.obj");
	DigimonLogo.LoadModel("objects/DigimonLogo/DigimonLogo.obj");
	Swing.LoadModel("objects/Swing/swing.obj");
	Sand.LoadModel("objects/Sand/Sand.obj");
	Horse.LoadModel("objects/Swing/horse.obj");
	wanyamon.LoadModel("objects/Swing/wanyamon.obj");
	jyarimon.LoadModel("objects/Swing/jyarimon.obj");
	ChaosBlackWarGreymon.LoadModel("objects/ChaosBlackWarGreymon/ChaosBlackWarGreymon.obj");
	MetalGarurumon.LoadModel("objects/MetalGarurumon/MetalGarurumon.obj");
	fireBall.LoadModel("objects/fireball/fireBall.obj");

	darkMagicianModel.LoadModel("objects/darkMagician/darkMagician.obj");
	darkMagicianModel.~Model3D();
	darkMagicianModel.LoadModel("objects/darkMagician/darkMagician.obj");


}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();

	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();

	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();

	depthMapShader.loadShader("shaders/lightVertex.vert", "shaders/lightVertex.frag");
	depthMapShader.useShaderProgram();


	frunzaShader.loadShader("shaders/frunza.vert", "shaders/frunza.frag");
	frunzaShader.useShaderProgram();


	faces.push_back("textures/skybox/right.tga");
	faces.push_back("textures/skybox/left.tga");
	faces.push_back("textures/skybox/top.tga");
	faces.push_back("textures/skybox/bottom.tga");
	faces.push_back("textures/skybox/back.tga");
	faces.push_back("textures/skybox/front.tga");

	night.push_back("textures/skybox/nightSky_right2.tga");
	night.push_back("textures/skybox/nightSky_right2.tga");
	night.push_back("textures/skybox/nightSky_right2.tga");
	night.push_back("textures/skybox/nightSky_right2.tga");
	night.push_back("textures/skybox/nightSky_right2.tga");
	night.push_back("textures/skybox/nightSky_right2.tga");

	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");

}



void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	darknessLoc = glGetUniformLocation(myCustomShader.shaderProgram, "darkness");
	glUniform1i(darknessLoc, darkness);

	lightningLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightning");
	glUniform1i(lightningLoc, lightning);

	////lightDir = glm::vec3(-25.7307f,19.454f,50.4833f);
//////lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
////lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
////glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::mat3(view) * lightDir));


	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	lightPos = glm::vec4(0.f, 1.f, 0.f, 1.f);
	//lightPos = glm::vec4(-25.7307f, 19.454f, 50.4833f,1.f);
	lightPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");	
	glUniform4fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	//felinarPos = glm::vec4(1.f, 1.f, 1.f, 1.f);
	felinarPos = glm::vec4(1.f, 1.f, 1.f,1.f);
	//felinarPos = glm::vec4(-17.9039,6.50407,38.2638,1.f);
	felinarPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosFelinar");
	glUniform4fv(felinarPosLoc, 4, glm::value_ptr(felinarPos));


	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	frunzaShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(frunzaShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	darknessLoc2 = glGetUniformLocation(frunzaShader.shaderProgram, "darkness");
	glUniform1i(darknessLoc2, darkness);

	lightningLoc2 = glGetUniformLocation(frunzaShader.shaderProgram, "lightning");
	glUniform1i(lightningLoc2, lightning);

	skyboxShader.useShaderProgram();
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));
}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3(lightRotation)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-200.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}


void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//nanosuit.Draw(shader);


	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//spaceship.Draw(shader);


	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	model = glm::mat4(1.f);
	model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	Dorugoramon.Draw(shader);
	Tsuchidarumon.Draw(shader);
	RizeGreymon.Draw(shader);
	ryzerGreymonS = computeSphere(glm::vec3(-41.7637,17.0816,200.348), 23.f);
	ChaosBlackWarGreymon.Draw(shader);
	Sand.Draw(shader);
	Sand.Draw(shader);
	wanyamon.Draw(shader);

	Courage.Draw(shader);
	CourageMetal.Draw(shader);
	Dodomon.Draw(shader);
	DigimonLogo.Draw(shader);

	//fireBall.Draw(shader);


	felinarStalp.Draw(shader);
	felinarS = computeSphere(glm::vec3(-25.0094, 1.33097, 60.1884), 23.f);

	shader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	model = glm::mat4(1.f);
	model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	scene.Draw(shader);

}

int randomRainDrop;
void drawRainObjects(gps::Shader shader) {
	shader.useShaderProgram();
	float x = 20.56f;
	float lastX = x;
	float y = 5.027f;
	float z = 60.3f;
	float oldZ = 60.3f;
	int sizeLine = 6;
	int sizeRow = 15;
	int sizeHeight = 6;
	for (int height = 0; height < sizeHeight; height++) {
		for (int row = 0; row < sizeRow; row++) {
			for (int i = 0; i < sizeLine; i++) {
				model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3(x, y, z));
				model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
				model = glm::rotate(model, glm::radians(angleY + 60.f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
				randomRainDrop = rand() % 10 + 1;
				if (randomRainDrop > 4 && randomRainDrop < 6)
					rainDrop.Draw(shader);
				x -= (34. / sizeLine);
			}
			z += 10.f;
			if (row < sizeRow / 2)
				y += 0.5f;
			else
				y -= 0.5f;
			lastX -= 3.5f;
			x = lastX;
		}
		z = oldZ;
		y += 10.f;
		lastX = 20.56f;
		x = 20.56f;
	}
}
float delta = 0;
float movementSpeed = 2; // units per second
void updateDelta(double elapsedSeconds) {
	delta = delta + movementSpeed * elapsedSeconds;
}
double lastTimeStamp = glfwGetTime();

float deltaMusic = 0;
double music = glfwGetTime();
void updateMusic(double elapsedSeconds) {
	deltaMusic = deltaMusic + movementSpeed * elapsedSeconds;
}

float deltaLightning = 0;
double lightningSec = glfwGetTime();
void updateLightning(double elapsedSeconds) {
	lightningSec = elapsedSeconds;
}
bool lightningBool = false;
int lightningCounter = 0;
int lightningShader = 0;

double timeL = 0;


float counter = 0.f;

int okay = 0;

float firstPlay = 0.f;

int counterHorse = 0;

float zFireBall = 0.f;
float xFireBall = 0.f;

float MetalGarurumonX = 0.f;
float MetalGarurumonY = 0.f;
float MetalGarurumonZ = 0.f;
float MetalGarurumonX2 = 0.f;
float MetalGarurumonY2 = 0.f;
float MetalGarurumonZ2 = 0.f;
bool MetalGarurumonBool = false;
int MetalGarurumonCounter = 0;
bool MetalGarurumonCounterBool = false;
bool MetalGarurumonCounterRotate = true;
bool initialize1 = false;
bool initialize2 = false;


int counterLeft = 0;
int counterRight = 0;
int counterBackward = 0;
int counterForward = 0;
int counterUp = 0;
int counterUp2 = 0;
int counterDown = 0;
int counterRotateLeft = 0;
int counterRotateLeft2 = 0;
int counterRotateRight = 0;
int counterRotateDown = 0;
int counterUmplutura = 0;


void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);


	drawObjects(depthMapShader, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// render depth map on screen - toggled with the M key

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {

		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);


		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(myCustomShader, false);

		//draw a white cube around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		model = glm::translate(model, 1.0f * lightDir);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lightCube.Draw(lightShader);



		// get current time
		double currentTimeStamp = glfwGetTime();
		if (currentTimeStamp - lastTimeStamp > 1.0f) {

			updateDelta(currentTimeStamp - lastTimeStamp);
			counter += 10.0f;
			counterHorse += 1;
			//std::cout << lastTimeStamp << " " << currentTimeStamp << " "<<counter<<std::endl;
			lastTimeStamp = currentTimeStamp;
		}

		if (lightning == 1) {
			double currentLight = glfwGetTime();
			if (lightningShader != 1) {
				myCustomShader.useShaderProgram();
				glUniform1i(lightningLoc, 0);
				frunzaShader.useShaderProgram();
				glUniform1i(lightningLoc2, 0);
				if (currentLight - lightningSec > .06f) {
					updateLightning(currentLight);

					lightningShader = 1;
					lightningCounter++;
				}
			}
			else {
				myCustomShader.useShaderProgram();
				glUniform1i(lightningLoc, 1);
				frunzaShader.useShaderProgram();
				glUniform1i(lightningLoc2, 1);
				if (currentLight - lightningSec > .06f) {
					updateLightning(currentLight);
					lightningShader = 0;

				}
			}
			if (lightningCounter > 2) {

				lightning = 0;
				lightningCounter = 0;
				lightningShader = 0;
			}

		}

		double currentMusic = glfwGetTime();
		if (currentMusic - music > firstPlay) {
			updateMusic(currentMusic - music);
			music = currentMusic;
			okay = 1;
			if (okay)
				firstPlay = 6.f;
		}
		frunzaShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(frunzaShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(counter), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(frunzaShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//frunzaModel.Draw(frunzaShader);

		//myCustomShader.useShaderProgram();
		//glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//model = glm::mat4(1.f);
		//model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//scene.Draw(myCustomShader);

		frunzaShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(frunzaShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(frunzaShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		natura.Draw(frunzaShader);

		//Sphere darkMagicianS = computeSphere(glm::vec3(-4.0f, -1.f, 0.0f), 2.0f);

		//std::cout << darkMagicianS.center.x << " " << darkMagicianS.center.y << " " << darkMagicianS.center.z << " " << darkMagicianS.radius<<std::endl;

		//std::cout << myCamera.getPositionVec().x << " " << myCamera.getPositionVec().y << " " << myCamera.getPositionVec().z << std::endl;

		//model = glm::translate(view, glm::vec3(+2.0f, 0.0f, 0.0f));
		
		myCustomShader.useShaderProgram();
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(xFireBall, 0.f, zFireBall));
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		fireBall.Draw(myCustomShader);
		zFireBall += 0.8f;
		xFireBall -= 0.32f;
		if (zFireBall > 63.f) {
			zFireBall = 0.0f;
			xFireBall = 0.0f;
		}

		if (raining) {
			rainDrop.LoadModel("objects/raindrop/rainDrop.obj");
			drawRainObjects(frunzaShader);
		}
		else {
			rainDrop.~Model3D();
		}



		myCustomShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		model = glm::mat4(1.f);
		if (counterHorse % 2 == 0) {

			model = glm::rotate(model, glm::radians(0.05f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else {

			model = glm::rotate(model, glm::radians(-0.05f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Horse.Draw(myCustomShader);
		jyarimon.Draw(myCustomShader);
		//lumina punctiforma
		myCustomShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		model = glm::mat4(1.f);
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		felinarBec.Draw(myCustomShader);

		myCustomShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		/*
		model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(40.8079f, 0.f, - 81.1492f));
		model = glm::translate(model, glm::vec3(MetalGarurumonX, MetalGarurumonY, MetalGarurumonZ));*/

		

		
		if (!MetalGarurumonCounterBool) {
			model = glm::mat4(1.f);
			if (initialize2) {
				MetalGarurumonX = 0.0f;
				MetalGarurumonY = 0.0f;
				MetalGarurumonZ = 0.0f;
				//std::cout << "da";
				initialize2 = false;
			}
			MetalGarurumonX2 = 0.0f;
			MetalGarurumonY2 = 0.0f;
			MetalGarurumonZ2 = 0.0f;
			//model = glm::translate(model, glm::vec3(40.8079f, 0.f, - 81.1492f));
			model = glm::translate(model, glm::vec3(MetalGarurumonX, MetalGarurumonY, MetalGarurumonZ));
		//	if (initialize1) {
		///*		model = glm::translate(model, glm::vec3(30.3481f, 6.81927f, -32.2812f));
		//		model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
		//		model = glm::translate(model, glm::vec3(-30.3481f, -6.81927f, 32.2812f));*/
		//		model = glm::translate(model, glm::vec3(12.8833f, 6.16742f, 3.49909f));
		//	//	model = glm::rotate(model, glm::radians(-180.f), glm::vec3(0.0f, 1.0f, 0.0f));
		//		model = glm::translate(model, glm::vec3(-12.8833f, -6.16742f, -3.49909f));
		//	}
			
		}
		else {
			
			model = glm::mat4(1.f);
			//model = glm::translate(model, glm::vec3(40.8079f, 0.f, - 81.1492f));
			model = glm::translate(model, glm::vec3(MetalGarurumonX2, MetalGarurumonY2,MetalGarurumonZ2));
			//model = glm::translate(model, glm::vec3(0., 0., 0.));
			model = glm::translate(model, glm::vec3(MetalGarurumonX, MetalGarurumonY, MetalGarurumonZ));
			/*model = glm::translate(model, glm::vec3(12.018, 6.14628, 6.39323));
			model = glm::rotate(model, glm::radians(-180.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-12.018, -6.14628, -6.39323));*/




			/*model = glm::translate(model, glm::vec3(30.3481f, 6.81927f, -32.2812f));
			model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-30.3481f, -6.81927f, 32.2812f));*/
			model = glm::translate(model, glm::vec3(12.8833f, 6.16742f, 3.49909f));
			model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-12.8833f, -6.16742f, -3.49909f));
			initialize1 = true;
			initialize2 = true;
			//std::cout << initialize2;
		}
		if (!MetalGarurumonCounterBool) {
			MetalGarurumonZ -= 0.1f;
			MetalGarurumonX += 0.05f;

			if (MetalGarurumonY > 10.f) {
				MetalGarurumonBool = true;
			}
			if (MetalGarurumonY < 0.f) {
				MetalGarurumonBool = false;
				MetalGarurumonCounter++;
			}

			if (MetalGarurumonBool == false) {

				MetalGarurumonY += 0.1f;
			}
			else {

				MetalGarurumonY -= 0.1f;
			}
		}

		if (MetalGarurumonCounterBool) {
			MetalGarurumonZ2 += 0.1f;
			MetalGarurumonX2 -= 0.05f;

			if (MetalGarurumonY2 > 10.f) {
				MetalGarurumonBool = true;
			}
			if (MetalGarurumonY2 < 0.f) {
				MetalGarurumonBool = false;
				MetalGarurumonCounter--;
			}

			if (MetalGarurumonBool == false) {

				MetalGarurumonY2 += 0.1f;
			}
			else {

				MetalGarurumonY2 -= 0.1f;
			}
		}


		if (MetalGarurumonCounter == 2) {
			//if (!MetalGarurumonCounterBool) {
			//	MetalGarurumonCounterBool = true;
			//	//initialize2 = true;
			//std::cout << MetalGarurumonX << " " << MetalGarurumonY << " " << MetalGarurumonZ << std::endl;
			//}
			MetalGarurumonCounterBool = true;
		}

		if (MetalGarurumonCounter == 0) {
			//if (MetalGarurumonCounterBool) {
			//	//initialize1 = true;
			//	MetalGarurumonCounterBool = false;
			//}
			MetalGarurumonCounterBool = false;
		}

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		MetalGarurumon.Draw(myCustomShader);




		if (play == 1) {
			if (counterBackward == 150) {
				if (counterLeft == 100) {
					if (counterUp == 80) {
						if (counterRotateLeft == 160) {
							if (counterUmplutura == 160) {
								if (counterRotateRight == 160) {
									if (counterUp2 == 400) {
										if (counterForward == 650) {
											if (counterRotateLeft2 == 100) {
												if (counterRotateDown == 70) {
													play = 0;
													counterBackward = 0;
													counterLeft = 0;
													counterUp = 0;
													counterRotateLeft = 0;
													counterUmplutura = 0;
													counterRotateRight = 0;
													counterUp2 = 0;
													counterForward = 0;
													counterRotateLeft2 = 0;
													counterRotateDown = 0;
												}
												else {
													counterRotateDown++;
													myCamera.rotate(-0.001f*(counterRotateDown/5.f), 0.f);
												}
											}
											else {
												counterRotateLeft2++;
												myCamera.rotate(0.f, -0.001f * (counterRotateLeft2 / 5.f));
											}
										}
										else {
											counterForward++;
											myCamera.move(gps::MOVE_FORWARD, 0.2f);

										}
									}
									else {

										counterUp2++;
										myCamera.move(gps::MOVE_UP, 0.2f);
									}
								}
								else {
									counterRotateRight++;
									myCamera.rotate(0.f, 0.001f * (counterRotateRight / 5.f));
								}
							}
							else {
								counterUmplutura++;
							}
						}
						else {
							counterRotateLeft++;
							myCamera.rotate(0.f, -0.001f * (counterRotateLeft / 5.f));
						}
					}
					else {
						counterUp++;
						myCamera.move(gps::MOVE_UP, 0.2f);
					}
				}
				else {
					counterLeft++;
					myCamera.move(gps::MOVE_LEFT, 0.2f);
				}
			}
			else {
				counterBackward++;
				myCamera.move(gps::MOVE_BACKWARD, 0.2f);
			}
		}







		skyboxShader.useShaderProgram();
		mySkyBox.Draw(skyboxShader, view, projection);


	}


}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {






	srand(time(NULL));

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {


		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);

	}

	cleanup();

	return 0;
}
