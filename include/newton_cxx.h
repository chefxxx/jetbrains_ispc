//
// Created by Mateusz Mikiciuk on 25/10/2025.
//

#ifndef NEWTON_CXX_H
#define NEWTON_CXX_H

#include <complex>

inline std::complex<float> Func(const std::complex<float> &z, const int n) {
    return std::pow(z, static_cast<float>(n)) - std::complex(1.0f, 0.0f);
}

inline std::complex<float> Deriv(const std::complex<float> &z, const int n) {
    const std::complex tmp{static_cast<float>(n), 0.f};
    return tmp * std::pow(z, static_cast<float>(n - 1));
}

inline void perform_newton_cxx(const float re, const float im, const int MAX_ITERS, const int IDX,
                               std::unique_ptr<int[]> &iters, std::unique_ptr<int[]> &found_roots,
                               const std::unique_ptr<float[]> &real, const std::unique_ptr<float[]> &imag,
                               const int n_roots) {
    std::complex z{re, im};

    for (int iter = 0; iter < MAX_ITERS; ++iter) {
        z -= Func(z, n_roots) / Deriv(z, n_roots);
        constexpr float tol = 0.000001;
        for (int i = 0; i < n_roots; ++i) {
            std::complex tmp_root{real[i], imag[i]};
            if (std::abs(z - tmp_root) < tol) {
                iters[IDX] = iter;
                found_roots[IDX] = i;
                return;
            }
        }
    }

    iters[IDX] = 0;
    found_roots[IDX] = n_roots;
}

inline void newton_cxx(const float x_min, const float y_min, const float x_max, const float y_max, const int WIDTH,
                       const int HEIGHT, const int MAX_ITERS, std::unique_ptr<int[]> &iters,
                       std::unique_ptr<int[]> &found_roots, const std::unique_ptr<float[]> &real,
                       const std::unique_ptr<float[]> &imag, const int n_roots) {
    const float dx = (x_max - x_min) / static_cast<float>(WIDTH);
    const float dy = (y_max - y_min) / static_cast<float>(HEIGHT);

    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            const float x = x_min + static_cast<float>(i) * dx;
            const float y = y_min + static_cast<float>(j) * dy;

            const int idx = j * WIDTH + i;
            perform_newton_cxx(x, y, MAX_ITERS, idx, iters, found_roots, real, imag, n_roots);
        }
    }
}

#endif // NEWTON_CXX_H
