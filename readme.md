# OniForge

OniForge is a C++23 modding tool for the game **Oni** (Bungie, 2001). It serves as a modern GUI for editing the XML files exported by [OniSplit](https://wiki.oni2.net/OniSplit), streamlining the process of creating and modifying characters, animations, and other game assets.

## Overview

OniForge reads and writes the XML files that OniSplit exports from Oni's binary `.oni` files. It provides a user-friendly interface for editing character classes (ONCC), character variants (ONCV), animation collections (TRAC), and animation files (TRAM). It also integrates an automated pipeline to feed these changes back into the game via OniSplit.

### Supported File Types

OniForge primarily interacts with the following file types:

*   **`.oni` files**: These are the binary game asset files used by the Oni game. OniForge, through the OniSplit pipeline, can convert XML definitions into these binary files for in-game use.
*   **XML files**: OniForge's core functionality revolves around editing XML files that are exported by OniSplit. These human-readable files define various game assets:
    *   **ONCC (Oni Character Class)**: Defines the properties and behaviors of character classes.
    *   **ONCV (Oni Character Variant)**: Specifies variations for character classes, often linking to ONCC files.
    *   **TRAC (Animation Collection)**: Manages collections of animations, linking to TRAM files.
    *   **TRAM (Animation)**: Contains detailed animation data, including flags, velocities, and events.
    *   **TRMA**: This file type is planned for future support.

## Tech Stack

*   **Language:** C++23
*   **Compiler:** MinGW 14.2.0 (Windows)
*   **Build System:** CMake (with FetchContent)
*   **Graphics/UI:** SDL2, OpenGL 3.3, Dear ImGui
*   **Dependencies:**
    *   pugixml (XML parsing)
    *   FreeType (Font rendering)
    *   filewatch (Cross-platform file system monitoring)

## Project Structure

```text
src/
├── main.cpp
├── gui/
│   ├── OniGuiRenderer.hpp/cpp        # SDL2 + OpenGL + ImGui lifecycle
│   ├── OniForgeApp.hpp/cpp           # Application orchestrator & main loop
│   └── views/                        # UI Components for specific file types
├── component/
│   ├── logger/                       # Logging infrastructure
│   └── xml/                          # XML reading/writing wrappers (pugixml)
├── model/
│   ├── OniFile.hpp                   # Generic file container
│   ├── mapping/                      # XML serialization mapping logic
│   └── [ONCC, ONCV, TRAC, TRAM].hpp  # Data models
├── repository/                       # Data access layer for XML files
└── service/                          # Business logic & OniSplit integration
assets/
    icon/                             # Application icons and resources
```

## Architecture

OniForge follows a strictly layered architecture with **Dependency Injection**:

1.  **GUI (Views):** Purely visual components that render data models.
2.  **Services:** Manage file collections and business logic (e.g., `VanillaCatalogService`, `ProjectCatalogService`).
3.  **Repositories:** Handle XML parsing and serialization.
4.  **Components:** Low-level wrappers for libraries like pugixml.

All dependencies are injected via constructors in the `OniForgeApp` composition root.

### Data Management & Concurrency

To manage game assets, OniForge uses a generic container called `OniFile<T>`. It pairs a data model (like `ONCC` or `TRAM`) with its `std::filesystem::path`.

**Thread Safety & Background Watcher:**
The `ProjectCatalogService` employs a background thread using `filewatch` to monitor the project directory. Any changes on disk (adds, modifications, removals) are automatically reflected in the application's memory. To ensure thread safety between the background watcher and the UI render loop, all catalog access is synchronized via a `std::recursive_mutex`.

### Logging Specification

Logging is decoupled via the `ILogger` interface. Components must inject this interface and use the following level-specific methods:

*   **`info()`**: General application flow, successful file loads, or user-initiated actions.
*   **`warning()`**: Non-critical issues (e.g., a missing optional XML field) that don't stop the process.
*   **`error()`**: Critical failures that prevent loading, saving, or running the pipeline.
*   **`separator()`**: Used to visually group related operations in the log output.

## Features

*   **Configuration:** Runtime settings for OniSplit paths, game data, and UI theme (saved in `oniforge.config.xml`).
*   **Live Sync:** A background file watcher keeps the project catalog in sync with manual disk changes.
*   **Editors:**
    *   **ONCC:** Character Class editing (General stats).
    *   **ONCV:** Character Variant linking.
    *   **TRAC:** Animation Collection management.
    *   **TRAM:** Detailed Animation editing (flags, velocities, events).
*   **Pipeline:** "Try in Oni" feature automates the OniSplit export/import commands, effectively compiling your mod and launching the game in one click.
*   **Management:** Separate views for Vanilla (read-only) and Project (editable) files.

## Roadmap & Pending Items

*   [x] **Configuration Screen:** Runtime settings for paths and preferences.
*   [x] **File System Watcher:** Real-time synchronization of the project catalog with disk changes.
*   [ ] **Background Processing:** Move the heavy OniSplit pipeline tasks to a background thread to prevent UI freezing.
*   [ ] **Expanded Editors:** Add support for remaining ONCC tabs (AI, Sounds, Physics).
*   [ ] **UI Improvements:** Convert flag text inputs to checkbox groups.
*   [ ] **New File Types:** Support for TRMA and other formats.
