#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw   = -90.0f;
float pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window) {
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos,1.0);
    ourColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor,1.0);
}
)";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"opengl в соло",nullptr,nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGL()){ std::cerr<<"Failed to initialize GLAD\n"; return -1; }

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // pos           // color
       -0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,  0.0f,1.0f,0.0f,
        0.5f, 0.5f,-0.5f,  0.0f,0.0f,1.0f,
        0.5f, 0.5f,-0.5f,  0.0f,0.0f,1.0f,
       -0.5f, 0.5f,-0.5f,  1.0f,1.0f,0.0f,
       -0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,

       -0.5f,-0.5f, 0.5f,  1.0f,0.0f,1.0f,
        0.5f,-0.5f, 0.5f,  0.0f,1.0f,1.0f,
        0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,
        0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,
       -0.5f, 0.5f, 0.5f,  0.3f,0.5f,0.7f,
       -0.5f,-0.5f, 0.5f,  1.0f,0.0f,1.0f,

       -0.5f, 0.5f, 0.5f,  0.3f,0.5f,0.7f,
       -0.5f, 0.5f,-0.5f,  1.0f,1.0f,0.0f,
       -0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
       -0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
       -0.5f,-0.5f, 0.5f,  1.0f,0.0f,1.0f,
       -0.5f, 0.5f, 0.5f,  0.3f,0.5f,0.7f,

        0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,
        0.5f, 0.5f,-0.5f,  0.0f,0.0f,1.0f,
        0.5f,-0.5f,-0.5f,  0.0f,1.0f,0.0f,
        0.5f,-0.5f,-0.5f,  0.0f,1.0f,0.0f,
        0.5f,-0.5f, 0.5f,  0.0f,1.0f,1.0f,
        0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,

       -0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,  0.0f,1.0f,0.0f,
        0.5f,-0.5f, 0.5f,  0.0f,1.0f,1.0f,
        0.5f,-0.5f, 0.5f,  0.0f,1.0f,1.0f,
       -0.5f,-0.5f, 0.5f,  1.0f,0.0f,1.0f,
       -0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,

       -0.5f, 0.5f,-0.5f,  1.0f,1.0f,0.0f,
        0.5f, 0.5f,-0.5f,  0.0f,0.0f,1.0f,
        0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,
        0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,
       -0.5f, 0.5f, 0.5f,  0.3f,0.5f,0.7f,
       -0.5f, 0.5f,-0.5f,  1.0f,1.0f,0.0f
    };

    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f,0.2f,0.25f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime(),glm::vec3(0.5f,1.0f,0.0f));
        glm::mat4 view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"model"),1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"view"),1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"projection"),1,GL_FALSE,glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
