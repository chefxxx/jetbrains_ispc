#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>

#include "newton.h"
#include "colours.h"

void usage(const std::string& pname)
{
    std::cerr << "USAGE: " << pname  << " [--n=<value>]\n";
    exit(EXIT_FAILURE);
}

// --------
// Settings
// --------
constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;
constexpr int BUF_N = WIDTH * HEIGHT;
constexpr int MAX_ITERS = 64;
constexpr float X_MIN = -2.5f;
constexpr float X_MAX = 2.5f;
constexpr float Y_MIN = -2.5f;
constexpr float Y_MAX = 2.5f;

void initRoots(const ispc::Roots* ptr) {
    for (int k = 0; k < ptr->size; ++k) {
        const float angle = M_PI * 2.0f * k / ptr->size;
        ptr->real[k] = cos(angle);
        ptr->imag[k] = sin(angle);
    }
}

void writePPM(
    const std::unique_ptr<int[]>& iters,
    const std::unique_ptr<int[]>& found_roots,
    const int size,
    const std::string& fn)
{
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

int main (const int argc, const char **argv) {
    // ---------
    // Read args
    // ---------
    int n = -1;
    if (argc < 2) {
        n = 5;
    }
    else if (argc == 2) {
        if (strncmp(argv[1], "--n=", 4) == 0) {
            n = static_cast<int>(strtol(argv[1] + 4, nullptr, 10));
        }
        else {
            usage(argv[0]);
        }
    }
    else {
        usage(argv[0]);
    }

    const std::unique_ptr<float[]> imag(new float[n]);
    const std::unique_ptr<float[]> real(new float[n]);
    Roots roots{};
    roots.imag = imag.get();
    roots.real = real.get();
    roots.size = n;
    initRoots(&roots);

    const std::unique_ptr<int[]> iters(new int[BUF_N]);
    const std::unique_ptr<int[]> found_roots(new int[BUF_N]);

    newton_ispc(X_MIN, Y_MIN, X_MAX, Y_MAX, WIDTH, HEIGHT, MAX_ITERS, iters.get(), found_roots.get(), roots);

    writePPM(iters, found_roots, n, "newton.ppm");
    return EXIT_SUCCESS;
}