#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "colours.h"
#include "newton.h"
#include "newton_cxx.h"
#include "timing.h"

void usage(const std::string &pname) {
    std::cerr << "USAGE: " << pname << " [--n=<value>]\n";
    exit(EXIT_FAILURE);
}

// ----------------
// Fractal settings
// ----------------
constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;
constexpr int BUF_N = WIDTH * HEIGHT;
constexpr int MAX_ITERS = 64;
constexpr float X_MIN = -2.5f;
constexpr float X_MAX = 2.5f;
constexpr float Y_MIN = -2.5f;
constexpr float Y_MAX = 2.5f;

// --------------
// Tests settings
// --------------
constexpr int TEST_ITERS = 3;

void initRoots(const std::unique_ptr<float[]> &real, const std::unique_ptr<float[]> &imag, const int n_roots) {
    for (int k = 0; k < n_roots; ++k) {
        const float angle = M_PI * 2.0f * k / n_roots;
        real[k] = cos(angle);
        imag[k] = sin(angle);
    }
}

void clearBuff(std::unique_ptr<int[]> &iters, std::unique_ptr<int[]> &found_roots) {
    iters.reset(new int[BUF_N]);
    found_roots.reset(new int[BUF_N]);
}

void writePPM(const std::unique_ptr<int[]> &iters, const std::unique_ptr<int[]> &found_roots, const int size,
              const std::string &fn) {
    std::ofstream ofs(fn, std::ios::binary);
    if (!ofs.is_open()) {
        throw std::runtime_error("Could not open file " + fn);
    }

    ofs << "P6\n" << WIDTH << ' ' << HEIGHT << "\n255\n";
    for (int i = 0; i < BUF_N; ++i) {
        RGB c = {0, 0, 0};

        if (found_roots[i] < size) {
            const float H = static_cast<float>(found_roots[i]) / static_cast<float>(size);
            const float V = 1.0f - static_cast<float>(iters[i]) / static_cast<float>(MAX_ITERS);
            c = HSVtoRGB(H, 1.0f, V);
        }
        ofs.put(static_cast<char>(c.r));
        ofs.put(static_cast<char>(c.g));
        ofs.put(static_cast<char>(c.b));
    }

    std::cout << "Wrote image file " << fn << '\n';
}

using namespace ispc;

int main(const int argc, const char **argv) {
    // ---------
    // Read args
    // ---------
    int n = -1;
    if (argc < 2) {
        n = 3;
    } else if (argc == 2) {
        if (strncmp(argv[1], "--n=", 4) == 0) {
            n = static_cast<int>(strtol(argv[1] + 4, nullptr, 10));
        } else {
            usage(argv[0]);
        }
    } else {
        usage(argv[0]);
    }

    const std::unique_ptr<float[]> real(new float[n]);
    const std::unique_ptr<float[]> imag(new float[n]);
    initRoots(real, imag, n);
    std::unique_ptr<int[]> iters;
    std::unique_ptr<int[]> found_roots;

    double min_ISPC = 1e30;
    for (int i = 0; i < TEST_ITERS; ++i) {
        clearBuff(iters, found_roots);
        reset_and_start_timer();
        newton_ispc(X_MIN, Y_MIN, X_MAX, Y_MAX, WIDTH, HEIGHT, MAX_ITERS, iters.get(), found_roots.get(), real.get(),
                    imag.get(), n);
        const double dt = get_elapsed_mcycles();
        std::cout << "@time of ISPC run:\t\t\t[" << dt << "] million cycles\n";
        min_ISPC = std::min(min_ISPC, dt);
    }

    std::cout << "@newton ispc best:\t\t\t[" << min_ISPC << "] million cycles\n";
    writePPM(iters, found_roots, n, "newton.ppm");

    double min_serial = 1e30;
    for (int i = 0; i < TEST_ITERS; ++i) {
        clearBuff(iters, found_roots);
        reset_and_start_timer();
        newton_cxx(X_MIN, Y_MIN, X_MAX, Y_MAX, WIDTH, HEIGHT, MAX_ITERS, iters, found_roots, real, imag, n);
        const double dt = get_elapsed_mcycles();
        std::cout << "@time of serial run:\t\t[" << dt << "] million cycles\n";
        min_serial = std::min(min_serial, dt);
    }

    std::cout << "@newton serial best:\t\t[" << min_serial << "] million cycles\n";
    writePPM(iters, found_roots, n, "newton_serial.ppm");

    double min_ISPC_tasks = 1e30;
    for (int i = 0; i < TEST_ITERS; ++i) {
        clearBuff(iters, found_roots);
        reset_and_start_timer();
        newton_ispc_tasks(X_MIN, Y_MIN, X_MAX, Y_MAX, WIDTH, HEIGHT, MAX_ITERS, iters.get(), found_roots.get(),
                          real.get(), imag.get(), n);
        const double dt = get_elapsed_mcycles();
        std::cout << "@time of ISPC tasks run:\t[" << dt << "] million cycles\n";
        min_ISPC_tasks = std::min(min_ISPC_tasks, dt);
    }

    std::cout << "@newton ISPC tasks best:\t[" << min_ISPC_tasks << "] million cycles\n";
    writePPM(iters, found_roots, n, "newton_tasks.ppm");

    std::cout << "\n\t\t\t\t(" << min_serial / min_ISPC << "x speedup from ISPC)\n";
    std::cout << "\n\t\t\t\t(" << min_serial / min_ISPC_tasks << "x speedup from ISPC tasks)\n";
    std::cout << "\n\t\t\t\t(" << min_ISPC / min_ISPC_tasks << "x speedup between ISPC and ISPC tasks)\n";
    return EXIT_SUCCESS;
}