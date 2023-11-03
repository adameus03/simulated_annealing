#include "../data/r2_r_example_functions.h"
#include "../sa/sa_r2_r_basic.h"
#include <stdio.h>

void* _fakir_sin(const void* arg) {
    static double output = 2137;
    output = fakir_sin(((r2_cart_t*)arg)->x, ((r2_cart_t*)arg)->y);
    return &output;
}

void* _oh_my_cosh(const void* arg) {
    static double output = 2137;
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

    r2_cart_t solution = sa_r2_r_basic_extreme(_fakir_sin, saConfig, constraint);
    puts("\n=========== Fakir sin global extreme obtained ===========");
    printf("Solution X: %4.2f\n", solution.x);
    printf("Solution Y: %4.2f\n", solution.y);
    printf("Extreme value: %4.5f\n\n", fakir_sin(solution.x, solution.y));
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

    r2_cart_t solution = sa_r2_r_basic_extreme(_oh_my_cosh, saConfig, constraint);
    puts("\n=========== Oh my cosh global extreme obtained ===========");
    printf("Solution X: %4.5f\n", solution.x);
    printf("Solution Y: %4.5f\n", solution.y);
    printf("Extreme value: %4.5f\n\n", oh_my_cosh(solution.x, solution.y));
}

int main() {
    fakir_sin_demo();
    oh_my_cosh_demo();
    return 0;
}
