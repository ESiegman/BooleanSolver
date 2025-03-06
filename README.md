# BooleanSolver

BooleanSolver is a C++ project designed to solve boolean expressions efficiently. This README provides instructions on how to compile and use the project using Nix and CMake.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
  - [Using Nix Shell (NixOS users only)](#using-nix-shell-nixos-users-only)
  - [Using CMake](#using-cmake)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction
BooleanSolver is a powerful tool for evaluating boolean expressions. It is implemented in C++ and uses CMake for the build process.

## Features
- Efficient boolean expression solving
- Easy-to-use API
- Cross-platform support

## Installation
### Using Nix Shell (NixOS users only)
#### Prerequisites
- Nix package manager (version 2.4 or higher)

#### Setting up Development Shell
1. Clone the repository:
   ```sh
   git clone https://github.com/ESiegman/BooleanSolver.git
   cd BooleanSolver
   ```

2. Start the Nix development shell:
   ```sh
   nix-shell
   ```

This command will set up the development environment with all the necessary dependencies.

### Using CMake
#### Prerequisites
- C++ compiler (e.g., g++, clang++)
- CMake (version 3.10 or higher)

#### Compilation
1. Ensure you are in the Nix development shell if you are a NixOS user (see above). Otherwise, proceed with the steps below.

2. Create a build directory and navigate into it:
   ```sh
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```sh
   cmake ..
   ```

4. Compile the project:
   ```sh
   cmake --build .
   ```

5. The compiled binaries will be placed in the `build` directory.

## Usage
To use the `BooleanSolver`, run the compiled binary in the `build` directory.
```sh
./BooleanSolver
```

Equations are written in plain English, for example, "A xor B and (C or D)"

## Contributing
Contributions are welcome! Please follow these steps to contribute:
1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes.
4. Push to the branch.
5. Create a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
