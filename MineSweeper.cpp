#include "MineSweeper.h"

MineSweeper::MineSweeper(int level, Panel ^ drawingPanel)
{
	bombed = false;
	win = false;
	difficulty = level;
	if (level == 1)
	{
		boardSize = EASY;
		numMines = EASY_MINE;
		time = EASY_MINE * TIME_MULTIPLIER;		
	}
	else if (level == 2)
	{
		boardSize = MID;
		numMines = MID_MINE;
		time = MID_MINE * TIME_MULTIPLIER;
	}
	else
	{
		boardSize = HARD;
		numMines = HARD_MINE;
		time = HARD_MINE * TIME_MULTIPLIER;
	}
	
	mines = new Board(boardSize, numMines);
	game = new Board(boardSize, numMines);
	PlaceMines();	

	panel = drawingPanel;
	panel->Height = CELLSIZE * boardSize;
	panel->Width = CELLSIZE * boardSize;
	zero = gcnew Bitmap("0.bmp");
	one = gcnew Bitmap("1.bmp");
	two = gcnew Bitmap("2.bmp");
	three = gcnew Bitmap("3.bmp");
	four = gcnew Bitmap("4.bmp");
	five = gcnew Bitmap("5.bmp");
	six = gcnew Bitmap("6.bmp");
	seven = gcnew Bitmap("7.bmp");
	eight = gcnew Bitmap("8.bmp");
	mine = gcnew Bitmap("mine.bmp");
	flag = gcnew Bitmap("flag.bmp");
	closed = gcnew Bitmap("closed.bmp");
}

void MineSweeper::PlaceMines()
{
	int count = 0;
	int x, y;
	while (count < numMines)
	{
		x = rand() % boardSize;
		y = rand() % boardSize;
		if (mines->GetCell(x,y) != -1)
		{
			mines->SetCell(x, y, -1);
			count++;
		}
	}
}

void MineSweeper::Mark(int row, int col)
{
	if (game->GetCell(row, col) == -1)
	{
		game->SetCell(row, col, INIT);
		numMines++;
	}
	else if (game->GetCell(row, col) == INIT)
	{
		game->SetCell(row, col, -2);
		numMines--;
	}
	Show();
	if (numMines == 0)
		CheckWin();
}

void MineSweeper::CheckWin() 
{
	
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
			if (mines->GetCell(i, j) == -1 && game->GetCell(i, j) != -2)
			{
				bombed = true;
				win = false;
				break;
			}
		if (bombed == true)
			break;
	}
	if (bombed == false)
		win = true;
}

void MineSweeper::Reveal(int row, int col)
{
	
	// Do: Implement Reveal function recursively.
	// Remember to explain the meaning of each base
	// and recursive case in clear English in 
	// the context of the game

	if (mines->GetCell(row, col) == -1) // Check if it is a bomb, if so show mines and end game.
	{
		ShowMines();
		CheckWin();
	}
	else if (game->GetCell(row, col) != 9) // Check if a spot is already revealed, do nothing if so.
	{
		return;
	}
	else // Check how many bombs around spot, set the spot. If spot has no bombs around it reveal all around it that are 0-8 count.
	{
		int count = AdjacentMineCount(row, col);
		game->SetCell(row, col, count);
		if (game->GetCell(row, col) < 9 && game->GetCell(row, col) > 0)
		{
			Show();
		}
		else if (game->GetCell(row, col) == 0)
		{
			Reveal(row - 1, col - 1);
			Reveal(row - 1, col);
			Reveal(row - 1, col + 1);
			Reveal(row, col - 1);
			Reveal(row, col + 1);
			Reveal(row + 1, col - 1);
			Reveal(row + 1, col);
			Reveal(row + 1, col + 1);
		}
	}
}

int MineSweeper::AdjacentMineCount(int row, int col) const
{
	int count = 0;
	if (mines->GetCell(row-1, col-1) == -1)
		count++;
	if (mines->GetCell(row-1, col) == -1)
		count++;
	if (mines->GetCell(row-1, col+1) == -1)
		count++;
	if (mines->GetCell(row, col-1) == -1)
		count++;
	if (mines->GetCell(row, col+1) == -1)
		count++;
	if (mines->GetCell(row+1, col-1) == -1)
		count++;
	if (mines->GetCell(row+1, col) == -1)
		count++;
	if (mines->GetCell(row+1, col+1) == -1)
		count++;
	return count;
}

void MineSweeper::Show() const
{
	int x = 0;
	int y = 0;
	Graphics^ g = panel->CreateGraphics();
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			int cell = game->GetCell(i, j);
			if (cell == 0)
				g->FillRectangle(gcnew TextureBrush(zero), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 1)
				g->FillRectangle(gcnew TextureBrush(one), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 2)
				g->FillRectangle(gcnew TextureBrush(two), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 3)
				g->FillRectangle(gcnew TextureBrush(three), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 4)
				g->FillRectangle(gcnew TextureBrush(four), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 5)
				g->FillRectangle(gcnew TextureBrush(five), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 6)
				g->FillRectangle(gcnew TextureBrush(six), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 7)
				g->FillRectangle(gcnew TextureBrush(seven), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 8)
				g->FillRectangle(gcnew TextureBrush(eight), x, y, CELLSIZE, CELLSIZE);
			else if (cell == 9)
				g->FillRectangle(gcnew TextureBrush(closed), x, y, CELLSIZE, CELLSIZE);
			else if (cell == -2)
				g->FillRectangle(gcnew TextureBrush(flag), x, y, CELLSIZE, CELLSIZE);

			x += CELLSIZE;
		}
		x = 0;
		y += CELLSIZE;
	}
}

void MineSweeper::ShowMines() const
{
	int x = 0;
	int y = 0;
	Graphics^ g = panel->CreateGraphics();
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			int cell = mines->GetCell(i, j);
			if (cell == -1)
				g->FillRectangle(gcnew TextureBrush(mine), x, y, CELLSIZE, CELLSIZE);

			x += CELLSIZE;
		}
		x = 0;
		y += CELLSIZE;
	}
}