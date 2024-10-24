#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include "ksdl.h"

#define T_FRAME 17
#define V_CAMERA 0.15

bool iniciar(int largura_janela, int altura_janela);
void sair();
bool loop(ksdl::frame *frame_atual, ksdl::camera *camera,SDL_Window *janela, SDL_Renderer *renderer);

SDL_Window* janela;
SDL_Renderer* renderer;

int main(int argc, char** args) {
    ksdl::frame frame_atual;
    ksdl::camera camera;
    camera.range = 10;
    camera.posicao_x = 0;
    camera.posicao_y = 0;
    int largura_janela, altura_janela;
    scanf("%d %d", &largura_janela, &altura_janela);
	if ( !iniciar(largura_janela, altura_janela) ){
        return 1;
	}

	while ( loop(&frame_atual, &camera, janela, renderer)) {
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

bool loop(ksdl::frame *frame_atual, ksdl::camera *camera, SDL_Window *janela, SDL_Renderer *renderer) {
    frame_atual->tempo_inicial = SDL_GetTicks();
	static const unsigned char* teclado = SDL_GetKeyboardState( NULL );

	SDL_Event evento;
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );

	int largura, altura;
	SDL_GetWindowSize(janela, &largura, &altura);
    int pixel_por_x = largura/camera->range, pixel_por_y= altura/camera->range;
    SDL_SetRenderDrawColor(renderer, 211, 211,211,255);


    /*for(int i = pixel_por_x; i < largura; i+=pixel_por_x){
        SDL_RenderDrawLine(renderer, i,0,i,altura);
    }

    for(int i = pixel_por_y; i < altura; i+=pixel_por_y){
        SDL_RenderDrawLine(renderer, 0,i,largura,i);
    }*/


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

	if (teclado[SDL_SCANCODE_LEFT]) {
        camera->posicao_x+=(pixel_por_x*V_CAMERA);
	}

    if (teclado[SDL_SCANCODE_RIGHT]) {
        camera->posicao_x-=pixel_por_x*V_CAMERA;
	}

	if (teclado[SDL_SCANCODE_UP]) {
        camera->posicao_y+=pixel_por_y*V_CAMERA;
	}

    if (teclado[SDL_SCANCODE_DOWN]) {
        camera->posicao_y-=pixel_por_y*V_CAMERA;
	}

	SDL_SetRenderDrawColor(renderer, 211, 211,211, 255);
    for(int i = camera->posicao_x % camera->range*pixel_por_x/10; i < largura; i+=camera->range*pixel_por_x/10){
        SDL_RenderDrawLine(renderer, i,0,i, altura);
    }

    for(int i = camera->posicao_y % camera->range*pixel_por_y/10; i < largura; i+=camera->range*pixel_por_y/10){
        SDL_RenderDrawLine(renderer, 0,i,largura, i);
    }

    //Reta eixo x
    if(abs(camera->posicao_x) - camera->range*pixel_por_x <= camera->range*pixel_por_x){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        if(camera->posicao_x - camera->range*pixel_por_x <= 0){
            SDL_RenderDrawLine(renderer, camera->posicao_x - camera->range*pixel_por_x + 3*largura/2,0, camera->posicao_x - camera->range*pixel_por_x + 3*largura/2 ,altura);
        }
        else{
            SDL_RenderDrawLine(renderer, camera->posicao_x + camera->range*pixel_por_x + 3*largura/2,0, camera->posicao_x + camera->range*pixel_por_x + 3*largura/2 ,altura);
        }
    }

    //Reta eixo y
    if(abs(camera->posicao_y) - camera->range*pixel_por_y <= camera->range*pixel_por_y){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        if(camera->posicao_y - camera->range*pixel_por_y <= 0){
            SDL_RenderDrawLine(renderer, 0,camera->posicao_y - camera->range*pixel_por_y + 3*largura/2, largura,camera->posicao_y - camera->range*pixel_por_y + 3*largura/2);
        }
        else{
            SDL_RenderDrawLine(renderer, 0,camera->posicao_y + camera->range*pixel_por_y + 3*largura/2,largura, camera->posicao_y + camera->range*pixel_por_y + 3*largura/2);
        }
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

	janela = SDL_CreateWindow( "Kplot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largura_janela, altura_janela, SDL_WINDOW_SHOWN );
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
