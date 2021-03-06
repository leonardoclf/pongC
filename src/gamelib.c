#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "gamelib.h"



// Determina nome valido
int validName(Player * player)
{
  for(int i = 0; i < strlen(player->name); i++)
  {
    if(player->name[i] == ' ')
    {
      return 0;
    }
  }
  return 1;
}

// Registro do nome dos jogadores
int validRegister(Player * playerA, Player * playerB)
{
  char buffer[500];
  while(1)
  {
    printf("Digite o nome do Jogador A: ");
    gets(buffer);
    strcpy(playerA->name, buffer);
    
    printf("Digite o nome do Jogador B: ");
    gets(buffer);
    strcpy(playerB->name, buffer);
    
    if (validName(playerA) + validName(playerB) == 2)
    {
      if(strcmp(playerA->name, playerB->name) != 0)
      {
        return false;
      }
      else
      {
        printf("Nomes dos jogadores precisam ser diferentes\n");
      }
    }
    else
    {
      printf("Nome digitado invalido. Digite sem espaco\n");
    }
  }
  return true;
}

// Determina qtd de ranks cadastrados
int rankSize(char * path)
{
  FILE * rankingTxt = NULL;
  char ch;
  int numberRank = 0;

  rankingTxt = fopen(path, "r");
  while((ch=fgetc(rankingTxt))!=EOF) 
  {
      if(ch=='\n')
      {
         numberRank+=1;
      }
   }
   fclose(rankingTxt);
   return numberRank;
}

// Exibi os ranks 
void showRanking(char * path)
{
  char rankLine[100];
  FILE * rankingTxt = NULL;
  rankingTxt = fopen(path, "r");
  if (rankingTxt == NULL)
  {
    printf("ERROR %s", path);
  }
  else
  {
    printf("\n-----------Exibindo RANK-----------\n");
    char * readName;
    char * readV;
    char * readL;

    while (fgets(rankLine, 100, rankingTxt) != NULL)
    {
      readName = strtok(rankLine, " ");
      readV = strtok(NULL, " ");
      readL = strtok(NULL, " ");
      printf("Jogador: %s\t\tVitoria: %s\t\tDerrota: %s\n", readName, readV, readL);
    }

    fclose(rankingTxt);
  }
}

// Processa os ranks na memoria
void processRanking(char * path, rank * rank, Player * playerA, Player * playerB, int numberRank)
{
  // Load Rank
  int i = 0;
  FILE * rankingTxt = NULL;
  rankingTxt = fopen(path, "r");
  if (rankingTxt == NULL)
  {
    printf("ERROR %s", path);
  }
  else
  {
    char rankLine[100];
    char * lineBuffer;
    while (fgets(rankLine, 100, rankingTxt) != NULL)
    {
      lineBuffer = strtok(rankLine, " ");
      strcpy(rank[i].playerName, lineBuffer);
      lineBuffer = strtok(NULL, " ");
      rank[i].v = atoi(lineBuffer);
      lineBuffer = strtok(NULL, " ");
      rank[i].l = atoi(lineBuffer);
      i++;
    }
    fclose(rankingTxt);
    // Atualizar o RANK de nome conhecido
    for(i=0; i < numberRank; i++)
    {
      if(strcmp(rank[i].playerName, playerA->name) == 0)
      {
        playerA->foundPlayer = 1;
        if(playerA->win)
        {
          rank[i].v++;
        }
        else
        {
          rank[i].l++;
        }
      }
      if(strcmp(rank[i].playerName, playerB->name) == 0)
      {
        playerB->foundPlayer = 1;
        if(playerB->win)
        {
          rank[i].v++;
        }
        else
        {
          rank[i].l++;
        }
      }
    }
  }
}

// Escreve os ranks num arquivo txt
void writeRanking(char * path, rank * rank, Player * playerA, Player * playerB, int numberRank)
{
  FILE * rankingTxt = NULL;
  char buffer[500];
  rankingTxt = fopen(path, "w");
  if (rankingTxt == NULL)
  {
    printf("ERROR %s", path);
  }
  else
  {  
    for(int i = 0; i < numberRank ; i++)
    {
      sprintf(buffer, "%s %d %d\n", rank[i].playerName, rank[i].v, rank[i].l);
      fputs(buffer, rankingTxt);
    }
    // Novos Jogadores
    if(playerA->foundPlayer == 0)
    {
      sprintf(buffer, "%s %d %d\n", playerA->name, playerA->win, playerA->lost);
      fputs(buffer, rankingTxt);
    }
    if(playerB->foundPlayer == 0)
    {
      sprintf(buffer, "%s %d %d\n", playerB->name, playerB->win, playerB->lost);
      fputs(buffer, rankingTxt);
    }
    fclose(rankingTxt);
  } 
}

// Processa os eventos do jogo 
int processEvents(SDL_Window *window, Player *playerA, Player *playerB, Ball * ball, Mix_Chunk * hitFx, int * stage)
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
          *stage = -1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
          *stage = -1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        *stage = -1;
        break;
    }
  }
  
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_SPACE])
  {
    *stage = 2;
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

  // bloco de lógica que começa partida
  if (*stage == 2)
  {
    ball->x += ball->velx;
    ball->y += ball->vely;
  }
  
  

  //colisão c/ a parede
  if (ball->y >= 470 || ball->y <= 0)
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
      playerCenter = playerB->y + 35;
      diag = playerCenter - ball->y;
      ball->vely += diag * - 0.1;
      ball->velx *= -1;
    } 
    else 
    {
      Mix_PlayChannel(-1, hitFx, 0);
      playerCenter = playerA->y + 35;
      diag = playerCenter - ball->y;
      ball->vely += diag * - 0.1;
      ball->velx *= -1;
    }
  }

  // fim de jogo
  if (playerA->score >= 5 || playerB->score >= 5 )
  {
    ball->x = 700;
    ball->y = 700;
    *stage = -1;
    if(playerA->score > playerB->score)
    {
      playerA->win = 1;
      playerB->lost = 1;
    }
    else
    {
      playerB->win = 1;
      playerA->lost = 1;
    }

  }

  return *stage;
}

// Renderiza o menu
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

  SDL_Surface * msgB = TTF_RenderText_Solid(textoVic->font, "Controle A: W / S", textoVic->color);
  SDL_Texture * msgTextB = SDL_CreateTextureFromSurface(renderer, msgB);
  SDL_QueryTexture(msgTextB, NULL, NULL, &textoVic->texW, &textoVic->texH);

  SDL_Surface * msgC = TTF_RenderText_Solid(textoVic->font, "Controle B: Seta Cima / Seta Baixo", textoVic->color);
  SDL_Texture * msgTextC = SDL_CreateTextureFromSurface(renderer, msgC);
  SDL_QueryTexture(msgTextC, NULL, NULL, &textoVic->texW, &textoVic->texH);
  
  SDL_Surface * msgD = TTF_RenderText_Solid(textoVic->font, "ESPACO - Para comecar", textoVic->color);
  SDL_Texture * msgTextD = SDL_CreateTextureFromSurface(renderer, msgD);
  SDL_QueryTexture(msgTextD, NULL, NULL, &textoVic->texW, &textoVic->texH);

  SDL_Surface * msgE = TTF_RenderText_Solid(textoVic->font, "ESC - Para finalizar jogo", textoVic->color);
  SDL_Texture * msgTextE = SDL_CreateTextureFromSurface(renderer, msgE);
  SDL_QueryTexture(msgTextE, NULL, NULL, &textoVic->texW, &textoVic->texH);

  
  SDL_Rect bemvindo = { 90, 40, textoVic->texW, textoVic->texH };
  SDL_Rect instrucaoA = { 90, 180, textoVic->texW, textoVic->texH };
  SDL_Rect instrucaoB = { 90, 240, textoVic->texW, textoVic->texH };
  SDL_Rect instrucaoC = { 90, 380, textoVic->texW, textoVic->texH };
  SDL_Rect instrucaoD = { 90, 420, textoVic->texW, textoVic->texH };
  

  SDL_RenderCopy(renderer, msgTextA, NULL, &bemvindo);
  SDL_RenderCopy(renderer, msgTextB, NULL, &instrucaoA);
  SDL_RenderCopy(renderer, msgTextC, NULL, &instrucaoB);
  SDL_RenderCopy(renderer, msgTextD, NULL, &instrucaoC);
  SDL_RenderCopy(renderer, msgTextE, NULL, &instrucaoD);
  SDL_RenderPresent(renderer);
}

// Rederiza o jogo
void doRenderGame(SDL_Renderer *renderer, Player * playerA, Player * playerB, Ball * ball, Texto * textoVic, int * stage)
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

  // Prende o resultado na tela 
  if(*stage == -1 ) {
    SDL_Delay(3000);
  }

}