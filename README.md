# 🎮 2-Player Shooter Game (C++ Console Game)

This is a simple C++ console-based 2-player shooting game built using only standard libraries like `<windows.h>`, `<conio.h>`, and `<iostream>`. It's a fun side project to practice real-time game logic and C++ programming fundamentals.

---

## 🧪 Project Status

- The main game (`game`) and `game_file_for_pr` are **bug-free**, **smooth**, and work well.
  - However, they **do not** include timer or score calculation yet.
  - These versions are the same in functionality and code.

- `game2_beta` was built with the help of **ChatGPT**.
  - It includes:
    - ⏱️ Timer functionality
    - 🏆 Score tracking for both players
  - But it still has a few bugs and rough edges due to these added features.

---

## 🎯 Game Rules & Features

- Real-time 2-player console gameplay
- Player A controls: `W`, `A`, `S`, `D` to move, `F` to shoot  
- Player B controls: `I`, `J`, `K`, `L` to move, `H` to shoot  
- 💣 Bombs spawn randomly and explode on bullet collision
- 💥 Side explosions affect nearby tiles
- ❌ If a player is hit by a bomb or its side fragments, they are eliminated
- ✅ The last player standing wins the game
- 🧪 In `game2_beta`: If time runs out, the player with the highest score wins

---

## 📅 Future Plans

I plan to improve the game further in **2026 or 2027**, possibly adding:
- 💥 Power-ups like shield, speed, and rapid fire
- ❤️ Health and healing system
- 🤖 AI-controlled bot enemies
- 🧨 Multi-hit bombs with large animations
- 🎨 Enhanced visual effects and screen feedback
- 🔊 Realistic sound using audio files (instead of `Beep()`)

---

## 🤝 Contributions Welcome

Want to contribute? Awesome!  
You can:
- Fix bugs in `game2_beta`
- Add new features to `game_file_for_pr` (same as `game`, clean base)

Feel free to fork the repo and open a pull request.

---

## 🙏 Final Note

I'm currently focused on **DSA and internship preparation**, so active development is paused for now. But I look forward to returning and improving the game further in the future.

Thanks for checking out the project! ❤️
