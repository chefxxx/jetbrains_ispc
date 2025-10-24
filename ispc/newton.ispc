/*
    Created by chefxxx on 24.10.2025.
*/

struct Roots {
    float* real;
    float* imag;
    int size;
};

static inline float* Func( float re, float im, int n ) {

}

export void newton_ispc( uniform float x_min, uniform float y_min,
                         uniform float x_max, uniform float y_max,
                         uniform int WIDTH, uniform int HEIGHT,
                         uniform int MAX_ITERS,
                         uniform Roots& roots,
                         uniform int output[] )
{
    float dx = (x_max - x_min) / WIDTH;
    float dy = (y_max - y_min) / HEIGHT;

    for (uniform int j = 0; j < height; j++) {
        foreach (i = 0 ... width) {

        }
    }
}
