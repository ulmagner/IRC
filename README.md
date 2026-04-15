# 💬 IRC Server

> A fully functional IRC server implemented in C++ with real-time communication and a custom multiplayer poker bot (bonus feature).

---

## 📌 Description

This project is a custom implementation of an **IRC (Internet Relay Chat) server**, developed in C++ as part of the 42 curriculum.

It supports multiple clients, channels, and real-time messaging using TCP sockets.

As a **bonus feature**, a fully automated **multiplayer poker bot 🤖♠️** has been integrated, capable of joining games and interacting with players in real time.

---

## ⚙️ Features

### 💬 Core IRC Server

* TCP socket-based communication
* Multi-client handling
* Channel creation and management
* Private messaging
* User authentication
* Nickname system
* Join / part channels
* Broadcast messaging

---

### 🤖 Bonus: Poker Bot

A custom AI bot that can:

* Join IRC poker rooms ♠️
* Play multiplayer poker games
* Make automated decisions (bet / call / fold)
* Interact with real players
* Simulate real user behavior

👉 The bot behaves like a real client connected to the server.

---

## 🧠 Technical Highlights

* Non-blocking I/O
* Select / poll system for handling multiple clients
* Event-driven architecture
* Socket programming (TCP/IP)
* State management for users and channels
* Game logic system for poker bot

---

## 🛠️ Technologies

* C++
* Sockets (TCP/IP)
* Unix system calls
* Makefile

---

## 📂 Project Structure

```id="j1p3kq"
irc/
├── src/
├── includes/
├── server/
├── client/
├── bot/
├── Makefile
```

---

## ⚙️ Installation & Usage

### 1. Clone repository

```bash id="c7kq2a"
git clone https://github.com/ulmagner/irc.git
cd irc
```

### 2. Compile

```bash id="l9z1pd"
make
```

### 3. Run server

```bash id="m2v8sx"
./irc_server <port>
```

### 4. Connect client

```bash id="q8x0aa"
./irc_client <ip> <port>
```

---

## 🃏 Poker Bot Usage

```
msg bot!
```

Then it will:

* Join a poker channel
* Automatically play against users
* Respond to game state events

---

## 🧠 Learning Outcomes

* Network programming (TCP/IP)
* Multi-client server architecture
* Real-time communication systems
* Concurrency handling
* State machines
* AI / automated decision logic
* System-level programming in C++

---

## 🚀 Possible Improvements

* Encryption (secure IRC)
* GUI client
* Matchmaking system
* Spectator mode 👀
* Anti-spam / moderation tools

---

## 📸 Preview

---

## 👨‍💻 Author

* 42 Student focused on systems programming, networking, and game logic

---

## 📄 License

Educational project (42 school)
