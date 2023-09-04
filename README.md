# Margolus CPP: Reversible cellular automaton simulator

## *WARNING: OBSOLETE*

[![CodeFactor](https://www.codefactor.io/repository/github/magnetrwn/margolus-cpp/badge)](https://www.codefactor.io/repository/github/magnetrwn/margolus-cpp)

Margolus CPP (`margolus-cpp`) is a simulator for different reversible cellular automata based on the [Margolus neighborhood](https://en.wikipedia.org/wiki/Block_cellular_automaton).

The behaviour of the simulator can be controlled by setting the ruleset used to control how each 2x2 block is changed for each iteration.

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
  -h, --help                Display help message.
  -l, --load arg            Path to the file to load.
  -s, --save arg            Path to the file to save, once finished.
  -S, --save-stdout         Show output file on stdout instead of an actual file.
  -i, --iters arg (=0)      Iterations to run, positive forwards, negative backwards.
  -r, --render arg (=ANSI)  Engine to use to render grid, options: q, ANSI, SDL2.
  -o, --odd-flip            Flip render colors on odd iterations.
  -a, --animated            Run with a 75ms step animation.
```
Running `./margolus` with no arguments will just display the state of the grid, with default width and height, without stepping the simulation, but additional parameters can change the behaviour of the program.

For example, when running `./margolus -i-100 -a`, a 100 step animation will run backward steps of the grid using the default ruleset from `margolus.ini`.

## Usage as library

Here is a sample program using Margolus CPP.

```cpp
#include <iostream>

#include "margolus.hpp"
#include "marg_render.hpp"

int main() {
    // Create a Margolus simulator with a 10x10 grid and specified block transforms
    std::vector<std::string> transforms = { "ROTATE_90_LEFT", "INVERT" };
    Margolus marg(10, 10, transforms);

    // Fill a rectangular region with random cells
    marg.fillRect(2, 2, 7, 7, Margolus::NOISE, 0.8);

    // Fill a specific point by swapping its value
    marg.fillPoint(3, 3, 6, 6, Margolus::TOGGLE);

    // You can also access the grid directly through indexes
    marg[3][8] = true;

    // Display the initial grid using the render functionality
    std::cout << "Initial Grid:" << std::endl;
    MargolusRender::basicANSI(marg);

    // Perform 20 forward steps
    for (size_t i = 0; i < 20; i++)
        marg.step(Margolus::FORWARD);

    // Show current grid
    std::cout << "Grid after 20 forward steps:" << std::endl;
    MargolusRender::basicANSI(marg, marg.getOffset());

    // Perform 20 backward steps
    for (size_t i = 0; i < 20; i++)
        marg.step(Margolus::BACKWARD);

    // Show current grid
    std::cout << "Grid after 20 backward steps:" << std::endl;
    MargolusRender::basicANSI(marg, marg.getOffset());


    return 0;
}

```

Right now the only renderer is `MargolusRender::basicANSI()`, which will output to STDOUT with nice colors, but other renderers will be added soon (SDL or Cairo sound good, maybe with GTK3 too).

## Contributing

If you feel like contributing, feel free to send issues and PRs! If you use VS Code, the `clangd` extension can be used with the project automatically detecting location of files after running `./build.sh` at least once.