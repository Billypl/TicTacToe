#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cctype>
#include <stdlib.h>
#define BG_GREY 0x808080

using namespace std;

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

const int starting_pos = 7;
int x = starting_pos + 4, y = starting_pos + 2;
int virtualX, virtualY;
bool win = false;
bool currTurn = true; //true = X, false = O

char board[3][3] = {
	{' ',' ',' '},
	{' ',' ',' '},
	{' ',' ',' '}
};

void create_virtuals() {
	//zmienne do używania w tablicy "board"
	virtualX = x - starting_pos;
	if (virtualX != 0) virtualX /= 4;
	virtualY = y - starting_pos;
	if (virtualY != 0) virtualY /= 2;
}

string XorY() {
	if (currTurn)
		return "X";
	else
		return "O";
}

void show_turn() {
	gotoxy(4, 4);
	cout << "Ruch gracza: \"" << XorY() << "\"";
}

void print_board() {
	gotoxy(starting_pos, starting_pos);
	cout << "  " << "|" << "   " << "|" << "  ";
	gotoxy(starting_pos, starting_pos + 1);
	cout << "--" << "|" << "---" << "|" << "--";
	gotoxy(starting_pos, starting_pos + 2);
	cout << "  " << "|" << "   " << "|" << "  ";
	gotoxy(starting_pos, starting_pos + 3);
	cout << "--" << "|" << "---" << "|" << "--";
	gotoxy(starting_pos, starting_pos + 4);
	cout << "  " << "|" << "   " << "|" << "  ";

}

void print_full_board() {
	for (int i = starting_pos, I = 0; I < 3; i += 4, I++)
	{
		for (int j = starting_pos, J = 0; J < 3; j += 2, J++)
		{
			gotoxy(i, j);
			cout << board[I][J];
		}
	}
}

void change_coord(int move_x, int move_y) {
	//zabezpieczenie nie wyjścia z zakresu wizualnego
	if (x + move_x < starting_pos + 9 && x + move_x >= starting_pos) x += move_x;
	if (y + move_y < starting_pos + 5 && y + move_y >= starting_pos) y += move_y;
	//gotoxy(2, 2); cout << "X: " << x << " Y: " << y; //wyświetlanie aktualnej pozycji

//czyszczenie zaznaczeń
	print_full_board();

	//zaznaczenie aktualnego okienka
	gotoxy(x, y);
	SetConsoleTextAttribute(console, BG_GREY);

	//stworzenie virtual, żeby móc wyświetlić wartość na tle
	create_virtuals();
	cout << board[virtualX][virtualY];

	SetConsoleTextAttribute(console, 7);
}

void restart()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			board[i][j] = ' ';
	}

	system("CLS");
	gotoxy(1, 1);
	cout << "Press W,A,S,D to move on the board and P to place your selection" << endl;
	cout << "To restart press R";

	print_board();
	currTurn = true;
	win = false;
	show_turn();
	x = starting_pos + 4, y = starting_pos + 2;
	gotoxy(starting_pos + 4, starting_pos + 2);
}

void play_again() {
	gotoxy(1, 16);
	cout << "Do you want to play again? (type Y/N): ";
	char again;
	while (again = _getch())
	{
		again = tolower(again);
		if (again == 'y')
		{
			restart();
			break;
		}
		else if (again == 'n')
		{
			exit(0);
		}
	}
}

void checking() {
	if ((board[0][0] != ' ' &&
		(
			(board[0][0] == board[0][1] && board[0][0] == board[0][2]) ||
			(board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
			(board[0][0] == board[1][0] && board[1][0] == board[2][0])
			)) ||

		((board[2][0] != ' ' &&
			(
				(board[2][0] == board[1][1] && board[1][1] == board[0][2]) ||
				(board[2][0] == board[2][1] && board[2][1] == board[2][2])
				)) ||

			(board[1][0] != ' ' && board[1][0] == board[1][1] && board[1][1] == board[1][2]) ||
			(board[0][1] != ' ' && board[0][1] == board[1][1] && board[1][1] == board[2][1]) ||
			(board[0][2] != ' ' && board[0][2] == board[1][2] && board[1][2] == board[2][2])))
	{
		gotoxy(4, 13);
		currTurn = !currTurn;
		cout << "Player \"" << XorY() << "\" wins!";
		win = true;
	}

	bool draw = false;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != ' ')
				draw = true;
			else
			{
				draw = false;
				goto out_draw;
			}
		}
	}
out_draw:
	if (draw == true)
	{
		gotoxy(1, 15);
		cout << "Draw!";
		play_again();
	}
}

void set_field() {
	//stworzenie X i Y potrzebnego do zmieniania wartości w tablicy
	create_virtuals();

	//gotoxy(3, 3);
	//cout << "X: " << virtualX << " Y:" << virtualY;
	show_turn();

	//sprawdzenie czy puste i zmiana wartości (pola i tury gracza)
	if (board[virtualX][virtualY] == ' ')
	{
		if (currTurn)
			board[virtualX][virtualY] = 'X';
		else
			board[virtualX][virtualY] = 'O';
		currTurn = !currTurn;
		print_full_board();
		checking();
	}
	show_turn();
	gotoxy(x, y);
}

int main()
{
	cout << "Press W,A,S,D to move on the board and P to place your selection" << endl;
	cout << "To restart press R";
	enum valid_key {
		up = 'w',
		down = 's',
		right = 'a',
		left = 'd',

		Restart = 'r',
		enter = 'p'
	};

	print_board();
	show_turn();

	gotoxy(x, y);
	while (char key = _getch())
	{
		switch (tolower(key))
		{
		case up:
			change_coord(0, -2);
			break;
		case down:
			change_coord(0, 2);
			break;
		case right:
			change_coord(-4, 0);
			break;
		case left:
			change_coord(4, 0);
			break;
		case enter:
			set_field();
			break;
		case Restart:
			restart();
			break;
		}
		if (win)
		{
			play_again();
		}
	}



	for (int u = 0; u < 5; u++) cout << endl;
	return 0;
}