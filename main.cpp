#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include "Header.h"

using namespace std;

void main() {

	Welcome();
	int q = -1;
	// Menu
	while (q != 0) {
		cout << "--------------------\nChoose:\n"
			<< "1 - NEW Game Set - on STANDART Board 3x3\n"
			<< "2 - RESTORE Last Session - on STANDART Board 3x3\n"
			<< "3 - NEW Game Set - on MANUAL SIZED Board\n"
			<< "4 - RESTORE Last Session - on MANUAL SIZED Board\n"
			<< "5 - PRINT the BEST Results Table\n"
			<< "6 - CLEAR the BEST Results Table\n"
			<< "7 - ONE of the few FACTS about Tic-Tac-Toe game\n"
			<< "8 - ABOUT Tic-Tac-Toe Game\n"
			<< "0 - to exit\n";
		cin >> q;

		switch (q) {
		case 0: Bye(); break;
		case 1:
		case 2:
		case 3:
		case 4: PlayGameSet(q); break;
		case 5: PrintBestResults(); break;
		case 6: ClearBestResults(); break;
		case 7: Facts(); break;
		case 8: AboutGame(); break;
		default: cout << "Wrong Input\n";
		}
	}

	system("Pause");
}

