#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <fstream>
#include <sstream>

using namespace glm;


GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n");
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path.c_str());
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path.c_str());
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

void Update_mesh(const GLuint& VAO, const std::vector<GLuint>& GLBuffers,
    std::vector<glm::vec3>& Positions,
    std::vector<glm::vec3> Normals,
    std::vector<glm::vec3> Colors,
    std::vector<unsigned int>& Indices)
{

    glBindVertexArray(VAO); // must bound before binding buffers

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(Positions[0]), &Positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Colors[0]), &Colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices[0]), &Indices[0], GL_STATIC_DRAW);

    //glEnableVertexAttribArray(0);
    //glBindBuffer (GL_ARRAY_BUFFER, 0); // Unbind VBO to avoid accidental modification
    glBindVertexArray(0); // Unbind VAO to avoid accidental modification
}



// settings
int Width = 512;
int Height = 512;

void resize_callback(GLFWwindow*, int nw, int nh)
{
    Width = nw;
    Height = nh;
    //Tell the viewport to use all of our screen estate
    glViewport(0, 0, nw, nh);
}

void processInput(GLFWwindow* window)
{
    //Close when the user hits 'q' or escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void generateSphere(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, int sectorCount = 40, int stackCount = 40)
{
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f;             // vertex normal
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep;
        xy = cosf(stackAngle);
        z = sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            positions.push_back(glm::vec3(x, y, z));
            normals.push_back(glm::normalize(glm::vec3(x, y, z)));
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}



int main(int argc, char* argv[])
{
    // Initialize Window

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current*/
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Create and compile our GLSL program from the shaders
    GLuint shaderProgram = LoadShaders("Phong.vert", "Phong.frag");

    // main 함수 안의 geometry 정의 부분 교체
    std::vector<glm::vec3> Positions, Normals;
    std::vector<unsigned int> Indices;
    generateSphere(Positions, Normals, Indices);

    std::vector<glm::vec3> Colors(Positions.size(), glm::vec3(0.0f, 1.0f, 0.0f)); // 임시 컬러
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    const int numBuffers = 4; // Buffers for Positions, Normals, Colors, Indices
    std::vector<GLuint> GLBuffers;
    GLBuffers.resize(numBuffers, 0);
    glGenBuffers(numBuffers, &GLBuffers[0]);

    Update_mesh(VAO, GLBuffers, Positions, Normals, Colors, Indices);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -7));
    model = glm::scale(model, glm::vec3(2.0f));
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

    glm::vec3 lightPos = glm::vec3(-4.0f, 4.0f, -3.0f);

    glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 materialAmbient = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 materialDiffuse = glm::vec3(0.0f, 0.5f, 0.0f);
    glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
    float materialShininess = 32.0f;
    float gamma = 2.2;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &camPos[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, &lightPos[0]);

        glUniform3fv(glGetUniformLocation(shaderProgram, "lightAmbient"), 1, &lightAmbient[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightDiffuse"), 1, &lightDiffuse[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightSpecular"), 1, &lightSpecular[0]);

        glUniform3fv(glGetUniformLocation(shaderProgram, "materialAmbient"), 1, &materialAmbient[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "materialDiffuse"), 1, &materialDiffuse[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "materialSpecular"), 1, &materialSpecular[0]);
        glUniform1f(glGetUniformLocation(shaderProgram, "materialShininess"), materialShininess);
        glUniform1f(glGetUniformLocation(shaderProgram, "gamma"), gamma);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    if (GLBuffers[0] != 0) glDeleteBuffers(numBuffers, GLBuffers.data());
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}