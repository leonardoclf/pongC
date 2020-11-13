#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

typedef struct
{
  int texW;
  int texH;
  TTF_Font * font;
  SDL_Color color;
} Texto;

// Var global que cuida do estado do jogo
int stage = 1;


// Função que processa os eventos do jogo 
int processEvents(SDL_Window *window, Player *playerA, Player *playerB, Ball * ball, Mix_Chunk * hitFx)
{

  SDL_Event event;

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
          stage = -1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
          stage = -1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        stage = -1;
        break;
    }
  }
  
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_SPACE])
  {
    stage = 2;
  }
  if(state[SDL_SCANCODE_W] && playerA->y > 5)
  {
    playerA->y -= 5;
  }
  if(state[SDL_SCANCODE_S] && playerA->y < 405)
  {
    playerA->y += 5;
  }
  
  if(state[SDL_SCANCODE_UP] && playerB->y > 5)
  {
    playerB->y -= 5;
  }
  if(state[SDL_SCANCODE_DOWN] && playerB->y < 405)
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

  // bloco de lógica que começa partida
  if (stage == 2)
  {
    ball->x += ball->velx;
    ball->y += ball->vely;
  }
  
  

  //colisão c/ a parede
  if (ball->y >= 480 || ball->y <= 0)
  {
    Mix_PlayChannel(-1, hitFx, 0);
    ball->vely *= -1; 
  } 
  
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
  if ((ball->y >= playerB->y && ball->y <= playerB->y + 100 && ball->x == playerB->x - 10) || (ball->y >= playerA->y && ball->y <= playerA->y + 100 && ball->x == playerA->x + 5))
  {
    if (ball->x > 320) 
    {
      Mix_PlayChannel(-1, hitFx, 0);
      printf("Ax: %d\n", playerA->x);
      printf("Ay: %d\n", playerA->y);
      printf("Ballx: %d\n", ball->x);
      printf("Bally: %d\n", ball->y);
      playerCenter = playerB->y + 35;
      diag = playerCenter - ball->y;
      ball->vely += diag * - 0.1;
      ball->velx *= -1;
    } 
    else 
    {
      Mix_PlayChannel(-1, hitFx, 0);
      printf("Bx: %d\n", playerB->x);
      printf("By: %d\n", playerB->y);
      printf("Ballx: %d\n", ball->x);
      printf("Bally: %d\n", ball->y);
      playerCenter = playerA->y + 35;
      diag = playerCenter - ball->y;
      ball->vely += diag * - 0.1;
      ball->velx *= -1;
    }
  }

  // fim de jogo
  if (playerA->score >= 5 || playerB->score >= 5 )
  {
    stage = -1;
  }

  return stage;
}

// Função que renderiza o menu
void doRenderMenu(SDL_Renderer *renderer, Texto * textoVic)
{
  
  //Escolhe a cor azul para renderizar 
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  
  //Deixa a tela azul
  SDL_RenderClear(renderer);
  
  //Escolhe a cor branca para novos desenhos 
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Surface * msgA = TTF_RenderText_Solid(textoVic->font, "PONG C", textoVic->color);
  SDL_Texture * msgTextA = SDL_CreateTextureFromSurface(renderer, msgA);
  SDL_QueryTexture(msgTextA, NULL, NULL, &textoVic->texW, &textoVic->texH);

  SDL_Surface * msgB = TTF_RenderText_Solid(textoVic->font, "ESPACO - Para comecar", textoVic->color);
  SDL_Texture * msgTextB = SDL_CreateTextureFromSurface(renderer, msgB);
  SDL_QueryTexture(msgTextB, NULL, NULL, &textoVic->texW, &textoVic->texH);

  SDL_Surface * msgC = TTF_RenderText_Solid(textoVic->font, "ESC - Para finalizar jogo", textoVic->color);
  SDL_Texture * msgTextC = SDL_CreateTextureFromSurface(renderer, msgC);
  SDL_QueryTexture(msgTextC, NULL, NULL, &textoVic->texW, &textoVic->texH);

  
  SDL_Rect bemvindo = { 90, 40, textoVic->texW, textoVic->texH };
  SDL_Rect instrucaoA = { 90, 160, textoVic->texW, textoVic->texH };
  SDL_Rect instrucaoB = { 90, 200, textoVic->texW, textoVic->texH };
  

  SDL_RenderCopy(renderer, msgTextA, NULL, &bemvindo);
  SDL_RenderCopy(renderer, msgTextB, NULL, &instrucaoA);
  SDL_RenderCopy(renderer, msgTextC, NULL, &instrucaoB);
  SDL_RenderPresent(renderer);
}

// Função que rederiza o jogo
void doRenderGame(SDL_Renderer *renderer, Player * playerA, Player * playerB, Ball * ball, Texto * textoVic)
{
  //Escolhe a cor azul para renderizar 
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  //Deixa a tela azul
  SDL_RenderClear(renderer);
  
  //Escolhe a cor branca para novos desenhos 
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // conversão do placar para str
  char scorePlayerAStr[10];
  char scorePlayerBStr[10];
  sprintf(scorePlayerAStr, "%i", playerA->score);
  sprintf(scorePlayerBStr, "%i", playerB->score);
  // sprintf(scorePlayerAStr, "%i", ball->x);
  // sprintf(scorePlayerBStr, "%i", ball->y);

  
  // Processamento do Placar - texto
  // criação do text do jogador A 
  SDL_Surface * surfaceA = TTF_RenderText_Solid(textoVic->font, scorePlayerAStr, textoVic->color);
  SDL_Texture * textureA = SDL_CreateTextureFromSurface(renderer, surfaceA);
  SDL_QueryTexture(textureA, NULL, NULL, &textoVic->texW, &textoVic->texH);

  // criação do text do jogador B
  SDL_Surface * surfaceB = TTF_RenderText_Solid(textoVic->font, scorePlayerBStr, textoVic->color);
  SDL_Texture * textureB = SDL_CreateTextureFromSurface(renderer, surfaceB);
  SDL_QueryTexture(textureB, NULL, NULL, &textoVic->texW, &textoVic->texH);
  


  // Desenho e renderiza os elementos do jogo na tela 
  SDL_Rect rectA = { playerA->x, playerA->y, 5, 70 };
  SDL_Rect rectB = { playerB->x, playerB->y, 5, 70 }; 
  SDL_Rect rectball = { ball->x, ball->y, 10, 10 };
  SDL_Rect net = { 320, 0 , 1, 640 };
  SDL_Rect scoreA = { 180, 120, textoVic->texW, textoVic->texH };
  SDL_Rect scoreB = { 390, 120, textoVic->texW, textoVic->texH };
  
  SDL_RenderFillRect(renderer, &rectA);
  SDL_RenderFillRect(renderer, &rectB);
  SDL_RenderFillRect(renderer, &rectball);
  SDL_RenderFillRect(renderer, &net);
  SDL_RenderCopy(renderer, textureA, NULL, &scoreA);
  SDL_RenderCopy(renderer, textureB, NULL, &scoreB);
  
  //Apresenta aquilo feito na tela 
  SDL_RenderPresent(renderer);
}

int main()
{
  SDL_Window *window;                    // Declaração de janela 
  SDL_Renderer *renderer;                // Declaração de renderização
  
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);              // Inicializa o SDL2 
  TTF_Init();
  
  // Inicializando as peça do jogo com seus atributos
  Player playerA;
  playerA.x = 5;
  playerA.y = 190;
  playerA.score = 0;

  Player playerB;
  playerB.x = 630;
  playerB.y = 190;
  playerB.score = 0;

  Ball ball;
  ball.x = 220;
  ball.y = 120;
  ball.velx = 2;
  ball.vely = 0;

  Texto textMenu;
  textMenu.texW = 0;
  textMenu.texH = 0;
  textMenu.color.r = 255;
  textMenu.color.b = 255;
  textMenu.color.g = 255;
  textMenu.color.a = 255;
  textMenu.font = TTF_OpenFont("Roboto.ttf", 38);

  Texto textVictory;
  textVictory.texW = 0;
  textVictory.texH = 0;
  textVictory.color.r = 255;
  textVictory.color.b = 255;
  textVictory.color.g = 255;
  textVictory.color.a = 255;
  textVictory.font = TTF_OpenFont("Roboto.ttf", 120);

  // Abre canal de audio
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  
  // Carrega o efeito sonoro
  Mix_Chunk * hitFx = Mix_LoadWAV("Hit.wav");
  
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

  //Esconde o mouse
  SDL_ShowCursor(0);

  //Loop principal do jogo
  while(stage > 0)
  {
    //Observar os eventos do jogo 
    stage = processEvents(window, &playerA, &playerB, &ball, hitFx);
    
    if(stage == 1)
    {
      // Renderiza o texto do menu
      doRenderMenu(renderer, &textMenu);
    }
    else
    {
      //Renderiza no display os eventos do jogo
      doRenderGame(renderer, &playerA, &playerB, &ball, &textVictory);
    }

    //Controla o tempo do interno do jogo
    SDL_Delay(10);
  }
  
  
  // Limpeza da memoria pós-jogo
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  TTF_CloseFont(textVictory.font);
  Mix_FreeChunk(hitFx);
  
  // Última etapa de limpeza
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
  return 0;
}