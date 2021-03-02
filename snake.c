#include <input.h>
#include <spectrum.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <sys/ioctl.h>

#define SIZE_X 32
#define SIZE_Y 24
#define ROZMIAR (SIZE_X * SIZE_Y)

#define ROZMIAR2 (ROZMIAR + ROZMIAR)

#define KROK_JABLKA 8
#define KROK_CZACHY 4

unsigned char *joy = (unsigned char *)23297;
unsigned char *joy2 = (unsigned char *)23298;
unsigned short *powolnosc = (unsigned short *)23299;

long heap;

char *poleco;
char *listax;
char *listay;

char *listax2;
char *listay2;

unsigned int (*joyfunc)(void);
unsigned int (*joyfunc2)(void);

char last_x, last_y, prev_x, prev_y, cur_x, cur_y, nowy_kierunek;
char last_x_2, last_y_2, prev_x_2, prev_y_2, cur_x_2, cur_y_2, nowy_kierunek_2;

char koniec, przerwa, blad, zjadl;
char przerwa2, zjadl2;


unsigned short int liczba_czach, liczba_jablek, zjedzone, rekord;

//unsigned short ile_wolnych;
signed short pocz, kon;
signed short pocz2, kon2;
unsigned short liczba_wisienek;
char stary_kierunek;
char zjadl_1;
char stary_kierunek_2;
char zjadl_2;


unsigned short licznik = 0;
unsigned char w_przerwaniu = 0;

unsigned char *gdzie;
int n;
int n2;

struct in_UDK keyb;

unsigned int spacja;
unsigned int caps_shift;

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
#define sciana ('S' - ' ' + 128)
#define czacha ('O' - ' ' + 128)
#define jablko ('J' - ' ' + 128)
#define wisienka ('W' - ' ' + 128)
#define glowa_lewo ('L' - ' ' + 128)
#define glowa_dol ('P' - ' ' + 128)
#define glowa_gora ('N' - ' ' + 128)
#define glowa_prawo ('G' - ' ' + 128)
#define poziom ('H' - ' ' + 128)
#define pion ('V' - ' ' + 128)
#define lewo_gora ('U' - ' ' + 128)
#define lewo_dol ('Z' - ' ' + 128)
#define prawo_gora ('Y' - ' ' + 128)
#define prawo_dol ('X' - ' ' + 128)

#define tlo1 ' '
#define sciana1 ('s' - ' ' + 128)
#define czacha1 ('o' - ' ' + 128)
#define jablko1 ('j' - ' ' + 128)
#define wisienka1 ('w' - ' ' + 128)
#define glowa_lewo1 ('l' - ' ' + 128)
#define glowa_dol1 ('p' - ' ' + 128)
#define glowa_gora1 ('n' - ' ' + 128)
#define glowa_prawo1 ('g' - ' ' + 128)
#define poziom1 ('h' - ' ' + 128)
#define pion1 ('v' - ' ' + 128)
#define lewo_gora1 ('u' - ' ' + 128)
#define lewo_dol1 ('z' - ' ' + 128)
#define prawo_gora1 ('y' - ' ' + 128)
#define prawo_dol1 ('x' - ' ' + 128)

char napisy[][2] = {
	{' ',' '}, /* TLO */
	{sciana, sciana1}, /* SCIANA */
	{czacha, czacha1}, /* CZACHA */
	{jablko, jablko1}, /* JABLKO */
	{wisienka, wisienka1}, /* WISIENKI */
	{glowa_lewo, glowa_lewo1}, /* GLOWA LEWO */
	{glowa_dol, glowa_dol1}, /* GLOWA DOL */
	{glowa_gora, glowa_gora1}, /* GLOWA GORA */
	{glowa_prawo, glowa_prawo1}, /* GLOWA PRAWO */
	{poziom, poziom1}, /* POZIOM */
	{pion, pion1}, /* PION */
	{lewo_gora, lewo_gora1}, /* LEWO GORA */
	{lewo_dol, lewo_dol1}, /* LEWO DOL */
	{prawo_gora, prawo_gora1}, /* PRAWO GORA */
	{prawo_dol, prawo_dol1}, /* PRAWO DOL */
};

#define LEWO 0
#define DOL 1
#define GORA 2
#define PRAWO 3

short int x_krok[] = { -1, 0, 0, 1 };
short int y_krok[] = { 0, 1, -1, 0 };

unsigned char udgs[] = {
0, 0, 0, 0, 0, 0, 0, 0, 
0, 24, 60, 24, 24, 0, 24, 0, 
0, 54, 54, 20, 0, 0, 0, 0, 
0, 108, 254, 108, 254, 108, 0, 0, 
0, 48, 124, 192, 120, 12, 248, 48, 
0, 198, 204, 24, 48, 102, 198, 0, 
0, 0, 56, 108, 58, 126, 204, 118, 
12, 12, 24, 0, 0, 0, 0, 0, 
0, 12, 24, 48, 48, 48, 24, 12, 
0, 48, 24, 12, 12, 12, 24, 48, 
0, 0, 108, 56, 254, 56, 108, 0, 
0, 0, 24, 24, 126, 24, 24, 0, 
0, 0, 0, 0, 0, 12, 12, 24, 
0, 0, 0, 0, 254, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 24, 24, 
0, 6, 12, 24, 48, 96, 192, 0, 
0, 124, 198, 198, 214, 198, 198, 124, 
0, 24, 120, 24, 24, 24, 24, 126, 
0, 124, 198, 140, 56, 96, 194, 254, 
0, 124, 198, 6, 60, 6, 198, 124, 
0, 28, 60, 108, 204, 254, 12, 30, 
0, 254, 192, 252, 6, 6, 198, 124, 
0, 124, 198, 192, 252, 198, 198, 124, 
0, 254, 198, 6, 12, 24, 48, 48, 
0, 124, 198, 198, 124, 198, 198, 124, 
0, 124, 198, 198, 126, 6, 198, 124, 
0, 12, 12, 0, 0, 12, 12, 0, 
0, 12, 12, 0, 0, 12, 12, 24, 
0, 24, 48, 96, 192, 96, 48, 24, 
0, 0, 0, 254, 0, 254, 0, 0, 
0, 48, 24, 12, 6, 12, 24, 48, 
0, 124, 198, 204, 24, 24, 0, 24, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 56, 108, 198, 198, 254, 198, 198, 
0, 252, 102, 102, 124, 102, 102, 252, 
0, 60, 102, 194, 192, 194, 102, 60, 
0, 248, 108, 102, 102, 102, 108, 248, 
0, 254, 102, 96, 124, 96, 102, 254, 
0, 254, 102, 96, 124, 96, 96, 240, 
63, 240, 230, 255, 255, 230, 240, 63, 
0, 255, 189, 129, 231, 255, 255, 0, 
0, 60, 24, 24, 24, 24, 24, 60, 
28, 3, 61, 127, 127, 63, 14, 0, 
0, 198, 204, 216, 240, 216, 204, 198, 
7, 31, 62, 243, 242, 62, 31, 7, 
0, 198, 198, 238, 254, 214, 198, 198, 
7, 27, 127, 195, 153, 153, 227, 63, 
31, 127, 227, 126, 17, 8, 7, 1, 
63, 227, 153, 153, 194, 127, 27, 7, 
0, 124, 198, 198, 214, 214, 124, 6, 
0, 252, 102, 102, 124, 120, 102, 230, 
236, 95, 43, 124, 63, 87, 43, 0, 
0, 255, 219, 153, 24, 24, 24, 60, 
63, 60, 18, 15, 7, 0, 0, 0, 
63, 35, 60, 35, 63, 63, 63, 63, 
15, 0, 57, 247, 125, 7, 7, 0, 
0, 240, 255, 252, 253, 254, 255, 63, 
63, 255, 254, 253, 252, 255, 248, 0, 
0, 0, 0, 7, 15, 18, 60, 63, 
0, 124, 96, 96, 96, 96, 96, 124, 
0, 192, 96, 48, 24, 12, 6, 0, 
0, 124, 12, 12, 12, 12, 12, 124, 
0, 56, 108, 198, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 255, 
0, 24, 24, 12, 0, 0, 0, 0, 
0, 0, 0, 120, 12, 124, 204, 118, 
0, 0, 224, 96, 124, 102, 102, 252, 
0, 0, 0, 120, 204, 192, 204, 120, 
0, 0, 28, 12, 124, 204, 204, 126, 
0, 0, 0, 120, 204, 252, 192, 120, 
0, 28, 54, 48, 252, 48, 48, 120, 
224, 248, 124, 207, 207, 124, 248, 224, 
0, 255, 231, 129, 189, 255, 255, 0, 
0, 24, 24, 0, 56, 24, 24, 60, 
48, 192, 62, 249, 249, 254, 248, 0, 
0, 224, 102, 108, 120, 108, 102, 230, 
252, 15, 103, 255, 255, 103, 15, 252, 
0, 0, 0, 236, 254, 214, 214, 198, 
224, 216, 254, 195, 153, 153, 231, 252, 
248, 254, 199, 126, 136, 16, 224, 128, 
252, 231, 153, 153, 195, 254, 216, 224, 
0, 0, 118, 204, 204, 124, 12, 30, 
0, 0, 0, 220, 102, 96, 96, 240, 
219, 171, 119, 171, 255, 123, 151, 3, 
0, 48, 48, 252, 48, 48, 54, 28, 
252, 255, 255, 191, 63, 255, 31, 0, 
252, 252, 252, 252, 196, 28, 196, 252, 
128, 64, 96, 189, 159, 208, 240, 0, 
0, 0, 0, 224, 240, 72, 60, 252, 
252, 60, 72, 240, 224, 0, 0, 0, 
0, 31, 255, 63, 191, 127, 255, 252, 
0, 14, 24, 24, 112, 24, 24, 14, 
24, 24, 24, 0, 24, 24, 24, 24, 
0, 112, 24, 24, 14, 24, 24, 112, 
0, 118, 220, 0, 0, 0, 0, 0, 
0, 16, 56, 56, 108, 108, 254, 0
};


int
numer(unsigned char y, unsigned char x)
{
	return (y << 5) + x;
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
push_front_2(unsigned char x, unsigned char y)
{
	pocz2--;
	if (pocz2 < 0) pocz2 = ROZMIAR - 1;
	listax2[pocz2] = x;
	listay2[pocz2] = y;
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
pop_back_2(void)
{ /* zwalnia miejsce zajmowane przez ogon */
	int n;
	unsigned char x, y;

	kon2--;
	if (kon2 < 0) kon2 = ROZMIAR - 1;
	x = listax2[kon2];
	y = listay2[kon2];
	n = numer(y, x);
	poleco[n] = TLO;
	last_x_2 = x;
	last_y_2 = y;
}


void
losuj(unsigned char co, unsigned char *last_x, unsigned char *last_y)
{
	unsigned short indeks;
	unsigned short indeks2;
	unsigned char x, y;

	do
	{
		indeks = rand() % ROZMIAR;
	}
	while (poleco[indeks] != TLO);

	x = indeks & 31;
	y = indeks >> 5;


	poleco[indeks] = co;

	//ile_wolnych--;
	*last_x = x;
	*last_y = y;
}

void
losuj_jablka(void)
{
	int i;

	for (i = 0; i < liczba_wisienek; i++) losuj(JABLKO, &last_x, &last_y);
}

void
losuj_czachy(void)
{
	int i;

	for (i = 0; i < liczba_czach; i++) losuj(CZACHA, &last_x, &last_y);
}

void
losuj_weza(void)
{
	unsigned char co;

	stary_kierunek = nowy_kierunek = rand() % 4;
	co = GLOWA_LEWO + nowy_kierunek;
	losuj(co, &last_x, &last_y);
	push_front(last_x, last_y);
}

void
losuj_weza_2(void)
{
	unsigned char co;

	stary_kierunek_2 = nowy_kierunek_2 = rand() % 4;
	co = GLOWA_LEWO + nowy_kierunek_2;
	losuj(co, &last_x_2, &last_y_2);
	push_front_2(last_x_2, last_y_2);
}

void
start(unsigned short l_jablek, unsigned short l_czach)
{
	unsigned char i, x, y;

	zjedzone = 0;
	for (i = KROK_JABLKA; i < l_jablek; i += KROK_JABLKA)
	{
		zjedzone += i * 2;
	}

	memset(poleco, TLO, ROZMIAR);

	for (x = 0; x < SIZE_X; x++)
	{
		poleco[x] = SCIANA;
		poleco[(SIZE_Y - 1) * SIZE_X + x] = SCIANA;
	}

	for (y = 1; y < SIZE_Y; y++)
	{
		poleco[y * SIZE_X] = SCIANA;
		poleco[y * SIZE_X + SIZE_X - 1] = SCIANA;
	}

//printf("33");
	liczba_wisienek = liczba_jablek = l_jablek;
	liczba_czach = l_czach;
	//ile_wolnych = i;
	losuj_jablka();
	losuj_czachy();
	pocz = pocz2 = 0;
	kon = kon2 = 0;
	losuj_weza();
	losuj_weza_2();
	koniec = blad = zjadl_1 = zjadl_2 = 0;
	przerwa = przerwa2 = 1;
	zjadl = zjadl2 = 1;
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

	if (zjadl_1)
	{
		if (stary_kierunek == nowy_kierunek)
		{
			if (stary_kierunek == LEWO || stary_kierunek == PRAWO) poleco[n] = POZIOM;
			else poleco[n] = PION;
		}
		else
		{
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
	switch (poleco[n])
	{
	case TLO:
		pop_back();
		break;
	case JABLKO:
		zjedzone++;
		zjadl = zjadl_1 = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		losuj(WISIENKA, &last_x, &last_y);
		break;
	case WISIENKA:
		liczba_wisienek--;
		zjedzone++;
		zjadl = zjadl_1 = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		if (liczba_wisienek == 0)
		{
			koniec = 1;
			przerwa = przerwa2 = 1;
		}
		else
		{
			losuj(CZACHA, &last_x, &last_y);
		}
		break;
	default:
		koniec = 1;
		przerwa = 1;
		przerwa2 = 1;
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
ruch2(void)
{
	unsigned char x = listax2[pocz2];
	unsigned char y = listay2[pocz2];
	int n = numer(y, x);
	int gdzie;

	if (zjadl_2)
	{
		if (stary_kierunek_2 == nowy_kierunek_2)
		{
			if (stary_kierunek_2 == LEWO || stary_kierunek_2 == PRAWO) poleco[n] = POZIOM;
			else poleco[n] = PION;
		}
		else
		{
			if (stary_kierunek_2 == LEWO && nowy_kierunek_2 == GORA) poleco[n] = LEWO_GORA;
			else if (stary_kierunek_2 == LEWO && (nowy_kierunek_2 == DOL || nowy_kierunek_2 == PRAWO)) poleco[n] = LEWO_DOL;
			else if (stary_kierunek_2 == PRAWO && (nowy_kierunek_2 == GORA || nowy_kierunek_2 == LEWO)) poleco[n] = PRAWO_GORA;
			else if (stary_kierunek_2 == PRAWO && nowy_kierunek_2 == DOL) poleco[n] = PRAWO_DOL;
			else if (stary_kierunek_2 == GORA && (nowy_kierunek_2 == LEWO || nowy_kierunek_2 == DOL)) poleco[n] = PRAWO_DOL;
			else if (stary_kierunek_2 == GORA && nowy_kierunek_2 == PRAWO) poleco[n] = LEWO_DOL;
			else if (stary_kierunek_2 == DOL && nowy_kierunek_2 == LEWO) poleco[n] = PRAWO_GORA;
			else if (stary_kierunek_2 == DOL && (nowy_kierunek_2 == PRAWO || nowy_kierunek_2 == GORA)) poleco[n] = LEWO_GORA;
		}
	}

	prev_x_2 = x;
	prev_y_2 = y;
	x += x_krok[nowy_kierunek_2];
	y += y_krok[nowy_kierunek_2];
	n = numer(y, x);
	switch (poleco[n])
	{
	case TLO:
		pop_back_2();
		break;
	case JABLKO:
		zjedzone++;
		zjadl = zjadl_2 = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		losuj(WISIENKA, &last_x_2, &last_y_2);
		break;
	case WISIENKA:
		liczba_wisienek--;
		zjedzone++;
		zjadl = zjadl_2 = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		if (liczba_wisienek == 0)
		{
			koniec = 1;
			przerwa = przerwa2 = 1;
		}
		else
		{
			losuj(CZACHA, &last_x_2, &last_y_2);
		}
		break;
	default:
		koniec = 1;
		przerwa = przerwa2 = 1;
		blad = 1;
		pop_back_2();
		break;
	}

	gdzie = GLOWA_LEWO + nowy_kierunek_2;
	poleco[n] = gdzie;
	cur_x_2 = x;
	cur_y_2 = y;
	push_front_2(x, y);
	stary_kierunek_2 = nowy_kierunek_2;
}

void
move_cursor(unsigned char y, unsigned char x)
{
	putch(22);
	putch(y);
	putch(x);
	//gotoxy(x, y);
	//printf("\033[%u;%uH", y + 1, x + 1);
}

void
prn(char *t)
{
	putch(*t);
	putch(*(++t));
}

void
text(void)
{
	gotoxy(4, 0);
	if (zjedzone <= 0xf) printf("000");
	else if (zjedzone <= 0xff) printf("00");
	else if (zjedzone <= 0xfff) printf("0");
	printf("%x", zjedzone);
	gotoxy(10, 0);
	if (rekord <= 0xf) printf("000");
	else if (rekord <= 0xff) printf("00");
	else if (rekord <= 0xfff) printf("0");
	printf("%x", rekord);
	zjadl = 0;
}

void
wyswietl(void)
{
	char i,j;
	int k;

	k = 0;
	for (i = 0; i < SIZE_Y; i++)
	{
		gotoxy(0, i);
		for (j = 0; j < SIZE_X; j++, k++) prn(napisy[poleco[k]]);
	}
	/* Drugi wąż na początku w negatywie */
	gotoxy(last_x_2 + last_x_2, last_y_2);
	putch(27);
	putch('p');
//	puts_cons("\033[7m");
	prn(napisy[poleco[last_y_2 * SIZE_X + last_x_2]]);
	putch(27);
	putch('q');
//	puts_cons("\033[27m");
	text();
}

unsigned int
joy_3(void)
{
	return in_JoyKeyboard(&keyb);
}

void
joystick11(void)
{
	unsigned int joy = joyfunc();
	unsigned char k;

	if (!joy) return;

	if (joy & in_FIRE) k = ' ';
	else if (joy & in_LEFT) k = '5';
	else if (joy & in_DOWN) k = '6';
	else if (joy & in_UP) k = '7';
	else if (joy & in_RIGHT) k = '8';

	switch (k)
	{
	case ' ':
		if (koniec)
		{
			if (blad)
			{
				start1();
				wyswietl();
			}
			else
			{
				start(liczba_jablek + KROK_JABLKA, liczba_czach + KROK_CZACHY);
				wyswietl();
			}
		}
		else if (!przerwa) przerwa = 1;
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

void
joystick12(void)
{
	unsigned int joy = joyfunc2();
	unsigned char k;

	if (!joy) return;

	if (joy & in_FIRE) k = ' ';
	else if (joy & in_LEFT) k = '5';
	else if (joy & in_DOWN) k = '6';
	else if (joy & in_UP) k = '7';
	else if (joy & in_RIGHT) k = '8';

	switch (k)
	{
	case ' ':
		if (koniec)
		{
			if (blad)
			{
				start1();
				wyswietl();
			}
			else
			{
				start(liczba_jablek + KROK_JABLKA, liczba_czach + KROK_CZACHY);
				wyswietl();
			}
		}
		else if (!przerwa2) przerwa2 = 1;
		break;
	case '5':
	case '6':
	case '7':
	case '8':
		nowy_kierunek_2 = k - '5';
		if (!koniec) przerwa2 = 0;
		break;
	default:
		break;
	}
}


void
narysuj(void)
{
	while (1)
	{
		for (licznik = 0; licznik < *powolnosc; licznik++);
		if (in_KeyPressed(spacja) && in_KeyPressed(caps_shift)) break;

		joystick11();
		if (!przerwa)
		{
			ruch();

			gotoxy(last_x + last_x, last_y);
			n = numer(last_y, last_x);
			prn(napisy[poleco[n]]);

			gotoxy(prev_x + prev_x, prev_y);
			n = numer(prev_y, prev_x);
			prn(napisy[poleco[n]]);

			gotoxy(cur_x + cur_x, cur_y);
			n = numer(cur_y, cur_x);

			if (blad) {
				putch(27);
				putch('p');
				; ///printf("\033[7m");
			}
			prn(napisy[poleco[n]]);
			if (blad) {
				//printf("\033[27m");
				putch(27);
				putch('q');
			}

			if (zjadl) text();
		}

		for (licznik = 0; licznik < *powolnosc; licznik++);
		if (in_KeyPressed(spacja) && in_KeyPressed(caps_shift)) break;

		joystick12();
		if (!przerwa2)
		{
			ruch2();

			gotoxy(last_x_2 + last_x_2, last_y_2);
			n2 = numer(last_y_2, last_x_2);
			prn(napisy[poleco[n2]]);

			gotoxy(prev_x_2 + prev_x_2, prev_y_2);
			n2 = numer(prev_y_2, prev_x_2);
			prn(napisy[poleco[n2]]);

			gotoxy(cur_x_2 + cur_x_2, cur_y_2);
			n2 = numer(cur_y_2, cur_x_2);
			if (blad) {
				putch(27);
				putch('p');
				//printf("\033[7m");
			}
			prn(napisy[poleco[n2]]);
			if (blad) {
				putch(27);
				putch('q');
				//printf("\033[27m");
			}

			if (zjadl2) text();
		}
	}
}

int
main(int argc, char *argv[])
{
#asm
	di
#endasm

	int mode;

	mallinit();
	sbrk(46000, 6000);

	mode = 62;

	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_UDGS, &param);
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	putch(1);
	putch(32);
//	putch(4);
///	putch(1);
//	int i = 0;
//	for (i = 0; i < 50; i++) {
//		printf("AB");
//	}

	poleco = malloc(ROZMIAR);
	listax = malloc(ROZMIAR);
	listay = malloc(ROZMIAR);
	listax2 = malloc(ROZMIAR);
	listay2 = malloc(ROZMIAR);

	// initialize the struct_in_UDK with keys for use with the keyboard joystick
	keyb.fire  = in_LookupKey('m');
	keyb.left  = in_LookupKey('o');
	keyb.right = in_LookupKey('p');
	keyb.up    = in_LookupKey('q');
	keyb.down  = in_LookupKey('a');

	spacja = in_LookupKey(' ');
	caps_shift = in_LookupKey(255);

	switch(*joy)
	{
	case 1:
		joyfunc = in_JoyTimex1;
		break;
	case 2:
		joyfunc = in_JoyTimex2;
		break;
	case 3:
		joyfunc = in_JoyKempston;
		break;
	case 4:
		joyfunc = in_JoySinclair1;
		break;
	case 5:
		joyfunc = in_JoySinclair2;
		break;
	default:
	case 6:
		joyfunc = joy_3;
		break;
	}

	switch(*joy2)
	{
	case 1:
		joyfunc2 = in_JoyTimex1;
		break;
	case 2:
		joyfunc2 = in_JoyTimex2;
		break;
	case 3:
		joyfunc2 = in_JoyKempston;
		break;
	case 4:
		joyfunc2 = in_JoySinclair1;
		break;
	case 5:
		joyfunc2 = in_JoySinclair2;
		break;
	default:
	case 6:
		joyfunc2 = joy_3;
		break;
	}

	srand(wpeek(23672));

	snake();
	wyswietl();
	narysuj();

	mode = 0;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
#asm
	ei
#endasm
	return 0;
}
