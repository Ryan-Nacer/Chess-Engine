# ♟️ Custom C++ Chess Engine (Qt Framework)

A sophisticated Chess engine and GUI built with **C++20** and **Qt 6**. This project implements core chess mechanics including check detection, move validation, and board state simulation using the RAII pattern.

---

## 📸 Interface & Gameplay
*A chronological look at the engine flow, from scenario selection to checkmate.*

### 1️⃣ Setup & Turn Logic
The game begins by selecting a scenario. The engine strictly enforces turn-based play.

| **Game Start & Scenarios** | **Turn Validation** |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/2bec9beb-7fef-4d5a-8eba-0210e5c3b130" width="380" /> | <img src="https://github.com/user-attachments/assets/e9945163-f555-4bbd-9934-3937217bd2e9" width="380" /> |
| *Scenario selector for custom endgames.* | *Enforcing turn-based mechanics via custom alerts.* |

### 2️⃣ Piece Movement
Each piece uses specialized logic to calculate available squares, accounting for blocked paths and board boundaries.

| **Knight Jump Validation** | **Rook Pathfinding** |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/37b07806-2ca8-451d-b864-2b60498bc4b2" width="380" /> | <img src="https://github.com/user-attachments/assets/8bab08ce-ed68-47a9-ba30-e3e345762a7f" width="380" /> |
| *L-shaped jump calculations.* | *Sliding movement logic for the Rook.* |

### 3️⃣ Check & Victory
The engine monitors king safety in real-time, restricting moves that are illegal during a check and identifying the final checkmate.

<p align="center">
  <img src="https://github.com/user-attachments/assets/20e34734-0fe4-46b4-b08d-0e3898c46b5d" width="550" /><br>
  <i><b>Check State:</b> Immediate visual feedback when a King is under attack.</i>
</p>

| **King Safety Logic** | **Victory Notification** |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/a97d7064-33e1-410a-9fd7-fef3f8fc3c57" width="380" /> | <img src="https://github.com/user-attachments/assets/a049f363-d1d5-48b9-b209-92ba861b1e1f" width="380" /> |
| *Restricting King moves to safe squares.* | *Checkmate detected: Game over.* |

---

## 🧩 Scenario-Based Gameplay
Instead of a standard start, you can test specific endgame configurations via the **Scenario Selector**:
* **Tower Duels:** 2 Rooks vs. 2 Rooks.
* **Knight Battles:** 2 Knights vs. 2 Knights.
* **Mixed Endgames:** Combinations of Knights and Rooks (e.g., 1 Knight + 1 Rook vs. 2 Rooks).

---

## 🚀 Key Features

<table>
  <tr>
    <td width="50%">
      <b>🧠 RAII Move Simulation</b><br>
      Uses a custom <code>RAII</code> class to "test" hypothetical board states. It simulates a move to check for King safety and automatically reverts the board when the object goes out of scope.
    </td>
    <td width="50%">
      <b>🛡️ Checkmate Detection</b><br>
      Robust algorithms prevent moves that leave the King in danger and implement the "Confronting Kings" rule (Kings cannot be adjacent).
    </td>
  </tr>
  <tr>
    <td width="50%">
      <b>🎨 Custom UI Architecture</b><br>
      Built with <code>QGraphicsView</code>. Features a custom beige-themed "Attention" dialog for illegal moves, styled with Qt Stylesheets.
    </td>
    <td width="50%">
      <b>⚙️ Modern C++ Logic</b><br>
      Leverages <code>std::unique_ptr</code> for memory safety and <code>cppitertools</code> for clean, Python-style range iterations.
    </td>
  </tr>
</table>

---

## 🛠️ Technical Architecture

### Core Logic
* **Polymorphism:** The `Piece` base class provides a unified interface for `BasicMovements` (geometry) vs. `SimpleMovements` (legal chess rules).
* **File Breakdown:**
    * `structure.h`: Central namespace for Board, Pieces, and Tiles.
    * `raii.cpp`: Logic for board state backup/restoration.
    * `utils.h`: Custom-styled Qt alerts (`Debug::show`).

---

## 📦 How to Run

1.  **Clone the repo:**
    ```bash
    git clone [https://github.com/yourusername/ChessQT.git](https://github.com/yourusername/ChessQT.git)
    ```
2.  **Build:**
    * Open `ChessQT.sln` in **Visual Studio**.
    * Ensure the **Qt VS Tools** extension is installed.
    * Build and Run (**F5**).

> [!NOTE]
> **Prerequisites:** Qt 6.x, C++20 compatible compiler, and the `cppitertools` library (located in `../include/`).
