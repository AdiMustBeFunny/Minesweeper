#include<iostream>
#include<vector>
#include<list>
#include<cstdlib>
#include<time.h>
#include<Windows.h>
using namespace std;

enum TileType
{
	Mine,
	Number
};

struct Tile
{
	TileType type=Number;
	int x, y;
	bool revealed = false;
	bool demined = false;
	int adjecentMineCount = 0;
};

class TileMap
{
public:




	void MainLoop()
	{
		int choice = -1;
		
		
		while (choice < 0 || choice > 2)
		{
			
			system("cls");
			cout << "Welocme to the Minesweeper" << endl;
			cout << "1. to start a new game" << endl;
			cout << "0. to quit" << endl;

			cin >> choice;

			switch (choice)
			{
			case 0:
				return;
				break;
			case 1:
				cout << "Set width: ";
				cin >> Width;
				cout << "Set height: ";
				cin >> Height;
				cout << "Set mine count: ";
				cin >> mineCount;
				firstTurn = true;
				playerRevealedCount = 0;
				playerDeminedCount = 0;
				maxPlayerDeminedCount = mineCount;
				clearBoard();
				//spawnMines();
				GameStatus status = gameLoop();

				if (status == Failure) 
				{
					system("cls");
					display();
					cout << "Maybe next time." << endl;
				}
				else {
					system("cls");
					display(); 
					cout << "Gratz mate you rock!!!" << endl; }
				system("pause");
				
				break;
			}
			choice = -1;
		}

		

	}

private:
	

	int Width, Height;
	int mineCount;
	int playerRevealedCount = 0;
	int playerDeminedCount = 0;
	int maxPlayerDeminedCount = 0;
	bool firstTurn = true;
	vector<vector<Tile>> board;

	void display()
	{
		for (int i = 0; i < Height; i++)
		{

			for (int j = 0; j < Width; j++)
			{
				if (board[i][j].revealed) {
					if (board[i][j].type == Number)cout << board[i][j].adjecentMineCount << ' ';
					else cout << 'X' << ' ';
				}
				else if(board[i][j].demined)
					cout << 'D' << ' ';
				else
					cout << '#' << ' ';
			}
			cout << endl;
		}
	}

	void clearBoard()
	{
		board.clear();
		for (int i = 0; i < Height; i++)
		{
			board.push_back(vector<Tile>());
			for (int j = 0; j < Width; j++)
			{
				Tile t;
				t.x = j;
				t.y = i;
				board[i].push_back(t);
			}
		}
	}
	void spawnMines(int x,int y)
	{
		for (int i = 0; i < mineCount; i++)
		{
			int rnd_x = rand() % Width;
			int rnd_y = rand() % Height;

			while (board[rnd_y][rnd_x].type == Mine && (rnd_x==x && rnd_y == y))//We make sure that we dont spawn mine on top of another 
			{
				rnd_x = rand() % Width;
				rnd_y = rand() % Height;
			}

			board[rnd_y][rnd_x].type = Mine;
			updateAdjacentNumbers(rnd_x, rnd_y);
		}
	}
	void updateAdjacentNumbers(int x,int y)
	{
		if (x > 0 && y > 0)board[y - 1][x - 1].adjecentMineCount += 1;
		if (x > 0 && y < Height-1)board[y + 1][x - 1].adjecentMineCount += 1;
		if (x < Width-1 && y > 0)board[y - 1][x + 1].adjecentMineCount += 1;
		if (x < Width - 1 && y < Height - 1)board[y + 1][x + 1].adjecentMineCount += 1;
		if (x > 0)board[y][x - 1].adjecentMineCount += 1;
		if (x < Width - 1)board[y][x + 1].adjecentMineCount += 1;
		if (y > 0)board[y - 1][x].adjecentMineCount += 1;
		if (y < Height - 1)board[y + 1][x].adjecentMineCount += 1;
	}

	
	enum GameStatus { Success, Failure} ;
	void setRevealed(int from_x, int from_y)
	{
		
		if (firstTurn)
		{
			firstTurn = false;
			spawnMines(from_x,from_y);
		}

		if (board[from_y][from_x].revealed == true || board[from_y][from_x].demined == true)return;

		list<Tile*> revealArea;
		revealArea.push_back(&board[from_y][from_x]);

		while (!revealArea.empty())
		{
			int beginSize = revealArea.size();
			list<Tile*>::iterator endd = revealArea.begin();
			advance(endd, beginSize);
			for (list<Tile*>::iterator tile = revealArea.begin(); tile !=endd;tile++)//auto tile : revealArea)
			{
				(*tile)->revealed = true;
				
				if ((*tile)->adjecentMineCount > 0)continue;//if it is not 0 then all we do is reveal and finish

				if ((*tile)->x > 0 && (*tile)->y > 0 && board[(*tile)->y - 1][(*tile)->x - 1].demined == false && board[(*tile)->y - 1][(*tile)->x - 1].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y - 1][(*tile)->x - 1]) { copy = true; break; }
					}
					if(!copy)
					revealArea.push_back(&board[(*tile)->y-1][(*tile)->x -1]);
				}
				if ((*tile)->x > 0 && (*tile)->y < Height-1 && board[(*tile)->y+1][(*tile)->x-1].demined == false && board[(*tile)->y+1][(*tile)->x-1].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y + 1][(*tile)->x - 1]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y + 1][(*tile)->x - 1]);
				}
				if ((*tile)->x < Width-1 && (*tile)->y > 0 && board[(*tile)->y-1][(*tile)->x+1].demined == false && board[(*tile)->y-1][(*tile)->x+1].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y - 1][(*tile)->x + 1]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y - 1][(*tile)->x + 1]);
				}
				if ((*tile)->x < Width-1 && (*tile)->y < Height-1 && board[(*tile)->y+1][(*tile)->x+1].demined == false && board[(*tile)->y+1][(*tile)->x+1].revealed == false)
				{ 
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y + 1][(*tile)->x + 1]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y + 1][(*tile)->x + 1]);
				}
				if ((*tile)->x > 0 && board[(*tile)->y][(*tile)->x-1].demined == false && board[(*tile)->y][(*tile)->x-1].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y][(*tile)->x - 1]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y][(*tile)->x - 1]);
				}
				if ((*tile)->x < Width-1 && board[(*tile)->y][(*tile)->x+1].demined == false && board[(*tile)->y][(*tile)->x+1].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y][(*tile)->x + 1]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y][(*tile)->x + 1]);
				}
				if ((*tile)->y > 0 && board[(*tile)->y-1][(*tile)->x].demined == false && board[(*tile)->y-1][(*tile)->x].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y - 1][(*tile)->x]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y - 1][(*tile)->x]);
				}
				if ((*tile)->y < Height-1 && board[(*tile)->y + 1][(*tile)->x].demined == false && board[(*tile)->y + 1][(*tile)->x].revealed == false)
				{
					bool copy = false;
					for (list<Tile*>::iterator t = revealArea.begin(); t != endd; t++)
					{
						if ((*t) == &board[(*tile)->y + 1][(*tile)->x]) { copy = true; break; }
					}
					if (!copy)
					revealArea.push_back(&board[(*tile)->y + 1][(*tile)->x]);
				}
			}
			for (int i = 0; i < beginSize; i++)
			{
				playerRevealedCount += 1;
				revealArea.pop_front();
			}

		}
	}
	void setDemined(int x, int y)
	{
		if (board[y][x].demined == true) { board[y][x].demined = false; playerDeminedCount -= 1; }
		else if(playerDeminedCount<maxPlayerDeminedCount)
		{
			board[y][x].demined = true;
			playerDeminedCount += 1;
		}
	}


	GameStatus gameLoop()
	{

		int desiredRevealCount = Width * Height - mineCount;

		while (playerRevealedCount != desiredRevealCount)
		{
			system("cls");
			cout << "Revealed: " << playerRevealedCount << ". To reveal: " << desiredRevealCount << endl;
			display();
			cout << "Which tile to reveal? (mode,x,y) modes: 0-reveal 1- mark mine. ex.\n0 1 2- reveal mine at x=1, y=2 \n1 0 0- mark mine at x=0, y=0" << endl;
			int p_x, p_y;
			int mode;
			cin >> mode >> p_x >> p_y;

			if (mode == 0)
			{
				if (board[p_y][p_x].type == Mine)return Failure;//exit
				else setRevealed(p_x, p_y); //board[p_y][p_x].revealed = true;
			}
			else//demine
			{
				setDemined(p_x, p_y);
			}

		}
		return Success;
	}





};


int main()
{
	srand(time(NULL));

	TileMap t;
	t.MainLoop();
	
	return 0;
}