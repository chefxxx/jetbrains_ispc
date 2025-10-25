/*
    Created by chefxxx on 25.10.2025.
*/

struct Complex {
    float re;
    float im;
};

static inline Complex make_complex(float x, float y)
{
    Complex res;
    res.re = x;
    res.im = y;
    return res;
}

static inline Complex c_pow(Complex z, int n) {
    float r = sqrt(z.re * z.re + z.im * z.im);
    float theta = atan2(z.im, z.re);
    float r_n = pow(r, (float)n);

    Complex res;
    res.re = r_n * cos(n * theta);
    res.im = r_n * sin(n * theta);
    return res;
}

static inline Complex c_div(Complex z1, Complex z2) {
    Complex res;
    float denominator = z2.re * z2.re + z2.im * z2.im;
    res.re = (z1.re * z2.re + z1.im * z2.im) / denominator;
    res.im = (z1.im * z2.re - z1.re * z2.im) / denominator;

    return res;
}

static inline Complex c_sub(Complex z1, Complex z2) {
    Complex res;
    res.re = z1.re - z2.re;
    res.im = z1.im - z2.im;
    return res;
}

static inline Complex c_mul(Complex z, int n)
{
    Complex res;
    res.re = z.re * n;
    res.im = z.im * n;
    return res;
}