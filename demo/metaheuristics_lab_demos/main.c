#include "data/r2_r_example_functions.h";
#include "sa/sa_r2_r_basic.h"

void ten_cones_demo() {
    r2_cart_t guess;
    guess.x = 0;
    guess.y = 0;

    sa_config_t saConfig;
    saConfig.guess = (void*)&guess;
    saConfig.temperature = 1400;
    saConfig.cooldown = 0.999;
    saConfig.epochs = 100;
    saConfig.epoch_iters = 1;
    saConfig.mem_mode = WITH_MEMORY;

    //double solution = sa_r2_r_basic_extreme()


}

void fakir_sin_demo() {
    //implement demo
}

int main() {
    
    
    return 0;
}