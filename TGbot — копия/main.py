import requests
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, ContextTypes, MessageHandler, filters

SERVER_URL = "http://localhost:9090"

player_data = {}

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """
    @brief Sends a welcome message and explains bot commands to the user.
    @details This function responds to the /start command with instructions on how to authenticate
             and how to make moves in the chess game.
    """
    await update.message.reply_text(
        "Привет! Я шахматный бот.\n"
        "Используй /auth <username> <email> для аутентификации.\n"
        "После этого можешь делать ходы, просто отправляя их в формате e2e4.\n"
        "Команды:\n"
        "/status — показать текущий ход\n"
    )

async def auth(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """
    @brief Authenticates the user with the chess server.
    @param update Telegram update object containing user message.
    @param context Context containing command arguments.
    @details Expects /auth <username> <email>. Retrieves player ID and color from the server
             and stores them in a local dictionary for future moves.
    """
    if len(context.args) < 2:
        await update.message.reply_text("Используй команду так: /auth <username> <email>")
        return

    username = context.args[0]
    email = context.args[1]

    try:
        r = requests.post(f"{SERVER_URL}/auth", data={"username": username, "email": email})
        if r.status_code == 200:
            content = r.text
            # Extract player ID and color from server response
            player_id = int(content.split("ID = ")[1].split(",")[0])
            color = content.split("Color = ")[1].strip()
            # Store player data for future moves
            player_data[update.effective_user.id] = {"player_id": player_id, "color": color}
            await update.message.reply_text(f"✅ {content}")
        else:
            await update.message.reply_text(f"Server error: {r.text}")
    except Exception as e:
        await update.message.reply_text(f"Connection error: {e}")

async def move(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """
    @brief Sends a player's move to the server.
    @param update Telegram update object containing the move.
    @param context Context (not used here directly).
    @details The user must be authenticated before making a move. Sends the move text to the
             /move endpoint and returns the server response.
    """
    if update.effective_user.id not in player_data:
        await update.message.reply_text("Сначала авторизуйтесь через /auth")
        return

    player_info = player_data[update.effective_user.id]
    player_id = player_info["player_id"]
    move_text = update.message.text.strip()

    try:
        r = requests.post(f"{SERVER_URL}/move", data={"id_player": player_id, "move": move_text})
        await update.message.reply_text(r.text)
    except Exception as e:
        await update.message.reply_text(f"Connection error: {e}")

async def status(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """
    @brief Retrieves the current game status from the server.
    @param update Telegram update object.
    @param context Context (not used directly).
    @details The user must be authenticated. Queries the /status endpoint to get the current
             turn or game information.
    """
    if update.effective_user.id not in player_data:
        await update.message.reply_text("Сначала авторизуйтесь через /auth")
        return

    player_info = player_data[update.effective_user.id]
    player_id = player_info["player_id"]

    try:
        r = requests.get(f"{SERVER_URL}/status", params={"id_player": player_id})
        await update.message.reply_text(r.text)
    except Exception as e:
        await update.message.reply_text(f"Connection error: {e}")

async def board(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """
    @brief Retrieves the full chessboard state from the server.
    @param update Telegram update object.
    @param context Context (not used directly).
    @details Queries the /board endpoint to display the current board for all users.
    """
    try:
        r = requests.get(f"{SERVER_URL}/board")
        await update.message.reply_text(r.text)
    except Exception as e:
        await update.message.reply_text(f"Connection error: {e}")

# Bot token

# Build the application
app = ApplicationBuilder().token(TOKEN).build()

# Register command handlers
app.add_handler(CommandHandler("start", start))
app.add_handler(CommandHandler("auth", auth))
app.add_handler(CommandHandler("status", status))

# Register handler for normal text moves
app.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, move))

# Start polling
app.run_polling()