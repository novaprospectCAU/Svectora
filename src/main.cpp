#include <iostream>
#include <SDL2/SDL.h>
#include <OpenGL/gl.h>

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Failed to initialize SDL : %s", SDL_GetError());
    return -1;
  }

  auto svectoraWindow = SDL_CreateWindow("Svectora", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 500, 0);
  if (!svectoraWindow)
  {
    SDL_Log("Failed to create window! : %s", SDL_GetError());
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
  }

  SDL_DestroyWindow(svectoraWindow);
  SDL_Quit();
  return 0;
}