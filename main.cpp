#include <iostream>
#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <cstdlib>

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

struct Roots {
    float* real;
    float* imag;
    int size;
};

struct rootsDeleter {
    void operator()(Roots* r) const noexcept {
        delete[] r->real;
        delete[] r->imag;
        delete r;
    }
};

void initRoots(const Roots* ptr) {
    for (int k = 0; k < ptr->size; ++k) {
        const float angle = M_PI * 2.0f * k / ptr->size;
        ptr->real[k] = cos(angle);
        ptr->imag[k] = sin(angle);
    }
}

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

    std::unique_ptr<int[]> buf(new int[BUF_N]);
    constexpr rootsDeleter deleter;
    const std::unique_ptr<Roots, rootsDeleter> roots(new Roots, deleter);
    roots->imag = new float[n];
    roots->real = new float[n];
    roots->size = n;
    initRoots(roots.get());
    for (int i = 0; i < roots->size; ++i) {
        std::cout << "(" << roots->real[i] << ", " << roots->imag[i] << ")" << "\n";
    }
    return EXIT_SUCCESS;
}