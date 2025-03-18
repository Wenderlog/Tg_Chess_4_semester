//  Table.h
//  Chess
//
//  Created by Кирилл Грибанов on 10.12.2024.
//

#pragma once

#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Bishop_Cell.h"
#include "King_Cell.h"
#include "Knight_Cell.h"
#include "Pawn_Cell.h"
#include "Queen_Cell.h"

/*!
 * \class Table
 * \brief Represents the chessboard and manages the game's state.
 * \details The `Table` class encapsulates the functionality of an 8x8 chessboard.
 * It tracks the positions of chess pieces, handles game logic, and evaluates game states
 * such as check, checkmate, stalemate, and valid moves. It provides methods to interact
 * with the board and manage player turns.
 */
class Table {
public:
    using Line = std::vector<std::shared_ptr<Cell>>;
    using Body = std::vector<Line>;

    /*!
     * \enum TurnVerdict
     * \brief Represents the outcomes of a turn or move.
     * \details This enumeration defines the possible states or results after a player attempts a move.
     */
    enum class TurnVerdict : std::uint8_t {
        correct,          ///< The move is valid and correctly performed.
        white_mate,       ///< Black wins with a checkmate against White.
        black_mate,       ///< White wins with a checkmate against Black.
        white_stalemate,  ///< Stalemate where White has no legal moves.
        black_stalemate,  ///< Stalemate where Black has no legal moves.
        unnatural_move,   ///< The move is not valid according to chess rules.
        check,            ///< The move results in a check.
        invalid_rotate,   ///< An invalid attempt to rotate the board or position.
        transformation,   ///< A pawn promotion or transformation.
        white_turn,       ///< Indicates it is White's turn.
        black_turn        ///< Indicates it is Black's turn.
    };

    /*!
     * \brief Retrieves a reference to a specific cell on the board.
     * \param row The row index of the desired cell (0-7).
     * \param col The column index of the desired cell (0-7).
     * \return A shared pointer to the cell at the specified coordinates.
     */
    std::shared_ptr<Cell>& GetCell(int row, int col);
    const std::shared_ptr<Cell>& GetCell(int row, int col) const;

    /*!
     * \brief Provides indexed access to rows of the board.
     * \param i The row index.
     * \return A reference to the specified row.
     */
    auto& operator[](size_t i);
    const auto& operator[](size_t i) const;

    /*!
     * \brief Evaluates the validity and result of a turn.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     * \return A `TurnVerdict` indicating the result of the move.
     */
    TurnVerdict CheckTurn(Coord from, Coord to) const;

    /*!
     * \brief Executes a valid move on the board.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     */
    void DoTurn(Coord from, Coord to);

    /*!
     * \brief Checks if a move involves an attack and validates the piece colors.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     * \param parametr Additional parameter to adjust rules or validations.
     * \return `true` if the attack is valid, otherwise `false`.
     */
    bool CheckColourToAtack(Coord from, Coord to, bool parametr) const;

    /*!
     * \brief Generates a string representation of the board's current state.
     * \details Useful for saving or analyzing the game state.
     * \return A string representing the board state.
     */
    std::string GenerateBoardState() const;

    /*!
     * \brief Provides a visual representation of the board.
     * \return A vector of strings where each string represents a row of the board.
     */
    std::vector<std::string> GetPicture() const;

    /*!
     * \brief Checks for repeated board states.
     * \details Used to detect draws due to threefold repetition.
     */
    void CheckForRepetition();

    /*!
     * \brief Retrieves the color of the player whose turn it is.
     * \return `Colour::White` if it is White's turn, otherwise `Colour::Black`.
     */
    Colour GetCurrentTurnColour() const;

    /*!
     * \brief Checks if a move is an attack.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     * \return `true` if the move is an attack, otherwise `false`.
     */
    bool CheckAttack(Coord from, Coord to) const;

    /*!
     * \brief Checks if a move is a valid move.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     * \return `true` if the move is valid, otherwise `false`.
     */
    bool CheckMove(Coord from, Coord to) const;

    /*!
     * \brief Checks if a move is valid by chess rules.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     * \return `true` if the move is valid, otherwise `false`.
     */
    bool CheckChessValid(Coord from, Coord to) const;

    /*!
     * \brief Executes an attack move on the board.
     * \param from The starting coordinates of the move.
     * \param to The destination coordinates of the move.
     */
    void DoAttack(Coord from, Coord to);

    /*!
     * \brief Promotes a pawn to another piece.
     * \param position The coordinates of the pawn to be promoted.
     * \param promotionType The character representing the desired piece (e.g., 'Q' for Queen).
     */
    void PromotePawn(Coord position, char promotionType);

    /*!
     * \brief Returns a constant reference to the internal board representation.
     * \return The 2D vector representing the board.
     */
    const std::vector<std::vector<std::shared_ptr<Cell>>>& getBoard() const {
        return body_;
    }

    /*!
     * \brief Constructs an empty chessboard with initial settings.
     */
    Table();

private:
    Coord WhiteKing() const;
    Coord BlackKing() const;

    mutable Body body_;  ///< 2D vector representing the chessboard grid.

    bool w_king_moves_;         ///< Tracks if the white king has moved.
    bool b_king_moves_;         ///< Tracks if the black king has moved.
    bool wl_rook_moves_;        ///< Tracks if the white left rook has moved.
    bool wr_rook_moves_;        ///< Tracks if the white right rook has moved.
    bool bl_rook_moves_;        ///< Tracks if the black left rook has moved.
    bool br_rook_moves_;        ///< Tracks if the black right rook has moved.
    bool last_move_is_w_pawn_;  ///< Tracks if the last move involved a white pawn.
    bool last_move_is_b_pawn_;  ///< Tracks if the last move involved a black pawn.
    Coord coord_w_pawn_;        ///< Coordinates of the last moved white pawn.
    Coord coord_b_pawn_;        ///< Coordinates of the last moved black pawn.
    Colour currentTurn;         ///< The color of the player whose turn it is.
};
