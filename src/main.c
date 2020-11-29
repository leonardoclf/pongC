#include "gamelib.h"

int main()
{
  int validNames = 1;
  int stage = 1;
  // char buffer[500];

  // Criar o jogadores
  Player playerA;
  Player playerB;
  
  printf("PONG C\n");
  printf("Insira nome sem espaco\n");

  while (validNames)
  {
    validNames = validRegister(&playerA, &playerB);
  }
  
  SDL_Window *window;                    // Declaração de janela 
  SDL_Renderer *renderer;                // Declaração de renderização
  
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);              // Inicializa o SDL2 
  TTF_Init();
  
  // Inicializando as peça do jogo com seus atributos
  playerA.x = 5;
  playerA.y = 190;
  playerA.score = 0;
  playerA.win = 0;
  playerA.lost = 0;
  playerA.foundPlayer = 0;
  

  playerB.x = 630;
  playerB.y = 190;
  playerB.score = 0;
  playerB.win = 0;
  playerB.lost = 0;
  playerB.foundPlayer = 0;

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
    stage = processEvents(window, &playerA, &playerB, &ball, hitFx, &stage);
    
    if(stage == 1)
    {
      // Renderiza o texto do menu
      doRenderMenu(renderer, &textMenu);
    }
    else
    {
      //Renderiza no display os eventos do jogo
      doRenderGame(renderer, &playerA, &playerB, &ball, &textVictory, &stage);
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

  
  // Ranking Logics

  // Função para conhecer o numero de ranking cadastrado
  int numberRank = rankSize("ranking.txt");

  // Criação dinamica do estrutura dos ranks
  rank * readRank = malloc(sizeof(rank) * numberRank);

  // Carrega o arquivo de ranking na memoria 
  processRanking("ranking.txt",readRank, &playerA, &playerB, numberRank);

  // Grava no txt o ranking atualizado
  writeRanking("ranking.txt", readRank, &playerA, &playerB, numberRank);

  // EXIBIR RANKING
  showRanking("ranking.txt");
  
  // Libera memoria dinamica
  free(readRank);

  return 0;
}