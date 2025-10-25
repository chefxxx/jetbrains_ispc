/*
    Created by chefxxx on 24.10.2025.
*/

#include "complex.ispc"

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
                           uniform int iters[], uniform int found_roots[],
                           uniform float real[], uniform float imag[],
                           uniform int n_roots )
{
    Complex z = make_complex(re, im);

    for (int iter = 0; iter < MAX_ITERS; ++iter) {
        Complex tmp = c_div(Func(z, n_roots), Deriv(z, n_roots));
        z = c_sub(z, tmp);

        float tol = 0.000001;
        for (int i = 0; i < n_roots; ++i) {
            Complex tmp_root = make_complex(real[i], imag[i]);
            Complex diff = c_sub(z, tmp_root);
            if (abs(diff.re) < tol && abs(diff.im) < tol)
            {
                iters[IDX] = iter;
                found_roots[IDX] = i;
                return;
            }
        }
    }

    iters[IDX] = 0;
    found_roots[IDX] = n_roots;
}

export void newton_ispc( uniform float x_min, uniform float y_min,
                         uniform float x_max, uniform float y_max,
                         uniform int WIDTH, uniform int HEIGHT,
                         uniform int MAX_ITERS,
                         uniform int iters[], uniform int found_roots[],
                         uniform float real[], uniform float imag[],
                         uniform int n_roots )
{
    float dx = (x_max - x_min) / WIDTH;
    float dy = (y_max - y_min) / HEIGHT;

    for (uniform int j = 0; j < HEIGHT; j++) {
        foreach (i = 0 ... WIDTH) {
            float x = x_min + i * dx;
            float y = y_min + j * dy;

            /* x - real part, y - imaginary part */
            int idx = j * WIDTH + i;
            newton(x, y, MAX_ITERS, idx, iters, found_roots, real, imag, n_roots);
        }
    }
}
