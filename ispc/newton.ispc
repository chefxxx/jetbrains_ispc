/*
    Created by chefxxx on 24.10.2025.
*/

#include "complex.ispc"

struct Roots {
    float* real;
    float* imag;
    int size;
};

struct Result {
    int *iters;
    int *found_roots;
};

static inline Complex Func(Complex z, int n) {
    Complex tmp = c_pow(z, n);
    tmp.re -= 1;
    return tmp;
}

static inline Complex Deriv(Complex z, int n) {
    return c_mul(c_pow(z, n - 1), n);
}

static inline void newton( varying float re, varying float im,
                           uniform int MAX_ITERS, varying int IDX,
                           uniform Result& result, uniform Roots& roots )
{
    Complex z;
    z.re = re;
    z.im = im;

    for (int i = 0; i < MAX_ITERS; ++i) {
        Complex tmp = c_div(Func(z, roots.size), Deriv(z, roots.size));

    }
}

export void newton_ispc( uniform float x_min, uniform float y_min,
                         uniform float x_max, uniform float y_max,
                         uniform int WIDTH, uniform int HEIGHT,
                         uniform int MAX_ITERS,
                         uniform Roots& roots,
                         uniform Result& result )
{
    float dx = (x_max - x_min) / WIDTH;
    float dy = (y_max - y_min) / HEIGHT;

    for (uniform int j = 0; j < HEIGHT; j++) {
        foreach (i = 0 ... WIDTH) {
            float x = x_min + i * dx;
            float y = y_min + j * dy;

            /* x - real part, y - imaginary part */
            int idx = j * WIDTH + i;
            newton(x, y, MAX_ITERS, idx, result, roots);
        }
    }
}
