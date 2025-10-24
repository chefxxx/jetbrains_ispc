#include <iostream>
#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <cstdlib>

#include "newton.h"

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
void usage(const char *pname)
{
    fprintf(stderr, "USAGE:%s [--n=<value>]\n", pname);
    exit(EXIT_FAILURE);
}
const std::string argOpt = "--n=";

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;
constexpr int BUF_N = WIDTH * HEIGHT;
constexpr int MAX_ITERS = 256;
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

    const std::unique_ptr<int[]> buf(new int[BUF_N]);
    Roots roots{};
    roots.imag = new float[n];
    roots.real = new float[n];
    roots.size = n;
    initRoots(&roots);

    newton_ispc(X_MIN, Y_MIN, X_MAX, Y_MAX, WIDTH, HEIGHT, MAX_ITERS, roots ,buf.get());

    delete[] roots.real;
    delete[] roots.imag;
    return EXIT_SUCCESS;
}