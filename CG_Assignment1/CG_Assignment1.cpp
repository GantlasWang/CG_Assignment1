#include <iostream>

#include "ObjLoader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


const string objFile = "C:/Users/Gantlas/Desktop/Assignment1/eight.uniform.obj";
const int WIDTH = 800;
const int HEIGHT = 600;
const int VERTICESNUM = 1009;
const int FACESNUM = 2022;
const float pi = 3.1415926;
const glm::mat4 oldView = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 3.0f)+ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
const glm::mat4 oldModel = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
glm::mat4 model = glm::mat4(1.0f);



float deltaTime = 0.0f;
float lastTime = 0.0f;

bool firstMouse = true;
float yaw = 0.0f;
float pitch = 0.0f;
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback_right(GLFWwindow* window, double xpos, double ypos);

void mouse_callback_left(GLFWwindow* window, double xpos, double ypos);

int main()
{
    std::cout << "Hello World!\n";
	ObjLoader objModel = ObjLoader(objFile);
	vector<float> vSets = objModel.getVSets();
	vector<unsigned int> fSets = objModel.getFSets();

	float vertices[VERTICESNUM * 3];
	unsigned int faces[FACESNUM * 3];

	for (int i = 0; i < vSets.size(); ++i) {

		vertices[i] = vSets[i];

	}

	for (int i = 0; i < fSets.size(); ++i) {

		faces[i] = fSets[i];

	}
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CG_Assignment1", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	while (!glfwWindowShouldClose(window)) {
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		int modelMatLocation = glGetUniformLocation(shaderProgram, "model");
		int viewMatLocation = glGetUniformLocation(shaderProgram, "view");
		int projectionMatLocation = glGetUniformLocation(shaderProgram, "projection");

		view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
		processInput(window);
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//更新深度测试

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawElements(GL_TRIANGLES, 6066, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		view = oldView;
		model = oldModel;
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		glfwSetCursorPosCallback(window, mouse_callback_left);

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwSetCursorPosCallback(window, mouse_callback_right);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ) {
		glfwSetCursorPosCallback(window, NULL);

	}
}

void mouse_callback_right(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - 400;
	float yoffset = ypos - 300;

	yaw = xoffset /400 * 3 * pi;
	pitch = yoffset /300 * 3 * pi;
	
	glm::vec3 front;
	front.x = 3 * sin(yaw) * cos(pitch);
	front.y = 3 * sin(pitch);
	front.z = 3 * cos(yaw) * cos(pitch);

	cameraPos = front;
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f) - cameraPos;
}

void mouse_callback_left(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = 89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	model = glm::rotate(model, glm::radians(1.0f * front.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f * front.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f * front.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

