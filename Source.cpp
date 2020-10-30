#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <cstring>
using namespace std;

// Start_finish program
void Welcome() {
	for (int i = 0; i <= 8; ++i) {
		if (i == 2) cout << "* Welcome to TIC-TAC-TOE! *\n";
		else if (i == 6) cout << "*       GOOD LUCK!        *\n";
		else {
			for (int j = 0; j <= 26; ++j) {
				if (i == 0 || i == 4 || i == 8) cout << "*";
				else if (j == 0 || j == 26) cout << "*";
				else cout << " ";
			}
			cout << "\n";
		}
	}
}
void Bye() {
	for (int i = 0; i <= 8; ++i) {
		if (i == 2) cout << "*    GOODBYE!    *\n";
		else if (i == 6) cout << "* SEE YOU LATER! *\n";
		else {
			for (int j = 0; j <= 17; ++j) {
				if (i == 0 || i == 4 || i == 8) cout << "*";
				else if (j == 0 || j == 17) cout << "*";
				else cout << " ";
			}
			cout << "\n";
		}
	}
}

// Structs
struct Player {
	char name[15];
	int wins = 0;
	int draw = 0;
	int loss = 0;
};
struct GameSet {
	char date[17];
	int game_set_number;
	int board_size = 3;
	char log_file[50];
	Player player1;
	Player player2;
	int counter = 0;
	bool move1 = true;
};
struct BestPlayer {
	char name[15];
	double coef;
};

// Initialization
void InitNames(GameSet* game_set) {
	char input;
	cout << "Do you want to Input Names instead of 'Player1' & 'Player2'?\n"
		<< "(y/n): ";
	cin >> input;
	if (input == 'y') {
		cin.ignore();
		cout << "Input FIRST Player name to play with 'X':\n";
		gets_s(game_set->player1.name, 15);
		cout << "Input SECOND Player name to play with '0':\n";
		gets_s(game_set->player2.name, 15);
	}
	else {
		strcpy(game_set->player1.name, "Player1");
		strcpy(game_set->player2.name, "Player2");
	}
}

void GetDate(char* date) {
	time_t now = time(0);
	tm *loc_time = localtime(&now);
	char tmp[5];
	_itoa(1900 + loc_time->tm_year, tmp, 10);
	strcpy(date, tmp);
	strcat(date, "_");
	_itoa(loc_time->tm_mon, tmp, 10);
	strcat(date, tmp);
	strcat(date, "_");
	_itoa(loc_time->tm_mday, tmp, 10);
	strcat(date, tmp);
}

void SaveLastGameSet(GameSet* game_set, int q) {
	char s0[25] = "last_game_set.txt";
	char s1[25] = "last_manual_board.txt";
	FILE* file;
	if (q==0) file = fopen(s0, "wt");
	else file = fopen(s1, "wt");
	fprintf(file, "%s %d %s %s %d %d %d %s %d %d %d %d %d %d", game_set->date,
		game_set->game_set_number, game_set->log_file, game_set->player1.name,
		game_set->player1.wins, game_set->player1.draw, game_set->player1.loss,
		game_set->player2.name, game_set->player2.wins, game_set->player2.draw,
		game_set->player2.loss, game_set->counter, game_set->move1, game_set->board_size);
	fclose(file);
}

bool RestoreSavedGameSet(GameSet* game_set, int q) {
	char s0[25] = "last_game_set.txt";
	char s1[25] = "last_manual_board.txt";
	FILE* file;
	if (q == 0) file = fopen(s0, "rt");
	else file = fopen(s1, "rt");

	if (file == nullptr) {
		cout << "No saved GameSet\nPlease choose 'NEW GameSet'\n";
		return false;
	}
	else {
		fscanf(file, "%s %d %s %s %d %d %d %s %d %d %d %d %d %d", &game_set->date,
			&game_set->game_set_number, &game_set->log_file, &game_set->player1.name,
			&game_set->player1.wins, &game_set->player1.draw, &game_set->player1.loss,
			&game_set->player2.name, &game_set->player2.wins, &game_set->player2.draw,
			&game_set->player2.loss, &game_set->counter, &game_set->move1, &game_set->board_size);
		fclose(file);
	}
	return true;
}

void LogFileNameCreate(GameSet* game_set, int q) {
	char tmp[50];
	strcpy(tmp, game_set->player1.name);
	strcat(tmp, "_");
	strcat(tmp, game_set->player2.name);
	strcat(tmp, "_");
	strcat(tmp, game_set->date);
	strcat(tmp, "_");
	if(q==1) strcat(tmp, "manual_");
	char t[5];
	_itoa(game_set->game_set_number, t, 10);
	strcat(tmp, t);
	strcat(tmp, ".log");
	strcpy(game_set->log_file, tmp);
}

void InitNewGameSet(GameSet* game_set, int q) {
	InitNames(game_set);
	GetDate(game_set->date);
	FILE* file;
	if (q==0) file = fopen("last_game_set.txt", "rt");
	else file = fopen("last_manual_board.txt", "rt");
	if (file == nullptr) game_set->game_set_number = 1;
	else {
		char date[20];
		int number;
		fscanf(file, "%s %d", &date, &number);
		number++;
		if (strcmp(date, game_set->date) == 0) game_set->game_set_number = number;
		else game_set->game_set_number = 1;
		fclose(file);
	}
	LogFileNameCreate(game_set, q);
}

// Board
char** CreateBoard(char **board, int n) {
	board = new char*[n];
	for (int i = 0; i < n; ++i) board[i] = new char[n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			board[i][j] = '.';
		}
	}
	return board;
}

// Print
void PrintBoard(char **board, int n) {
	bool first_row = true, first_col = true;
	int line = n * 4 + 4;
	for (int z = 0; z < line + 4; ++z) cout << "#";
	cout << "\n";
	for (int i = 0; i < n; ++i) {
		if (first_row) {
			for (int j = 0; j < n; ++j) {
				if (first_col) {
					cout << "# a-" << i+1 << " | " << board[i][j];
					first_col = false;
				}
				else {
					cout << " | " << board[i][j];
				}
			}
			first_row = false;
			cout << "  #\n";
		}
		else {
			cout << "# ";
			for (int z = 0; z < line; ++z) cout << "-";
			cout << " #\n";
			for (int j = 0; j < n; ++j) {
				if (first_col) {
					if (i < 10) cout << "#   " << i + 1 << " | " << board[i][j];
					else cout << "#  " << i + 1 << " | " << board[i][j];
					first_col = false;
				}
				else {
					cout << " | " << board[i][j];
				}
			}
			cout << "  #\n";
		}
		first_col = true;
	}
	cout << "# ";
	for (int z = 0; z < line; ++z) cout << "-";
	cout << " #\n";
	cout << "#   b-| 1 ";
	for (int z = 1; z < n; ++z) {
		if (z < 10) cout << "| " << z + 1 << " ";
		else cout << "| " << z + 1;
	}
	cout << " #\n";
	for (int z = 0; z < line + 4; ++z) cout << "#";
	cout << "\n";
}

void PrintGameSet(GameSet* game_set) {
	cout << game_set->date << " - GameSet #" << game_set->game_set_number << "\n"
		<< "You have played " << game_set->counter << " games\n"
		<< game_set->player1.name << " plays with 'X'\n"
		<< "Win - " << game_set->player1.wins
		<< "; Draw - " << game_set->player1.draw
		<< "; Loss - " << game_set->player1.loss << "\n"
		<< game_set->player2.name << " plays with '0'\n"
		<< "Win - " << game_set->player2.wins
		<< "; Draw - " << game_set->player2.draw
		<< "; Loss - " << game_set->player2.loss << "\n";
}

// the Best Result Table
void MakeBestResFile() {
	FILE* best;
	fopen_s(&best, "best_results.res", "wb");
	BestPlayer tmp;
	strcpy(tmp.name, "-----");
	tmp.coef = 0;
	for (int i = 0; i < 10; ++i) {
		fwrite(&tmp, sizeof(BestPlayer), 1, best);
	}
	fclose(best);
}

void ClearBestResults() {
	char c;
	cout << "CLEAR the Best Results Table?\n"
		<< "(y/n): "; cin >> c;
	if (c == 'y') MakeBestResFile();
	cout << "The BEST Results Table is clear\n";
}

double CountCoefficient(Player player, int counter) {
	double Coef = (double)((player.wins + player.draw * 0.25
		- player.loss) / counter) * 100 + counter;
	if (counter <= 20) Coef += counter;
	else if (counter > 20 && counter <= 40) Coef += (double)counter * 1.2;
	else if (counter > 40 && counter <= 100) Coef += (double)counter * 1.5;
	else if (counter > 100) Coef += (double)counter * 2;
	return Coef;
}

void PutInBestBase(BestPlayer* base, double Coef, Player player) {
	BestPlayer tmp;
	strcpy(base[9].name, player.name);
	base[9].coef = Coef;
	// put in right order
	for (int i = 9; i > 0; --i) {
		if (base[i].coef > base[i - 1].coef) {
			strcpy(tmp.name, base[i].name);
			strcpy(base[i].name, base[i - 1].name);
			strcpy(base[i - 1].name, tmp.name);
			tmp.coef = base[i].coef;
			base[i].coef = base[i - 1].coef;
			base[i - 1].coef = tmp.coef;
		}
	}
	cout << "Congratulations!\n"
		<< player.name << " - was added to the Best Result Table!\n";
}

void CountPlayerResult(GameSet* game_set, BestPlayer* base, bool& changed) {
	// count player1
	double Coef = CountCoefficient(game_set->player1, game_set->counter);
	if (Coef > base[9].coef) {
		PutInBestBase(base, Coef, game_set->player1);
		changed = true;
	}
	// count player2
	Coef = CountCoefficient(game_set->player2, game_set->counter);
	if (Coef > base[9].coef) {
		PutInBestBase(base, Coef, game_set->player2);
		changed = true;
	}
}

void SaveBestResult(GameSet* game_set) {
	// read best list file
	FILE* best;
	fopen_s(&best, "best_results.res", "rb");
	if (!best) {
		MakeBestResFile();
		fopen_s(&best, "best_results.res", "rb");
	}
	// copy to base
	BestPlayer* base = new BestPlayer[10];
	for (int i = 0; i < 10; ++i) {
		fread(&base[i], sizeof(BestPlayer), 1, best);
	}
	fclose(best);
	// count players
	bool changed = false;
	CountPlayerResult(game_set, base, changed);
	// if base was changed - write base to file
	if (changed) {
		fopen_s(&best, "best_results.res", "wb");
		for (int i = 0; i < 10; ++i) {
			fwrite(&base[i], sizeof(BestPlayer), 1, best);
		}
		fclose(best);
	}
	delete[] base;
}

void PrintBestResults() {
	FILE* best;
	fopen_s(&best, "best_results.res", "rb");
	if (best == nullptr) {
		MakeBestResFile();
		fopen_s(&best, "best_results.res", "rb");
	}
	cout << " Name\t\tPoints\n";
	BestPlayer tmp;
	for (int i = 0; i < 10; ++i) {
		fread(&tmp, sizeof(BestPlayer), 1, best);
		cout << tmp.name;
		if (strlen(tmp.name) < 8) cout << "\t\t";
		else cout << "\t";
		cout << tmp.coef << "\n";
	}
	fclose(best);
}

// Playing
void MakeMove(char **board, int size, char* name, char ch, FILE* log) {
	bool cell_empty = false;
	bool a_b = false;
	int a, b;
	cout << name << ", input a and b for your '" << ch << "'\n";
	while (!cell_empty || !a_b) {
		cin >> a >> b;
		if (a < 1 || a > size || b < 1 || b > size) {
			cout << "Sorry, wrong input\n"
				<<"Input a and b from 1 to " << size << ": ";
		}
		else {
			a_b = true;
			if (board[a - 1][b - 1] != '.') {
				cout << "Sorry, this CELL in use!\nInput another one: ";
			}
			else cell_empty = true;
		}
	}
	board[a - 1][b - 1] = ch;
	fprintf(log, "%d %d ", a, b);
}

int CheckWin(char** board, int size) {
	int counter_X = 0, counter_0 = 0, counter_Xc = 0, counter_0c = 0;
	// check rows & columns
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] == 'X') counter_X++;
			else if (board[i][j] == '0') counter_0++;
			if (board[j][i] == 'X') counter_Xc++;
			else if (board[j][i] == '0') counter_0c++;
		}
		if (counter_X == size || counter_Xc == size) return 1;
		else if (counter_0 == size || counter_0c == size) return 2;
		counter_X = counter_0 = counter_Xc = counter_0c = 0;
	}
	// check diagonals
	for (int i = 0, j = size - 1; i < size; ++i, --j) {
		if (board[i][i] == 'X') counter_X++;
		else if (board[i][i] == '0') counter_0++;
		if (board[i][j] == 'X') counter_Xc++;
		else if (board[i][j] == '0') counter_0c++;
	}
	if (counter_X == size || counter_Xc == size) return 1;
	else if (counter_0 == size || counter_0c == size) return 2;

	return 0;
}

void CountWinner(GameSet* game_set, FILE* log) {
	cout << "GameSet finished with:\n";
	fprintf(log, "GameSet_Results:\n");
	if (game_set->player1.wins > game_set->player2.wins) {
		cout << game_set->player1.name << " WON!\nCongratulations!\n";
		fprintf(log, "%s WON\n", game_set->player1.name);
	}
	else if (game_set->player1.wins < game_set->player2.wins) {
		cout << game_set->player2.name << " WON!\nCongratulations!\n";
		fprintf(log, "%s WON\n", game_set->player2.name);
	}
	else {
		cout << "DRAW!\n";
		fprintf(log, "DRAW\n");
	}
}

void PlayGame(GameSet* game_set, FILE* log) {
	int a, b, winner=0;
	char** board = new char*;
	// new board
	board = CreateBoard(board, game_set->board_size);
	game_set->counter++;
	cout << "Game #" << game_set->counter << "\n";
	fprintf(log, "Game# %d;", game_set->counter);
	PrintBoard(board, game_set->board_size);
	// X or 0
	if (game_set->counter % 2 == 1) {
		fprintf(log, " Starts 'X'\n");
		game_set->move1 = true;
	}
	else {
		fprintf(log, " Starts '0'\n");
		game_set->move1 = false;
	}
	// game
	int number_games = game_set->board_size * game_set->board_size;
	for (int i = 0; i < number_games; ++i) {
		// move
		if (game_set->move1) MakeMove(board, game_set->board_size,
			game_set->player1.name, 'X', log);
		else MakeMove(board, game_set->board_size,
			game_set->player2.name, '0', log);
		PrintBoard(board, game_set->board_size);
		// check
		if (i >= game_set->board_size * 2 - 2) {
			winner = CheckWin(board, game_set->board_size);
			if (winner == 1) {
				cout << "GAME OVER\n" << game_set->player1.name
					<< " - WIN! Congratulations!\n";
				game_set->player1.wins++;
				game_set->player2.loss++;
				break;
			}
			else if (winner == 2) {
				cout << "\tGAME OVER\n" << game_set->player2.name
					<< " - WIN! Congratulations!\n";
				game_set->player2.wins++;
				game_set->player1.loss++;
				break;
			}
		}
		// change move order
		if (game_set->move1) game_set->move1 = false;
		else game_set->move1 = true;
	}
	// draw
	if (winner == 0) {
		game_set->player1.draw++;
		game_set->player2.draw++;
		cout << "GAME OVER\nWith DRAW\n";
	}
	// end game log
	if (winner == 1) fprintf(log, "\nWin: %s;\n", game_set->player1.name);
	else if (winner == 2) fprintf(log, "\nWin: %s;\n", game_set->player2.name);
	else fprintf(log, "\nDraw\n");
}

void SubMenuGameSet(GameSet* game_set, FILE* log) {
	int q = -1;;
	while (q != 0) {
		cout << "---------------\n";
		cout << "1 - PLAY a new Game in this Set\n"
			<< "2 - PRINT this Set results\n"
			<< "0 - FINISH this GameSet & RETURN to the Main Menu\n"
			<< "Choose: ";
		cin >> q;

		if (q == 0) break;
		else if (q == 1) PlayGame(game_set, log);
		else if (q == 2) {
			cout << "---------------\n";
			PrintGameSet(game_set);
		}
		else cout << "Wrong Input!";
	}
}

bool ChooseGameSetMode(GameSet* game_set, int N) {
	bool restore = false;
	if (N == 1) InitNewGameSet(game_set, 0);
	else if (N == 2) {
		restore = RestoreSavedGameSet(game_set, 0);
		if (!restore) return false;
		cout << "---------------\n"
			<< "Your GameSet results for NOW:\n";
		PrintGameSet(game_set);
	}
	else if (N == 3) {
		cout << "You want to play a Manual Sized Board!\n"
			<< "Input SIZE for Your Board\n$:";
		cin >> game_set->board_size;
		InitNewGameSet(game_set, 1);
	}
	else if (N == 4) {
		restore = RestoreSavedGameSet(game_set, 1);
		if (!restore) return false;
		cout << "---------------\n"
			<< "Your GameSet results for NOW:\n";
		PrintGameSet(game_set);
	}
	return true;
}

void PlayGameSet(int N) {
	// make GameSet
	GameSet* game_set = new GameSet;
	FILE* log;

	// new or restore
	bool restore = ChooseGameSetMode(game_set, N);
	if (!restore) return;

	// start log
	log = fopen(game_set->log_file, "at");
	if (N == 1 || N == 3) fprintf(log, "Date: %s; GameSet# %d; BoardSize: %d;\n'X': %s; '0': %s;\n",
		game_set->date, game_set->game_set_number, game_set->board_size,
		game_set->player1.name, game_set->player2.name);

	// SubMenu
	SubMenuGameSet(game_set, log);

	// count winner in GameSet
	CountWinner(game_set, log);

	// end log
	fprintf(log, "%s: W %d; D %d; L %d\n%s: W %d; D %d; L %d\n",
		game_set->player1.name, game_set->player1.wins, game_set->player1.draw,
		game_set->player1.loss, game_set->player2.name, game_set->player2.wins,
		game_set->player2.draw, game_set->player2.loss);
	fclose(log);
	// save last GameSet
	if (N == 1 || N == 2) SaveLastGameSet(game_set, 0);
	else if (N == 3 || N == 4) SaveLastGameSet(game_set, 1);

	if (game_set->counter >= 10) SaveBestResult(game_set);
	delete game_set;
}

// about game
void AboutGame() {
	cout << "---------------\n"
		<< "To Play - Start New GameSet.\n"
		<< "or Restore the LAST GameSet.\n"
		<< "Each GamesSet contains an unlimited number of a single Games.\n"
		<< "After each GameSet with more than 10 games in it, your Points will be counted.\n"
		<< "Points depend on Number of Win and Number of Games in GameSet.\n"
		<< "The Best Points will be placed into the Best Result Table\n";
}

void Facts() {
	int n = rand() % 7;
	cout << "---------------\n";
	cout << "Do you know?\n";
	if (n == 0) {
		cout << "Tic-Tac-Toe games played on three-in-a-row boards\ncan be "
			<< "traced back to ancient Egypt,\nwhere such game boards"
			<< " have been found\non roofing tiles dating from around 1300 BCE.\n";
	}
	else if (n == 1) {
		cout << "An early variation of tic-tac-toe was played in the Roman Empire,\n"
			<< "around the first century BC. It was called 'terni lapilli'\n"
			<< "(three pebbles at a time) and instead of having any number of pieces,\n"
			<< "each player only had three, thus they had to move them around to empty\n"
			<< "spaces to keep playing. The game's grid markings have been found\n"
			<< "chalked all over Rome. Another closely related ancient game is three\n"
			<< "men's morris which is also played on a simple grid and requires three\n"
			<< "pieces in a row to finish,[8] and Picaria, a game of the Puebloans.\n";
	}
	else if (n == 2) {
		cout << "In 1952, OXO (or Noughts and Crosses),\n"
			<< "developed by British computer scientist Sandy Douglas\n"
			<< "for the EDSAC computer at the University of Cambridge,\n"
			<< "became one of the first known video games.\n"
			<< "The computer player could play perfect games\n"
			<< "of tic-tac-toe against a human opponent.\n";
	}
	else if (n == 3) {
		cout << "In 1975, tic-tac-toe was also used by MIT students\n"
			<< "to demonstrate the computational power of Tinkertoy elements.\n"
			<< "The Tinkertoy computer, made out of (almost) only Tinkertoys,\n"
			<< "is able to play tic-tac-toe perfectly.\n"
			<< "It is currently on display at the Museum of Science, Boston.\n";
	}
	else if (n == 4) {
		cout << "When considering only the state of the board,\n"
			<< "and after taking into account board symmetries\n"
			<< "(i.e. rotations and reflections),\n"
			<< "there are only 138 terminal board positions.\n"
			<< "A combinatorics study of the game shows that\n"
			<< "when 'X' makes the first move every time,\n"
			<< "the game outcomes are as follows:\n"
			<< "91 distinct positions are won by(X)\n"
			<< "44 distinct positions are won by(O)\n"
			<< "3 distinct positions are drawn(often called a \"cat's game\"";
	}
	else cout << "For more Facts and for Strategy to WIN go to:\n"
		<< "https://en.wikipedia.org/wiki/Tic-tac-toe\n";
}