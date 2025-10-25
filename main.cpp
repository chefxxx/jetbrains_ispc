#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>

#include "newton.h"

void usage(const std::string& pname)
{
    std::cerr << "USAGE: " << pname  << "[--n=<value>]\n";
    exit(EXIT_FAILURE);
}
const std::string argOpt = "--n=";

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;
constexpr int BUF_N = WIDTH * HEIGHT;
constexpr int MAX_ITERS = 128;
constexpr float X_MIN = -2.5f;
constexpr float X_MAX = 1.0f;
constexpr float Y_MIN = -2.0f;
constexpr float Y_MAX = 1.0f;

void initRoots(const ispc::Roots* ptr) {
    for (int k = 0; k < ptr->size; ++k) {
        const float angle = M_PI * 2.0f * k / ptr->size;
        ptr->real[k] = cos(angle);
        ptr->imag[k] = sin(angle);
    }
}

void writePMM(
    const std::unique_ptr<int[]>& iters,
    const std::unique_ptr<int[]>& found_roots,
    const int size,
    const std::string& fn)
{
    std::ofstream ofs(fn, std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Could not open file " << fn << " for writing.\n";
    }

    ofs << "P6\n" << WIDTH << ' ' << HEIGHT << "\n255\n";
    for (int i = 0; i < BUF_N; ++i) {
        const float t = static_cast<float>(found_roots[i]) / static_cast<float>(size);
        const float brightness = 1.0f - static_cast<float>(iters[i]) / static_cast<float>(MAX_ITERS);
    }
}

using namespace ispc;

int main (const int argc, const char **argv) {
    // ---------
    // Read args
    // ---------
    int n = -1;
    if (argc < 2) {
        n = 3;
    }
    else if (argc == 2) {
        if (strncmp(argv[1], argOpt.c_str(), argOpt.size()) == 0) {
            n = static_cast<int>(strtol(argOpt.c_str() + argOpt.size(), nullptr, 10));
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
    Result result{};
    result.iters = iters.get();
    result.found_roots = found_roots.get();

    newton_ispc(X_MIN, Y_MIN, X_MAX, Y_MAX, WIDTH, HEIGHT, MAX_ITERS, roots , result);

    return EXIT_SUCCESS;
}