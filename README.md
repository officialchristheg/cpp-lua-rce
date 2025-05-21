# 🧠 Remote Lua RCE (C++ & Lua Demo)
This project shows how you can send Lua scripts from a C++ server to a C++ client over TCP, then run them on the client using Lua 5.4. The Lua code is encrypted before being sent, and the client decrypts and executes it at runtime using static xor.

---

## 📦 Features
- ⚙️ C++ TCP Server & Client (WinSock)
- 🔐 Static XOR encryption of Lua payloads
- 📜 Runtime Lua execution using `lua.hpp`
- 📡 Simple JSON-based protocol (`nlohmann/json`)
- 🖥️ Console color control via WinAPI (as Lua functions)

---

## 🛠️ Requirements
- Windows 10+
- C++17 compiler (MSVC)
