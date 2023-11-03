#define __USE_MISC
#include <math.h>

double fakir_sin(const double x, const double y) {
    return 21.5 
            + x * sin(4 * M_PI * x) 
            + y * sin(20 * M_PI * y);
}

double oh_my_cosh(const double x, const double y) {
    const double xp12 = x + 12;
    const double yp12 = y + 12;
    const double ym12 = y - 12;
    const double cxm12 = cosh(x - 12);
    const double cyp12 = cosh(yp12); 
    return 8 * exp(-xp12*xp12 - yp12*yp12) 
    + 9 / (1 + xp12*xp12 + ym12*ym12) 
    + 20 / (cxm12*cxm12 + cyp12*cyp12) 
    + 176 / ((exp(x - 12) + 2 + exp(-x + 12)) * (exp(y - 12) + 2 + exp(-y + 12)));
}

double ten_cones(const double x, const double y) {
    static const double conesX[10] = {
        0.0000, 293.8926, 475.5283, 475.5283, 293.8926, 
        0.0000, -293.8926, -475.5283, -475.5283, -293.8926
    };
    static const double conesY[10] = {
        500.0000, 404.5085, 154.5085, -154.5085, -404.5085,
        -500.0000, -404.5085, -154.5085, 154.5085, 404.5085
    };
    static const double conesH[10] = {
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1
    };
    static const double r = 25;
    static const double rSquared = 625;
    for (unsigned char i = 0; i < 10; i++) {
        static double xDiff;
        static double yDiff;
        xDiff = conesX[i] - x;
        yDiff = conesY[i] - y;
        static double distSquared;
        distSquared = xDiff * xDiff + yDiff * yDiff;
        if (distSquared < rSquared) {
            return (1 - sqrt(distSquared) / r) * conesH[i];
        }
    }
    return 0;
}