#include "olcPixelGameEngine.h"
#include "ChessBoard.h"
#include "MoveStack.h"

class myApp : public olc::PixelGameEngine
{
public:
	ChessBoard myboard;
	MoveStack myboard_movestack = MoveStack(10);
	bool has_selected_piece = false;
	ChessPiece selected_piece;
	int selected_piece_x, selected_piece_y;

	myApp()
	{
		sAppName = "ChessBoard Application";
	}
	bool OnUserCreate() override
	{
		CB_InitBoard(myboard, this, "chess-pieces.png");
		return true;
	}

	bool OnUserUpdate(float elapsed) override
	{
		if (GetKey(olc::LEFT).bPressed)
		{
			myboard_movestack.BackwardBoardState(myboard);
		}
		if (GetKey(olc::RIGHT).bPressed)
		{
			myboard_movestack.ForwardBoardState(myboard);
		}

		if (GetMouse(0).bPressed)
		{
			olc::vi2d mouse_pos = GetMousePos();
			olc::vi2d piece_coord = CB_ScreenToBoard(mouse_pos);
			if (piece_coord.x >= 0)
			{
				// Get the piece
				selected_piece_x = piece_coord.x;
				selected_piece_y = piece_coord.y;
				has_selected_piece = true;
				selected_piece = CB_GetPiece(myboard, selected_piece_x, selected_piece_y);
				CB_DeletePiece(myboard, selected_piece_x, selected_piece_y);
			}
		}

		if (GetMouse(0).bReleased)
		{
			has_selected_piece = false;
			olc::vi2d mouse_pos = GetMousePos();
			olc::vi2d place_coord = CB_ScreenToBoard(mouse_pos);
			if (place_coord.x > 0)
			{
				// Now place the new piece.
				CB_PlacePiece(myboard, place_coord.x, place_coord.y, selected_piece);
				// Push the current board state
				myboard_movestack.PushBoardState(myboard);
			}
			else
			{
				CB_PlacePiece(myboard, selected_piece_x, selected_piece_y, selected_piece);
			}
			//std::cout << CB_GetFEN(myboard) << std::endl;
		}
		Clear(olc::BLUE);
		CB_DrawBoard(myboard, this);

		if (has_selected_piece)
		{
			CB_DrawPieceOnScreen(this, GetMousePos() - 0.5 * olc::vi2d(PIECE_SIZE, PIECE_SIZE), selected_piece);
		}
		return true;
	}
};

int main(void)
{
	myApp demo;
	if (demo.Construct(800, 600, 1, 1))
		demo.Start();
	return 0;
}
