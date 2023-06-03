// libreria
#include <iostream>
#include <windows.h>
#include <conio.h>

// Teclas de Juego
#define TOP 72
#define LEFT 75
#define RIGHT 77
#define BOTTOM 80
#define SPACE 56
#define ESC 27
// datos Juego
#define BORDER_VERTICAL 219	  // █
#define BORDER_HORIZONTAL 220 // ▄
#define HIGH 25
#define WIDTH 115
#define INIT_X_SNAKE WIDTH / 2 + 3
#define INIT_Y_SNAKE HIGH / 2 + 1
#define FOOD_ICON 64

// Snake Global
int Snake[200][2],
	teclaDirection = RIGHT,
	tamanioSnake = 10,
	ICON_SNAKE = 64,
	food[2],
	score = 0,
	teclaAnterior;

// funcion para posicionar cursor para pintar
void gotoxy(int x, int y)
{
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}
// renderiza el caudro de juego
void renderLimit()
{
	// lineas Verticales
	for (int y = 1; y < HIGH + 1; y++)
	{
		gotoxy(3, y);
		printf("%c", BORDER_VERTICAL);
		gotoxy(WIDTH, y);
		printf("%c", BORDER_VERTICAL);
	}
	// Lineas horizontales
	for (int x = 3; x < WIDTH + 1; x++)
	{
		gotoxy(x, 1);
		printf("%c", BORDER_HORIZONTAL);
		gotoxy(x, HIGH);
		printf("%c", BORDER_HORIZONTAL);
	}
	// esquinas
	gotoxy(3, HIGH);
	printf("%c", BORDER_VERTICAL);
	gotoxy(WIDTH, HIGH);
	printf("%c", BORDER_VERTICAL);
	// score
	gotoxy(WIDTH - 8, HIGH + 1);
	printf("Score: %d", score);
}
// renderizado y logica de Snake
void renderSnake()
{
	// renderizado de Snake
	for (int i = 0; i < tamanioSnake; i++)
	{
		gotoxy(Snake[i][0], Snake[i][1]);
		printf("%c", ICON_SNAKE);
	}
}
// elimina residuos del ultimo renderizado
void deletePosition()
{
	gotoxy(Snake[tamanioSnake - 1][0], Snake[tamanioSnake - 1][1]);
	printf(" ");
}
// inicio del Snake (solo se ejecuta una vez)
void initSnake()
{
	// ubicacion de snake
	Snake[0][0] = WIDTH / 2 + 3;
	Snake[0][1] = HIGH / 2 + 1;
	int temporal = Snake[0][1];
	//renderiza a partir de la cabeza todo el cuerpo
	for (int i = 1; i < tamanioSnake; i++)
	{
		Snake[i][0] = Snake[0][0];
		Snake[i][1] = temporal + 1;
		temporal++;
	}
}
// actualiza la posicion dependiendo de
void UpdatePosition()
{
	// seguimiento de Snake
	int temporal[2];
	temporal[0] = Snake[0][0];
	temporal[1] = Snake[0][1];
	if (teclaDirection == TOP)
	{
		Snake[0][1]--;
	}
	else if (teclaDirection == BOTTOM)
	{
		Snake[0][1]++;
	}
	else if (teclaDirection == LEFT)
	{
		Snake[0][0]--;
	}

	else if (teclaDirection == RIGHT)
	{
		Snake[0][0]++;
	}
	//sigue la cabeza del snake
	for (int i = 1; i < tamanioSnake; i++)
	{
		int temp[2];
		temp[0] = Snake[i][0];
		temp[1] = Snake[i][1];
		Snake[i][0] = temporal[0];
		Snake[i][1] = temporal[1];
		temporal[0] = temp[0];
		temporal[1] = temp[1];
	}
}
// logica de la comida del Snake
void footLogic(bool generate)
{	//dependiendo de repead se ejecuta uno o otro codigo
	bool repeat = false;
	if (generate)
	//codigo general aleatoriamente el foot
	{
		food[0] = rand() % (WIDTH - 1) + 4;
		food[1] = rand() % (HIGH - 1) + 2;
		for (int i = 0; i < tamanioSnake; i++)
		{
			if (Snake[i][0] == food[0] && Snake[i][1] == food[1])
			{
				repeat = true;
			}
			if (repeat)
			{
				footLogic(true);
			}
		}
		gotoxy(food[0], food[1]);
		printf("%c", FOOD_ICON);
	}
	else if (Snake[0][0] == food[0] && Snake[0][1] == food[1])
	{	//codigo para añadir al score y volver a renderizar foot
		score++;
		gotoxy(WIDTH - 1, HIGH + 1);
		printf("%d", score);
		footLogic(true);
		tamanioSnake++;
	}
}
// Evalua si se termina el juego
void gameOver()
{	//cuando se toca una pared se acaba el juego
	if (Snake[0][0] == 3 || Snake[0][1] == 1 || Snake[0][0] == WIDTH || Snake[0][1] == HIGH)
	{
		gotoxy(WIDTH / 2 + 3 - 4, HIGH + 1);
		printf("Game Over");
		teclaDirection = ESC;
	}
	int temporal[2];
	for (int i = 1; i < tamanioSnake; i++)
	{
		if (Snake[0][0] == Snake[i][0] && Snake[0][1] == Snake[i][1]) {
                gotoxy(WIDTH / 2 + 3 - 4, HIGH + 1);
		printf("Game Over");
			teclaDirection = ESC;
			break;
		}
	}

}
// escucha la tecla a pulsar
void keyLogic()
{	//escucha si se ha oprimido una tecla
	if (kbhit())
	{	//evalua que no sea la misma ya antes guardada
		if (getch() != teclaDirection)
		{
			teclaAnterior = teclaDirection;
			teclaDirection = getch();

			//evalua que no sea direccion contraria
			if (teclaAnterior == RIGHT && teclaDirection == LEFT)
				teclaDirection = teclaAnterior;
			else if (teclaAnterior == LEFT && teclaDirection == RIGHT)
				teclaDirection = teclaAnterior;
			else if (teclaAnterior == TOP && teclaDirection == BOTTOM)
				teclaDirection = teclaAnterior;
			else if (teclaAnterior == BOTTOM && teclaDirection == TOP)
				teclaDirection = teclaAnterior;
		}
	}
}

// funcion de ejecucion principal
int main()
{
	initSnake();
	renderLimit();
	renderSnake();
	footLogic(true);
	//bucle del juego
	while (teclaDirection != ESC)
	{
		gameOver();
		Sleep(150);
		footLogic(false);
		deletePosition();
		UpdatePosition();
		renderSnake();
		gotoxy(WIDTH, HIGH);
		keyLogic();
	}
	teclaDirection = TOP;
	gotoxy(INIT_X_SNAKE - 10, INIT_Y_SNAKE);
	printf("Pulse ESC para salir.");
	while (teclaDirection != ESC)
	{
		Sleep(150);
		keyLogic();
	}
	return 0;
}
