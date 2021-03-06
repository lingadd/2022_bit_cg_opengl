#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <windows.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "skyboxcube.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "gamecontroller.h"
#include <iostream>

// material
std::vector<float>Shininess = {11.264, 51.2, 61.2, 171.200001f };

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader shader("resources/shader/normal_mapping.vs", "resources/shader/normal_mapping.fs");
	Shader skyboxShader("resources/shader/skybox.vs", "resources/shader/skybox.fs");
	Skybox skybox(0);
	Shader modelShader("resources/shader/light_casters.vs", "resources/shader/light_casters.fs");
	Shader lightingShader("resources/shader/light_casters.vs", "resources/shader/light_casters.fs");
	Shader floorShader("resources/shader/blending.vs", "resources/shader/blending.fs");

	// shader configuration
    // --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);

	// model
	// -----
	// plane
	Model plane1("resources/objects/Piper/piper.obj"); 
	Model plane2("resources/objects/Piper/piper.obj");
	Model plane3("resources/objects/Piper/piper.obj");
	Model plane4("resources/objects/Piper/piper.obj");
	Model plane5("resources/objects/Piper/piper.obj");
	Model plane6("resources/objects/Piper/piper.obj");
	Model plane7("resources/objects/Piper/piper.obj");
	Model plane8("resources/objects/Piper/piper.obj");
	Model plane9("resources/objects/Piper/piper.obj");

	//Boat
	Model boat1("resources/objects/boat1/Boat.obj");
	Model boat2("resources/objects/boat1/Boat.obj");
	Model boat3("resources/objects/boat1/Boat.obj");
	Model boat4("resources/objects/boat1/Boat.obj");
	Model boat5("resources/objects/boat1/Boat.obj");
	Model boat6("resources/objects/boat1/Boat.obj");

	//Temple
	Model temple1("resources/objects/Temple1/Temple1.obj");

	
	//floor
	unsigned int floorTexture = loadTexture("resources/textures/floor.tga");

	// lighting info
	// -------------
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	// skybox shader configuration
// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	//float fogColor[] = { 1,1,1,1 };

		// lights	右左、上下、后前
	// ------
	glm::vec3 lightPositions[] = {
		//glm::vec3(8.0f, -24.0f, 2.0f),
		glm::vec3(8.0f, 4.0f, -8.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f, 150.0f),
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	// set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	float planeVertices[] = {
		// positions          // texture Coords 
		 5000.0f, -5.5f,  5000.0f,  2.0f, 0.0f,
		-5000.0f, -5.5f,  5000.0f,  0.0f, 0.0f,
		-5000.0f, -5.5f, -5000.0f,  0.0f, 2.0f,

		 5000.0f, -5.5f,  5000.0f,  2.0f, 0.0f,
		-5000.0f, -5.5f, -5000.0f,  0.0f, 2.0f,
		 5000.0f, -5.5f, -5000.0f,  2.0f, 2.0f
	};

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int amount = 100;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 10.0;
	float offset = 2.5f;
	float x = 0, y = -0.5, z = -1;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		x += 0.1;
		if (i % 10 == 0)
		{
			y += 0.1;
			x = 0;
		}
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		modelMatrices[i] = model;
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		updateGameController(window);

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw model
		lightingShader.use();
		lightingShader.setVec3("light.position", camera.Position);
		lightingShader.setVec3("light.direction", camera.Front);
		/*lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));//-------*/
		lightingShader.setVec3("light.ambient", 2.8f, 2.8f, 2.8f);
		lightingShader.setVec3("light.diffuse", 2.4f, 2.4f, 2.4f);
		lightingShader.setVec3("light.specular", 2.0f, 2.0f, 2.0f);
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.09f);
		lightingShader.setFloat("light.quadratic", 0.032f);
		lightingShader.setFloat("material.shininess", 2.0f);
		glm::mat4 view = camera.GetViewMatrix(); 
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// plane: example	
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 5.0f, -2.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.53f, 0.53f, 0.53f));	
		lightingShader.setMat4("model", model);
		plane1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 2.5f, 2.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));//-8.0f, -8.0f, -8.0f
		lightingShader.setMat4("model", model);
		plane2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-13.0f, 3.0f, 6.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		lightingShader.setMat4("model", model);
		plane3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.0f, 7.0f, 15.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.55f, 1.55f, 1.55f));
		lightingShader.setMat4("model", model);
		plane4.Draw(lightingShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 10.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.92f, 0.92f, 0.92f));
		lightingShader.setMat4("model", model);
		plane5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, 6.5f, 9.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.81f, 0.81f, 0.81f));
		lightingShader.setMat4("model", model);
		plane6.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 3.0f, 10.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.07f, 1.07f, 1.07f));
		lightingShader.setMat4("model", model);
		plane7.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, 7.0f, -4.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.54f, 0.54f, 0.54f));
		lightingShader.setMat4("model", model);
		plane8.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.72f, 0.72f, 0.72f));
		lightingShader.setMat4("model", model);
		plane9.Draw(lightingShader);

		//Boat
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));//-8.0f, -8.0f, -8.0f
		lightingShader.setMat4("model", model);
		boat1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.5f, -4.0f, 10.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setMat4("model", model);
		boat2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, -4.0f, 9.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setMat4("model", model);
		boat3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, -4.0f, 13.5f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		lightingShader.setMat4("model", model);
		boat4.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, -4.0f, -4.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		lightingShader.setMat4("model", model);
		boat5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, -4.0f, -6.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		lightingShader.setMat4("model", model);
		boat6.Draw(lightingShader);

		//Temple
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.055f, -4.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.027f, 0.027f, 0.027f));
		lightingShader.setMat4("model", model);
		temple1.Draw(lightingShader);



		// floor
		floorShader.use();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
		floorShader.setMat4("projection", projection);
		floorShader.setMat4("view", view);
		floorShader.setVec3("light.ambient", 0.7f, 0.7f, 0.7f);
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4(1.0f);
		floorShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//draw skybox at last
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_WIDTH, 0.1f, 100.0f);
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		skybox.Draw();

		glDepthFunc(GL_LESS); // set depth function back to default

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}