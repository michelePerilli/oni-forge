# OniForge

OniForge is a C++23 modding tool for the game **Oni** (Bungie, 2001). It serves as a modern GUI for editing the XML files exported by [OniSplit](https://wiki.oni2.net/OniSplit), streamlining the process of creating and modifying characters, animations, and other game assets.

## Overview

OniForge reads and writes the XML files that OniSplit exports from Oni's binary `.oni` files. It provides a user-friendly interface for editing character classes (ONCC), character variants (ONCV), animation collections (TRAC), and animation files (TRAM). It also integrates an automated pipeline to feed these changes back into the game via OniSplit.

## Tech Stack

*   **Language:** C++23
*   **Compiler:** MinGW 14.2.0 (Windows)
*   **Build System:** CMake (with FetchContent)
*   **Graphics/UI:** SDL2, OpenGL 3.3, Dear ImGui
*   **Dependencies:**
    *   pugixml (XML parsing)
    *   FreeType (Font rendering)

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

## Features

*   **Editors:**
    *   **ONCC:** Character Class editing (General stats).
    *   **ONCV:** Character Variant linking.
    *   **TRAC:** Animation Collection management.
    *   **TRAM:** Detailed Animation editing (flags, velocities, events).
*   **Pipeline:** "Try in Oni" feature automates the OniSplit export/import commands, effectively compiling your mod and launching the game in one click.
*   **Management:** Separate views for Vanilla (read-only) and Project (editable) files.

## Roadmap & Pending Items

*   [ ] **Configuration Screen:** Replace hardcoded paths for OniSplit and Game Data with a user settings UI.
*   [ ] **Background Processing:** Move the OniSplit pipeline to a background thread to prevent UI freezing.
*   [ ] **Expanded Editors:** Add support for remaining ONCC tabs (AI, Sounds, Physics).
*   [ ] **UI Improvements:** Convert flag text inputs to checkbox groups.
*   [ ] **New File Types:** Support for TRMA and other formats.
