# 🌌 Neon Gravity & Particle Physics Engine

An interactive 2D physics simulation and particle engine built from scratch using **C++** and **SFML**.

## ✨ Features
- **Real-time Physics Simulation:** Implements 2D vector kinematics, gravitational acceleration, air drag, and elastic boundary collisions.
- **Interactive Black Hole Gravity:** Hold Left-Click to create a gravitational singularity that attracts and orbits nearby particles.
- **Additive Neon Rendering:** Uses `sf::BlendAdd` and alpha-decay overlays to render dynamic motion trails, glowing halos, and proximity constellation webs.
- **Optimized Draw Calls:** Batches proximity connection lines into a single `sf::VertexArray` for smooth 60 FPS performance.

## 🎮 Controls
| Input | Action |
| :--- | :--- |
| **Left Click (Hold)** | Activate Black Hole attractor at cursor |
| **Right Click** | Spawn 25 new neon particles at cursor |
| **Spacebar** | Toggle downward gravity ON / OFF |
| **R** | Reset and respawn particles |
| **C** | Clear the screen |

## 🛠️ Built With
- **Language:** C++
- **Graphics Library:** SFML 2.6.2 (Graphics, Window, System)
- **IDE:** Visual Studio (x64)

## 🚀 How to Run
1. Install **SFML 2.6.2 (64-bit)**.
2. Open the solution in Visual Studio and set configuration to `x64`.
3. Link SFML `include` and `lib` directories in Project Properties.
4. Build and run (`Ctrl + F5`).


## Author & Copyright
Developed by **Hasnaa Khaled** © 2026. All rights reserved.
