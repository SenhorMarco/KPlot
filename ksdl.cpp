#include "ksdl.h"

int ksdl::frame::duracao(){
    ultima_duracao = tempo_final - tempo_inicial;
    return tempo_final - tempo_inicial;
}
