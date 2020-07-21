// SpaceShip.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string.h>

using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80



//Este metodo posiciona el cursor en x,y sobre la consola.
void gotoxy(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_MODERN;
	cfi.FontWeight = FW_NORMAL;

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

//De la misma forma que antes, nos paramos en la consola, pero esta vez modificamos el cursor del aconsola ocultandolo.
void ocultarCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	

	SetConsoleCursorInfo(hCon, &cci); //& pasa la variable por referencia.
}

void pintarLimites() {

	for (int i = 2; i < 78; i++) {
		gotoxy(i, 3); printf("%c", 205);
		gotoxy(i, 33); printf("%c", 205);
	}

	for (int i = 4; i < 33; i++) {
		gotoxy(2, i); printf("%c", 186);
		gotoxy(77, i); printf("%c", 186);
	}

	gotoxy(2, 3); printf("%c", 201);
	gotoxy(2, 33); printf("%c", 200);
	gotoxy(77, 3); printf("%c", 187);
	gotoxy(77, 33); printf("%c", 188);
}


class NAVE {
	int x, y; //Atributos privados
	int corazones;
	int vidas;
public: //De esta forma definimos la interfaz publica de nuestro objeto para poder acceder a sus metodos.
	
	//Definimos un constructor y lo inicializamos como se muestra en los comentarios de abajo, pero todo a la vez.
	NAVE(int _x, int _y, int _corazones, int _vidas) : x(_x), y(_y), corazones(_corazones), vidas(_vidas) {};
	int X() { return x; }
	int Y() { return y; }
	void restar_corazones() { corazones--; }
	int get_vidas() { return vidas; }
	int get_corazones() { return corazones; }
	void pintar();
	void borrar();
	void mover();
	void pintar_corazones();
	void morir();

};//Se debe poner el ; sino da error.


/*
Para incializar el constructor se puede hacer esto:
	
	NAVE::NAVE(int _x, int _y) : x(_x), y(_y) {}

Otra forma de inicializar el constructor, es como se muestra abajo:
	
	NAVE::NAVE(int _x, int _y) {
	x = _x;
	y = _y;
}
*/

void NAVE::pintar() { //con :: podemos acceder a sus metodos.
	gotoxy(x, y);   printf("   %c", 30); //Si mandamos a imprimir un caracter con un numero entero, hace referencia al ascii
	gotoxy(x, y+1); printf("  %c%c%c", 40, 207, 41);
	gotoxy(x, y+2); printf(" %c%c %c%c", 30, 190, 190, 30);
}

void NAVE::borrar() {
	gotoxy(x, y);   printf("      ");
	gotoxy(x, y+1); printf("      ");
	gotoxy(x, y+2); printf("      ");
}

void NAVE::mover() {
	if (_kbhit()) {
		char tecla = _getch();
		borrar(); //Con esto vamos limpiando en asterisco de la posicion anterior cada que nos movemos.
		/*
		if (tecla == ARRIBA && y > 4) y--;
		if (tecla == DERECHA && x+6 < 77) x++;
		if (tecla == ABAJO && y+3 < 33) y++;
		if (tecla == IZQUIERDA && x > 3) x--;
		*/
		switch (tecla){
			case ARRIBA:
				if (y > 4) {
					y--;
				}
				break;
			case DERECHA:
				if (x+6 < 77) {
					x++;
				}
				break;
			case ABAJO:
				if (y+3 < 33) {
					y++;
				}
				break;
			case IZQUIERDA:
				if (x > 3) {
					x--;
				}
				break;
			default:
				x = x;
				y = y;
		}

		pintar();
		pintar_corazones();
	}
}

void NAVE::pintar_corazones() {
	gotoxy(50, 2); printf("VIDAS %d", vidas);
	gotoxy(64, 2); printf("Salud");
	gotoxy(70, 2); printf("      ");

	for (int i = 0; i < corazones; i++) {
		gotoxy(70 + i, 2); printf("%c", 3);
	}
}

void NAVE::morir() {
	if (corazones == 0) {
		borrar();
		gotoxy(x, y);	printf("  **  ");
		gotoxy(x, y+1); printf(" **** ");
		gotoxy(x, y+2); printf("  **  ");
		
		Sleep(200);

		borrar();
		gotoxy(x, y);	  printf("* ** *");
		gotoxy(x, y + 1); printf(" **** ");
		gotoxy(x, y + 2); printf("* ** *");
		
		Sleep(200);
		borrar();

		vidas--;
		corazones = 3;
		pintar_corazones();
		pintar();
	}
}

class ASTEROIDE {
	int x, y;
public:
	ASTEROIDE(int _x, int _y) : x(_x), y(_y) {};
	int X() { return x; }
	int Y() { return y; }
	void pintar();
	void mover();
	void choque(NAVE &Nave);
};

void ASTEROIDE::pintar() {
	gotoxy(x, y); printf("%c", 184);
}

void ASTEROIDE::mover() {
	gotoxy(x, y); printf(" ");
	y++;
	if (y > 32) {
		x = rand()%71 + 4; //Esto nos da un random entre 0 y 71, si sale 0 le suma 4 y si sale 71, le suma 4
		y = 4;				 //Esto hace que quede dentro de los limites de las paredes.
	}
	pintar(); //Llama al pintar de la clase ASTEROIDE porque estoy dentro de su clase.
}

void ASTEROIDE::choque(NAVE &Nave) {
	if (x >= Nave.X() && x < Nave.X() + 6 && y >= Nave.Y() && y <= Nave.Y() + 2) {
		Nave.restar_corazones();
		if (Nave.get_corazones() != 0) {
			for (int i = 0; i < 3; i++) {
				Nave.borrar();
				Sleep(100);
				Nave.pintar();
				Sleep(100);
			}
		}

		Nave.pintar_corazones();

		x = rand() % 71 + 4; //Esto nos da un random entre 0 y 71, si sale 0 le suma 4 y si sale 71, le suma 4
		y = 4;				 //Esto hace que quede dentro de los limites de las paredes.
	}
}

class BALA {
	int x, y;
public:
	BALA(int _x, int _y) : x(_x), y(_y) {};
	int X() { return x; }
	int Y() { return y; }
	void mover();
	bool fuera();
};

bool BALA::fuera() {
	if (y == 4) return true; //Si la bala llego al limite de arriba retorna true, sino false.
	return false;
}

void BALA::mover() {
	gotoxy(x, y); printf(" ");
	y--;
	gotoxy(x, y); printf("*");
}


int main()
{

	ocultarCursor();
	pintarLimites();
	//Instanciamos nuestra nave con sus posiciones inciales (7,7) y sus vidas 3.
	NAVE nave(37, 30, 3, 3);
	nave.pintar();
	nave.pintar_corazones();
	
	list<ASTEROIDE*> asteroides;
	list<ASTEROIDE*>::iterator asteroidesIterator;

	for (int i = 0; i < 5; i++) { //Aca podemos usar un for, porque solo estamos agregando objetos a la lista de punteros.
		asteroides.push_back(new ASTEROIDE(rand() % 75+3, rand() % 5+4));
	}

	list<BALA*> balas; //Esto es una lista de punteros del objetos BALA, se usa * para eso.
	list<BALA*>::iterator balasIterator; //No se puede recorrer una lista de punteros con indices, se debe crear un iterador de punteros.

	bool game_over = false;
	int puntos = 0;

	while (!game_over) {

		gotoxy(4, 2); printf("Puntos: %d", puntos);

		if (_kbhit()) {
			char tecla = _getch();
			if (tecla == 'a') {
				balas.push_back(new BALA(nave.X() + 2, nave.Y() - 1));
			}
		}

		for (balasIterator = balas.begin(); balasIterator != balas.end();) {
			//Con el * desreferenciamos el puntero y actua como un objeto de tipo BALA
			(*balasIterator)->mover();
			if ((*balasIterator)->fuera()) {
				gotoxy((*balasIterator)->X(), (*balasIterator)->Y()); printf(" ");
				delete(*balasIterator); //Elimina el iterador de la lista, pero si quitamos uno el iterador se invalida
				balasIterator = balas.erase(balasIterator); //Por eso debemos poner asignar al iterador al siguiente elemento del puntero.
			} else {
				balasIterator++;
			}
		}

		for (asteroidesIterator = asteroides.begin(); asteroidesIterator != asteroides.end(); asteroidesIterator++) {
			(*asteroidesIterator)->mover();
			(*asteroidesIterator)->choque(nave);
		}

		for (asteroidesIterator = asteroides.begin(); asteroidesIterator != asteroides.end(); asteroidesIterator++) { //Recorro la lista de punteros de asteroides
			for (balasIterator = balas.begin(); balasIterator != balas.end();) { //recorro la lista de punteros de balas
				//Si las balas se cruzan en una coordenada, o si se pasan de largo, borro la bala y el asteroide
				if ((*balasIterator)->X() == (*asteroidesIterator)->X() && ((*asteroidesIterator)->Y() +1 == (*balasIterator)->Y() || (*asteroidesIterator)->Y() == (*balasIterator)->Y())) {
		
					gotoxy((*balasIterator)->X(), (*balasIterator)->Y()); printf(" ");//Borro la bala
					delete(*balasIterator);
					balasIterator = balas.erase(balasIterator);

					asteroides.push_back(new ASTEROIDE(rand() % 75+3, 4)); //Meto un nuevo asteroide en la lista de asteroides.
					gotoxy((*asteroidesIterator)->X(), (*asteroidesIterator)->Y()); printf(" "); //Borro el asteroide que colisiona.
					delete(*asteroidesIterator);
					asteroidesIterator = asteroides.erase(asteroidesIterator);

					puntos += 100;

				} else {
					balasIterator++;
				}
			}
		}

		if (nave.get_vidas() == 0) {
			game_over = true;
		}

		nave.morir();
		nave.mover();
		
		Sleep(30);

	}

	gotoxy(35, 15); printf("GAME OVER");
	Sleep(5000);
	return 0;
}





// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
