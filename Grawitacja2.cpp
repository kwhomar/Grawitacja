// Grawitacja_klockow.cpp : Defines the entry point for the console application.
//



//Program jest wysoce nieoptymalny. Należy stworzyć char mapkę[x][y] do wyświetlania wysokosci. Jej wartosci zmieniac w funkcjach ruchu, stawiania
//i zdejmowania. Wyswietlac zawartosc tej mapki zeby za kazdym razem nie obliczac wszystkiego od nowa
#include "stdafx.h"

const int roz_x = 29, roz_y = 79, roz_z = 30;
// od 1 do roz elementy, reszta brzegi.
int mapa[roz_x + 2][roz_y + 2][roz_z + 2];
int najwyzszy = 0;

int koparka_x = 3, koparka_y = 3;

void grawitacja_lokalna(int x, int y, int z);
void grawituj(int x, int y, int z);
void wyswietl();
void global_grawituj();
int wysokosc(int, int);

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
COORD Position;
CONSOLE_CURSOR_INFO cursor_info;

//grawitacja(x,y,z)-> grawitacja(x+-, y+-, z+)
void setCursor(int x, int y)
{
	Position.Y = x;
	Position.X = y;
	SetConsoleCursorPosition(hOut, Position);
}





int main()
{
	cout << "ruch wasd, spacja stawia klocek ktory spada grawitacyjnie jak piasek, e usuwa" << endl;
	cout << "Liczby wskazuja wysokosc w danym miejscu. Minimalnie -1 (wyswietla sie jak 0)." << endl;
	cout << "Liczby wieksze od 9 sa mylace przez swoja dwucyfrowosc" << endl;
	cin.get();

	GetConsoleCursorInfo(hOut, &cursor_info);
	cursor_info.bVisible = 0;
	SetConsoleCursorInfo(hOut, &cursor_info);

	char klawisz;
	//inicjalizacja
	for (int i = 0; i < roz_x + 2; i++)
		for (int j = 0; j < roz_y + 2; j++)
			for (int k = 0; k < roz_z + 2; k++)
			{
				if ((i == 0) || (i == roz_x + 1) || (j == 0) || (j == roz_y + 1) || (k == 0) || (k == roz_z + 1))
					mapa[i][j][k] = 1;
				else mapa[i][j][k] = 0;
			}

	//for (int i = 0; i < 400; i++)
	while(1)
	{
		//mapa[15][20][20] = 1;
		//grawituj(15, 20, 20);
		if (_kbhit())
		{
			klawisz = _getch();
			switch (klawisz)
			{
			case 'w':
				koparka_x--;
				break;
			case 'a':
				koparka_y--;
				break;
			case 's':
				koparka_x++;
				break;
			case 'd':
				koparka_y++;
				break;
			case ' ':
				mapa[koparka_x][koparka_y][20] = 1;
				grawituj(koparka_x, koparka_y, 20);
				break;
			case 'e':
				mapa[koparka_x][koparka_y][wysokosc(koparka_x, koparka_y)] = 0;
				global_grawituj();
				break;
			}
		}
		wyswietl();
		setCursor(0, 0);
	}
	wyswietl();
	return 0;
}

void grawitacja_lokalna(int x, int y, int z)
{
	if (mapa[x][y][z - 1] == 0) mapa[x][y][z - 1] = 1;
	else if (mapa[x - 1][y][z - 1] == 0) mapa[x - 1][y][z - 1] = 1;
	else if (mapa[x + 1][y][z - 1] == 0) mapa[x + 1][y][z - 1] = 1;
	else if (mapa[x][y - 1][z - 1] == 0) mapa[x][y - 1][z - 1] = 1;
	else if (mapa[x][y + 1][z - 1] == 0) mapa[x][y + 1][z - 1] = 1;
	else mapa[x][y][z] = 1;
}

//odpala lokalna dopoki nie spadnie na maksa
void grawituj(int x, int y, int z)
{
	if (mapa[x][y][z] == 1)
	{
		if (mapa[x][y][z - 1] == 0)
		{
			mapa[x][y][z] = 0;
			mapa[x][y][z - 1] = 1;
			grawituj(x, y, z - 1);
		}
		else if (mapa[x - 1][y][z - 1] == 0)
		{
			mapa[x][y][z] = 0;
			mapa[x - 1][y][z - 1] = 1;
			grawituj(x - 1, y, z - 1);
		}
		else if (mapa[x + 1][y][z - 1] == 0)
		{
			mapa[x][y][z] = 0;
			mapa[x + 1][y][z - 1] = 1;
			grawituj(x + 1, y, z - 1);
		}
		else if (mapa[x][y - 1][z - 1] == 0)
		{
			mapa[x][y][z] = 0;
			mapa[x][y - 1][z - 1] = 1;
			grawituj(x, y - 1, z - 1);
		}
		else if (mapa[x][y + 1][z - 1] == 0)
		{
			mapa[x][y][z] = 0;
			mapa[x][y + 1][z - 1] = 1;
			grawituj(x, y + 1, z - 1);
		}
		else
			mapa[x][y][z] = 1;
	}
}

//wyswietla stan po zastosowaniu grawitacji
void wyswietl()
{
	int wys = 0;

	for (int i = 1; i <= roz_x; i++)
	{
		for (int j = 1; j <= roz_y; j++)
		{
			wys = wysokosc(i, j);
			if ((i == koparka_x) && (j == koparka_y))
				cout << 'x';
			else if (wys == 0)
				cout << " ";
			else
				cout << wys;
			if (wys > najwyzszy) najwyzszy = wys;
			wys = 0;
		}
		cout << endl;
	}

}

void global_grawituj()
{
	for (int k = 1; k <= najwyzszy; k++)
	{
		for (int i = 1; i <= roz_x; i++)
		{
			for (int j = 1; (j <= roz_y); j++)
				grawituj(i, j, k);
		}
	}
}

int wysokosc(int i, int j)
{
	int wysokosc = 0;
	for (int k = 1; (k <= roz_z && (mapa[i][j][k] == 1)); k++)
	{
		wysokosc++;
	}
	return wysokosc;
}