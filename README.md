# Margolus CPP: Reversible cellular automaton simulator

[![CodeFactor](https://www.codefactor.io/repository/github/magnetrwn/margolus-cpp/badge)](https://www.codefactor.io/repository/github/magnetrwn/margolus-cpp)

Margolus CPP (`margolus-cpp`) is a simulator for different reversible cellular automata based on the [Margolus neighborhood](https://en.wikipedia.org/wiki/Block_cellular_automaton).

## Asciinema Demo
The following demo shows Margolus CPP running with a Critters ruleset (you can see `demo1()` and `demo2()` in `src/main.cpp`, alternating every few seconds).

[![asciicast](https://asciinema.org/a/qB18st3xDxeRCZtGxPj10cpAW.svg)](https://asciinema.org/a/qB18st3xDxeRCZtGxPj10cpAW)

## Build

To build Margolus CPP on Linux systems, you can run `./build.sh` from project root.
Other platforms such as Windows might require slight modifications, such as in the use of `<unistd.h>` for `usleep()`, and will need you to manually run `cmake` inside the build folder.

## Configuration

At the moment, the only config file is `margolus.ini`, which contains the transformations list to apply on a block that is found to match a transform condition (for now only three alive cells in a block, but will make into a proper INI soon).

## Usage as CLI utility

Output of `./margolus -h`:

```
Available command line options:
  -h, --help                    display help message
  -W, --width arg (=36)         horizontal size of the grid
  -H, --height arg (=14)        vertical size of the grid
  -i, --iters arg (=0)          iterations to run, positive forwards, negative backwards
  -a, --animated                run with a 75ms step animation
  --config arg (=margolus.ini)  location of the config INI file
  --demo                        run the demo loop

```
**Note:** in a few commits I will add grid loading and saving, making the program actually usable.
Running `./margolus` with no arguments will just display the state of the grid, with default width and height, without stepping the simulation, but additional parameters can change the behaviour of the program.
For example, when running `./margolus -i-100 -a`, a 100 step animation will run backward steps of the grid using the default ruleset from `margolus.ini`.

## Usage as library

Here is a sample program using Margolus CPP.

```cpp
#include <iostream>

#include "margolus.hpp"
#include "render.hpp"

int main() {
    // Create a Margolus simulator with a 10x10 grid and specified block transforms
    std::vector<std::string> transforms = { "ROTATE_90_LEFT", "INVERT" };
    Margolus margolusSimulator(10, 10, transforms);

    // Fill a rectangular region with random cells
    margolusSimulator.fillRect(2, 2, 7, 7, Margolus::NOISE, 0.8);

    // Fill a rectangular region with alive cells
    margolusSimulator.fillRect(3, 3, 6, 6, Margolus::UP);

    // Display the initial grid using the render functionality
    std::cout << "Initial Grid:" << std::endl;
    RenderGrid::basicANSI(margolusSimulator.getGrid());

    // Perform 20 forward steps
    for (size_t i = 0; i < 20; i++)
        margolusSimulator.step(Margolus::FORWARD);

    // Show current grid
    std::cout << "Grid after 20 forward steps:" << std::endl;
    RenderGrid::basicANSI(margolusSimulator.getGrid(), margolusSimulator.getOffset());

    // Perform 20 backward steps
    for (size_t i = 0; i < 20; i++)
        margolusSimulator.step(Margolus::BACKWARD);

    // Show current grid
    std::cout << "Grid after 20 backward steps:" << std::endl;
    RenderGrid::basicANSI(margolusSimulator.getGrid(), margolusSimulator.getOffset());


    return 0;
}

```

Right now the only renderer is `RenderGrid::basicANSI()`, which will output to STDOUT with nice colors, but other renderers will be added soon (SDL or Cairo sound good, maybe with GTK3 if I feel like it).
