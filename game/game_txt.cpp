#include "game_txt.h"

Pionek::Pionek(Pozycja _p) : pozycja_pionka(_p) {}

Pozycja Pionek::pozycja() {
  return pozycja_pionka;
}

void Pionek::przesun(int krok, int rozmiar) {
  pozycja_pionka.x += krok;
  if (krok >= 0 && pozycja_pionka.x > rozmiar + 1)
    pozycja_pionka.x = rozmiar + 1;
  else if (krok < 0 && pozycja_pionka.x < 0)
    pozycja_pionka.x = 0;
}

int Pionek::zycie() {
  return pozostale_zycie;
}

void Pionek::zmniejsz_zycie(int ile) {
  pozostale_zycie -= ile;
  if (pozostale_zycie < 0)
    pozostale_zycie = 0;
}

Rycerz::Rycerz(Pozycja _p) : Pionek(_p) {
  pozostale_zycie = dlugosc_zycia;
}

int Rycerz::czas() { return czas_tworzenia; }
int Rycerz::krok() { return dlugosc_kroku; }
int Rycerz::sila() { return sila_ataku; }
int Rycerz::obrona() { return skutecznosc_obrony; }
char Rycerz::repr() { return 'R'; }
int Rycerz::max_zycie() { return dlugosc_zycia; }

Smok::Smok(Pozycja _p) : Pionek(_p) {
  pozostale_zycie = dlugosc_zycia;
}

int Smok::czas() { return czas_tworzenia; }
int Smok::krok() { return dlugosc_kroku; }
int Smok::sila() { return sila_ataku; }
int Smok::obrona() { return skutecznosc_obrony; }
char Smok::repr() { return 'S'; }
int Smok::max_zycie() { return dlugosc_zycia; }

bool Plansza::porownaj_pionki(Pionek *p1, Pionek *p2) {
  return p1->repr() == p2->repr() && p1->zycie() == p2->zycie();
}

Plansza::Plansza(int _r) : rozmiar(_r) {
  plansza.resize(rozmiar);
  for (int i = 0; i < rozmiar; i++)
    plansza[i].resize(rozmiar + 2);
}

void Plansza::dodaj_na_plansze(Pionek *pionek) {
  Pozycja pozycja = pionek->pozycja();
  plansza[pozycja.y][pozycja.x].push_back(pionek);
}

void Plansza::dodaj_do_pionkow(Pionek *pionek) {
  pionki.push_back(pionek);
}

list<Pionek*>::iterator Plansza::szukaj_na_planszy(Pionek *pionek) {
  list<Pionek*>::iterator it;
  Pozycja pozycja = pionek->pozycja();
  for (it = plansza[pozycja.y][pozycja.x].begin(); it != plansza[pozycja.y][pozycja.x].end(); it++) {
    if (porownaj_pionki(*it, pionek))
      return it;
  }
  return it;
}

list<Pionek*>::iterator Plansza::szukaj_wsrod_pionkow(Pionek *pionek) {
  list<Pionek*>::iterator it;
  for (it = pionki.begin(); it != pionki.end(); it++) {
    if (porownaj_pionki(*it, pionek))
      return it;
  }
  return it;
}

list<Pionek*>::iterator Plansza::usun_z_planszy(list<Pionek*>::iterator it) {
  Pozycja pozycja = (*it)->pozycja();
  return plansza[pozycja.y][pozycja.x].erase(it);;
}

list<Pionek*>::iterator Plansza::usun_z_pionkow(list<Pionek*>::iterator it) {
  return pionki.erase(it);
}

list<Pionek*>& Plansza::lista_pionkow() {
  return pionki;
}

list<Pionek*>& Plansza::lista_pionkow(int y, int x) {
  return plansza[y][x];
}

Pionek* Plansza::operator()(int y, int x) {
  return !plansza[y][x].empty() ? plansza[y][x].front() : 0;
}

Gracz::Gracz(int _r, Mode _m) : punkty(0), czas_blokady(0), nowy_pionek(0), rozmiar_planszy(_r), mode(_m) {
  plansza_gracza = Plansza(rozmiar_planszy);
}

int Gracz::wynik() {
  return punkty;
}

bool Gracz::blokada() {
  return czas_blokady > 0;
}

void Gracz::dodaj_pionek(Pionek *p) {
  plansza_gracza.dodaj_na_plansze(p);
  plansza_gracza.dodaj_do_pionkow(p);
}

Pionek* Gracz::plansza(int y, int x) {
  return plansza_gracza(y, x);
}

Plansza& Gracz::plansza() {
  return plansza_gracza;
}

Zenek::Zenek(int _r, Mode _m) : Gracz(_r, _m) {}

void Zenek::wczytaj_ruch_G(Akcja *akcja) {
  char rodzaj_akcji = akcja->rodzaj_akcji;
  if (czas_blokady > 0) {
    if (rodzaj_akcji == 'Z') {
      nowy_pionek = 0;
      czas_blokady = 0;
    } else {
      czas_blokady -= 1;
      if (czas_blokady == 0 && nowy_pionek) {
	dodaj_pionek(nowy_pionek);
	nowy_pionek = 0;
      }
    }
  } else {
    if (rodzaj_akcji == 'P') {
      char znak_pionka = akcja->znak_pionka;
      int pozycja_y = akcja->pozycja_y;
      pozycja_y -= 1;
      if (znak_pionka == 'R')
	nowy_pionek = new Rycerz(Pozycja(1, pozycja_y));
      else
	nowy_pionek = new Smok(Pozycja(1, pozycja_y));
      czas_blokady = nowy_pionek->czas();
    }
    if (czas_blokady == 0 && nowy_pionek) {
      dodaj_pionek(nowy_pionek);
      nowy_pionek = 0;
    }
  }
}

void Zenek::wczytaj_ruch_A(Akcja*) {
  char rodzaj_akcji;
  if (czas_blokady > 0) {
    rodzaj_akcji = rand() % 8 ? 'X' : 'Z';
    if (rodzaj_akcji == 'Z') {
      nowy_pionek = 0;
      czas_blokady = 0;
    } else {
      czas_blokady -= 1;
      if (czas_blokady == 0 && nowy_pionek) {
	dodaj_pionek(nowy_pionek);
	nowy_pionek = 0;
      }
    }
  } else {
    rodzaj_akcji = rand() % 10 ? 'P' : 'X';
    if (rodzaj_akcji == 'P') {
      char znak_pionka = rand() % 2 ? 'R' : 'S';
      int pozycja = rand() % rozmiar_planszy;
      if (znak_pionka == 'R')
	nowy_pionek = new Rycerz(Pozycja(1, pozycja));
      else
	nowy_pionek = new Smok(Pozycja(1, pozycja));
      czas_blokady = nowy_pionek->czas();
    }
    if (czas_blokady == 0 && nowy_pionek) {
      dodaj_pionek(nowy_pionek);
      nowy_pionek = 0;
    }
  }
}

void Zenek::wczytaj_ruch(Akcja* akcja) {
  if (mode == GRACZ)
    wczytaj_ruch_G(akcja);
  else
    wczytaj_ruch_A(akcja);
}

int Zenek::przesun_pionki(Plansza plansza_wroga) {
  list<Pionek*>::iterator it, itp;
  for (it = plansza_gracza.lista_pionkow().begin(); it != plansza_gracza.lista_pionkow().end(); ) {
    Pozycja pozycja = (*it)->pozycja();
    itp = plansza_gracza.szukaj_na_planszy(*it);
    if (itp != plansza_gracza.lista_pionkow(pozycja.y, pozycja.x).end()) {
      Pozycja nowa_pozycja = pozycja;
      int liczba_krokow = (*it)->krok();
      for (int i = pozycja.x + 1; i <= rozmiar_planszy + 1 && i <= pozycja.x + liczba_krokow; i++) {
	if (!plansza_wroga(pozycja.y, i))
	  nowa_pozycja.x += 1;
	else
	  break;
      }
      if (nowa_pozycja.x >= rozmiar_planszy + 1) {
	plansza_gracza.usun_z_planszy(itp);
	it = plansza_gracza.usun_z_pionkow(it);
	punkty++;
      } else if (nowa_pozycja.x != pozycja.x) {
	plansza_gracza.usun_z_planszy(itp);
	(*it)->przesun(nowa_pozycja.x - pozycja.x, rozmiar_planszy);
	plansza_gracza.dodaj_na_plansze(*it);
	it++;
      } else it++;
    } else it++;
  }
  return punkty;
}

Wrog::Wrog(int _r, Mode _m) : Gracz(_r, _m) {}

void Wrog::wczytaj_ruch_G(Akcja *akcja) {
  char rodzaj_akcji = akcja->rodzaj_akcji;
  if (czas_blokady > 0) {
    if (rodzaj_akcji == 'Z') {
      nowy_pionek = 0;
      czas_blokady = 0;
    } else {
      czas_blokady -= 1;
      if (czas_blokady == 0 && nowy_pionek) {
	dodaj_pionek(nowy_pionek);
	nowy_pionek = 0;
      }
    }
  } else {
    if (rodzaj_akcji == 'P') {
      char znak_pionka = akcja->znak_pionka;
      int pozycja_y = akcja->pozycja_y;
      pozycja_y -= 1;
      if (znak_pionka == 'R')
	nowy_pionek = new Rycerz(Pozycja(rozmiar_planszy, pozycja_y));
      else
	nowy_pionek = new Smok(Pozycja(rozmiar_planszy, pozycja_y));
      czas_blokady = nowy_pionek->czas();
    }
    if (czas_blokady == 0 && nowy_pionek) {
      dodaj_pionek(nowy_pionek);
      nowy_pionek = 0;
    }
  }
}

void Wrog::wczytaj_ruch_A(Akcja*) {
  char rodzaj_akcji;
  if (czas_blokady > 0) {
    rodzaj_akcji = rand() % 8 ? 'X' : 'Z';
    if (rodzaj_akcji == 'Z') {
      nowy_pionek = 0;
      czas_blokady = 0;
    } else {
      czas_blokady -= 1;
      if (czas_blokady == 0 && nowy_pionek) {
	dodaj_pionek(nowy_pionek);
	nowy_pionek = 0;
      }
    }
  } else {
    rodzaj_akcji = rand() % 10 ? 'P' : 'X';
    if (rodzaj_akcji == 'P') {
      char znak_pionka = rand() % 2 ? 'R' : 'S';
      int pozycja = rand() % rozmiar_planszy;
      if (znak_pionka == 'R')
	nowy_pionek = new Rycerz(Pozycja(rozmiar_planszy, pozycja));
      else
	nowy_pionek = new Smok(Pozycja(rozmiar_planszy, pozycja));
      czas_blokady = nowy_pionek->czas();
    }
    if (czas_blokady == 0 && nowy_pionek) {
      dodaj_pionek(nowy_pionek);
      nowy_pionek = 0;
    }
  }
}

void Wrog::wczytaj_ruch(Akcja* akcja) {
  if (mode == GRACZ)
    wczytaj_ruch_G(akcja);
  else
    wczytaj_ruch_A(akcja);
}

int Wrog::przesun_pionki(Plansza plansza_wroga) {
  list<Pionek*>::iterator it, itp;
  for (it = plansza_gracza.lista_pionkow().begin(); it != plansza_gracza.lista_pionkow().end(); ) {
    Pozycja pozycja = (*it)->pozycja();
    itp = plansza_gracza.szukaj_na_planszy(*it);
    if (itp != plansza_gracza.lista_pionkow(pozycja.y, pozycja.x).end()) {
      Pozycja nowa_pozycja = pozycja;
      int liczba_krokow = (*it)->krok();
      for (int i = pozycja.x - 1; i >= 0 && i >= pozycja.x - liczba_krokow; i--) {
	if (!plansza_wroga(pozycja.y, i))
	  nowa_pozycja.x -= 1;
	else
	  break;
      }
      if (nowa_pozycja.x <= 0) {
	plansza_gracza.usun_z_planszy(itp);
	it = plansza_gracza.usun_z_pionkow(it);
	punkty++;
      } else if (nowa_pozycja.x != pozycja.x) {
	plansza_gracza.usun_z_planszy(itp);
	(*it)->przesun(nowa_pozycja.x - pozycja.x, rozmiar_planszy);
	plansza_gracza.dodaj_na_plansze(*it);
	it++;
      } else it++;
    } else it++;
  }
  return punkty;
}

Sedzia::Sedzia(int _r) : max_roznica_punktow(10), rozmiar_planszy(_r) {}

bool Sedzia::koniec_gry(Gracz *zenek, Gracz *wrog) {
  return abs(zenek->wynik() - wrog->wynik()) > max_roznica_punktow;
}

void Sedzia::rozegraj_bitwe(Plansza &plansza_gracza, Plansza &plansza_wroga) {
  list<Pionek*>::iterator itg, itw, itp;
  for (int i = 0; i < rozmiar_planszy; i++)
    for (int j = 0; j < rozmiar_planszy + 1; j++) {
      list<Pionek*> &pionki_gracza = plansza_gracza.lista_pionkow(i, j);
      if (!pionki_gracza.empty()) {
	list<Pionek*> &pionki_wroga = plansza_wroga.lista_pionkow(i, j + 1);
	if (!pionki_wroga.empty()) {
	  int sila_gracza = 0,
	    sila_wroga = 0,
	    obrona_gracza = 0,
	    obrona_wroga = 0;
	  for (itg = pionki_gracza.begin(); itg != pionki_gracza.end(); itg++) {
	    sila_gracza += (*itg)->sila();
	    obrona_gracza += (*itg)->obrona();
	  }
	  for (itw = pionki_wroga.begin(); itw != pionki_wroga.end(); itw++) {
	    sila_wroga += (*itw)->sila();
	    obrona_wroga += (*itw)->obrona();
	  }
	  sila_gracza = (0.4 + (rand() % 50) / 100.0) * sila_gracza;
	  sila_wroga = (0.4 + (rand() % 50) / 100.0) * sila_wroga;
	  int szkoda_gracza = max(0, sila_wroga - obrona_gracza),
	    szkoda_wroga = max(0, sila_gracza - obrona_wroga);
	  for (itg = pionki_gracza.begin(); itg != pionki_gracza.end() && szkoda_gracza > 0; ) {
	    int zycie = (*itg)->zycie();
	    itp = plansza_gracza.szukaj_wsrod_pionkow(*itg);
	    if (zycie <= szkoda_gracza) {
	      plansza_gracza.usun_z_pionkow(itp);
	      itg = plansza_gracza.usun_z_planszy(itg);

	    } else {
	      (*itg)->zmniejsz_zycie(szkoda_gracza);
	      (*itp)->zmniejsz_zycie(szkoda_gracza);
	      itg++;
	    }
	    szkoda_gracza -= zycie;
	  }
	  for (itw = pionki_wroga.begin(); itw != pionki_wroga.end() && szkoda_wroga > 0; ) {
	    int zycie = (*itw)->zycie();
	    itp = plansza_wroga.szukaj_wsrod_pionkow(*itw);
	    if (zycie <= szkoda_wroga) {
	      plansza_wroga.usun_z_pionkow(itp);
	      itw = plansza_wroga.usun_z_planszy(itw);
	    } else {
	      (*itw)->zmniejsz_zycie(szkoda_wroga);
	      (*itp)->zmniejsz_zycie(szkoda_wroga);
	      itw++;
	    }
	    szkoda_wroga -= zycie;
	  }
	}
      }
    }
}
