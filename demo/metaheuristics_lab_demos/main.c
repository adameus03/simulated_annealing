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
    

}

void fakir_sin_demo() {
    
}

int main() {
    
    
    return 0;
}