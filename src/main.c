#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


// Estruturas que serão usadas no jogo

typedef struct
{
  // posição dos jogadores
  int x, y, score;
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
  bool done = false;
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
  // Reset da Bola 
  if(state[SDL_SCANCODE_R])
  {
    ball->x = 220;
    ball->y = 120;
    ball->velx = 2;
    ball->vely = 0;
  }

  // bloco de lógica da movimentação da bola
  ball->x += ball->velx;
  ball->y += ball->vely;
  

  //colisão c/ a parede
  if (ball->y >= 480 || ball->y <= 0) ball->vely *= -1; 
  
  // detecção de pontuação
  if (ball->x >= 640 || ball->x <= 0) 
  {
    if(ball->x >= 640)
    {
      playerA->score += 1;
      ball->x = 220;
      ball->y = 120;
      ball->velx = 2;
      ball->vely = 0;
    } 
    else if (ball->x <= 0) 
    {
      playerB->score += 1;
      ball->x = 220;
      ball->y = 120;
      ball->velx = -2;
      ball->vely = 0;      
    }
  }; 
  
  // colisão com o jogador
  if ((ball->y >= playerB->y && ball->y <= playerB->y + 100 && ball->x == playerB->x - 10) || (ball->y >= playerA->y && ball->y <= playerA->y + 100 && ball->x == playerA->x + 10))
  {
    if (ball->x > 320) 
    {
      playerCenter = playerB->y + 50;
      diag = playerCenter - ball->y;
      ball->vely += diag * - 0.1;
      ball->velx *= -1;
    } 
    else 
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

void doRender(SDL_Renderer *renderer, Player * playerA, Player * playerB, Ball * ball, TTF_Font * font)
{
  //Escolhe a cor azul para renderizar 
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  //Deixa a tela azul
  SDL_RenderClear(renderer);
  
  //Escolhe a cor branca para novos desenhos 
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // Processamento do Placar - texto
  // conversão do placar para str
  char scorePlayerAStr[10];
  sprintf(scorePlayerAStr, "%i", playerA->score);
  char scorePlayerBStr[10];
  sprintf(scorePlayerBStr, "%i", playerB->score);

  // seleção da cor e var comum de texto
  SDL_Color color = { 255, 255, 255 };
  int texW = 0;
  int texH = 0;

  // criação do text do jogador A 
  SDL_Surface * surfaceA = TTF_RenderText_Solid(font, scorePlayerAStr, color);
  SDL_Texture * textureA = SDL_CreateTextureFromSurface(renderer, surfaceA);
  SDL_QueryTexture(textureA, NULL, NULL, &texW, &texH);

  // criação do text do jogador B
  SDL_Surface * surfaceB = TTF_RenderText_Solid(font, scorePlayerBStr, color);
  SDL_Texture * textureB = SDL_CreateTextureFromSurface(renderer, surfaceB);
  SDL_QueryTexture(textureB, NULL, NULL, &texW, &texH);
  
  // Desenho e renderiza os elementos do jogo na tela 
  SDL_Rect rectA = { playerA->x, playerA->y, 5, 70 };
  SDL_Rect rectB = { playerB->x, playerB->y, 5, 70 };
  SDL_Rect rectball = { ball->x, ball->y, 10, 10 };
  SDL_Rect net = { 320, 0 , 1, 640 };
  SDL_Rect scoreA = { 180, 100, texW, texH };
  SDL_Rect scoreB = { 400, 100, texW, texH };
  
  SDL_RenderFillRect(renderer, &rectA);
  SDL_RenderFillRect(renderer, &rectB);
  SDL_RenderFillRect(renderer, &rectball);
  SDL_RenderFillRect(renderer, &net);
  SDL_RenderCopy(renderer, textureA, NULL, &scoreA);
  SDL_RenderCopy(renderer, textureB, NULL, &scoreB);
  
  //Apresenta aquilo feito na tela 
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(textureA);
  SDL_FreeSurface(surfaceA);
  SDL_DestroyTexture(textureB);
  SDL_FreeSurface(surfaceB);
}

int main()
{
  SDL_Window *window;                    // Declaração de janela 
  SDL_Renderer *renderer;                // Declaração de renderização
  
  SDL_Init(SDL_INIT_VIDEO);              // Inicializa o SDL2 
  TTF_Init();
  
  // Inicializando as peça do jogo com seus atributos
  Player playerA;
  playerA.x = 0;
  playerA.y = 0;
  playerA.score = 0;

  Player playerB;
  playerB.x = 630;
  playerB.y = 0;
  playerB.score = 0;

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


  // Carrega a fonte
  TTF_Font * font = TTF_OpenFont("Roboto.ttf", 120);

  // Seta a variável que continua o loop
  bool done = false;
  
  //Loop principal do jogo
  while(!done)
  {
    //Observar os eventos 
    done = processEvents(window, &playerA, &playerB, &ball);
    
    //Renderiza no display 
    doRender(renderer, &playerA, &playerB, &ball, font);
    
    //Controla o tempo do interno do jogo
    SDL_Delay(10);
  }
  
  
  // Limpeza da memoria pós-jogo
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  TTF_CloseFont(font);
  
  // Última etapa de limpeza
  TTF_Quit();
  SDL_Quit();
  return 0;
}