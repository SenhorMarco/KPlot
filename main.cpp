#include "ksdl.h"

#define T_FRAME 17
#define V_CAMERA 20 //VELOCIDADE DA CAMERA DADA EM PIXELS
#define PASSO_GRAFICO 0.01
#define LINHA_MINIMA 2 //TAMANHO DE LINHA MINIMO A SER DESENHADO EM PIXELS

bool iniciar(int largura_janela, int altura_janela);
void sair();
bool loop(ksdl::frame *frame_atual, ksdl::camera *camera,SDL_Window *janela, SDL_Renderer *renderer, std::vector<ksdl::equacao*> graficos);
bool visivel(ksdl::camera *camera,float x_numerico, float y_numerico);
int cpox(int largura, int x);
int cpoy(int altura, int y);
float distancia(ksdl::camera *camera, int x1, int y1, int x2, int y2);

SDL_Window* janela;
SDL_Renderer* renderer;

exprtk::symbol_table<float> simbolos;
exprtk::parser<float> parser_expressao;
float x;

int main(int argc, char** args) {
    ksdl::frame frame_atual;
    ksdl::camera camera;
    camera.range_x = 40;
    camera.range_y = 40; //range da camera nos eixos
    int largura_janela =800, altura_janela = 800;
    std::vector<ksdl::equacao*> graficos;
    SDL_Color vermelho = {255,0,0,255}, azul = {0,0,255,255}, verde = {0,120,0,255};
    std::string leitura;
    while(true){
        std::cin >> leitura;
        if(leitura == "fim"){
            break;
        }
        else{
            graficos.push_back(new ksdl::equacao(leitura, vermelho));
        }
    }
	if ( !iniciar(largura_janela, altura_janela) ){
        return 1;
	}
    camera.posicao_x = 0; //posicao do centro da camera (em pixels)
    camera.posicao_y = 0;
    camera.posicao_numerica_x = 0; //posicao do centro da camera (em valores do eixo)
    camera.posicao_numerica_y = 0;

    simbolos.add_variable("x",x);
    for(ksdl::equacao* grafico : graficos){
        grafico->expressao.register_symbol_table(simbolos);
        parser_expressao.compile(grafico->string_expressao, grafico->expressao);
    }


	while ( loop(&frame_atual, &camera, janela, renderer, graficos)) {
        if(frame_atual.duracao() > T_FRAME){
            continue;
        }
        else{
           SDL_Delay(T_FRAME-frame_atual.duracao());
        }
	}

	sair();
	return 0;
}

bool loop(ksdl::frame *frame_atual, ksdl::camera *camera, SDL_Window *janela, SDL_Renderer *renderer, std::vector<ksdl::equacao*> graficos) {
    frame_atual->tempo_inicial = SDL_GetTicks();
	static const unsigned char* teclado = SDL_GetKeyboardState( NULL );

	SDL_Event evento;
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );

	int largura =0, altura=0;
	SDL_GetWindowSize(janela, &largura, &altura);
    int pixel_por_x = largura/camera->range_x, pixel_por_y= altura/camera->range_y;
    SDL_SetRenderDrawColor(renderer, 211, 211,211,255);
    bool espaco = false;
    bool adicionar = false;

	while ( SDL_PollEvent( &evento ) != 0) {
		switch ( evento.type ) {
			case SDL_QUIT:
				return false;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
		}
	}

	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

	if(teclado[SDL_SCANCODE_A]){
        std::string leitura;
        std::cin >> leitura;
        SDL_Color vermelho = {255,0,0,255};
        ksdl::equacao* novo_grafico= new ksdl::equacao(leitura,vermelho);
        novo_grafico->expressao.register_symbol_table(simbolos);
        parser_expressao.compile(novo_grafico->string_expressao, novo_grafico->expressao);
        graficos.push_back(novo_grafico);
	}

	if (teclado[SDL_SCANCODE_LEFT]) {
        camera->posicao_x-=(V_CAMERA);
        camera->posicao_numerica_x-=((float)V_CAMERA/(float)pixel_por_x);
	}

    if (teclado[SDL_SCANCODE_RIGHT]) {
        camera->posicao_x+=V_CAMERA;
        camera->posicao_numerica_x+=((float)V_CAMERA/(float)pixel_por_x);
	}

	if (teclado[SDL_SCANCODE_UP]) {
        camera->posicao_y+=V_CAMERA;
        camera->posicao_numerica_y+=((float)V_CAMERA/(float)pixel_por_y);
	}

    if (teclado[SDL_SCANCODE_DOWN]) {
        camera->posicao_y-=V_CAMERA;
        camera->posicao_numerica_y-=((float)V_CAMERA/(float)pixel_por_y);
	}

	if (teclado[SDL_SCANCODE_SPACE]){
        espaco = true;
	}

	SDL_SetRenderDrawColor(renderer, 211, 211,211, 255);

    //Desenhar linhas horizontais cinzas
    for(int i = camera->posicao_x % pixel_por_x -altura/2; i <= altura/2; i+=pixel_por_y){
        SDL_RenderDrawLine(renderer, cpox(largura, -largura/2), cpoy(altura, i), cpox(largura, largura/2), cpoy(altura,i));
    }

    //Desenhar linhas verticais cinzas
    for(int i = camera->posicao_y % pixel_por_y - largura/2; i <= largura/2; i+=pixel_por_x){
        SDL_RenderDrawLine(renderer, cpox(largura, i), cpoy(altura, -altura/2), cpox(largura, i), cpoy(altura,altura/2));
    }

    //Desenho do eixo x
    if(visivel(camera, camera->posicao_numerica_x,0)){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderDrawLine(renderer, cpox(largura, -largura/2), cpoy(altura, camera->posicao_y), cpox(largura, largura/2), cpoy(altura, camera->posicao_y));
    }

    //Desenho do eixo y
    if(visivel(camera, 0,camera->posicao_numerica_y)){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderDrawLine(renderer, cpox(largura, -camera->posicao_x), cpoy(altura, altura/2), cpox(largura, -camera->posicao_x), cpoy(altura, -altura/2));
    }

    //Desenho dos gráficos

    for(ksdl::equacao* grafico : graficos){
        SDL_SetRenderDrawColor(renderer, grafico->cor.r, grafico->cor.g, grafico->cor.b, grafico->cor.a);
        std::vector<std::pair<float,float>> pontos_numericos;
        std::vector<std::pair<int,int>> pontos;
        for(float i = -camera->range_x/2; i <= camera->range_x/2; i+=PASSO_GRAFICO){
            float posicao_numerico_x = i + camera->posicao_numerica_x;
            x = posicao_numerico_x;
            float posicao_numerico_y = grafico->expressao.value();
            if(visivel(camera, posicao_numerico_x, posicao_numerico_y)){
                pontos_numericos.push_back(std::make_pair(posicao_numerico_x, posicao_numerico_y));
                int posicao_x = posicao_numerico_x * pixel_por_x;
                int posicao_y = -posicao_numerico_y * pixel_por_y;
                pontos.push_back(std::make_pair(posicao_x, posicao_y));
            }
        }
        if(!pontos.empty()){
            //em pontos de derivada muito pequena (pequena variação de y dada uma variação padronizada em x), evitamos desenhar várias micro linhas sem perda visual mas com ganho grande de eficiencia
            for(std::vector<std::pair<int,int>>::iterator it = pontos.begin(); it != (pontos.end()-1); /* :P */){
                int i = 1;
                while(distancia(camera, it->first, it->second, (it+i)->first, (it+i)->second) < LINHA_MINIMA){
                    i++;
                }
                if((it+i) > (pontos.end() - 1)){
                    break;
                }
                SDL_RenderDrawLine(renderer, cpox(largura, it->first - camera->posicao_x), cpoy(altura,it->second + camera->posicao_y),
                                cpox(largura, (it+i)->first - camera->posicao_x), cpoy(altura,(it+i)->second + camera->posicao_y));
                it+=i;
            }

        }

    }

    if(espaco){
        camera->posicao_numerica_x =0;
        camera->posicao_numerica_y =0;
        camera->posicao_x=0;
        camera->posicao_y=0;
    }

	SDL_RenderPresent( renderer );
    frame_atual->tempo_final = SDL_GetTicks();
	return true;
}

bool iniciar(int largura_janela, int altura_janela) {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		printf("Erro na inicializacao!\n");
		return false;
	}

	janela = SDL_CreateWindow( "KPlot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largura_janela, altura_janela, SDL_WINDOW_SHOWN );
	if ( !janela ) {
		printf("Erro ao criar janela : %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer( janela, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		printf("Erro ao criar renderer : %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
	return true;
}

void sair() {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( janela );
	SDL_Quit();
}

//engenharia!!!
bool visivel(ksdl::camera *camera, float x, float y){
    //x e y são em unidades de eixo
    if(abs(camera->posicao_numerica_x - x) <= camera->range_x/2 && abs(camera->posicao_numerica_y - y) <= camera->range_y/2){
        return true;
    }
    else{
        return false;
    }
}

int cpox(int largura, int x){
    return x + largura/2;
}

int cpoy(int altura, int y){
    return y + altura/2;
}

float distancia(ksdl::camera *camera, int x1, int y1, int x2, int y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
