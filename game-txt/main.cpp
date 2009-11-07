#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
using namespace std;

struct Pozycja {
  int x, y;
  Pozycja(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

class Pionek {
  protected:
    Pozycja pozycja_pionka;
    int pozostale_zycie;

  public:
    Pionek(Pozycja _p) : pozycja_pionka(_p) {}

    Pozycja pozycja() {
      return pozycja_pionka;
    }

    void przesun(int krok) {
      pozycja_pionka.x += krok;
    }

    int zycie() {
      return pozostale_zycie;
    }

    void zmniejsz_zycie(int ile) {
      pozostale_zycie -= ile;
    }

    virtual int czas() = 0;
    virtual int krok() = 0;
    virtual int sila() = 0;
    virtual int obrona() = 0;
    virtual char repr() = 0;
};

class Rycerz : public Pionek {
  private:
    static const int czas_tworzenia = 2,
      dlugosc_kroku = 3,
      dlugosc_zycia = 8;
    static const int sila_ataku = 10,
      skutecznosc_obrony = 3;

  public:
    Rycerz(Pozycja _p) : Pionek(_p) {
      pozostale_zycie = dlugosc_zycia;
    }

    int czas() { return czas_tworzenia; }
    int krok() { return dlugosc_kroku; }
    int sila() { return sila_ataku; }
    int obrona() { return skutecznosc_obrony; }
    char repr() { return 'R'; }
};

class Smok : public Pionek {
  private:
    static const int czas_tworzenia = 3,
      dlugosc_kroku = 2,
      dlugosc_zycia = 10;
    static const int sila_ataku = 15,
      skutecznosc_obrony = 6;

  public:
    Smok(Pozycja _p) : Pionek(_p) {
      pozostale_zycie = dlugosc_zycia;
    }

    int czas() { return czas_tworzenia; }
    int krok() { return dlugosc_kroku; }
    int sila() { return sila_ataku; }
    int obrona() { return skutecznosc_obrony; }
    char repr() { return 'S'; }
};

class Plansza {
  private:
    int rozmiar;
    vector< vector< list<Pionek*> > > plansza;
    list<Pionek*> pionki;

    bool porownaj_pionki(Pionek *p1, Pionek *p2) {
      return p1->repr() == p2->repr() && p1->zycie() == p2->zycie();
    }

  public:
    Plansza(int _r = 0) : rozmiar(_r) {
      plansza.resize(rozmiar);
      for (int i = 0; i < rozmiar; i++)
        plansza[i].resize(rozmiar + 2);
    }

    void dodaj_na_plansze(Pionek *pionek) {
      Pozycja pozycja = pionek->pozycja();
      plansza[pozycja.y][pozycja.x].push_back(pionek);
    }

    void dodaj_do_pionkow(Pionek *pionek) {
      pionki.push_back(pionek);
    }

    list<Pionek*>::iterator szukaj_na_planszy(Pionek *pionek) {
      list<Pionek*>::iterator it;
      Pozycja pozycja = pionek->pozycja();
      for (it = plansza[pozycja.y][pozycja.x].begin(); it != plansza[pozycja.y][pozycja.x].end(); it++) {
        if (porownaj_pionki(*it, pionek))
          return it;
      }
    }

    list<Pionek*>::iterator szukaj_wsrod_pionkow(Pionek *pionek) {
      list<Pionek*>::iterator it;
      for (it = pionki.begin(); it != pionki.end(); it++) {
        if (porownaj_pionki(*it, pionek))
          return it;
      }
    }

    list<Pionek*>::iterator usun_z_planszy(list<Pionek*>::iterator it) {
      Pozycja pozycja = (*it)->pozycja();
      return plansza[pozycja.y][pozycja.x].erase(it);;
    }

    list<Pionek*>::iterator usun_z_pionkow(list<Pionek*>::iterator it) {
      return pionki.erase(it);
    }

    list<Pionek*>& lista_pionkow() {
      return pionki;
    }

    list<Pionek*>& lista_pionkow(int x, int y) {
      return plansza[x][y];
    }

    Pionek* operator()(int x, int y) {
      return !plansza[x][y].empty() ? plansza[x][y].front() : 0;
    }
};

class Gracz {
  protected:
    int punkty,
      czas_blokady,
      rozmiar_planszy;
    Plansza plansza_gracza;
    Pionek *nowy_pionek;

  public:
    Gracz(int _r) : punkty(0), czas_blokady(0), nowy_pionek(0), rozmiar_planszy(_r) {
      plansza_gracza = Plansza(rozmiar_planszy);
    }

    int wynik() {
      return punkty;
    }

    void dodaj_pionek(Pionek *p) {
      plansza_gracza.dodaj_na_plansze(p);
      plansza_gracza.dodaj_do_pionkow(p);
    }

    Pionek* plansza(int x, int y) {
      return plansza_gracza(x, y);
    }

    Plansza& plansza() {
      return plansza_gracza;
    }

    virtual void wczytaj_ruch() = 0;
    virtual int przesun_pionki(Plansza) = 0;
};

class Zenek : public Gracz {
  public:
    Zenek(int _r) : Gracz(_r) {}

    void wczytaj_ruch() {
      char rodzaj_akcji;
      if (czas_blokady > 0) {
        do {
          cout << "akcja (z - zatrzymanie produkcji, x - brak akcji): ";
          cin >> rodzaj_akcji;
        } while (rodzaj_akcji != 'z' && rodzaj_akcji != 'x');
        if (rodzaj_akcji == 'z') {
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
        do {
          cout << "akcja (p - produkcja, x - brak akcji): ";
          cin >> rodzaj_akcji;
        } while (rodzaj_akcji != 'p' && rodzaj_akcji != 'x');
        if (rodzaj_akcji == 'p') {
          char znak_pionka;
          int pozycja;
          do {
            cout << "pionek (r - rycerz, s - smok): ";
            cin >> znak_pionka;
          } while (znak_pionka != 'r' && znak_pionka != 's');
          do {
            cout << "pozycja [1.." << rozmiar_planszy << "]: ";
            cin >> pozycja;
            pozycja -= 1;
          } while (pozycja < 0 || pozycja >= rozmiar_planszy);
          if (znak_pionka == 'r')
            nowy_pionek = new Rycerz(Pozycja(0, pozycja));
          else
            nowy_pionek = new Smok(Pozycja(0, pozycja));
          czas_blokady = nowy_pionek->czas();
        }
      }
    }

    int przesun_pionki(Plansza plansza_wroga) {
      list<Pionek*>::iterator it, itp;
      for (it = plansza_gracza.lista_pionkow().begin(); it != plansza_gracza.lista_pionkow().end(); ) {
        itp = plansza_gracza.szukaj_na_planszy(*it);
        plansza_gracza.usun_z_planszy(itp);
        Pozycja pozycja = (*it)->pozycja();
        Pozycja nowa_pozycja = pozycja;
        int liczba_krokow = (*it)->krok();
        for (int i = pozycja.x + 1; i <= rozmiar_planszy + 1 && i <= pozycja.x + liczba_krokow; i++) {
          if (!plansza_wroga(pozycja.y, i))
            nowa_pozycja.x += 1;
          else
            break;
        }
        if (nowa_pozycja.x >= rozmiar_planszy + 1) {
          it = plansza_gracza.usun_z_pionkow(it);
          punkty++;
        } else {
          (*it)->przesun(nowa_pozycja.x - pozycja.x);
          plansza_gracza.dodaj_na_plansze(*it);
          it++;
        }
      }
      return punkty;
    }
};

class Wrog : public Gracz {
  public:
    Wrog(int _r) : Gracz(_r) {}

    void wczytaj_ruch() {
      char rodzaj_akcji;
      if (czas_blokady > 0) {
        rodzaj_akcji = rand() % 8 ? 'x' : 'z';
        if (rodzaj_akcji == 'z') {
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
        rodzaj_akcji = rand() % 10 ? 'p' : 'x';
        if (rodzaj_akcji == 'p') {
          int pozycja = rand() % rozmiar_planszy;
          char znak_pionka = rand() % 2 ? 'r' : 's';
          if (znak_pionka == 'r')
            nowy_pionek = new Rycerz(Pozycja(rozmiar_planszy + 1, pozycja));
          else
            nowy_pionek = new Smok(Pozycja(rozmiar_planszy + 1, pozycja));
          czas_blokady = nowy_pionek->czas();
        }
      }
    }

    int przesun_pionki(Plansza plansza_wroga) {
      list<Pionek*>::iterator it, itp;
      for (it = plansza_gracza.lista_pionkow().begin(); it != plansza_gracza.lista_pionkow().end(); ) {
        itp = plansza_gracza.szukaj_na_planszy(*it);
        plansza_gracza.usun_z_planszy(itp);
        Pozycja pozycja = (*it)->pozycja();
        Pozycja nowa_pozycja = pozycja;
        int liczba_krokow = (*it)->krok();
        for (int i = pozycja.x - 1; i >= 0 && i >= pozycja.x - liczba_krokow; i--) {
          if (!plansza_wroga(pozycja.y, i))
            nowa_pozycja.x -= 1;
          else
            break;
        }
        if (nowa_pozycja.x <= 0) {
          it = plansza_gracza.usun_z_pionkow(it);
          punkty++;
        } else {
          (*it)->przesun(nowa_pozycja.x - pozycja.x);
          plansza_gracza.dodaj_na_plansze(*it);
          it++;
        }
      }
      return punkty;
    }
};

class Sedzia {
  private:
    int rozmiar_planszy,
      max_liczba_punktow;

  public:
    Sedzia(int _r) : max_liczba_punktow(10), rozmiar_planszy(_r) {}

    bool koniec_gry(Gracz *zenek, Gracz *wrog) {
      return zenek->wynik() == max_liczba_punktow || wrog->wynik() == max_liczba_punktow;
    }

    void rozegraj_bitwe(Plansza &plansza_gracza, Plansza &plansza_wroga) {
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
              int szkoda_gracza = max(0, sila_wroga - obrona_gracza),
                szkoda_wroga = max(0, sila_gracza - obrona_wroga);
              for (itg = pionki_gracza.begin(); itg != pionki_gracza.end() && szkoda_gracza > 0; ) {
                int zycie = (*itg)->zycie();
                itp = plansza_gracza.szukaj_wsrod_pionkow(*itg);
                if (zycie <= szkoda_gracza) {
                  plansza_gracza.usun_z_pionkow(itp);
                  itg = plansza_gracza.usun_z_planszy(itg);
                  cout << "Pionek gracza usmiercony!" << endl;

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
                  cout << "Pionek wroga usmiercony!" << endl;
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
};

class Gra {
  private:
    Gracz *zenek,
      *wrog;
    Sedzia *sedzia;
    int rozmiar_planszy,
      ilosc_rund;

  public:
    Gra(int _r = 9) : ilosc_rund(100), rozmiar_planszy(_r) {
      zenek = new Zenek(rozmiar_planszy);
      wrog = new Wrog(rozmiar_planszy);
      sedzia = new Sedzia(rozmiar_planszy);
    }

    void wczytaj_ruch() {
      zenek->wczytaj_ruch();
      wrog->wczytaj_ruch();
    }

    void przesun_pionki() {
      zenek->przesun_pionki(wrog->plansza());
      wrog->przesun_pionki(zenek->plansza());
    }

    void rozegraj_bitwe() {
      sedzia->rozegraj_bitwe(zenek->plansza(), wrog->plansza());
    }

    bool koniec_gry() {
      return sedzia->koniec_gry(zenek, wrog);
    }

    void wyswietl_plansze() {
      cout << 'x' << ' ';
      for (int i = 1; i < rozmiar_planszy + 1; i++)
        cout << i << ' ';
      cout << endl;
      for (int i = 0; i < rozmiar_planszy; i++) {
        cout << i + 1 << ' ';
        for (int j = 1; j < rozmiar_planszy + 1; j++) {
          if (zenek->plansza(i, j))
            cout << zenek->plansza(i, j)->repr();
          else if (wrog->plansza(i, j))
            cout << wrog->plansza(i, j)->repr();
          else cout << 'x';
          cout << ' ';
        }
        cout << endl;
      }
    }

    void wyswietl_wynik() {
      cout << "____ G " << zenek->wynik() << " : " << wrog->wynik() << " W ____" << endl;
    }

    void zagraj() {
      for (int i = 0; i < ilosc_rund && !koniec_gry(); i++) {
        wczytaj_ruch();
        przesun_pionki();
        wyswietl_plansze();
        rozegraj_bitwe();
        wyswietl_wynik();
      }
    }
};

int main() {
  srand(time(0));

  Gra nowa_gra;
  nowa_gra.zagraj();

  return 0;
}
