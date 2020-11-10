#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>


// Estruturas que serão usadas no jogo

typedef struct
{
  // posição dos jogadores
  int x, y;
} Player;

typedef struct
{
  // posição da bola 
  int x, y;
  // atributo de velocidade horizontal e vertical
  int velx, vely;
} Ball;

// Função que processa os eventos do jogo 

int processEvents(SDL_Window *window, Player *playerA, Player *playerB, Ball * ball)
{
  SDL_Event event;
  int done = 0;
  float playerCenter, diag;

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

  
  ball->x += ball->velx;
  ball->y += ball->vely;
  

  
  
  //colisão c/ a parede
  if (ball->y >= 480 || ball->y <= 0) ball->vely *= -1; 
  
  // detecção de pontuação
  // if (ball->x >= 640 || ball->x <= 0) ball->velx *= -1; 
  
  // colisão com o jogador
  if ((ball->y >= playerB->y && ball->y <= playerB->y + 100 && ball->x == playerB->x - 10) || (ball->y >= playerA->y && ball->y <= playerA->y + 100 && ball->x == playerA->x + 10))
  {
    if (ball->x > 320) 
    {
    playerCenter = playerB->y + 50;
    diag = playerCenter - ball->y;
    ball->vely += diag * - 0.1;
    ball->velx *= -1;
    } else 
    {
    playerCenter = playerA->y + 50;
    diag = playerCenter - ball->y;
    ball->vely += diag * - 0.1;
    ball->velx *= -1;
    }
  }

  return done;
}

// Função que rederiza na tela os elementos do jogo

void doRender(SDL_Renderer *renderer, Player * playerA, Player * playerB, Ball * ball)
{
  //Escolhe a cor azul para renderizar 
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  //Deixa a tela azul
  SDL_RenderClear(renderer);
  
  //Escolhe a cor branca para novos desenhos 
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  // Desenho e renderiza os elementos do jogo na tela 
  SDL_Rect rectA = { playerA->x, playerA->y, 5, 70 };
  SDL_Rect rectB = { playerB->x, playerB->y, 5, 70 };
  SDL_Rect rectball = { ball->x, ball->y, 10, 10 };
  SDL_Rect net = { 320, 0 , 1, 640 };
  
  SDL_RenderFillRect(renderer, &rectA);
  SDL_RenderFillRect(renderer, &rectB);
  SDL_RenderFillRect(renderer, &rectball);
  SDL_RenderFillRect(renderer, &net);
  
  //Apresenta aquilo feito na tela 
  SDL_RenderPresent(renderer);
}

int main()
{
  SDL_Window *window;                    // Declaração de janela 
  SDL_Renderer *renderer;                // Declaração de renderização
  
  SDL_Init(SDL_INIT_VIDEO);              // Inicializa o SDL2 
  
  // Inicializando as peça do jogo com seus atributos
  Player playerA;
  playerA.x = 0;
  playerA.y = 0;

  Player playerB;
  playerB.x = 630;
  playerB.y = 0;

  Ball ball;
  ball.x = 220;
  ball.y = 120;
  ball.velx = 2;
  ball.vely = 0;

  
  // Criar a janela da aplicação com as seguintes configs: 
  window = SDL_CreateWindow("PongC",                           // título da janela
                            SDL_WINDOWPOS_UNDEFINED,           // posição inicial x 
                            SDL_WINDOWPOS_UNDEFINED,           // posição inicial y
                            640,                               // largura em pixels
                            480,                               // altura em pixels
                            0                                  // flags
                            );
  
  renderer = SDL_CreateRenderer(
                            window, 
                            -1, 
                            SDL_RENDERER_ACCELERATED
                            );


  // Seta a variável que continua o loop
  int done = 0;
  
  //Loop principal do jogo
  while(!done)
  {
    //Observar os eventos 
    done = processEvents(window, &playerA, &playerB, &ball);
    
    //Renderiza no display 
    doRender(renderer, &playerA, &playerB, &ball);
    
    //Controla o tempo do interno do jogo
    SDL_Delay(10);
  }
  
  
  // Limpeza da memoria pós-jogo
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  
  // Última etapa de limpeza
  SDL_Quit();
  return 0;
}