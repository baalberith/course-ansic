#ifndef _game_txt_h_
#define _game_txt_h_

#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>

using namespace std;

struct Pozycja {
  int x, y;
  Pozycja(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

struct Akcja {
  char rodzaj_akcji, znak_pionka;
  int pozycja_y;
  Akcja(char _r, char _z, int _y) : rodzaj_akcji(_r), znak_pionka(_z), pozycja_y(_y) {}
};

enum Mode { GRACZ = 0, AI = 1 };

class Pionek {
  protected:
    Pozycja pozycja_pionka;
    int pozostale_zycie;
  public:
    Pionek(Pozycja _p);
    Pozycja pozycja();
    void przesun(int krok, int rozmiar);
    int zycie();
    void zmniejsz_zycie(int ile);
    virtual int czas() = 0;
    virtual int krok() = 0;
    virtual int sila() = 0;
    virtual int obrona() = 0;
    virtual char repr() = 0;
    virtual int max_zycie() = 0;
};

class Rycerz : public Pionek {
  private:
    static const int czas_tworzenia = 1,
      dlugosc_kroku = 3,
      dlugosc_zycia = 32;
    static const int sila_ataku = 13,
      skutecznosc_obrony = 3;
  public:
    Rycerz(Pozycja _p);
    int czas();
    int krok();
    int sila();
    int obrona();
    char repr();
    int max_zycie();
};

class Smok : public Pionek {
  private:
    static const int czas_tworzenia = 2,
      dlugosc_kroku = 1,
      dlugosc_zycia = 49;
    static const int sila_ataku = 17,
      skutecznosc_obrony = 5;
  public:
    Smok(Pozycja _p);
    int czas();
    int krok();
    int sila();
    int obrona();
    char repr();
    int max_zycie();
};

class Plansza {
  private:
    int rozmiar;
    vector< vector< list<Pionek*> > > plansza;
    list<Pionek*> pionki;
    bool porownaj_pionki(Pionek *p1, Pionek *p2);
  public:
    Plansza(int _r = 0);
    void dodaj_na_plansze(Pionek *pionek);
    void dodaj_do_pionkow(Pionek *pionek);
    list<Pionek*>::iterator szukaj_na_planszy(Pionek *pionek);
    list<Pionek*>::iterator szukaj_wsrod_pionkow(Pionek *pionek);
    list<Pionek*>::iterator usun_z_planszy(list<Pionek*>::iterator it);
    list<Pionek*>::iterator usun_z_pionkow(list<Pionek*>::iterator it);
    list<Pionek*>& lista_pionkow();
    list<Pionek*>& lista_pionkow(int y, int x);
    Pionek* operator()(int y, int x);
};

class Gracz {
  protected:
    int punkty,
      czas_blokady,
      rozmiar_planszy;
    Mode mode;
    Plansza plansza_gracza;
    Pionek *nowy_pionek;
  public:
    Gracz(int _r, Mode _m);
    int wynik();
    bool blokada();
    void dodaj_pionek(Pionek *p);
    Pionek* plansza(int y, int x);
    Plansza& plansza();
    virtual int przesun_pionki(Plansza) = 0;
    virtual void wczytaj_ruch(Akcja*) = 0;
};

class Zenek : public Gracz {
  public:
    Zenek(int _r, Mode _m);
    void wczytaj_ruch_G(Akcja *akcja);
    void wczytaj_ruch_A(Akcja*);
    void wczytaj_ruch(Akcja* akcja);
    int przesun_pionki(Plansza plansza_wroga);
};

class Wrog : public Gracz {
  public:
    Wrog(int _r, Mode _m);
    void wczytaj_ruch_G(Akcja *akcja);
    void wczytaj_ruch_A(Akcja*);
    void wczytaj_ruch(Akcja* akcja);
    int przesun_pionki(Plansza plansza_wroga);
};

class Sedzia {
  private:
    int rozmiar_planszy,
      max_roznica_punktow;
  public:
    Sedzia(int _r);
    bool koniec_gry(Gracz *zenek, Gracz *wrog);
    void rozegraj_bitwe(Plansza &plansza_gracza, Plansza &plansza_wroga);
};

#endif
