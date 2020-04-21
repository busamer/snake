#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string.h>
#define ACIK_EKRAN STD_OUTPUT_HANDLE
using namespace std;

enum YONLER{ sag, sol ,asagi , yukari , bos };


const int MAX_YILAN_BOYU = 200;
const int YUKSEKLIK = 20;
const int GENISLIK = 80;

const char ch = 219;
//const int genislik=80,yukseklik=20;
//char sahne[genislik][yukseklik];
char tuslar[256];



struct parca{
	int x;
	int y;
	int kontrol;
	
	YONLER yon;
};

class YEM{
	public:
		int x;
		int y;
		char sekil = '*';
		void yeniYemOlustur();
		void yemiSahneyeYaz();


};

YEM yem;

void parcaHareketEttir(parca *p){
	if((*p).yon == sag){
		(*p).x++;
	}
	else if((*p).yon == sol){
		(*p).x--;
	}
	else if((*p).yon == asagi){
		(*p).y++;
	}
	else if((*p).yon == yukari){
		(*p).y--;
	}
}
void klavyeKontrol(){
	for(int i = 0;i<256;i++){
		tuslar[i] = (char)(GetAsyncKeyState(i) >> 8);
	}
}

YONLER yeniYon(){	//klavyede basýlý olan yönü döndürür
	klavyeKontrol();
	if(tuslar['A']){
		return sol;
	}else if(tuslar['S']){
		return asagi;
	}else if(tuslar['D']){
		return sag;
	}else if(tuslar['W']){
		return yukari;
	}else{
		return bos;
	}
}




class SAHNE{
	public:
		int genislik;
		int yukseklik ;
		char sahne[GENISLIK][YUKSEKLIK];


		void cursorGizle();
		void ciz();
		void temizle();
		void sinirYaz();
		void git(int x, int y);
		
};

class YILAN{
	public:
		int kendiniYediMi;
		parca yilan[MAX_YILAN_BOYU];		
		int yilan_boyu;		
		void sifirla();
		void olustur(int);
		void sahneyeYaz();
		void parcaEkle();
		void hareketEttir();
		void yemiYe();
		void gameOver();
		void kontrol();
};

YILAN snake;

void parcaOlustur(int x, int y , YONLER yon , int sira){
	snake.yilan[sira].x = x;
	snake.yilan[sira].y = y;
	snake.yilan[sira].kontrol = 1;
	snake.yilan[sira].yon = yon;
}

SAHNE alan;


void YILAN::sifirla(){ //yilandaki kontrolleri 0 YAZAR
	for(int i = 0; i<MAX_YILAN_BOYU ; i++){
		snake.yilan[i].kontrol = 0;	
	}
}
void YILAN::olustur(int size){ //yilanin parcalarinin bilgilerini OLUÞTURUR
	for(int i = 0; i<size ; i++){
		snake.yilan[i].x = size-i;
		snake.yilan[i].y = 2;
		snake.yilan[i].kontrol = 1;
		snake.yilan[i].yon = sag;
	}
	snake.yilan_boyu = size;
}
void YILAN::sahneyeYaz(){ //dolu elemanlarýn x ve ylerini sahneye YAZAR
	int i = 0;
	while(snake.yilan[i].kontrol){
		alan.sahne[snake.yilan[i].x][snake.yilan[i].y] = ch;	
		i++;
	}
}
void YILAN::parcaEkle(){
	if(snake.yilan[snake.yilan_boyu-1].yon == sag){
		parcaOlustur(snake.yilan[snake.yilan_boyu-1].x-1 , snake.yilan[snake.yilan_boyu-1].y , snake.yilan[snake.yilan_boyu-1].yon, snake.yilan_boyu);
		snake.yilan_boyu++;
	}
	else if(snake.yilan[snake.yilan_boyu-1].yon == sol){
		parcaOlustur(snake.yilan[snake.yilan_boyu-1].x+1 , snake.yilan[snake.yilan_boyu-1].y , snake.yilan[snake.yilan_boyu-1].yon, snake.yilan_boyu);
		snake.yilan_boyu++;
	}else if(yilan[snake.yilan_boyu-1].yon == asagi){
		parcaOlustur(snake.yilan[snake.yilan_boyu-1].x , snake.yilan[snake.yilan_boyu-1].y - 1 , snake.yilan[snake.yilan_boyu-1].yon,  snake.yilan_boyu);
		snake.yilan_boyu++;
	}else if(snake.yilan[yilan_boyu-1].yon == yukari){
		parcaOlustur(snake.yilan[snake.yilan_boyu-1].x , snake.yilan[snake.yilan_boyu-1].y + 1 , snake.yilan[snake.yilan_boyu-1].yon , snake.yilan_boyu);
		snake.yilan_boyu++;
	}
}
void YILAN::hareketEttir(){
	int i=yilan_boyu-1;
	while(i>0){
		yilan[i].yon = yilan[i-1].yon;
		i--;
	}
	YONLER y = yeniYon();
	if(y != bos){
		yilan[0].yon = yeniYon();		
	}

	
	int s = 0;
	while(yilan[s].kontrol){
		parcaHareketEttir(&yilan[s]);
		s++;		
	}	
}
void YILAN::yemiYe(){
	if(yilan[0].x == yem.x && yilan[0].y == yem.y){
		alan.sahne[yem.x][yem.y] = ' ';
		snake.parcaEkle();
		yem.yeniYemOlustur();
	}
}
void YILAN::kontrol(){
	for(int i = 1; i < snake.yilan_boyu; i++){
		if(snake.yilan[0].x == snake.yilan[i].x  && snake.yilan[0].y == snake.yilan[i].y ){
			kendiniYediMi = 1;
			break;
		}
	}
}
void YILAN::gameOver(){
	alan.temizle();
	char gameOv[10] = "GAME OVER";
	
	char sonuclar[30] = "yilanin son uzunlugu: ";
	sonuclar[22] = snake.yilan_boyu/10 + '0';
	sonuclar[23] = snake.yilan_boyu%10 + '0';
	 
	for(int i = 0 ; i < strlen(gameOv) ; i++){
		alan.sahne[35+i][10] = gameOv[i];
	}	
	for(int i = 0; i<strlen(sonuclar) ; i++){
		alan.sahne[35+i][12] = sonuclar[i];
	}
	alan.ciz();
	
}


void SAHNE::cursorGizle(){
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorI;
	GetConsoleCursorInfo(out , &cursorI);
	cursorI.bVisible = false;
	SetConsoleCursorInfo(out , &cursorI);
}
void SAHNE::sinirYaz(){ //sahneye sýnýrlarý YAZAR
	for(int i=0;i<alan.genislik ; i++){
		alan.sahne[i][0] = 219;
		alan.sahne[i][alan.yukseklik-1] = 219; 
	}	
	
	for(int i = 0;i<alan.yukseklik ; i++){
		alan.sahne[0][i] = 219 ;
		alan.sahne[alan.genislik-1][i] = 219;
	}
}
void SAHNE::git(int x,int y){ //istenen yere gider
	COORD coord ;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(ACIK_EKRAN) , coord);
}
void SAHNE::ciz(){ //sahneyi ÇÝZER
	git(0,0);
	for(int y = 0; y < alan.yukseklik ; y++){
		for(int x=0 ; x < alan.genislik ; x++){
			cout << alan.sahne[x][y];
		}
		cout<<endl;
	}
}
void SAHNE::temizle(){ //sahnede sinirlara 219 YAZAR
	for(int i = 0 ; i <alan.yukseklik ; i++){
		for(int s = 0 ; s<alan.genislik ; s++){
			alan.sahne[s][i] = ' ';	
		}
	}
}


void YEM::yeniYemOlustur(){
	
	srand(time(0));
	int x = rand()%(alan.genislik-2) + 1;
	int y = rand()%(alan.yukseklik-2) + 1;
	yem.x = x;
	yem.y = y;
}
void YEM::yemiSahneyeYaz(){
	
	alan.sahne[yem.x][yem.y] = yem.sekil;
}




int main(){
	snake.kendiniYediMi = 0;
	alan.genislik = GENISLIK;
	alan.yukseklik = YUKSEKLIK;
	snake.sifirla();
	snake.olustur(5);
	
	yem.yeniYemOlustur();
	alan.cursorGizle();
	
	while(true){
		Sleep(30);
		alan.temizle();
		/*
		
		alan.sahne[1][10] = yem.x/10 + '0';
		alan.sahne[2][10] = yem.x%10 + '0';
		alan.sahne[1][11] = yem.y/10 + '0';
		alan.sahne[2][11] = yem.y%10 + '0';
		*/
		alan.sinirYaz();
		yem.yemiSahneyeYaz();		
	
		snake.hareketEttir();
	
		snake.sahneyeYaz();
		snake.yemiYe();
			
		alan.ciz();
		snake.kontrol();
		if(snake.kendiniYediMi){
			snake.gameOver();
			break;
		}
			
	}
	
	

		Sleep(1000);
	
	
}
