#include <string>

#include "ChessBoard.h"
#include "olcPixelGameEngine.h"

const int BOARD_WIDTH = PIECE_SIZE * 8;
const int BOARD_HEIGHT = PIECE_SIZE * 8;
const int BOARD_BORDER_SIZE = 20;

olc::vi2d board_start_position;
std::unique_ptr<olc::Sprite> chess_piece_sprites;
std::unique_ptr<olc::Decal> chess_piece_decal;

void CB_InitBoard(ChessBoard& chess_board, olc::PixelGameEngine* app, std::string piece_sprite_sheet)
{
	board_start_position = olc::vi2d((app->ScreenWidth() - BOARD_WIDTH) / 2, (app->ScreenHeight() - BOARD_HEIGHT) / 2);
	CB_EmptyBoard(chess_board);
	CB_ReadFEN(chess_board, "rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR");
	chess_piece_sprites = std::make_unique<olc::Sprite>(piece_sprite_sheet);
	chess_piece_decal = std::make_unique<olc::Decal>(chess_piece_sprites.get());
}

void CB_EmptyBoard(ChessBoard& chess_board)
{
	// First set all pieces to NONE
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			chess_board[i][j].type = PIECE_NONE;
		}
	}
}

void CB_PlacePiece(ChessBoard& chess_board, int x, int y, ChessPiece piece)
{
	x = x % 8;
	y = y % 8;
	chess_board[y][x].type = piece.type;
	chess_board[y][x].colour = piece.colour;
}

void CB_DeletePiece(ChessBoard& chess_board, int x, int y)
{
	x = x % 8;
	y = y % 8;
	chess_board[y][x].type = PIECE_NONE;
}

void CB_ReadFEN(ChessBoard& chess_board, std::string FEN)
{
	CB_EmptyBoard(chess_board);
	int cur_board_y = 7;
	int cur_board_x = 0;
	for (int i = 0; i < FEN.length(); i++)
	{
		char c = FEN[i];
		int piece_colour = 0;
		ChessPieceType piece_type = PIECE_NONE;
		if (isalpha(c))
		{
			if (c < 'a')
			{
				// Capital, white piece
				piece_colour = 1;
			}
			c = tolower(c);
			switch (c)
			{
			case 'r':
				piece_type = PIECE_ROOK;
				break;
			case 'n':
				piece_type = PIECE_KNIGHT;
				break;
			case 'b':
				piece_type = PIECE_BISHOP;
				break;
			case 'q':
				piece_type = PIECE_QUEEN;
				break;
			case 'k':
				piece_type = PIECE_KING;
				break;
			case 'p':
				piece_type = PIECE_PAWN;
				break;
			}
			chess_board[cur_board_y][cur_board_x].type = piece_type;
			chess_board[cur_board_y][cur_board_x].colour = piece_colour;
			cur_board_x++;
		}
		else if (c == '/')
		{
			cur_board_x = 0;
			cur_board_y--;
		}
		else if (isdigit(c))
		{
			cur_board_x += c - '0';
		}
		cur_board_x = cur_board_x % 8;
		cur_board_y = cur_board_y % 8;
	}
}
void CB_DrawPieceOnScreen(olc::PixelGameEngine* app, olc::vi2d pos, ChessPiece piece)
{
	if (piece.type == PIECE_NONE)
	{
		return;
	}
	int piece_start_x = piece.type * 60;
	int piece_start_y = piece.colour * 60;
	app->SetPixelMode(olc::Pixel::ALPHA);
	//app->DrawPartialSprite(pos, chess_piece_sprites.get(), olc::vi2d(piece_start_x, piece_start_y), olc::vi2d(60, 60), (PIECE_SIZE) / 60);
	app->DrawPartialDecal(pos, olc::vi2d(PIECE_SIZE, PIECE_SIZE), chess_piece_decal.get(), olc::vi2d(piece_start_x, piece_start_y), olc::vi2d(60, 60));
	app->SetPixelMode(olc::Pixel::NORMAL);
}
void CB_DrawPiece(olc::PixelGameEngine* app, int x, int y, ChessPiece piece)
{
	x = x % 8;
	y = y % 8;
	CB_DrawPieceOnScreen(app, CB_BoardToScreenXY(x, y), piece);
}

void CB_DrawBoard(ChessBoard chess_board, olc::PixelGameEngine* app)
{
	// Draw the shadow
	app->FillRect(board_start_position + 0.5 * olc::vi2d(BOARD_BORDER_SIZE, BOARD_BORDER_SIZE), olc::vi2d(BOARD_WIDTH + BOARD_BORDER_SIZE, BOARD_HEIGHT + BOARD_BORDER_SIZE), olc::GREY);
	// Draw a border
	app->FillRect(board_start_position - olc::vi2d(BOARD_BORDER_SIZE, BOARD_BORDER_SIZE), olc::vi2d(BOARD_WIDTH + BOARD_BORDER_SIZE*2, BOARD_HEIGHT + BOARD_BORDER_SIZE*2), olc::WHITE);
	// Draw row & column labels
	olc::vi2d column_text_pos = board_start_position - 0.5 * olc::vi2d(-2*BOARD_BORDER_SIZE, BOARD_BORDER_SIZE);
	olc::vi2d row_text_pos = board_start_position - 0.5 * olc::vi2d(BOARD_BORDER_SIZE, - 2 * BOARD_BORDER_SIZE);
	char c = 'a';
	for(int i = 0; i < 8; i++)
	{
		app->DrawString(column_text_pos + i * olc::vi2d(PIECE_SIZE, 0), std::string(1, c + i), olc::BLACK);
		app->DrawString(row_text_pos + i * olc::vi2d(0, PIECE_SIZE), std::string(1, '0' + i + 1), olc::BLACK);
	}
	// Now draw the board
	olc::vi2d block_size = olc::vi2d(PIECE_SIZE, PIECE_SIZE);
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			app->FillRect(CB_BoardToScreenXY(i, j), block_size, (i + j) % 2 ? olc::Pixel(0xFF, 0xE9, 0xC5) : olc::Pixel(0xC6, 0xB5, 0x99));
		}
	}
	
	// Draw the pieces
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			CB_DrawPiece(app, i, j, chess_board[j][i]);
		}
	}
}

ChessPiece CB_GetPiece(ChessBoard& chess_board, int x, int y)
{
	x = x % 8;
	y = y % 8;
	return chess_board[y][x];
}

olc::vi2d CB_BoardToScreenXY(int x, int y)
{
	return board_start_position + olc::vi2d(x, 7 - y) * olc::vi2d(PIECE_SIZE, PIECE_SIZE);
}

olc::vi2d CB_ScreenToBoard(olc::vi2d spos)
{
	olc::vi2d res;
	olc::vi2d board_end = board_start_position + olc::vi2d(8 * PIECE_SIZE, 8 * PIECE_SIZE);
	if (spos.x < board_start_position.x || spos.y < board_start_position.y || spos.x > board_end.x || spos.y > board_end.y)
	{
		res.x = -1;
		res.y = -1;
		return res;
	}
	
	res = spos - board_start_position;
	res /= PIECE_SIZE;
	res.y = 7 - res.y;
	return res;
}

std::string CB_GetFEN(ChessBoard chess_board)
{
	std::string result = "";
	int empty_cell_count = 0;
	for (int j = 7; j >= 0; j--)
	{
		for (int i = 0; i < 8; i++)
		{
			if (chess_board[j][i].type == PIECE_NONE)
			{
				empty_cell_count++;
				continue;
			}
			if (empty_cell_count > 0)
			{
				result += (empty_cell_count % 8) + '0';
				empty_cell_count = 0;
			}
			char piece_to_add = 'k';
			switch (chess_board[j][i].type)
			{
			case PIECE_KNIGHT:
				piece_to_add = 'n';
				break;
			case PIECE_PAWN:
				piece_to_add = 'p';
				break;
			case PIECE_BISHOP:
				piece_to_add = 'b';
				break;
			case PIECE_ROOK:
				piece_to_add = 'r';
				break;
			case PIECE_QUEEN:
				piece_to_add = 'q';
				break;
			default:
				break;
			}
			if (chess_board[j][i].colour)
			{
				piece_to_add = toupper(piece_to_add);
			}
			result += piece_to_add;
		}
		if (empty_cell_count != 0)
		{
			result += empty_cell_count + '0';
			empty_cell_count = 0;
		}
		result += "/";
	}
	return result;
}