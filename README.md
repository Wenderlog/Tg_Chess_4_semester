# Tg_Chess_4_semester


# 1. Project Overview

This project implements an online chess platform for two players, supporting multiple simultaneous games. Users interact with the system via a Telegram bot, which serves as the frontend for the platform. Players can authenticate, make moves, check the game status, and view the current board.

# 2. Domain Overview

The system models a simplified chess domain:

	•	Each game has exactly two players.
 
	•	Moves are validated according to standard chess rules.
 
	•	The server keeps track of the current board state and the history of moves.
 
	•	The architecture is designed to support online multiplayer games safely and efficiently.

 # 3. Roles
 
	•	Player: Only role in the system. Players can:
 
	•	Authenticate via Telegram bot
 
	•	Make moves
 
	•	Check current game status
 
	•	Exit the game

  No administrators or observers are needed because all operations are fully controlled and secure.

  # 4. ER Diagram of Entities

  <img width="426" height="243" alt="image" src="https://github.com/user-attachments/assets/cb5ae4b4-bcc3-46ec-b9d7-0dc52a2ab2c4" />


Entities:

	•	User
 
	•	user_id — unique identifier
 
	•	user_name — name of the player
 
	•	game_id — ID of the game the user is participating in
 
	•	user_colour — player colour (White or Black)


 
	•	Game
 
	•	game_id — unique identifier
 
	•	board_states — current state of the chessboard

  # 5. MVC
  
  <img width="3840" height="2706" alt="Chess_MVC_2" src="https://github.com/user-attachments/assets/5cebef83-89ca-4099-a464-051ae2e9f3f4" />


 # 6. User Scenarios

 <img width="3840" height="3411" alt="Chess_MVC_1" src="https://github.com/user-attachments/assets/9d18b838-119b-4451-84d9-edb08a53ca3d" />

 
	1.	Access via Telegram Bot: Users start the bot by clicking a link.
 
	2.	Registration / Authentication: Users register with /auth <username> <email>.
 
 
	3.	Gameplay:
 
	•	Users make moves by sending text commands in the format e2e4.
 
	•	The bot checks move validity and updates the board.
 
 
	4.	Game Status: Users check the current game turn via /status.
 
 
	5.	Exit: Users can terminate participation or finish the game naturally.
 

 # 7. Technology Stack
 
	•	Frontend: Telegram Bot (Python)
 
	•	Backend: C++ Chess Engine (multi-threaded, handles game logic and move validation)
 
	•	Database: PostgreSQL
 
	•	Libraries/Frameworks:
 
	•	Python: python-telegram-bot, requests
 
	•	C++: httplib, pqxx, Boost

Notes on Multithreading:

	•	The server uses mutexes and condition variables to manage simultaneous game sessions safely.
 
	•	Each game may run in its own thread, ensuring that multiple players can play independently without conflicts.

 # 8. High-Level Component Architecture

 	•	Frontend: Handles user input, authentication, and message display.
  
	•	Backend: Implements chess rules, validates moves, manages multiple games in parallel.
 
	•	Data Access Layer: Stores user info and board states securely in PostgreSQL.

 # 9. Future Development

  Potential extensions for this project include:
  
	•	Implementing real-time notifications for opponent moves.
 
	•	Adding chess timers for turn-based timing.
 
	•	Expanding to support spectators or tournament mode.
 
	•	Improving move history and undo functionality.
 
	•	Creating a web-based frontend in addition to Telegram for richer UX.
