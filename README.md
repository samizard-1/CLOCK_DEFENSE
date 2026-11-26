# CLOCK DEFENSE

CLOCK DEFENSE is a 2D arena survival shooter built with C++ and raylib. Protect the cannon in the center of the screen by aiming with the mouse and shooting gears and clocks before they reach you.

The game starts at 800x600, supports fullscreen, and has five progressively faster levels plus a secret level with a hidden true ending.

## Requirements

- Windows
- [raylib](https://www.raylib.com/) source files
- A MinGW-w64 C++ compiler, including `g++`
- Visual Studio Code (optional, for the included build and debug configuration)

The repository's VS Code configuration expects the following Windows paths:

- raylib source: `C:/raylib/raylib/src`
- MinGW-w64 kit: `C:/raylib/w64devkit`
- Compiler: `C:/raylib/w64devkit/bin/g++.exe`
- Debugger: `C:/raylib/w64devkit/bin/gdb.exe`

## Download and Play

No C++ build is required if the included `game.exe` is already compatible with your Windows system.

1. Keep `game.exe` in the project root.
2. Keep the `resources/` folder beside it.
3. Run `game.exe`.

Do not move or rename individual files inside `resources/`; the executable loads them relative to the current working directory.

## Play Instructions

### Controls

| Input | Action |
| --- | --- |
| Mouse | Aim the cannon |
| Left mouse button | Fire |
| F11 | Toggle fullscreen |
| Enter | Enter the secret level after winning the fifth level |
| R | Restart after winning or losing |
| Red on-screen `R` button | Restart after winning or losing |
| Escape | Close the game |

### Objective

- Destroy approaching enemies before they collide with the cannon.
- Normal gear enemies cost one life when they reach the cannon.
- Larger clock enemies cost two lives.
- Normal enemies increase the enemies-killed counter.
- The game starts with 10 lives and 10 ammo.
- Ammo regenerates automatically, one round about every second.
- The cannon upgrades at a random chance after 50 kills, increasing cannonball size.

### Levels

Enemy speed, size, spawn rate, and the maximum number of active enemies increase across five levels:

| Level | Kill target |
| --- | --- |
| 1 | 50 |
| 2 | 100 |
| 3 | 150 |
| 4 | 200 |
| 5 | 250 |

Each level has its own music. Level announcements appear in the center of the screen when a target is reached.

### Super enemies

Clock enemies are super enemies. Shooting one:

- Destroys it with a purple poof effect.
- Converts it into a faster, smaller regular gear enemy.
- May occasionally reincarnate a regular enemy as a new super enemy.

### Secret level and restart

After reaching 250 kills, press **Enter** to access the secret level. The secret level begins with 10 lives and 15 ammo; defeat 50 super enemies or regular enemies to unlock the true ending.

At a game-over or ending screen, press **R** or click the red `R` button to reset the entire run.

## Build the Project

### Option 1: Visual Studio Code

1. Install raylib and place it at `C:/raylib/raylib`.
2. Install or place the MinGW-w64 kit at `C:/raylib/w64devkit`.
3. Open this project folder in Visual Studio Code.
4. Open `main.cpp`.
5. Press `Ctrl+Shift+B` to run the default **Build Raylib Game** task.
6. Press `F5` to build and launch `game.exe`.

The workspace task in `.vscode/tasks.json` is defined as:

```powershell
g++ main.cpp -o game.exe `
  -IC:/raylib/raylib/src `
  -LC:/raylib/raylib/src `
  -lraylib -lopengl32 -lgdi32 -lwinmm
```

The working directory must remain the project root so that `resources/` is found.

### Option 2: Build from PowerShell

Set up raylib and MinGW-w64 in the paths listed above, then run:

```powershell
Set-Location "C:\Users\Sam\Documents\samgametest"
& "C:\raylib\w64devkit\bin\g++.exe" main.cpp -o game.exe `
  -IC:/raylib/raylib/src `
  -LC:/raylib/raylib/src `
  -lraylib -lopengl32 -lgdi32 -lwinmm
.\game.exe
```

If `g++` is available on `PATH`, the build command can also be shortened to:

```powershell
g++ main.cpp -o game.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
.\game.exe
```

## Project Structure

```text
main.cpp                 Game logic, rendering, input, and audio
game.exe                 Prebuilt Windows executable
resources/
  bg.png                 Main background
  cannon.png             Player cannon
  cannonball.png         Projectile
  clock.png              Super enemy
  gear.png               Normal enemy
  poof.png               4x4 destruction effect spritesheet
  clockfont.ttf          Game font
  L1.mp3 - L5.ogg        Level music
  secretmusic.mp3        Secret-level music
  shootsound.wav         Cannon sound
  enemy_death.wav        Enemy collision/destruction sound
.vscode/tasks.json       Default build task
.vscode/launch.json      Build-and-run launch configuration
.clangd                  clangd include configuration
```

## Troubleshooting

**`game.exe` starts, but images or audio are missing**

- Run the game from the project root.
- Do not remove or relocate `resources/`.
- Check that file and folder names match those listed above exactly.

**`g++` is not found**

- Add `C:\raylib\w64devkit\bin` to `PATH`, or use the full compiler path in the build command.

**The linker cannot find raylib**

- Confirm that `C:/raylib/raylib/src/raylib.h` exists.
- Confirm the raylib static library is present in `C:/raylib/raylib/src`.
- Make sure the raylib build is compatible with the installed MinGW-w64 compiler.

**The compiler cannot find raylib headers**

- Verify the `-IC:/raylib/raylib/src` include flag.
- Update the VS Code include paths in `.vscode/c_cpp_properties.json` and `.clangd` if raylib is installed elsewhere.
