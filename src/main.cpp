#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/readFile.h"
#include "resources/vertices.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif

int main()
{
  // ------------------------------------ SDL Setting ------------------------------------
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Failed to initialize SDL : %s", SDL_GetError());
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  auto svectoraWindow = SDL_CreateWindow("Svectora", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL);
  if (!svectoraWindow)
  {
    SDL_Log("Failed to create window! : %s", SDL_GetError());
    return -1;
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(svectoraWindow);
  if (!glContext)
  {
    SDL_Log("Failed to create OpenGL Context : %s", SDL_GetError());
    return -1;
  }
  SDL_GL_SetSwapInterval(1);

  // ------------------------------------ GLAD Setting ------------------------------------
  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD!" << std::endl;
    return -1;
  }

  auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if (!vertexShader)
  {
    std::cerr << "Failed to intialize GLAD vertex shader!" << std::endl;
    return -1;
  }
  std::string vertexShaderSrc = readFile("../src/shaders/basic.vert");
  if (vertexShaderSrc == "")
  {
    std::cerr << "Failed to read GLAD vertex shader file!" << std::endl;
    glDeleteShader(vertexShader);
    return -1;
  }
  const char *vSrc = vertexShaderSrc.c_str();

  glShaderSource(vertexShader, 1, &vSrc, nullptr);
  glCompileShader(vertexShader);

  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char log[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, log);
    std::cerr << "Failed to compile GLAD vertex shader! : " << log << std::endl;
    glDeleteShader(vertexShader);
    return -1;
  }

  auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (!fragmentShader)
  {
    std::cerr << "Failed to initialize GLAD fragment shader!" << std::endl;
    return -1;
  }

  std::string fragmentShaderSrc = readFile("../src/shaders/basic.frag");
  if (fragmentShaderSrc == "")
  {
    std::cerr << "failed to read GLAD fragment shader file!" << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return -1;
  }
  const char *fSrc = fragmentShaderSrc.c_str();
  glShaderSource(fragmentShader, 1, &fSrc, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char log[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
    std::cerr << "Failed to compile GLAD fragment shader!" << log << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return -1;
  }

  auto program = glCreateProgram();
  if (!program)
  {
    std::cerr << "Failed to initialize GLAD program!" << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return -1;
  }

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glUseProgram(program);

  // ------------------------------------ Event Init ------------------------------------
  glEnable(GL_DEPTH_TEST);

  extern float _vertices[];
  extern const size_t _verticesCount;

  // auto vertices = _vertices;
  const size_t verticesCount = _verticesCount;

  GLuint VBO,
      VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float), _vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(9 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // ------------------------------------ Event Variables ------------------------------------
  float camera_dx = 0.0f;
  // float camera_dy = 0.0f;
  float camera_dz = -2.0f;

  //  ------------------------------------ Init Texture ------------------------------------

  int image_px = 0;
  int image_py = 0;
  int image_pc = 4;
  const unsigned char *buf = stbi_load("../src/resources/textures/image.png", &image_px, &image_py, &image_pc, 0);
  if (!buf)
  {
    std::cerr << "failed to load image file! : " << stbi_failure_reason() << std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(svectoraWindow);
    SDL_Quit();
  }
  GLuint textureID;
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum image_format = (image_pc == 3) ? GL_RGB : GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, image_format, image_px, image_py, 0, image_format, GL_UNSIGNED_BYTE, buf);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free((void *)buf);

  // ------------------------------------ Uniform settings ------------------------------------
  GLint textureLoc = glGetUniformLocation(program, "uTexture");
  glUniform1i(textureLoc, 0);

  GLint roughnessLoc = glGetUniformLocation(program, "uRoughness");
  GLint kdLoc = glGetUniformLocation(program, "uK_d");
  GLint ksLoc = glGetUniformLocation(program, "uK_s");
  GLint f0Loc = glGetUniformLocation(program, "uF0");

  glUniform1f(roughnessLoc, 0.5f);
  glUniform1f(kdLoc, 1.0f);
  glUniform1f(ksLoc, 1.0f);
  glUniform3f(f0Loc, 0.04f, 0.04f, 0.04f);

  // ------------------------------------ Event Loop ------------------------------------
  SDL_Event event;
  bool isRunning{true};
  while (isRunning)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        isRunning = false;
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_a:
          camera_dx -= 0.01f;
          break;
        case SDLK_d:
          camera_dx += 0.01f;
          break;
        case SDLK_w:
          camera_dz -= 0.1f;
          break;
        case SDLK_s:
          camera_dz += 0.1f;
          break;
        }
      }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
      isRunning = false;
    }

    float angle = (float)SDL_GetTicks() / 1000.0f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -0.5f));
    model = glm::rotate(model, angle, glm::vec3(1, 1, 0));
    model = glm::translate(model, glm::vec3(0, 0, 0.5f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera_dx, 0, camera_dz));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 20.0f);

    GLuint modelLoc = glGetUniformLocation(program, "uModel");
    GLuint viewLoc = glGetUniformLocation(program, "uView");
    GLuint projLoc = glGetUniformLocation(program, "uProj");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    GLuint uAmbientColor = glGetUniformLocation(program, "uAmbientColor");
    GLuint uAmbientStrength = glGetUniformLocation(program, "uAmbientStrength");
    // GLuint uSpecStrength = glGetUniformLocation(program, "uSpecStrength");

    GLuint lightPosLoc = glGetUniformLocation(program, "uLightPos");
    GLuint lightColorLoc = glGetUniformLocation(program, "uLightColor");
    GLuint viewPosLoc = glGetUniformLocation(program, "uViewPos");

    glUniform3f(uAmbientColor, 1.0f, 0.0f, 0.0f);
    glUniform1f(uAmbientStrength, 0.1f);
    // glUniform1f(uSpecStrength, 256.0f);

    glUniform3f(lightPosLoc, 3.0f, 3.0f, 3.0f);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(viewPosLoc, 0.0f, 0.0f, 2.0f);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    SDL_GL_SwapWindow(svectoraWindow);
  }

  // ------------------------------------ Exit ------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(program);
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(svectoraWindow);
  SDL_Quit();
  return 0;
}