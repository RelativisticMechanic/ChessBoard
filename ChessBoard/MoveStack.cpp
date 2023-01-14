#include "MoveStack.h"
#include "ChessBoard.h"

#include <cstdlib>

MoveStack::MoveStack(int max_backs, std::string initial_position)
{
	this->FENs = std::vector<std::string>(max_backs);
	this->max_backs = max_backs;
	this->current_max = 1;
	this->current_state = 1;
	this->FENs[0] = initial_position;
}

void MoveStack::PushBoardState(ChessBoard& chess_board)
{
	if (current_state < max_backs)
	{
		this->FENs[this->current_state] = CB_GetFEN(chess_board);
		this->current_state += 1;
	}
	else
	{
		this->FENs.erase(this->FENs.begin());
		this->FENs.push_back(std::string());
		this->current_state = this->max_backs - 1;
		this->FENs[this->current_state] = CB_GetFEN(chess_board);
	}
#ifdef MOVE_STACK_DEBUG
	std::cout << "Push board state: (" << this->current_state << "): " << this->FENs[this->current_state - 1] << std::endl;
#endif
	this->current_max = this->current_state;
}

void MoveStack::ForwardBoardState(ChessBoard& chess_board)
{
	if (this->current_state < this->current_max)
	{
		this->current_state += 1;
#ifdef MOVE_STACK_DEBUG
		std::cout << "Forward Board State (" << this->current_state << ")" << this->FENs[this->current_state - 1] << std::endl;
#endif
		CB_ReadFEN(chess_board, this->FENs[this->current_state - 1]);
	}
}

void MoveStack::BackwardBoardState(ChessBoard& chess_board)
{
	if (this->current_state > 0)
	{
		this->current_state -= 1;
#ifdef MOVE_STACK_DEBUG
		std::cout << "Backward Board State (" << this->current_state << ")" << this->FENs[this->current_state - 1] << std::endl;
#endif
		CB_ReadFEN(chess_board, this->FENs[this->current_state - 1]);
	}
}