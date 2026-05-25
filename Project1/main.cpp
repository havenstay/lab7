#define GLFW_DLL
#define GLEW_DLL

#include "Shader.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <algorithm>
#include "Model.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace std;

float OX1_Angle = 0.0f; 
float OX2_Angle = 0.0f;
float OX3_Angle = 0.0f;
float OX2_Height = 0.0f;
glm::mat4 OX3_Pos = glm::mat4(1.0f);
float vertex[] = { 0.0f, 0.5f, 0.0f,
                   0.0f, -0.5f, 0.0f,
                  -0.5f, -0.5f, 0.0f };
GLuint indices[] = { 0, 1, 2
};

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

float pitch = 0.0f;
float yaw = -90.0f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 OX1_Pos(1.0);
glm::mat4 OX2_Pos(1.0);




int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Error");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* myWindow = glfwCreateWindow(512, 512, "Ignatkov", NULL, NULL);

    if (!myWindow)
    {
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(myWindow);
    glewExperimental = GL_TRUE;

    GLenum ret = glewInit();
    if (GLEW_OK != ret)
    {
        fprintf(stderr, "error: glew");
        return -3;
    }

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(myWindow, mouse_callback);
    glfwSetScrollCallback(myWindow, scroll_callback);
    glfwSetInputMode(myWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    void PrintControls();

    Shader ourShader("vertex.glsl", "frag.glsl");

    std::cout << " G/H  - Движение каретки" << std::endl;
    std::cout << " T/Y  - Подъем/Опускание" << std::endl;
    std::cout << " J/K  - Поворот клешней" << std::endl;
    
    
    Model ourModel("./lab3.obj");


    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(myWindow))
    {
        processInput(myWindow);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 transform = glm::mat4(1.0f);
        //glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        //ourShader.setMat3("normalMatrix", normalMatrix);

        ourShader.Use();


        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("model", model);
        ourShader.setMat4("transform", transform);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);


        glm::vec3 ambientLight = lightColor * 0.1f;
        glm::vec3 diffuseLight = lightColor * 0.8f;
        glm::vec3 specularLight = lightColor * 1.0f;

        ourShader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        ourShader.setVec3("light.ambient", ambientLight.x, ambientLight.y, ambientLight.z);
        ourShader.setVec3("light.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
        ourShader.setVec3("light.specular", specularLight.x, specularLight.y, specularLight.z);

        ourShader.setVec3("mat.ambient", 1.0f, 0.5f, 0.5f);
        ourShader.setVec3("mat.diffuse", 1.0f, 0.5f, 0.5f);
        ourShader.setVec3("mat.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("mat.shininess", 32.0f);

        ourShader.setVec3("ViewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);

        ourModel.Draw(ourShader, OX1_Pos, OX2_Pos, OX3_Pos);


        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }

    glfwTerminate();
}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        float NewAngle = OX1_Angle - deltaTime * 50;
        OX1_Pos = glm::rotate(glm::mat4(1.0), glm::radians(NewAngle), glm::vec3(0.0, 1.0, 0.0));
        OX1_Angle = NewAngle;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        float NewAngle = OX1_Angle + deltaTime * 50;
        OX1_Pos = glm::rotate(glm::mat4(1.0), glm::radians(NewAngle), glm::vec3(0.0, 1.0, 0.0));
        OX1_Angle = NewAngle;
    }

    const float speed = 2.0f;
    const float moveSpeed = 1.5f;  
    const float rotateSpeed = 150.0f; 

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) OX2_Angle -= deltaTime * moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) OX2_Angle += deltaTime * moveSpeed;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) OX2_Height += deltaTime * moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) OX2_Height -= deltaTime * moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        OX3_Angle += rotateSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        OX3_Angle -= rotateSpeed * deltaTime;
    }

    if (OX2_Angle < -0.8f) OX2_Angle = -0.8f;
    if (OX2_Angle > 0.35f)  OX2_Angle = 0.35f;  

    if (OX2_Height < -0.4f) OX2_Height = -0.4f; 
    if (OX2_Height > 0.17f)  OX2_Height = 0.17f;  
    if (OX3_Angle < -30.5f) OX3_Angle = -30.5f;
    if (OX3_Angle > 30.5f) OX3_Angle = 30.5f;

    OX2_Pos = glm::translate(glm::mat4(1.0f), glm::vec3(OX2_Angle, OX2_Height, 0.0f));
    OX3_Pos = glm::rotate(glm::mat4(1.0f), glm::radians(OX3_Angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
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
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

