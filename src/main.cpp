#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/readFile.h"

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

  float vertices[] = {
      // 앞면 (z = 0.0)
      -0.5f, -0.5f, 0.0f, // v0
      -0.5f, 0.5f, 0.0f,  // v2
      0.5f, -0.5f, 0.0f,  // v4

      -0.5f, 0.5f, 0.0f, // v2
      0.5f, 0.5f, 0.0f,  // v6
      0.5f, -0.5f, 0.0f, // v4

      // 뒷면 (z = -1.0)
      0.5f, -0.5f, -1.0f,  // v5
      0.5f, 0.5f, -1.0f,   // v7
      -0.5f, -0.5f, -1.0f, // v1

      0.5f, 0.5f, -1.0f,   // v7
      -0.5f, 0.5f, -1.0f,  // v3
      -0.5f, -0.5f, -1.0f, // v1

      // 왼쪽면
      -0.5f, -0.5f, -1.0f, // v1
      -0.5f, 0.5f, -1.0f,  // v3
      -0.5f, -0.5f, 0.0f,  // v0

      -0.5f, 0.5f, -1.0f, // v3
      -0.5f, 0.5f, 0.0f,  // v2
      -0.5f, -0.5f, 0.0f, // v0

      // 오른쪽면
      0.5f, -0.5f, 0.0f,  // v4
      0.5f, 0.5f, 0.0f,   // v6
      0.5f, -0.5f, -1.0f, // v5

      0.5f, 0.5f, 0.0f,   // v6
      0.5f, 0.5f, -1.0f,  // v7
      0.5f, -0.5f, -1.0f, // v5

      // 윗면
      -0.5f, 0.5f, 0.0f,  // v2
      -0.5f, 0.5f, -1.0f, // v3
      0.5f, 0.5f, 0.0f,   // v6

      -0.5f, 0.5f, -1.0f, // v3
      0.5f, 0.5f, -1.0f,  // v7
      0.5f, 0.5f, 0.0f,   // v6

      // 아랫면
      -0.5f, -0.5f, -1.0f, // v1
      -0.5f, -0.5f, 0.0f,  // v0
      0.5f, -0.5f, -1.0f,  // v5

      -0.5f, -0.5f, 0.0f, // v0
      0.5f, -0.5f, 0.0f,  // v4
      0.5f, -0.5f, -1.0f  // v5
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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
      }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
      isRunning = false;
    }

    float angle = (float)SDL_GetTicks() / 1000.0f;
    std::cout << "angle : " << angle << '\n';

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -0.5f));
    model = glm::rotate(model, angle, glm::vec3(1, 1, 0));
    model = glm::translate(model, glm::vec3(0, 0, 0.5f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2.0f));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);

    GLuint modelLoc = glGetUniformLocation(program, "uModel");
    GLuint viewLoc = glGetUniformLocation(program, "uView");
    GLuint projLoc = glGetUniformLocation(program, "uProj");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

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