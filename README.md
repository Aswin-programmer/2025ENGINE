# 2025ENGINE

## Overview

**2025ENGINE** is a modern, experimental **C++ game engine** focused on learning, performance, and low-level graphics programming. The engine is being developed with a strong emphasis on **modern OpenGL 4.6**, data-oriented design, and real-time rendering techniques typically found in professional engines.

This repository representsussatodes the **successor** to my earlier engine projects and reflects the cumulative knowledge gained from building multiple engines from scratch.

The current goal of the project is to **complete a stable basic version of the engine**, validate the architecture, and incrementally extend it with advanced rendering, tooling, and runtime systems.

---

## Engine Demo

[![Engine Demo Video](https://img.youtube.com/vi/Je9Aj9f1vdE/0.jpg)](https://www.youtube.com/watch?v=Je9Aj9f1vdE)

**Full Demo Video (YouTube):**  
https://www.youtube.com/watch?v=Je9Aj9f1vdE

This video demonstrates the current state of **2025ENGINE**, including:
- Modern OpenGL 4.6–based rendering pipeline
- Dynamic lighting and shadow mapping
- Skeletal animation support
- Runtime entity–component workflow
- In-engine debug and editor tooling


## Key Features Implemented So Far

### Rendering & Graphics

* **Optimized Rendering Pipeline**

  * Designed around the goal of *minimal draw calls* (ideally one draw call per frame where applicable)
  * Uses **modern OpenGL 4.6 core features**
  * Heavy use of:

    * Shader Storage Buffer Objects (**SSBOs**)
    * Direct State Access (**DSA**)
    * **Indirect Draw Calls** (`glDrawElementsIndirect`)

* **Lighting System**

  * Phong lighting model
  * Supported light types:

    * Directional Light
    * Point Light
    * Spot Light

* **Shadow Mapping**

  * Basic shadow mapping implemented for dynamic lights
  * Directional light shadow support currently in place

---

### Animation

* **Skeletal Animation Support**

  * GPU-friendly skeletal animation pipeline
  * Animation data integrated into the rendering system

---

### Engine Architecture & ECS

* **Entity-Component System (ECS)**

  * Entities can be created dynamically at runtime
  * Components can be added and modified during runtime
  * Designed for flexibility and future editor integration

---

### Scripting & Runtime Systems

* **Native C++ Scripting System**

  * Supports **hot native reload** of gameplay code
  * Enables rapid iteration without restarting the engine
  * Intended as a foundation for advanced runtime scripting workflows

---

### Tools & Debugging

* **Embedded Debug UI**

  * Integrated **Micro UI (C library)** for in-engine debug windows
  * Used for inspecting engine state, rendering data, and runtime behavior

---

## Build Information

**Current Platform Support**

* **Windows only** (for now)

### Build Notes

* The engine is under active development
* Build system and platform abstraction will be improved in future iterations
* Cross-platform support (Linux/macOS) is a planned long-term goal

Detailed build instructions will be added once the engine architecture stabilizes.

---

## Engine Lineage (Previous Projects)

This engine is the **third major iteration** in my engine-development journey. The knowledge and architecture choices in 2025ENGINE are heavily influenced by the following predecessor projects:

* **2dEngine2**
  [https://github.com/Aswin-programmer/2dEngine2](https://github.com/Aswin-programmer/2dEngine2)

* **2dEngine3**
  [https://github.com/Aswin-programmer/2dEngine3](https://github.com/Aswin-programmer/2dEngine3)

Each engine was built from scratch and served as a learning platform for rendering, engine architecture, and tooling. **2025ENGINE** represents a more mature and performance-focused evolution of those ideas.

---

## Acknowledgements

Special thanks to:

* **dwjclark11**
  [https://github.com/dwjclark11](https://github.com/dwjclark11)

For inspiration, guidance, and contributions that helped shape parts of the engine design and development approach.

---

## Project Status

* Actively under development
* Experimental and educational in nature
* APIs and internal systems are subject to change

This project is intended as both a **learning exercise** and a **long-term foundation** for exploring advanced game engine concepts.

---

## Future Plans (High-Level)

* Improved shadow quality and cascaded shadow maps
* Editor tooling and asset pipeline improvements
* Better abstraction layers for rendering backends
* Cross-platform support
* Physics, audio, and advanced scene management

---

