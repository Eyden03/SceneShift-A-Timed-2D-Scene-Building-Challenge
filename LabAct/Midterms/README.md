# SceneShift: A Timed 2D Scene-Building Challenge

## Project Description

SceneShift is an interactive 2D scene-building game developed using C++ and OpenGL/GLUT.

The player is given a target environment and must recreate it by changing five scene categories:

- Ground
- Building
- Time of Day
- Sky Feature
- Weather

The game contains 10 predefined objectives that are shuffled for every new playthrough. Each objective has a countdown timer, and the player must match as many of the five target categories as possible before submitting the scene or before the timer reaches zero.

SceneShift also includes animated environmental effects, scoring, three lives, Pause/Resume, Game Over, Final Results, and Play Again.

## Controls

### Keyboard

- `A` - Change Building
- `S` - Change Time of Day
- `D` - Change Weather
- `ENTER` - Play / Start / Submit / Continue
- `P` - Pause / Resume
- `R` - Play Again from Game Over or Final Results
- `ESC` - Exit the program

### Mouse

- `Left Click` - Change Ground
- `Right Click` - Change Sky Feature

Mouse scene editing only works inside the visible gameplay scene area.

## Game Rules

Each objective is evaluated using the five scene categories.

- `5 / 5` - PERFECT
- `4 / 5` - PASS
- `0-3 / 5` - FAIL

The player starts with three lives. A failed objective removes one life.

The available time becomes shorter as the player progresses:

- Objectives 1-3 - 30 seconds
- Objectives 4-6 - 25 seconds
- Objectives 7-9 - 20 seconds
- Objective 10 - 15 seconds

The game ends when the player loses all three lives or completes all 10 objectives.

## Requirements

SceneShift was developed and tested on Windows using Visual Studio Code with the course OpenGL setup.

The project uses:

- C++17
- OpenGL
- FreeGLUT
- GLEW
- GLU
- MinGW / g++
- Visual Studio Code

The provided VS Code build configuration expects the bundled MinGW/OpenGL setup to be available in the project workspace.

## Compilation and Running

### Compile Using Visual Studio Code

1. Open the main project folder in Visual Studio Code.
2. Open `Midterms.cpp` and make sure it is the active file.
3. Press `Ctrl + Shift + B`.
4. Select `Build active OpenGL file` if prompted.
5. Wait for the build to finish.

The compiled executable will be created inside the `bin` folder as:

`bin\Midterms.exe`

The build task uses C++17 and links the required OpenGL libraries:

- GLEW
- FreeGLUT
- OpenGL
- GLU
- WinMM
- GDI32

### Run Using Visual Studio Code

1. Keep `Midterms.cpp` as the active file.
2. Press `F5`.
3. Select `Debug active OpenGL file` if prompted.
4. Visual Studio Code will automatically run the `Build active OpenGL file` task before launching SceneShift.

### Run the Compiled Program Manually

After compiling, the program can also be started from the workspace terminal using:

```powershell
.\bin\Midterms.exe
```

## OpenGL and Programming Concepts Used

SceneShift applies the following concepts from the course:

- C++ functions and variables
- Conditions
- Loops
- Ordinary arrays
- Vertex and color arrays
- OpenGL geometric primitives
- Colors and coordinates
- Keyboard callbacks
- Mouse callbacks
- Bitmap and stroke text
- OpenGL transformations
- GLUT timer callbacks
- Animation
- Multiple application states

## Main Creative Features

SceneShift includes:

- Animated Aurora using vertex and color arrays
- Animated clouds
- Animated rain
- Animated snowfall
- 10 predefined objectives with randomized order
- Progressive countdown timer
- Objective scoring
- Total Match Score
- Three-life system
- Pause and Resume
- Automatic submission when time reaches zero
- Objective Result screen
- Game Over and Final Results
- Play Again with a newly shuffled objective order

## Project Goal

The goal of SceneShift is to combine the OpenGL concepts learned throughout the course into one interactive application. Instead of creating only a static scene, the project allows the player to build different environments, respond to timed objectives, and interact with the program using both the keyboard and mouse.