# Infants and Incantations

A solo C++ vertical-slice prototype exploring game-engine architecture, real-time rendering, physics, and gameplay systems with OGRE, SDL2, and Bullet.

## Overview


The project is in active development and currently provides a controllable, physics-driven player inside a rendered test level.

## Current Build

Implemented:

- Resizable, high-DPI macOS window and OpenGL 3+ rendering
- Custom SDL2 and OGRE integration, including the application loop and resize handling
- Keyboard input and physics-driven player movement
- Bullet physics world with player and terrain collision bodies
- Scene loading and unloading through a dedicated `LevelScene` lifecycle
- Runtime asset and material loading with OGRE's RT Shader System
- Smooth camera rig that follows and tracks the player
- Installable macOS application bundle assembled by CMake

In progress:

- Player spell casting

Planned for the vertical slice:

- Pathfinding for one enemy type
- A basic enemy melee attack
- A complete player-versus-enemy combat encounter

## Controls

| Input | Action |
| --- | --- |
| `W` | Move forward |
| `A` | Move left |
| `S` | Move backward |
| `D` | Move right |

## Technical Overview

| Area | Technology |
| --- | --- |
| Language | C++17 and Objective-C++ for macOS integration |
| Rendering engine | OGRE 14.5.2 |
| Windowing and input | SDL2 |
| Physics | Bullet through OgreBullet |
| Graphics API | OpenGL 3+ |
| Build and packaging | CMake 3.24+ |
| Supported platform | macOS |

## Architecture

The project separates platform and rendering setup from game-specific state and behaviour:

| Component | Responsibility |
| --- | --- |
| `Engine` | Initialises SDL and OGRE, loads resources, owns the application loop, and renders each frame |
| `GameSession` | Owns the physics world, player, camera, and active level; coordinates their update order |
| `LevelScene` | Loads and unloads level resources and updates level-owned game objects |
| `GameObject` | Provides a base for gameplay objects with exclusive ownership of an OGRE scene node. Child inherit from this class  |
| `Player` | Reads input and applies movement and rotation through a Bullet rigid body |
| `PlayerCamera` | Smoothly follows the player's scene node and keeps the camera aimed at the player |

At runtime, input updates the player, Bullet advances the physics simulation, OGRE scene nodes receive the resulting transforms, the camera follows the player, and OGRE renders the frame.

## Engineering Problems and Solutions

### Custom SDL2 and OGRE window integration

**Problem:** I initially planned to use OGRE's `ApplicationContext` to configure the window and Metal renderer. However, OGRE's Metal support was not suitable for the project, and `ApplicationContext` did not support the OpenGL 3+ configuration I needed on macOS. This left the project without OGRE's standard window setup, application loop, or automatic material configuration.

**Solution:** I used SDL2 to create and own the native OpenGL window, then passed that window to OGRE as an external render target. I implemented the surrounding application lifecycle myself, including render-system selection, event processing, resize handling, frame timing, simulation updates, resource loading, material preparation, rendering, and shutdown. I kept this integration inside [`Engine`](https://github.com/callum-legendre/infants-and-incantations/blob/main/src/iaiEngine.cpp) so that the platform-specific setup remains separate from the gameplay code.

**Result:** Infants and Incantations uses SDL2 to create the window and handle inputs so that I do not rely on Ogre's outdated macOS compatability.

### A single source of truth for GameObject locations

**Problem:** My initial [`GameObject`](https://github.com/callum-legendre/infants-and-incantations/blob/main/src/GameObject.cpp) design stored a custom transform alongside the transform already owned by its OGRE scene node. When I introduced Bullet, the physics integration updated the scene node while the custom transform remained unchanged. The two representations could therefore disagree about an object's position and rotation.

**Solution:** I removed the redundant transform abstraction and made each object's OGRE scene node the single source of truth for its scene transform. OgreBullet can now write physics updates directly to the sceneNode without requiring the duplicate position and rotation state to be synchronised manually.

**Result:** The transform properties owned by Ogre's sceneNodes are the single source of truth for the position of GameObjects.

### Reproducible macOS packaging

**Problem:** The application initially depended on OGRE frameworks, plugins, configuration files, and media stored inside the local build directory. A bundle that worked only in my local environment and could not be moved or shared reliably.

**Solution:** I configured CMake to fetch the pinned OGRE 14.5.2 source, build only the required rendering and physics components, and copy every runtime dependency into an installable `.app` bundle. The install process fixes the bundled library paths and applies an ad-hoc code signature, producing an application that runs independently of the build directory.

**Result:** The application is now fully portable onto any compatable macOS system.

## Building and Running

The project currently supports macOS only.

### Prerequisites

- Xcode Command Line Tools
- [Homebrew](https://brew.sh/)
- CMake 3.24 or later
- SDL2 compatibility package

Install the command-line tools:

```sh
xcode-select --install
```

Install CMake and SDL:

```sh
brew install cmake sdl2-compat
```

### Build

From the repository root, run:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
cmake --install build --prefix install
```

The first build downloads and compiles OGRE 14.5.2 and its dependencies, so it requires an internet connection and may take several minutes.

### Run

Launch the installed application bundle:

```sh
open install/iai.app
```

## Known Limitations

- The prototype is not yet a complete combat encounter; spell casting and enemy behaviour remain in development.
- The project has only been built and tested on macOS. Windows and Linux are not currently supported.
- The current level is a test environment used to develop movement, rendering, physics, and scene systems.

## Credits and Licensing

Full third-party licence texts are stored in [`third_party_licenses`](third_party_licenses) and are copied into `iai.app/Contents/Resources/Licenses` when the application is built or installed.

- [OGRE](https://www.ogre3d.org/) — rendering engine
- [SDL](https://github.com/libsdl-org/SDL) — windowing and input
- [Bullet Physics](https://github.com/bulletphysics/bullet3) — physics through OgreBullet
- [Third-party licences](third_party_licenses/THIRD_PARTY_NOTICES.md) — complete notices for redistributed software dependencies
