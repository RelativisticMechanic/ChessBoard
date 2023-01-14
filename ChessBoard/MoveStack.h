#ifndef MOVE_STACK_H
#define MOVE_STACK_H

#include <vector>
#include "ChessBoard.h"

#define FEN_START "rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR"

class MoveStack
{
private:
	std::vector<std::string> FENs;
	int max_backs;
	int current_max;
	int current_state;

public:
	MoveStack(int max_backs, std::string initial_position=FEN_START);
	void PushBoardState(ChessBoard& chess_board);
	void ForwardBoardState(ChessBoard& chess_board);
	void BackwardBoardState(ChessBoard& chess_board);
};
#endif
