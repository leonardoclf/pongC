#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct
{
  int x, y;
} Player;

typedef struct
{
  int x, y;
  int vel;
} Ball;




int processEvents(SDL_Window *window, Player *playerA, Player *playerB, Ball * ball)
{
  SDL_Event event;
  int done = 0;

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(window)
        {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
          done = 1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        //quit out of the game
        done = 1;
        break;
    }
  }
  
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_W])
  {
    playerA->y -= 5;
  }
  if(state[SDL_SCANCODE_S])
  {
    playerA->y += 5;
  }
  
  if(state[SDL_SCANCODE_UP])
  {
    playerB->y -= 5;
  }
  if(state[SDL_SCANCODE_DOWN])
  {
    playerB->y += 5;
  }

  
  ball->x += ball->vel;
  

  
  
  // colisão com a parede
  
  
  
  // colisão com jogadores 
  if ((ball->y >= playerB->y && ball->y <= playerB->y + 100 && ball->x == playerB->x - 10) || (ball->y >= playerA->y && ball->y <= playerA->y + 100 && ball->x == playerA->x + 10))
  {
    ball->vel *= -1;
  }
  

  
  
  
  
  

  return done;
}

void doRender(SDL_Renderer *renderer, Player * playerA, Player * playerB, Ball * ball)
{
  //set the drawing color to blue
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  //Clear the screen (to blue)
  SDL_RenderClear(renderer);
  
  //set the drawing color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  SDL_Rect rectA = { playerA->x, playerA->y, 10, 100 };
  SDL_Rect rectB = { playerB->x, playerB->y, 10, 100 };
  SDL_Rect rectball = { ball->x, ball->y, 10, 10 };
  
  SDL_RenderFillRect(renderer, &rectA);
  SDL_RenderFillRect(renderer, &rectB);
  SDL_RenderFillRect(renderer, &rectball);
  
  //We are done drawing, "present" or show to the screen what we've drawn
  SDL_RenderPresent(renderer);
}

int main()
{
  SDL_Window *window;                    // Declare a window
  SDL_Renderer *renderer;                // Declare a renderer
  
  SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
  
  Player playerA;
  playerA.x = 0;
  playerA.y = 0;

  Player playerB;
  playerB.x = 630;
  playerB.y = 0;

  Ball ball;
  ball.x = 220;
  ball.y = 120;
  ball.vel = 2;

  
  //Create an application window with the following settings:
  window = SDL_CreateWindow("PongC",                           // window title
                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                            640,                               // width, in pixels
                            480,                               // height, in pixels
                            0                                  // flags
                            );
  
  renderer = SDL_CreateRenderer(
                            window, 
                            -1, 
                            SDL_RENDERER_ACCELERATED
                            );


  // The window is open: enter program loop (see SDL_PollEvent)
  int done = 0;
  
  //Event loop
  while(!done)
  {
    //Check for events
    done = processEvents(window, &playerA, &playerB, &ball);
    
    //Render display
    doRender(renderer, &playerA, &playerB, &ball);
    
    //don't burn up the CPU
    SDL_Delay(10);
  }
  
  
  // Close and destroy the window
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  
  // Clean up
  SDL_Quit();
  return 0;
}