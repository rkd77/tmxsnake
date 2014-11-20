#include <input.h>
#include <spectrum.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_X 32
#define SIZE_Y 24
#define ROZMIAR (SIZE_X * SIZE_Y)

#define ROZMIAR2 (ROZMIAR + ROZMIAR)

#define KROK_JABLKA 8
#define KROK_CZACHY 4

unsigned char *joy = (unsigned char *)23297;
unsigned char *joy2 = (unsigned char *)23298;
unsigned short *powolnosc = (unsigned short *)23299;

unsigned char zmiana;

long heap;

unsigned char *poleco;
unsigned char *listax;
unsigned char *listay;

unsigned char *listax2;
unsigned char *listay2;

void *joyfunc;
void *joyfunc2;

unsigned char last_x, last_y, prev_x, prev_y, cur_x, cur_y, nowy_kierunek;
unsigned char last_x_2, last_y_2, prev_x_2, prev_y_2, cur_x_2, cur_y_2, nowy_kierunek_2;

unsigned char koniec, przerwa, blad, zjadl;
unsigned char przerwa2, zjadl2;


unsigned short int liczba_czach, liczba_jablek, zjedzone, rekord;

//unsigned short ile_wolnych;
signed short pocz, kon;
signed short pocz2, kon2;
unsigned short liczba_wisienek;
unsigned char stary_kierunek;
unsigned char zjadl_jablko;
unsigned char stary_kierunek_2;
unsigned char zjadl_jablko_2;


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
#define lewo_gora 'U'
#define lewo_dol 'X'
#define prawo_gora 'Y'
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
#define lewo_gora1 'u'
#define lewo_dol1 'x'
#define prawo_gora1 'y'
#define prawo_dol1 'z'

char *napisy[] = {
	"  ", /* TLO */
	"Ss", /* SCIANA */
	"Oo", /* CZACHA */
	"Jj", /* JABLKO */
	"Ww", /* WISIENKI */
	"Ll", /* GLOWA LEWO */
	"Pp", /* GLOWA DOL */
	"Nn", /* GLOWA GORA */
	"Gg", /* GLOWA PRAWO */
	"Hh", /* POZIOM */
	"Vv", /* PION */
	"Uu", /* LEWO GORA */
	"Zz", /* LEWO DOL */
	"Yy", /* PRAWO GORA */
	"Xx", /* PRAWO DOL */
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
	koniec = blad = zjadl_jablko = zjadl_jablko_2 = 0;
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

	if (zjadl_jablko)
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
		zjadl = zjadl_jablko = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		losuj(WISIENKA, &last_x, &last_y);
		break;
	case WISIENKA:
		liczba_wisienek--;
		zjedzone++;
		zjadl = 1;
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

	if (zjadl_jablko_2)
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
		zjadl = zjadl_jablko_2 = 1;
		if (zjedzone > rekord) rekord = zjedzone;
		losuj(WISIENKA, &last_x_2, &last_y_2);
		break;
	case WISIENKA:
		liczba_wisienek--;
		zjedzone++;
		zjadl = 1;
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
	for (i = 0; i < SIZE_Y; i++)
	{
		move_cursor(i, 0);
		for (j = 0; j < SIZE_X; j++, k++) puts_cons(napisy[poleco[k]]);
	}
	text();
}

void
joystick(void)
{
	unsigned int joy = joyfunc(&keyb);
	unsigned char k;

	if (!joy) return;

	if (joy & in_FIRE) k = ' ';
	else if (joy & in_LEFT) k = '5';
	else if (joy & in_DOWN) k = '6';
	else if (joy & in_UP) k = '7';
	else if (joy & in_RIGHT) k = '8';

	switch (k)
	{
	case 27:
		/*wypad = 1;*/
		break;
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
joystick2(void)
{
	unsigned int joy = joyfunc2(&keyb);
	unsigned char k;

	if (!joy) return;

	if (joy & in_FIRE) k = ' ';
	else if (joy & in_LEFT) k = '5';
	else if (joy & in_DOWN) k = '6';
	else if (joy & in_UP) k = '7';
	else if (joy & in_RIGHT) k = '8';

	switch (k)
	{
	case 27:
		/*wypad = 1;*/
		break;
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


		if (zmiana)
		{
			if (przerwa) goto joy_pierwszy;

			ruch();

			move_cursor(last_y, last_x + last_x);
			n = numer(last_y, last_x);
			puts_cons(napisy[poleco[n]]);

			move_cursor(prev_y, prev_x + prev_x);

			n = numer(prev_y, prev_x);

			puts_cons(napisy[poleco[n]]);

			move_cursor(cur_y, cur_x + cur_x);
			n = numer(cur_y, cur_x);
			puts_cons(napisy[poleco[n]]);

			if (zjadl) text();
joy_pierwszy:
			joystick();
			zmiana = 0;
		}
		else
		{
			if (przerwa2) goto joy_drugi;

			ruch2();

			move_cursor(last_y_2, last_x_2 + last_x_2);
			n2 = numer(last_y_2, last_x_2);
			puts_cons(napisy[poleco[n2]]);

			move_cursor(prev_y_2, prev_x_2 + prev_x_2);

			n2 = numer(prev_y_2, prev_x_2);

			puts_cons(napisy[poleco[n2]]);

			move_cursor(cur_y_2, cur_x_2 + cur_x_2);
			n2 = numer(cur_y_2, cur_x_2);
			puts_cons(napisy[poleco[n2]]);

			if (zjadl2) text();
joy_drugi:
			joystick2();
			zmiana = 1;
		}
	}
}

int
main(int argc, char *argv[])
{
#asm
	di
#endasm
	mallinit();
	sbrk(46000, 6000);
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
		joyfunc = in_JoyKeyboard;
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
		joyfunc2 = in_JoyKeyboard;
		break;
	}

	srand(wpeek(23672));

	snake();
	wyswietl();

	narysuj();

#asm
	xor a
	out (255),a
	ei
#endasm
	return 0;
}
