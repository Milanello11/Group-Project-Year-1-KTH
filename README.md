# Snömos  
*A Network-Based Battle Royale Prototype in C (KTH Project)*  

---

## Overview  
**Snömos** is a network-based multiplayer game prototype developed at the **Royal Institute of Technology (KTH)** as part of the *Application Development Project Course (CM1008)*.  
Created by a team of five students, the project aimed to apply agile methods and technical skills from courses in programming, digital systems, and engineering methodology.

The goal was to design and implement a **client–server based 2D game** written in **C** using **SDL** libraries, featuring a graphical interface, keyboard and mouse input, and network communication between multiple clients.

---

## Game Concept  
**Snömos** is a light-hearted **2D battle royale** where players move across an icy map and throw snowballs to freeze opponents.  
The last remaining unfrozen player wins the match.  
The design emphasizes simplicity, quick matches, and local multiplayer fun.

---

## Key Features  
- **Client–Server model** using UDP for low-latency communication  
- **Real-time movement and collision detection**  
- **Sprite-based graphics and animation** with SDL_image  
- **Sound and music** via SDL_mixer  
- **Menu system** with multiple states (Start, Game, Credits, Game Over)  
- **Basic testing and memory leak verification**

---

## System Structure  
- **Server:** Controls game logic, synchronizes state, and manages clients using SDL_net  
- **Client:** Handles rendering, input, and communication with the server  
- **Graphics:** 2D tile-based map (50×50 grid) rendered with SDL  
- **Audio:** Background music and sound effects integrated with SDL_mixer  

---

## Development Process  
The project followed an **agile workflow (Scrum)** using **Taiga** for project management and **GitHub** for version control.  
Technologies:  
C · SDL2 · SDL_net · SDL_image · SDL_mixer · Linux  

---

## Lessons & Future Work  
**Lessons learned:**  
- Practical experience in C-based network programming  
- Applying agile teamwork in a technical context  
- Managing real-time synchronization and debugging memory errors  

**Future improvements:**  
- Power-ups and new projectiles  
- Enhanced aiming and player customization  
- In-game server hosting and matchmaking  

---

## Acknowledgment  
Developed by a team of five students at **KTH** under the supervision of **Jonas Willén** and **Nicklas Brandefelt**.  

---

**Keywords:** C · SDL · UDP · Client–Server · Game Development · Multiplayer · KTH  
