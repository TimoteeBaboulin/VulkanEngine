# MoonlitEngine

## Project Goal
A modern C++ game engine targeting accessibility: runs on any low-budget gaming setup from the last decade, with a codebase approachable for new game developers. Uses a hybrid OOP/ECS architecture to balance ease-of-use with performance. Similar in scope to Unity or Unreal but with a lower hardware and learning barrier.

## CMake Targets

| Target | Type | Output | Purpose |
|--------|------|--------|---------|
| `MoonlitEngine` | SHARED DLL | `bin/<Config>/MoonlitEngine.dll` | Core engine — all engine systems; used by both editor and game builds |
| `DefaultPlugin` | SHARED DLL | `bin/<Config>/Modules/DefaultPlugin.dll` | Default module with built-in behaviours (MeshRenderer, Transform, etc.) |
| `MoonlitEditor` | EXE | `bin/<Config>/MoonlitEditor.exe` | Qt-based visual editor; always ships with a clean copy of the engine DLL and DefaultPlugin |

## Module System
The engine uses a **dynamic plugin/module system** — modules are DLLs loaded/unloaded at runtime via `ModuleManager`. A module is a self-contained chunk of gameplay code (e.g. a car simulation module loaded only for the open-world level of a TRPG). `DefaultPlugin` is the baseline module and always loaded.

## Folder Structure

```
MoonlitEngine/
├── MoonlitEngine/Sources/          # Core engine DLL source
│   ├── Engine/
│   │   ├── Component/              # GameObject + Behaviour system
│   │   ├── Entity/                 # Entity interface
│   │   ├── Events/                 # Generic event system
│   │   ├── Inputs/                 # Input abstraction (Windows + Qt)
│   │   ├── Modules/                # Dynamic DLL module loading
│   │   ├── Renderer/               # Vulkan renderer + material system
│   │   │   ├── Material/
│   │   │   └── Helpers/
│   │   ├── ResourceManagement/     # Asset loading, caching, ref-counting
│   │   │   ├── Banks/              # MeshBank, TextureBank
│   │   │   ├── Resources/          # Mesh, Image data types
│   │   │   ├── Helpers/            # File I/O, image loading (STB)
│   │   │   └── Serialization/
│   │   ├── Scene/                  # Scene container + lifecycle
│   │   └── Tasks/                  # Multi-threaded task scheduler
│   └── Debug/                      # Logger, Profiler, custom exceptions
│
├── DefaultPlugin/Sources/          # Default module DLL source
│   └── Behaviours/                 # TransformBehaviour, MeshRendererBehaviour, etc.
│
├── MoonlitEditor/Sources/          # Editor EXE source
│   └── Editor/
│       ├── Controllers/            # Scene view controller
│       ├── ModelClasses/           # Qt models (hierarchy tree, properties)
│       ├── Widgets/                # Inspector widgets, parameter editors
│       │   └── ParameterTypes/     # Per-type editors (Float, Vector3, MeshHandle)
│       └── Windows/                # Main window, docking, panels
│           ├── General/            # SceneHierarchy, ObjectEditor, SceneView, FileExplorer
│           └── Debug/              # ProfilerWindow
│
├── Resources/                      # Game assets (meshes, shaders, textures)
│   ├── Meshes/
│   ├── Shaders/                    # Slang source + compiled SPIR-V
│   └── Textures/
│
└── external/                       # Git submodules + vendored deps
    ├── slang-rhi, spirv-headers, spirv-tools
    ├── tinyobjloader, unordered_dense, vulkan
```

## Architecture Overview

### Component System (OOP/ECS Hybrid)
- `GameObject` — hierarchical scene node with a unique ID, holds a collection of behaviours
- `ObjectBehaviour` — base class for all components; uses a **parameter system** for serialization and editor reflection
- `BehaviourRegistry` — runtime registry for dynamic behaviour instantiation (required for editor and module system)
- `BehaviourBank` / `BehaviourManager` — storage and lifecycle management for behaviour instances

### Parameter System
Behaviours expose data to the editor via a typed `Parameter<T>` template. `ParameterBase` is the interface, `ParameterMemento` handles state snapshots. The editor auto-generates UI using `ParameterEditor<T>` templates registered in `ParameterTypeRegistry`.

### Renderer
Vulkan-based. Key classes:
- `Renderer` — main renderer, manages render passes, draw calls, material/mesh binding
- `RendererContext` — Vulkan instance, device, queues, swapchain
- `RendererDeviceManager` — GPU selection and device management
- `Material` / `MaterialInstance` — shader + parameter data
- Shaders written in **Slang**, compiled to SPIR-V

### Resource Management
Template-based with reference counting:
- `ResourceManager` — central access point
- `ResourceBank<T>` — typed asset storage
- `ResourceHandle<T>` — ref-counted handle to an asset
- Specific banks: `MeshBank` (ASSIMP), `TextureBank` (STB)

### Scene
- `Scene` — owns GameObjects, drives update loop
- `SceneManager` — scene lifecycle (load, unload, transition)

### Editor
Qt 6.10+ application using Qt Advanced Docking System (ADS). Main panels:
- **SceneHierarchy** — tree view of GameObjects
- **ObjectEditor** — inspector for behaviours and parameters
- **SceneViewWindow** — 3D Vulkan viewport
- **FileExplorer** — asset browser

## Tech Stack
| Concern | Library |
|---------|---------|
| Graphics | Vulkan |
| Shaders | Slang → SPIR-V |
| Math | GLM |
| 3D import | ASSIMP + tinyobjloader |
| Image loading | STB |
| GUI | Qt 6.10 (MSVC 2022, 64-bit) |
| Docking | Qt Advanced Docking System (ADS) |
| JSON | nlohmann_json |
| Hash maps | unordered_dense |
| Threading | Custom TaskManager + worker pool |

## Conventions

### Naming
- Classes: `PascalCase`
- Methods: `PascalCase`
- Private/protected member variables: `m_camelCase`
- Public member variables: `PascalCase`
- Parameters: `_camelCase` (leading underscore)
- Template type parameters: `ALL_CAPS_SNAKE` (e.g. `RESOURCE_TYPE`, `BANK_TYPE`)
- Namespaces: `PascalCase`, nested with `::` (e.g. `Moonlit::ResourceManagement`)

### Output parameters
- Prefixed with `_out` in the parameter name, passed by reference
  ```cpp
  bool TryGetResource(std::string _name, ResourceHandle<T>& _outResource);
  ```

### Try-pattern
- Methods that can fail return `bool` and are prefixed with `Try`
- Their private implementation counterparts are suffixed with `Internal`
  ```cpp
  static bool TryGetResource(...);       // public API
  bool TryGetResourceInternal(...);      // private impl
  ```

### Singleton
- Static `Get()` method returning a reference, lazy-initialized via a local `static`
  ```cpp
  static MyClass& Get() {
      static MyClass instance;
      return instance;
  }
  ```

### Logging
- Use `LOG_ERROR(...)` / `LOG_INFO(...)` macros
- Message format: `"ClassName::MethodName - Description: " + detail`

### Template implementations
- Split into `.ipp` files, `#include`d at the bottom of the corresponding `.h`
- Each class owns its own `.ipp` — do not mix implementations from different classes into one `.ipp`

### Include guards
- Use `#pragma once` — no `#ifndef`/`#define`/`#endif` guards

### `#include` paths
- Maximum one level of `../` in relative includes
- If the target is more than one level up, use the root-level source folder as anchor
  ```cpp
  // Bad
  #include "../../Debug/Logger.h"
  // Good
  #include "Debug/Logger.h"
  ```

## Build Notes
- **C++20** required (`/Zc:__cplusplus` on MSVC)
- Qt path is hardcoded: `C:/Qt/6.10.2/msvc2022_64`
- Build outputs go to `cmake-build-debug/` or `cmake-build-release/`
- Post-build steps copy Qt runtime DLLs and Resources into the output folder
