#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>

int main()
{
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

  auto svectoraWindow = SDL_CreateWindow("Svectora", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 500, SDL_WINDOW_OPENGL);
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

  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD!" << std::endl;
    return -1;
  }

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
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(svectoraWindow);
  }

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(svectoraWindow);
  SDL_Quit();
  return 0;
}