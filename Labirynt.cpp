// POP 2016-12-05 projekt 1 Kraszkiewicz Dominik EiT 5 160256 Visual Studio 2015 Update 2
//Biblioteki
///////////////////////
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;
/////////Stale//////////
////////////////////////
//ilosc segmentow mapy
const int M_WIDTH = 2;
const int M_HEIGHT = 4;
//dlugosc segmentu
const int S_LENGTH = 5;
//ilosc skarbow
const int NUMBER_TREASURE = 5;
//ilosc segmentow
const int AMOUNT_SE = 6;
//zasieg widoku w ciezkim poziomie trudnosci
const int SIGHT = 2;
////////////////////////
////////////////////////
//Wybor segmentow przy losowaniu
enum Segments {
	SegmentA,
	SegmentB,
	SegmentC,
	SegmentD,
	SegmentE,
	SegmentF
};
////////////////////////
////////////////////////
//struktury
////////////////////////
static struct Segment{
	int segmentA[S_LENGTH][S_LENGTH] = { {-1,-1,0,-1,-1} ,{-1,0,0,-1,-1} ,{0,0,-1,0,0} ,{-1,0,0,0,-1} ,{-1,-1,0,-1,-1} };
	int segmentB[S_LENGTH][S_LENGTH] = { {-1,-1,0,0,-1}, {0,-1,-1,0,-1}, {0,0,0,0,0}, {-1,0,0,0,-1}, {-1,-1,0,0,-1} };
	int segmentC[S_LENGTH][S_LENGTH] = { {0,0,0,0,0},{0,0,-1,0,0},{0,0,-1,-1,0},{-1,0,-1,-1,-1},{-1,0,0,0,0} };
	int segmentD[S_LENGTH][S_LENGTH] = { {-1,-1,0,0,0},{-1,0,0,-1,0},{0,0,-1,0,0},{0,-1,0,0,-1},{-1,0,0,-1,-1} };
	int segmentE[S_LENGTH][S_LENGTH] = { {0,0,0,-1,-1},{-1,0,0,0,-1},{0,0,-1,0,0},{0,0,-1,0,0},{0,0,0,0,-1} };
	int segmentF[S_LENGTH][S_LENGTH] = { {-1,-1,0,-1,-1},{ -1,-1,0,-1,-1 },{0,0,0,0,0},{ -1,-1,0,-1,-1 },{ -1,-1,0,-1,-1 } };
};
struct Gracz {
	int points;
	int x;
	int y;
	int moves;
	Gracz(int nx, int ny) {
		this->x = nx;
		this-> y = ny;
		this->points = 0;
		this->moves = 0;
	}
};
////////////////////////
////////////////////////
///////Funkcje//////////
//WPROWADZENIE
void Intro() {
	cout << "Dominik Kraszkiewicz - Labirynt" << endl;
	cout << "Sterowanie :"<<endl<<"1.WSAD - poruszanie po mapie" << endl
		<< "2.M - Zmiania poziomu trudnosci."<<endl
		<<"3.Q - Szybkie wyjscie"<<endl;
	cout << "Nacisnij enter, by zaczac rozgrywke" <<endl<<endl<<endl<<endl;
	getchar();
}
//WYGRANA
void Outro(int moves) {
	std::cout << "Gratuluje przejscia Labiryntu!!!" << endl;
	std::cout << "Twoja ilosc ruchow w tej parti to: " << moves << endl;
	std::cout << "Nacisnij T aby kontynuowac lub N by zakonoczyc rozgrywke"<<endl;
}
//PRZEGRANA
void OutroL() {
	std::cout << "Moze nastepnym razem uda Ci sie przejsc labirynt ;(" << endl<<"Dziekuje za gre!!!"<<endl;
	std::cout << "Nacisnij T aby kontynuowac lub N by zakonoczyc rozgrywke"<<endl;
}
//Sterowanie
//przestawia gracza
void Move(Gracz &player, int map[][M_WIDTH*S_LENGTH],int nextx, int nexty,bool point) {
	map[player.y][player.x] = 0;
	player.x += nextx;
	player.y += nexty;
	map[player.y][player.x] = 1;
	if (point)player.points++;
	player.moves++;
}
//przetwarza wejscie z klawiatury
void Input(char input,bool &gameOver,Gracz &player, int map[][M_WIDTH*S_LENGTH], bool &mode) {
	if (input == 'Q' ) gameOver = false;
	else if (input == 'W') {
		if (map[player.y - 1][player.x] == 0 && player.y - 1 >= 0) {
			Move(player, map,0,-1,0);
		}
		else if (map[player.y - 1][player.x] == 2) {
			Move(player, map, 0, -1, 1);
		}
	}
	else if (input == 'A') {
		if (map[player.y][player.x-1] == 0 && player.x-1 >=0) {
			Move(player, map,-1,0,0);
		}
		else if (map[player.y][player.x - 1] == 2) {
			Move(player, map, -1, 0, 1);
		}
	}
	else if (input == 'S' && player.y + 1 < M_HEIGHT*S_LENGTH){
		if (map[player.y + 1][player.x] == 0) {
			Move(player, map, 0, +1, 0);
		}
		else if (map[player.y + 1][player.x] == 2) {
			Move(player, map, 0, +1, 1);
		}
	}
	else if (input == 'D' && player.x + 1 < M_WIDTH*S_LENGTH) {
		if (map[player.y][player.x+1] == 0) {
			Move(player, map, +1, 0, 0);
		}
		else if (map[player.y][player.x + 1] == 2) {
			Move(player, map, +1, 0, 1);
		}
	}
	else if (input == 'M') {
		mode = !mode;
	}
}
///////OBSLUGA MAPY//////
//Kopiuje segment w podany obszar mapy 
void CopyBoard(int map[][M_WIDTH*S_LENGTH],int source[][S_LENGTH],const int y,const int x) {
	for (int i = 0, a = y; i < S_LENGTH; i++,a++) {
		for (int j = 0, b = x; j < S_LENGTH; j++,b++) {
			map[a][b] = source[i][j];
		}
	}
}
//wypelnia mape segmentami uzywajac CopyBoard
void FillBoard(int map[][M_WIDTH*S_LENGTH]){
	int which;
	Segment temp;
	for (int i = 0; i < M_HEIGHT; i++) {
		for (int j = 0; j < M_WIDTH; j++) {
			which = rand() % AMOUNT_SE;
			switch(which) {
			case SegmentA:
				CopyBoard(map, temp.segmentA, i*S_LENGTH, j*S_LENGTH);
				break;
			case SegmentB:
				CopyBoard(map, temp.segmentB, i*S_LENGTH, j*S_LENGTH);
				break;
			case SegmentC:
				CopyBoard(map, temp.segmentC, i*S_LENGTH, j*S_LENGTH);
				break;
			case SegmentD:
				CopyBoard(map, temp.segmentD, i*S_LENGTH, j*S_LENGTH);
				break;
			case SegmentE:
				CopyBoard(map, temp.segmentE, i*S_LENGTH, j*S_LENGTH);
				break;
			case SegmentF:
				CopyBoard(map, temp.segmentF, i*S_LENGTH, j*S_LENGTH);
				break;
			}
		}
	}
}
///////WYGENEROWANIE GRACZA
Gracz SummonPlayer(int map[][M_WIDTH*S_LENGTH]) {
	int tempx = rand() % M_WIDTH * S_LENGTH;
	int tempy = rand() % M_HEIGHT * S_LENGTH;
	while (map[tempy][tempx] != 0) {
		tempx = (rand() % (M_WIDTH * S_LENGTH));
		tempy = (rand() % (M_WIDTH * S_LENGTH));
	}
	map[tempy][tempx] = 1;
	Gracz temp(tempx, tempy);
	return temp;
}
//WYGENEROWANIE SKARBU
void SpawnTreasure(int map[][M_WIDTH*S_LENGTH]) {
	int tempx = rand() % M_WIDTH * S_LENGTH;
	int tempy = rand() % M_HEIGHT * S_LENGTH;
	while (map[tempy][tempx] != 0) {
		tempx = (rand() % (M_WIDTH * S_LENGTH));
		tempy = (rand() % (M_WIDTH * S_LENGTH));
	}
	map[tempy][tempx] = 2;
}
//WIECEJ SKARBOW
inline void MoreTreasures(int map[][M_WIDTH*S_LENGTH]) {
	for (int i = 0; i < NUMBER_TREASURE; i++) {
		SpawnTreasure(map);
	}
}
//easy i hard to poziomy gry
//Renderuje mape
//-1 sciana #
// 0 = puste pole " "
// 1 = Gracz = P
// 2 = skarb = T
void EasyRender(int map[][M_WIDTH*S_LENGTH]) {
	for (int i = 0; i < S_LENGTH*M_WIDTH; i++) {
		cout << "^";
	}
	cout << endl;
	for (int i = 0; i < M_HEIGHT*S_LENGTH; i++) {
		for (int j = 0; j < M_WIDTH*S_LENGTH; j++) {
			if (map[i][j] == -1) cout << "#";
			else if (map[i][j] == 0) cout << " ";
			else if (map[i][j] == 1) cout << "P";
			else if (map[i][j] == 2) cout << "T";
		}
		cout << "|" << endl;
	}
	for (int i = 0; i < S_LENGTH*M_WIDTH; i++) {
		cout << "^";
	}
}
void HardRender(int map[][M_WIDTH*S_LENGTH], Gracz player) {
	//sprawdzamy odleglosc
	if (player.y - SIGHT < 0) {
		for (int i = 0; i < S_LENGTH * M_WIDTH; i++) {
			if (i <= player.x + SIGHT && i >= player.x - SIGHT) {
				cout << "^";
			}
			else cout << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < M_HEIGHT*S_LENGTH; i++) {
		for (int j = 0; j < M_WIDTH*S_LENGTH; j++) {
			if (map[i][j] == -1 && (abs(player.x - j) <= SIGHT && abs(player.y - i) <= SIGHT)) cout << "#";
			else if (map[i][j] == 0 && (abs(player.x - j) <= SIGHT && abs(player.y - i) <= SIGHT)) cout << " ";
			else if (map[i][j] == 1 && (abs(player.x - j) <= SIGHT && abs(player.y - i) <= SIGHT)) cout << "P";
			else if (map[i][j] == 2 && (abs(player.x - j) <= SIGHT && abs(player.y - i) <= SIGHT)) cout << "T";
			else cout << " ";
		}
		if (player.x + SIGHT >= S_LENGTH*M_WIDTH && (i <= player.y + SIGHT && i >= player.y - SIGHT)) {
			cout << "|";
		}
		cout << endl;
	}
	if (player.y + SIGHT >= S_LENGTH * M_HEIGHT) {
		for (int i = 0; i < S_LENGTH * M_WIDTH; i++) {
			if (i <= player.x + SIGHT && i >= player.x - SIGHT) {
				cout << "^";
			}
			else cout << " ";
		}
	}
}
void RenderBoard(int map[][M_WIDTH*S_LENGTH],bool mode,Gracz player) {
	//wyswietlamy mape zaleznie od trudnosci gry
	if (mode == true) EasyRender(map);
	else HardRender(map, player);	
}

//czy konczymy juz grac?
void GameOverStatement(bool &gameLoop) {
	char input = 0;
	while (true) {
		cin >> input;
		input = toupper(input);
		if (input == 'N') {
			gameLoop = false;
			break;
		}
		else if (input == 'T') {
			break;
		}
	}
}
//wypelnia mape (tablice) zerami
inline void Zeruj(int map[][M_WIDTH*S_LENGTH]) {
	for (int i = 0; i < M_HEIGHT*S_LENGTH; i++) {
		for (int j = 0; j < M_WIDTH*S_LENGTH; j++) {
			map[i][j] = 0;
		}
	}
}
///////////////////////////////
	//GLOWNA PETLA GRY!!!
	void GameLoop() {
		int map[M_HEIGHT*S_LENGTH][M_WIDTH*S_LENGTH];
		bool gameLoop = true;
		while (gameLoop) {
			Intro();
			bool gameOver = true;
			bool mode = false;
			char input;
			Zeruj(map);
			FillBoard(map);
			Gracz player = SummonPlayer(map);
			MoreTreasures(map);
			do {
				RenderBoard(map, mode, player);
				cin >> input;
				Input(input = toupper(input), gameOver, player, map, mode);
				cout << endl<<endl<<endl<<endl;
				if (player.points == NUMBER_TREASURE) gameOver = false;
			} while (gameOver);
			if (player.points == NUMBER_TREASURE) Outro(player.moves);
			else OutroL();
			GameOverStatement(gameLoop);
		}
	}
//////MAIN
//////////
//////////
int main() {
	srand(time(NULL));
	GameLoop();
}