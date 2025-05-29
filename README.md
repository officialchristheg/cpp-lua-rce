# 🧠 Remote Lua RCE (C++ & Lua Demo)
This project shows how you can send Lua scripts from a C++ server to a C++ client over TCP, then run them on the client using Lua 5.4. The Lua code is encrypted before being sent, and the client decrypts and executes it at runtime using static xor.

Great for updating features or scripts on connected clients

---

## 📦 Features
- ⚙️ C++ TCP Server & Client (WinSock)
- 🔐 Static XOR encryption of Lua payloads
- 📜 Runtime Lua execution using `lua54`
- 📡 Simple JSON-based protocol (`nlohmann && json`)
- 🖥️ Console color control via WinAPI (as Lua functions)

## ⚠️ Disclaimer
> This project is intended for **educational purposes only**. Downloading and executing external files without validation poses a **serious security risk** and should never be used in production or malicious contexts.
