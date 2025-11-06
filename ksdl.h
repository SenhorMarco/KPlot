#include "exprtk.hpp"
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

namespace ksdl{
    class frame{
    public:
        int tempo_inicial;
        int tempo_final;
        int ultima_duracao;
        int duracao();
    };

    class camera{
    public:
        int posicao_x;
        int posicao_y;
        float posicao_numerica_x;
        float posicao_numerica_y;
        int range_x;
        int range_y;
    };

    class equacao{
    public:
        equacao(std::string string_expressao, SDL_Color cor);
        SDL_Color cor;
        std::string string_expressao;
        exprtk::expression<float> expressao;
    };
}


