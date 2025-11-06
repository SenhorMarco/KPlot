#include "ksdl.h"

int ksdl::frame::duracao(){
    ultima_duracao = tempo_final - tempo_inicial;
    return tempo_final - tempo_inicial;
}

ksdl::equacao::equacao(std::string string_expressao, SDL_Color cor){
    this->cor = cor;
    this->string_expressao = string_expressao;
}


