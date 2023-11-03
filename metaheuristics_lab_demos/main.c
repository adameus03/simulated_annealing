#include "../data/r2_r_example_functions.h"
#include "batch.h"
#include <stdio.h>

void* _fakir_sin(const void* arg) {
    static double output;
    output = fakir_sin(((r2_cart_t*)arg)->x, ((r2_cart_t*)arg)->y);
    return &output;
}

void* _oh_my_cosh(const void* arg) {
    static double output;
    output = oh_my_cosh(((r2_cart_t*)arg)->x, ((r2_cart_t*)arg)->y);
    return &output;
}

void fakir_sin_demo() {
    r2_cart_t guess;
    guess.x = 4.5;
    guess.y = 4.95;

    sa_config_t saConfig;
    saConfig.guess = (void*)&guess;
    saConfig.temperature = 100;
    saConfig.cooldown = 0.999;
    saConfig.boltzmann_k = 0.2;
    saConfig.epochs = 7000;
    saConfig.epoch_iters = 10;
    saConfig.mem_mode = WITH_MEMORY;
    saConfig.emode = MAX;

    r2_cart_rect_constraint_t constraint;
    constraint.xMin = -3.0;
    constraint.xMax = 12.0;
    constraint.yMin = 4.1;
    constraint.yMax = 5.8;

    batch("Fakir sine", 1000, _fakir_sin, saConfig, constraint, QUIET);

}

void oh_my_cosh_demo() {
    r2_cart_t guess;
    guess.x = 0;
    guess.y = 0;

    sa_config_t saConfig;
    saConfig.guess = (void*)&guess;
    saConfig.temperature = 90;
    saConfig.cooldown = 0.999;
    saConfig.boltzmann_k = 0.5;
    saConfig.epochs = 200;
    saConfig.epoch_iters = 100;
    saConfig.mem_mode = WITH_MEMORY;
    saConfig.emode = MAX;

    r2_cart_rect_constraint_t constraint;
    constraint.xMin = -15.0;
    constraint.xMax = 15.0;
    constraint.yMin = -15.0;
    constraint.yMax = 15.0;

    batch("Oh my cosh", 1000, _oh_my_cosh, saConfig, constraint, QUIET);
}

int main() {
    fakir_sin_demo();
    oh_my_cosh_demo();
    return 0;
}
