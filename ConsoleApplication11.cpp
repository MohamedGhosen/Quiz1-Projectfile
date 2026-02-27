#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Scene", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform vec2 offset;
        void main()
        {
            vec3 pos = aPos;
            pos.xy += offset;
            gl_Position = vec4(pos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform int isSky;
        uniform vec3 objectColor;
        uniform float sunX;  
        void main()
        {
            if(isSky == 1)
            {
                float factor = clamp((sunX + 1.2) / 2.0, 0.0, 1.0);
                vec3 dayColor   = vec3(0.4, 0.7, 1.0);
                vec3 nightColor = vec3(0.0, 0.0, 0.2);
                vec3 finalColor = mix(nightColor, dayColor, factor);
                FragColor = vec4(finalColor, 1.0);
            }
            else
            {
                FragColor = vec4(objectColor, 1.0);
            }
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    float skyVertices[] = {
       -1.0f,  1.0f, 0.0f,
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f
    };
    unsigned int skyIndices[] = { 
        0,1,2,
        0,2,3
    };
    unsigned int skyVAO, skyVBO, skyEBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glGenBuffers(1, &skyEBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyIndices), skyIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float groundVertices[] = {
       -1.0f, -0.3f, 0.0f,
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, -0.3f, 0.0f
    };
    unsigned int groundIndices[] = { 
        0,1,2,
        0,2,3 
    };
    unsigned int groundVAO, groundVBO, groundEBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glGenBuffers(1, &groundEBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float sunVertices[] = {
        0.0f, 0.0f, 0.0f,
        0.1f,0.0f,0.0f,
        0.097f,0.026f,0.0f,
        0.087f,0.05f,0.0f,
        0.071f,0.071f,0.0f,
        0.05f,0.087f,0.0f,
        0.026f,0.097f,0.0f,
        0.0f,0.1f,0.0f,
        -0.026f,0.097f,0.0f,
        -0.05f,0.087f,0.0f,
        -0.071f,0.071f,0.0f,
        -0.087f,0.05f,0.0f,
        -0.097f,0.026f,0.0f,
        -0.1f,0.0f,0.0f,
        -0.097f,-0.026f,0.0f,
        -0.087f,-0.05f,0.0f,
        -0.071f,-0.071f,0.0f,
        -0.05f,-0.087f,0.0f,
        -0.026f,-0.097f,0.0f,
        0.0f,-0.1f,0.0f,
        0.026f,-0.097f,0.0f,
        0.05f,-0.087f,0.0f,
        0.071f,-0.071f,0.0f,
        0.087f,-0.05f,0.0f,
        0.097f,-0.026f,0.0f,
        0.1f,0.0f,0.0f
    };
    unsigned int sunVAO, sunVBO;
    glGenVertexArrays(1, &sunVAO);
    glGenBuffers(1, &sunVBO);
    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float houseVertices[] = { 
        -0.75f,-0.1f,0.0f,
        -0.20f,-0.1f,0.0f,
        -0.20f,-0.6f,0.0f,
        -0.75f,-0.6f,0.0f 
    };
    unsigned int houseVAO, houseVBO;
    glGenVertexArrays(1, &houseVAO);
    glGenBuffers(1, &houseVBO);
    glBindVertexArray(houseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float roofVertices[] = { 
        -0.8f,-0.1f,0.0f,
        -0.15f,-0.1f,0.0f,
        -0.5f,0.1f,0.0f 
    };
    unsigned int roofVAO, roofVBO;
    glGenVertexArrays(1, &roofVAO);
    glGenBuffers(1, &roofVBO);
    glBindVertexArray(roofVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roofVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roofVertices), roofVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float windowVertices[] = { 
        -0.65f,-0.3f,0.0f,
        -0.55f,-0.3f,0.0f,
        -0.55f,-0.4f,0.0f,
        -0.65f,-0.4f,0.0f 
    };
    unsigned int windowVAO, windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float doorVertices[] = {
        -0.4f,-0.3f,0.0f,
        -0.28f,-0.3f,0.0f,
        -0.28f,-0.6f,0.0f,
        -0.4f,-0.6f,0.0f 
    };
    unsigned int doorVAO, doorVBO;
    glGenVertexArrays(1, &doorVAO);
    glGenBuffers(1, &doorVBO);
    glBindVertexArray(doorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float sunX = 0.8f;
    float sunY = 0.7f;
    float speedX = 0.3f;
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float t = glfwGetTime();
        float deltaTime = t - lastTime;
        lastTime = t;

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glUniform1i(glGetUniformLocation(shaderProgram, "isSky"), 1);
        glUniform1f(glGetUniformLocation(shaderProgram, "sunX"), sunX);
        glUniform2f(glGetUniformLocation(shaderProgram, "offset"), 0.0f, 0.0f);
        glBindVertexArray(skyVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform1i(glGetUniformLocation(shaderProgram, "isSky"), 0);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.1f, 0.6f, 0.2f);
        glUniform2f(glGetUniformLocation(shaderProgram, "offset"), 0.0f, 0.0f);
        glBindVertexArray(groundVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        sunX -= speedX * deltaTime;
        if (sunX < -1.2f) sunX = 0.8f;
        glUniform2f(glGetUniformLocation(shaderProgram, "offset"), sunX, sunY);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.9f, 0.2f);
        glBindVertexArray(sunVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 26);

        glUniform2f(glGetUniformLocation(shaderProgram, "offset"), 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.5f, 0.5f, 0.5f); 
        glBindVertexArray(houseVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.6f, 0.3f, 0.1f);
        glBindVertexArray(roofVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.6f, 0.8f, 1.0f);
        glBindVertexArray(windowVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.6f, 0.3f, 0.1f);
        glBindVertexArray(windowVAO);
        glDrawArrays(GL_LINE_LOOP, 0, 4);

        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.6f, 0.3f, 0.1f);
        glBindVertexArray(doorVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}