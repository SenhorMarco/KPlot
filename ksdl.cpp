#include "exprtk_wrapper.h"
#include "ksdl.h"
#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

int ksdl::frame::duracao(){
    ultima_duracao = tempo_final - tempo_inicial;
    return tempo_final - tempo_inicial;
}

ksdl::equacao::equacao(std::string string_expressao, SDL_Color cor){
    this->cor = cor;
    this->string_expressao = string_expressao;
}


