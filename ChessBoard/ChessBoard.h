#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

// 60x60 chess pieces 
#define PIECE_SIZE 60

#include "olcPixelGameEngine.h"
#include <string>

typedef enum
{
	PIECE_QUEEN = 0,
	PIECE_KING = 1,
	PIECE_ROOK = 2,
	PIECE_KNIGHT = 3,
	PIECE_BISHOP = 4,
	PIECE_PAWN = 5,
	PIECE_NONE = -1
} ChessPieceType;

typedef struct
{
	ChessPieceType type;
	// 0 for white, 1 for black
	int colour;
} ChessPiece;

typedef ChessPiece ChessBoard[8][8];

void CB_InitBoard(ChessBoard& chess_board, olc::PixelGameEngine* app, std::string piece_sprite_sheet);
void CB_DrawPieceOnScreen(olc::PixelGameEngine* app, olc::vi2d pos, ChessPiece piece);
void CB_DrawPiece(olc::PixelGameEngine* app, int x, int y, ChessPiece piece);
void CB_DrawBoard(ChessBoard chess_board, olc::PixelGameEngine* app);
void CB_EmptyBoard(ChessBoard& chess_board);
void CB_PlacePiece(ChessBoard& chess_board, int x, int y, ChessPiece piece);
void CB_DeletePiece(ChessBoard& chess_board, int x, int y);
ChessPiece CB_GetPiece(ChessBoard& chess_board, int x, int y);
void CB_ReadFEN(ChessBoard& chess_board, std::string FEN);
std::string CB_GetFEN(ChessBoard chess_board);
olc::vi2d CB_BoardToScreenXY(int x, int y);
olc::vi2d CB_ScreenToBoard(olc::vi2d spos);

#endif
