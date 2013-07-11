#include <im2.h>
#include <input.h>
#include <spectrum.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_X 40
#define SIZE_Y 24
#define ROZMIAR (SIZE_X * SIZE_Y)

#define ROZMIAR2 (ROZMIAR + ROZMIAR)

#define KROK_JABLKA 8
#define KROK_CZACHY 4

long heap;

//unsigned char poleco[ROZMIAR];
//unsigned char listax[ROZMIAR];
//unsigned char listay[ROZMIAR];

unsigned char *poleco;
unsigned char *listax;
unsigned char *listay;

void *joyfunc;

unsigned char last_x, last_y, prev_x, prev_y, cur_x, cur_y, nowy_kierunek;
unsigned char koniec, przerwa, blad, zjadl;
unsigned short int liczba_czach, liczba_jablek, zjedzone, rekord;

//unsigned short ile_wolnych;
signed short pocz, kon;
unsigned short liczba_wisienek;
unsigned char stary_kierunek;
unsigned char zjadl_jablko;

unsigned char licznik = 0;
unsigned char w_przerwaniu = 0;

unsigned char *gdzie;
int n;

uchar in_KeyDebounce = 1;       // no debouncing
uchar in_KeyStartRepeat = 20;   // wait 20/50s before key starts repeating
uchar in_KeyRepeatPeriod = 5;  // repeat every 10/50s
uint in_KbdState;               // reserved

#define START 68

#define TLO 0
#define SCIANA 1
#define CZACHA 2
#define JABLKO 3
#define WISIENKA 4
#define GLOWA_LEWO 5
#define GLOWA_DOL 6
#define GLOWA_GORA 7
#define GLOWA_PRAWO 8
#define POZIOM 9
#define PION 10
#define LEWO_GORA 11
#define LEWO_DOL 12
#define PRAWO_GORA 13
#define PRAWO_DOL 14

#define tlo ' '
#define sciana 'S'
#define czacha 'O'
#define jablko 'J'
#define wisienka 'W'
#define glowa_lewo 'L'
#define glowa_dol 'N'
#define glowa_gora 'G'
#define glowa_prawo 'P'
#define poziom 'H'
#define pion 'V'
#define lewo_gora 'Y'
#define lewo_dol 'X'
#define prawo_gora 'u'
#define prawo_dol 'Z'

#define tlo1 ' '
#define sciana1 's'
#define czacha1 'o'
#define jablko1 'j'
#define wisienka1 'w'
#define glowa_lewo1 'l'
#define glowa_dol1 'n'
#define glowa_gora1 'g'
#define glowa_prawo1 'p'
#define poziom1 'h'
#define pion1 'v'
#define lewo_gora1 'y'
#define lewo_dol1 'x'
#define prawo_gora1 'u'
#define prawo_dol1 'z'

char *napisy[] = {
	"  ",
	"Ss",
	"Oo",
	"Jj",
	"Ww",
	"Ll",
	"Nn",
	"Gg",
	"Pp",
	"Hh",
	"Vv",
	"Yy", //"Uu", "11"
	"Xx", //"Xx", "22"
	"Uu", //"Yy", "33"
	"Zz", //"Zz", "44"
};

#define LEWO 0
#define DOL 1
#define GORA 2
#define PRAWO 3

short int x_krok[] = { -1, 0, 0, 1 };
short int y_krok[] = { 0, 1, -1, 0 };

int
numer(unsigned char y, unsigned char x)
{
	return y * SIZE_X + x;
}

void
push_front(unsigned char x, unsigned char y)
{
	pocz--;
	if (pocz < 0) pocz = ROZMIAR - 1;
	listax[pocz] = x;
	listay[pocz] = y;
}

void
pop_back(void)
{ /* zwalnia miejsce zajmowane przez ogon */
	int n;
	unsigned char x, y;

	kon--;
	if (kon < 0) kon = ROZMIAR - 1;
	x = listax[kon];
	y = listay[kon];
	n = numer(y, x);
	poleco[n] = TLO;
	last_x = x;
	last_y = y;
}

void
losuj(unsigned char co)
{
	unsigned short indeks;
	unsigned short indeks2;
	unsigned char x, y;

	do {
		indeks = rand() % ROZMIAR;
	} while (poleco[indeks] != TLO);

	x = indeks % SIZE_X;
	y = indeks / SIZE_X;

	poleco[indeks] = co;

	//ile_wolnych--;
	last_x = x;
	last_y = y;
}

void
losuj_jablka(void)
{
	int i;

	for (i = 0; i < liczba_wisienek; i++) losuj(JABLKO);
}

void
losuj_czachy(void)
{
	int i;

	for (i = 0; i < liczba_czach; i++) losuj(CZACHA);
}

void
losuj_weza(void)
{
	unsigned char co;

	stary_kierunek = nowy_kierunek = rand() % 4;
	co = GLOWA_LEWO + nowy_kierunek;
	losuj(co);
	push_front(last_x, last_y);
}

void
start(unsigned short l_jablek, unsigned short l_czach)
{
	unsigned char i, x, y;

	zjedzone = 0;
	for (i = KROK_JABLKA; i < l_jablek; i += KROK_JABLKA) {
		zjedzone += i * 2;
	}

	memset(poleco, TLO, ROZMIAR);

	for (x = 0; x < SIZE_X; x++) {
		poleco[x] = SCIANA;
		poleco[(SIZE_Y - 1) * SIZE_X + x] = SCIANA;
	}

	for (y = 1; y < SIZE_Y; y++) {
		poleco[y * SIZE_X] = SCIANA;
		poleco[y * SIZE_X + SIZE_X - 1] = SCIANA;
	}

//printf("33");
	liczba_wisienek = liczba_jablek = l_jablek;
	liczba_czach = l_czach;
	//ile_wolnych = i;
	losuj_jablka();
	losuj_czachy();
	pocz = 0;
	kon = 0;
	losuj_weza();
	koniec = blad = zjadl_jablko = 0;
	przerwa = 1;
	zjadl = 1;
}

void
start1(void)
{
	start(liczba_jablek, liczba_czach);
}

void
snake(void)
{
	rekord = 0;
	start(8, 4);
}

void
ruch(void)
{
	unsigned char x = listax[pocz];
	unsigned char y = listay[pocz];
	int n = numer(y, x);
	int gdzie;

	if (zjadl_jablko) {
		if (stary_kierunek == nowy_kierunek) {
			if (stary_kierunek == LEWO || stary_kierunek == PRAWO) poleco[n] = POZIOM;
			else poleco[n] = PION;
		} else {
			if (stary_kierunek == LEWO && nowy_kierunek == GORA) poleco[n] = LEWO_GORA;
			else if (stary_kierunek == LEWO && (nowy_kierunek == DOL || nowy_kierunek == PRAWO)) poleco[n] = LEWO_DOL;
			else if (stary_kierunek == PRAWO && (nowy_kierunek == GORA || nowy_kierunek == LEWO)) poleco[n] = PRAWO_GORA;
			else if (stary_kierunek == PRAWO && nowy_kierunek == DOL) poleco[n] = PRAWO_DOL;
			else if (stary_kierunek == GORA && (nowy_kierunek == LEWO || nowy_kierunek == DOL)) poleco[n] = PRAWO_DOL;
			else if (stary_kierunek == GORA && nowy_kierunek == PRAWO) poleco[n] = LEWO_DOL;
			else if (stary_kierunek == DOL && nowy_kierunek == LEWO) poleco[n] = PRAWO_GORA;
			else if (stary_kierunek == DOL && (nowy_kierunek == PRAWO || nowy_kierunek == GORA)) poleco[n] = LEWO_GORA;
		}
	}

	prev_x = x;
	prev_y = y;
	x += x_krok[nowy_kierunek];
	y += y_krok[nowy_kierunek];
	n = numer(y, x);
	switch (poleco[n]) {
	case TLO:
		pop_back();
		break;
	case JABLKO:
		zjedzone++;
		zjadl = zjadl_jablko = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		losuj(WISIENKA);
		break;
	case WISIENKA:
		liczba_wisienek--;
		zjedzone++;
		zjadl = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		if (liczba_wisienek == 0) {
			koniec = 1;
			przerwa = 1;
		} else {
			losuj(CZACHA);
		}
		break;
	default:
		koniec = 1;
		przerwa = 1;
		blad = 1;
		pop_back();
		break;
	}

	gdzie = GLOWA_LEWO + nowy_kierunek;
	poleco[n] = gdzie;
	cur_x = x;
	cur_y = y;
	push_front(x, y);
	stary_kierunek = nowy_kierunek;
}

void
move_cursor(unsigned char y, unsigned char x)
{
	printf("\033[%u;%uH", y + 1, x + 1);
}

void
text(void)
{
	move_cursor(0, 4);
	if (zjedzone <= 0xf) puts_cons("000");
	else if (zjedzone <= 0xff) puts_cons("00");
	else if (zjedzone <= 0xfff) puts_cons("0");
	printf("%x", zjedzone);
	move_cursor(0, 10);
	if (rekord <= 0xf) puts_cons("000");
	else if (rekord <= 0xff) puts_cons("00");
	else if (rekord <= 0xfff) puts_cons("0");
	printf("%x", rekord);
	zjadl = 0;
}

void
wyswietl(void)
{
	int i,j,k;

	k = 0;
	for (i = 0; i < SIZE_Y; i++) {
		move_cursor(i, 0);
		for (j = 0; j < SIZE_X; j++, k++) puts_cons(napisy[poleco[k]]);
	}
	text();
}

void
klawiatura(void)
{
	unsigned char k = in_Inkey();
	unsigned int joy = joyfunc();

	if (!k && !joy) return;

	if (joy & in_FIRE) k = ' ';
	else if (joy & in_LEFT) k = '5';
	else if (joy & in_DOWN) k = '6';
	else if (joy & in_UP) k = '7';
	else if (joy & in_RIGHT) k = '8';

	switch (k) {
	case 27:
		/*wypad = 1;*/
		break;
	case ' ':
		if (koniec) {
			if (blad) {
				start1();
				wyswietl();
			} else {
				start(liczba_jablek + KROK_JABLKA, liczba_czach + KROK_CZACHY);
				wyswietl();
			}
		} else if (!przerwa) przerwa = 1;
		break;
	case '5':
	case '6':
	case '7':
	case '8':
		nowy_kierunek = k - '5';
		if (!koniec) przerwa = 0;
		break;
	default:
		break;
	}
}

M_BEGIN_ISR(narysuj)
	if (w_przerwaniu) goto end;

	if (przerwa) goto end2;

	licznik++;
	if (licznik < 3) goto end2;

	w_przerwaniu = 1;
	licznik = 0;

	ruch();

	move_cursor(last_y, last_x + last_x);
	n = numer(last_y, last_x);
	puts_cons(napisy[poleco[n]]);

	move_cursor(prev_y, prev_x + prev_x);

	n = numer(prev_y, prev_x);

	puts_cons(napisy[poleco[n]]);

#if 0
	if (blad) {
		dc->setForeground(FXRGB(255, 0, 0));
	} else {
		dc->setForeground(FXRGB(0, 255, 0));
	}
#endif
	move_cursor(cur_y, cur_x + cur_x);
	n = numer(cur_y, cur_x);
	puts_cons(napisy[poleco[n]]);

	if (zjadl) text();
end2:
	klawiatura();

	w_przerwaniu = 0;
end:
M_END_ISR

int
main(int argc, char *argv[])
{
	mallinit();
	sbrk(44000, 4000);
	poleco = malloc(ROZMIAR);
	listax = malloc(ROZMIAR);
	listay = malloc(ROZMIAR);

	joyfunc = (zx_type() == 2) ? in_JoyTimex1 : in_JoyKempston;
	snake();
	wyswietl();
#asm
	di
#endasm

	im2_Init(0xfd00);
	memset(0xfd00, 0xfe, 257);
	bpoke(0xfefe, 195);
	wpoke(0xfeff, narysuj);
#asm
	ei
#endasm

	while (1) {
#asm
	halt
#endasm
	}
	return 0;
}
