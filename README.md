# Infants and Incantations

A solo C++ action-RPG prototype focused on engine integration and component-based gameplay. I implemented the SDL2–OGRE integration, application lifecycle, component system, gameplay scripts, and macOS packaging, using OGRE for rendering and Bullet for physics.

## Overview

*Infants and Incantations* is a C++ prototype for a real-time action RPG about a wizard baby escaping from a demonically managed daycare. It establishes the technical foundation for a playable combat encounter while keeping the scope focused on engine integration and core gameplay systems.

The project is in active development and currently provides a controllable, physics-driven player inside a rendered test level.

![Gameplay showing player movement, physics, and camera tracking](docs/gameplay.gif)

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
- Reusable input, physics, and gameplay-script components attached to OGRE scene nodes

In progress:

- Player spell casting and runtime swapping of equipped spells

Planned for the vertical slice:

- Dynamic, physics based level objects that the player can manipulate
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

| Class | Responsibility |
| --- | --- |
| `Engine` | Initialises SDL and OGRE, loads resources, owns the application loop, and renders each frame |
| `GameSession` | Owns the physics world and active level |
| `LevelScene` | Loads and unloads level resources and updates level-owned game objects |
| `ObjectFactory` | Configures and attaches the components for each SceneNode, and creates and configures the player camera node |
| `ComponentManager` | Owns all of the components for a scene node and provides templated methods to add, retrieve, and check for components |
| `InputComponent` | Reads keyboard input and stores movement state |
| `PhysicsComponent` | References a Bullet rigid body and provides movement and rotation behaviour |
| `ScriptComponent` | Provides the base interface for each gameplay script |
| `PlayerScript` | The main script for the player. Controls player movement by fetching the user's input from `InputComponent` |
| `PlayerCameraScript` | Controls the movement of the camera that follows the player |

Each configured scene node stores a `ComponentManager` through OGRE's user object bindings. The manager owns one component per concrete type, allowing scripts to access other components on the same object.

During level loading, `LevelScene` reads the `objectType` metadata on imported entities and asks `ObjectFactory` to configure them. The player receives input, physics, and player-script components; boxes and terrain receive physics components. The camera receives its own follow script.

Each frame, the level dispatches input and updates scripts, then `GameSession` advances Bullet's physics simulation. OgreBullet synchronises physics transforms with OGRE scene nodes before rendering.

## Engineering Problems and Solutions

### Custom SDL2 and OGRE window integration

**Problem:** I initially planned to use OGRE's `ApplicationContext` to configure the window and Metal renderer. However, OGRE's Metal support was not suitable for the project, and `ApplicationContext` did not support the OpenGL 3+ configuration I needed on macOS. This left the project without OGRE's standard window setup, application loop, or automatic material configuration.

**Solution:** I used SDL2 to create and own the native OpenGL window, then passed that window to OGRE as an external render target. I implemented the surrounding application lifecycle myself, including render-system selection, event processing, resize handling, frame timing, simulation updates, resource loading, material preparation, rendering, and shutdown. I kept this integration inside [`Engine`](src/iaiEngine.cpp) so that the platform-specific setup remains separate from the gameplay code.

**Result:** The project now has a resizable, high-DPI SDL window rendered through OGRE's OpenGL 3+ backend, with explicit control over application startup, resizing, frame updates, and shutdown.

### Moving from inheritance to components

**Problem:** My initial design used `Player` and `PlayerCamera` classes derived from `GameObject`. As I added physics and input handling, I wanted those capabilities to be reusable across different objects. After learning about component-based design in my Building Game Engines course, I applied it to the prototype.

**Solution:** I replaced the specialised object classes with components attached to OGRE scene nodes. Each node's `ComponentManager` owns its components and provides templated methods to add and retrieve them. `PlayerScript` combines input and physics components to control movement, while `ObjectFactory` configures component combinations using metadata from the loaded scene.

**Result:** The player, boxes, and terrain now share `PhysicsComponent`, while only the player receives `InputComponent` and `PlayerScript`. Camera-follow behaviour lives in a separate script. This lets objects share capabilities without adding more subclasses.

**Tradeoff:** Component combinations are currently configured in C++ factory functions. Scene metadata selects among supported object types; it does not yet define arbitrary combinations of components.

### Reproducible macOS packaging

**Problem:** The application initially depended on OGRE frameworks, plugins, configuration files, and media stored inside the local build directory. As a result, the bundle worked only in my local environment and could not be moved or shared reliably.

**Solution:** I configured CMake to fetch the pinned OGRE 14.5.2 source, build only the required rendering and physics components, and copy every runtime dependency into an installable `.app` bundle. The install process fixes the bundled library paths and signs the app locally, producing an application that runs independently of the build directory.

**Result:** The installed application is self-contained and relocatable, and it runs independently of the build directory on an Apple M3 Mac running macOS Tahoe 26.5.2. Further testing on other devices remains to be done.

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
cmake --build build
cmake --install build --prefix install
```

The first build downloads and compiles OGRE 14.5.2 and its dependencies, so it requires an internet connection and may take several minutes.

### Run

Launch the installed application bundle:

```sh
open install/iai.app
```

## Known Limitations

- Visual assets are currently placeholders; development is focused on engine integration and gameplay systems rather than content production.
- The prototype is not yet a complete combat encounter; spell casting and enemy behaviour remain in development.
- The project has only been built and tested on macOS. Windows and Linux are not currently supported.
- The current level is a test environment used to develop movement, rendering, physics, and scene systems.

## Credits and Licensing

Full third-party licence texts are stored in [`third_party_licenses`](third_party_licenses) and are copied into `iai.app/Contents/Resources/Licenses` when the application is built or installed.

- [OGRE](https://www.ogre3d.org/) — rendering engine
- [SDL](https://github.com/libsdl-org/SDL) — windowing and input
- [Bullet Physics](https://github.com/bulletphysics/bullet3) — physics through OgreBullet
- [Third-party licences](third_party_licenses/THIRD_PARTY_NOTICES.md) — complete notices for redistributed software dependencies
