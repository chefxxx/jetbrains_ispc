#include <iostream>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void usage(const char *pname)
{
    fprintf(stderr, "USAGE:%s [--n=<value>]\n", pname);
    exit(EXIT_FAILURE);
}

template <size_t N>
double func(double z) {

}

const std::string argOpt = "--n=";

// int main (const int argc, const char **argv) {
//     long n = -1;
//     if (argc < 2) {
//         n = 3;
//     }
//     else if (argc == 2) {
//         if (strncmp(argv[1], argOpt.c_str(), argOpt.size()) == 0) {
//             n = strtol(argOpt.c_str() + argOpt.size(), nullptr, 10);
//         }
//     }
//     else {
//         usage(argv[0]);
//     }
//
//     return EXIT_SUCCESS;
// }

/*
  Copyright (c) 2010-2023, Intel Corporation

  SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdio.h>
#include <stdlib.h>

// Include the header file that the ispc compiler generates
#include "test.h"
using namespace ispc;

int main() {
    float vin[16], vout[16];

    // Initialize input buffer
    for (int i = 0; i < 16; ++i)
        vin[i] = (float)i;

    // Call simple() function from simple.ispc file
    simple(vin, vout, 16);

    // Print results
    for (int i = 0; i < 16; ++i)
        printf("%d: simple(%f) = %f\n", i, vin[i], vout[i]);

    return 0;
}
