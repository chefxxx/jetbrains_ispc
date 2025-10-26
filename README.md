# ❄️ ISPC Newton fractal

This repository demonstrates how to generate a **Newton fractal** using both **serial C++ code** and an **ISPC
(Intel SPMD Program Compiler)** implementation, and then **measure performance differences** between the two approaches.  


## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Repository Structure](#repository-structure)
- [Building & Running in CLion](#building--running-in-clion)
- [License](#-license)


## Overview

- The CMake configuration handles compiling `.ispc` files, linking them appropriately, and exposing targets you can run.
- From CLion, you simply click **Run** or **Debug**, and everything — C++ and ISPC — builds and executes.
- There is an additional header, `timing.h`, included for measuring execution time. (This was adapted from Intel’s sample code.)


## Prerequisites

Before building, ensure you have:

1. **ISPC compiler** installed and accessible in your `PATH` (or specify its location).
2. A working **C++ compiler** and toolchain (e.g. GCC, Clang).
3. **CMake** (minimum version requirement as defined in `CMakeLists.txt`).
4. (Optional) CLion or another IDE that supports CMake — but you can also build from the command line.

If ISPC is not in your `PATH`, you may need to adjust the CMake variables (e.g. `ISPC_EXECUTABLE`) to point to it.


## Repository Structure

Here is the top-level layout and what each part does:
```
/
├── CMakeLists.txt
├── main.cpp
├── ispc/
│ ├── newton.ispc
│ └── complex.ispc
└── include/
  ├── timing.h
  ├── colours.h
  └── newton_cxx.h
```

### Key Components

- **CMakeLists.txt**  
  Handles automatic ISPC compilation, linking, and build target setup.

- **mian.cpp**  
  C++ source file, main entry point.

- **ispc/**  
  Stores `.ispc` files with the parallel ISPC implementation.

- **include/**  
  Contains headers shared across the project, including `timing.h`.


## 🛠️ Building & Running in CLion

Thanks to the integrated CMake configuration:

1. Open the project in **CLion**.
2. Let it load the CMake configuration.
3. Select the main target and click **Run** or **Debug**.

CLion automatically:
- Invokes ISPC for `.ispc` files,
- Compiles C++ sources,
- Links everything together,
- Runs the program with no manual setup needed.

Alternatively, you can build from the command line:

```bash
mkdir build
cd build
cmake ..
cmake --build .
./newton_frac
```

## 📑 License

This project is licensed under GPL-3.0 license. See the [LICENSE](LICENSE) file for the full license text.
