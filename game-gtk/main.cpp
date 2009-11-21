#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>

#include <cairo.h>
#include <gtk/gtk.h>

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
    Pionek(Pozycja _p) : pozycja_pionka(_p) {}

    Pozycja pozycja() {
      return pozycja_pionka;
    }

    void przesun(int krok, int rozmiar) {
      pozycja_pionka.x += krok;
      if (krok >= 0 && pozycja_pionka.x > rozmiar + 1)
        pozycja_pionka.x = rozmiar + 1;
      else if (krok < 0 && pozycja_pionka.x < 0)
        pozycja_pionka.x = 0;
    }

    int zycie() {
      return pozostale_zycie;
    }

    void zmniejsz_zycie(int ile) {
      pozostale_zycie -= ile;
      if (pozostale_zycie < 0)
        pozostale_zycie = 0;
    }

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
    Rycerz(Pozycja _p) : Pionek(_p) {
      pozostale_zycie = dlugosc_zycia;
    }

    int czas() { return czas_tworzenia; }
    int krok() { return dlugosc_kroku; }
    int sila() { return sila_ataku; }
    int obrona() { return skutecznosc_obrony; }
    char repr() { return 'R'; }
    int max_zycie() { return dlugosc_zycia; }
};

class Smok : public Pionek {
  private:
    static const int czas_tworzenia = 2,
      dlugosc_kroku = 1,
      dlugosc_zycia = 49;
    static const int sila_ataku = 17,
      skutecznosc_obrony = 5;

  public:
    Smok(Pozycja _p) : Pionek(_p) {
      pozostale_zycie = dlugosc_zycia;
    }

    int czas() { return czas_tworzenia; }
    int krok() { return dlugosc_kroku; }
    int sila() { return sila_ataku; }
    int obrona() { return skutecznosc_obrony; }
    char repr() { return 'S'; }
    int max_zycie() { return dlugosc_zycia; }
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
      return it;
    }

    list<Pionek*>::iterator szukaj_wsrod_pionkow(Pionek *pionek) {
      list<Pionek*>::iterator it;
      for (it = pionki.begin(); it != pionki.end(); it++) {
        if (porownaj_pionki(*it, pionek))
          return it;
      }
      return it;
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

    list<Pionek*>& lista_pionkow(int y, int x) {
      return plansza[y][x];
    }

    Pionek* operator()(int y, int x) {
      return !plansza[y][x].empty() ? plansza[y][x].front() : 0;
    }
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
    Gracz(int _r, Mode _m) : punkty(0), czas_blokady(0), nowy_pionek(0), rozmiar_planszy(_r), mode(_m) {
      plansza_gracza = Plansza(rozmiar_planszy);
    }

    int wynik() {
      return punkty;
    }

    bool blokada() {
      return czas_blokady > 0;
    }

    void dodaj_pionek(Pionek *p) {
      plansza_gracza.dodaj_na_plansze(p);
      plansza_gracza.dodaj_do_pionkow(p);
    }

    Pionek* plansza(int y, int x) {
      return plansza_gracza(y, x);
    }

    Plansza& plansza() {
      return plansza_gracza;
    }

    virtual int przesun_pionki(Plansza) = 0;
    virtual void wczytaj_ruch(Akcja*) = 0;
};

class Zenek : public Gracz {
  public:
    Zenek(int _r, Mode _m) : Gracz(_r, _m) {}

    void wczytaj_ruch_G(Akcja *akcja) {
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

    void wczytaj_ruch_A(Akcja*) {
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

    void wczytaj_ruch(Akcja* akcja) {
      if (mode == GRACZ)
        wczytaj_ruch_G(akcja);
      else
        wczytaj_ruch_A(akcja);
    }

    int przesun_pionki(Plansza plansza_wroga) {
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
};

class Wrog : public Gracz {
  public:
    Wrog(int _r, Mode _m) : Gracz(_r, _m) {}

    void wczytaj_ruch_G(Akcja *akcja) {
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

    void wczytaj_ruch_A(Akcja*) {
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

    void wczytaj_ruch(Akcja* akcja) {
      if (mode == GRACZ)
        wczytaj_ruch_G(akcja);
      else
        wczytaj_ruch_A(akcja);
    }

    int przesun_pionki(Plansza plansza_wroga) {
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
};

class Sedzia {
  private:
    int rozmiar_planszy,
      max_roznica_punktow;

  public:
    Sedzia(int _r) : max_roznica_punktow(10), rozmiar_planszy(_r) {}

    bool koniec_gry(Gracz *zenek, Gracz *wrog) {
      return abs(zenek->wynik() - wrog->wynik()) > max_roznica_punktow;
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
};

const int rozmiar_planszy = 10,
  ilosc_rund = 20;
Gracz *zenek,
  *wrog;
Sedzia *sedzia = new Sedzia(rozmiar_planszy);

const int rozmiar_pola = 50,
  rozmiar_x = (rozmiar_planszy + 2) * rozmiar_pola,
  rozmiar_y = (rozmiar_planszy) * rozmiar_pola;

char rodzaj_akcji, rodzaj_akcji2,
  znak_pionka, znak_pionka2;
int pozycja, pozycja2;
bool kto;

enum { TARGET_ROOTWIN };
static GtkTargetEntry target_list[] = { { "application/x-rootwindow-drop", 0, TARGET_ROOTWIN } };
static guint n_targets = G_N_ELEMENTS (target_list);

void wyswietl_widget(void *widget, void* data) {
  gtk_widget_show ((GtkWidget*) widget);
}

void schowaj_widget(void *widget, void* data) {
  gtk_widget_hide ((GtkWidget*) widget);
}

void nowa_gra(GtkWidget *widget, GList *list) {
  g_list_foreach (list, schowaj_widget, NULL);
}

void nowa_gra_ga(GtkWidget *widget, GList *list) {
  zenek = new Zenek(rozmiar_planszy, GRACZ);
  wrog = new Wrog(rozmiar_planszy, AI);

  g_list_foreach (list, wyswietl_widget, NULL);
}

void nowa_gra_aa(GtkWidget *widget, GList *list) {
  zenek = new Zenek(rozmiar_planszy, AI);
  wrog = new Wrog(rozmiar_planszy, AI);

  g_list_foreach (list, wyswietl_widget, NULL);
}

void nowa_gra_gg(GtkWidget *widget, GList *list) {
  zenek = new Zenek(rozmiar_planszy, GRACZ);
  wrog = new Wrog(rozmiar_planszy, GRACZ);

  g_list_foreach (list, wyswietl_widget, NULL);
}

void wyswietl_plansze(GtkWidget *widget, GdkEventExpose *event, gpointer data) {
  cairo_t *cr, *cr2;

  cr = gdk_cairo_create (widget->window);
  cr2 = gdk_cairo_create (widget->window);

  cairo_surface_t *zdim, *zwim, *wdim, *wwim;

  zdim = cairo_image_surface_create_from_png ("png/zd.png");
  zwim = cairo_image_surface_create_from_png ("png/zw.png");
  wdim = cairo_image_surface_create_from_png ("png/wd.png");
  wwim = cairo_image_surface_create_from_png ("png/ww.png");

  int zycie, max_zycie;
  double x, y = 3.0;

  for (int i = 0; i < rozmiar_planszy; i++) {
    for (int j = 1; j <= rozmiar_planszy; j++) {
      if (zenek->plansza(i, j)) {
        switch (zenek->plansza(i, j)->repr()) {
          case 'R' : cairo_set_source_surface (cr, zwim, j * rozmiar_pola + 3, i * rozmiar_pola + 3);
            break;
          case 'S' : cairo_set_source_surface (cr, zdim, j * rozmiar_pola + 3, i * rozmiar_pola + 3);
            break;
        }
        cairo_paint (cr);

        zycie = zenek->plansza(i, j)->zycie();
        zycie = zycie > 0 ? zycie : 0;
        max_zycie = zenek->plansza(i, j)->max_zycie();
        x = (((zycie * 100.0) / max_zycie) * rozmiar_pola) / 100.0;

        cairo_set_source_rgb (cr2, 0.4, 0.9, 0.4);
        cairo_rectangle (cr2, j * rozmiar_pola, i * rozmiar_pola, x, y);
        cairo_fill (cr2);

        cairo_set_source_rgb (cr2, 0.9, 0.1, 0.1);
        cairo_rectangle (cr2, j * rozmiar_pola + x, i * rozmiar_pola, rozmiar_pola - x, y);
        cairo_fill (cr2);
      }
      else if (wrog->plansza(i, j)) {
        switch (wrog->plansza(i, j)->repr()) {
          case 'R' : cairo_set_source_surface (cr, wwim, j * rozmiar_pola + 3, i * rozmiar_pola + 3);
            break;
          case 'S' : cairo_set_source_surface (cr, wdim, j * rozmiar_pola + 3, i * rozmiar_pola + 3);
            break;
        }
        cairo_paint (cr);

        zycie = wrog->plansza(i, j)->zycie();
        zycie = zycie > 0 ? zycie : 0;
        max_zycie = wrog->plansza(i, j)->max_zycie();
        x = (((zycie * 100.0) / max_zycie) * rozmiar_pola) / 100.0;

        cairo_set_source_rgb (cr2, 0.4, 0.9, 0.4);
        cairo_rectangle (cr2, j * rozmiar_pola, i * rozmiar_pola, x, y);
        cairo_fill (cr2);

        cairo_set_source_rgb (cr2, 0.9, 0.1, 0.1);
        cairo_rectangle (cr2, j * rozmiar_pola + x, i * rozmiar_pola, rozmiar_pola - x, y);
        cairo_fill (cr2);
      }
    }
  }

  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 0.1);

  for (int i = 1; i < rozmiar_planszy + 2; i++) {
    cairo_move_to (cr, i * rozmiar_pola, 0);
    cairo_line_to (cr, i * rozmiar_pola, (rozmiar_planszy) * rozmiar_pola);
  }

  for (int i = 0; i < rozmiar_planszy + 1; i++) {
    cairo_move_to (cr, 0, i * rozmiar_pola);
    cairo_line_to (cr, (rozmiar_planszy + 2) * rozmiar_pola, i  * rozmiar_pola);
  }

  cairo_stroke (cr);

  cairo_set_source_rgb (cr2, 0.1, 0.1, 0.1);
  cairo_select_font_face (cr2, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size (cr2, 26);

  for (int i = 0; i <= rozmiar_planszy; i++) {
    char *str = (char*) malloc(3 * sizeof(char));
    sprintf(str, "%d", i + 1);
    if (i + 1 < 10) {
      cairo_move_to(cr2, 15, i  * rozmiar_pola + rozmiar_pola * 0.65);
      cairo_show_text(cr2, str);
      cairo_move_to(cr2, (rozmiar_planszy + 1) * rozmiar_pola + 15, i  * rozmiar_pola + rozmiar_pola * 0.65);
      cairo_show_text(cr2, str);
    }
    else {
      cairo_move_to(cr2, 9, i  * rozmiar_pola + rozmiar_pola * 0.65);
      cairo_show_text(cr2, str);
      cairo_move_to(cr2, (rozmiar_planszy + 1) * rozmiar_pola + 9, i  * rozmiar_pola + rozmiar_pola * 0.65);
      cairo_show_text(cr2, str);
    }
  }

  cairo_destroy (cr);
  cairo_destroy (cr2);
}

void aktualizuj_plansze(GtkWidget *widget, gpointer data) {
  GdkRegion *region = gdk_drawable_get_clip_region (widget->window);

  gdk_window_invalidate_region (widget->window, region, TRUE);
  gdk_window_process_all_updates();

  usleep(150000);
}

void wczytaj_ruch(GtkWidget *widget, gpointer data) {
  Akcja *akcja = new Akcja(rodzaj_akcji, znak_pionka, pozycja);
  Akcja *akcja2 = new Akcja(rodzaj_akcji2, znak_pionka2, pozycja2);

  zenek->wczytaj_ruch(akcja);
  wrog->wczytaj_ruch(akcja2);

  rodzaj_akcji = 'X';
  rodzaj_akcji2 = 'X';
}

void przesun_pionki(GtkWidget *widget, gpointer data) {
  zenek->przesun_pionki(wrog->plansza());
  wrog->przesun_pionki(zenek->plansza());
}

void rozegraj_bitwe(GtkWidget *widget, gpointer data) {
  sedzia->rozegraj_bitwe(zenek->plansza(), wrog->plansza());
}

void aktualizuj_wynik(GtkWidget *widget, GtkLabel *label) {
  char *str = (char*) malloc(6 * sizeof(char));
  sprintf(str, "%d : %d", zenek->wynik(), wrog->wynik());
  gtk_label_set_text (label, (gchar*) str);
}

void zeruj_wynik(GtkWidget *widget, GtkLabel *label) {
  gtk_label_set_text (label, (gchar*) "0 : 0");
}

void zmien_akcje(GtkWidget *widget, const char *rodzaj) {
  rodzaj_akcji = *rodzaj;
  kto = 0;
}

void zmien_akcje2(GtkWidget *widget, const char *rodzaj) {
  rodzaj_akcji2 = *rodzaj;
  kto = 1;
}

void zmien_pionek(GtkWidget *widget, gpointer data) {
  znak_pionka = *gtk_combo_box_get_active_text (GTK_COMBO_BOX (widget));
}

void zmien_pozycje(GtkWidget *widget, gpointer data) {
  pozycja = atoi(gtk_combo_box_get_active_text (GTK_COMBO_BOX (widget)));
}

void zmien_pionek1(GtkWidget *widget, GdkDragContext *context, const char* rodzaj) {
  cout << "zmien pionek 1" << endl;
  rodzaj_akcji = 'P';
  znak_pionka = *rodzaj;
  kto = 0;

  GtkWidget *image;
  GdkPixbuf *pixbuf;

  if (znak_pionka == 'R')
    image = gtk_image_new_from_file ("png/zw.png");
  else
    image = gtk_image_new_from_file ("png/zd.png");

  pixbuf = gtk_image_get_pixbuf (GTK_IMAGE (image));
  gtk_drag_set_icon_pixbuf (context, pixbuf, -2, -2);
}

void zmien_pionek2(GtkWidget *widget, GdkDragContext *context, const char* rodzaj) {
  cout << "zmien pionek 2" << endl;
  rodzaj_akcji2 = 'P';
  znak_pionka2 = *rodzaj;
  kto = 1;

  GtkWidget *image;
  GdkPixbuf *pixbuf;

  if (znak_pionka2 == 'R')
    image = gtk_image_new_from_file ("png/ww.png");
  else
    image = gtk_image_new_from_file ("png/wd.png");

  pixbuf = gtk_image_get_pixbuf (GTK_IMAGE (image));
  gtk_drag_set_icon_pixbuf (context, pixbuf, -2, -2);
}

void zmien_pozycje12(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data) {
  if (kto == 0) {
    pozycja = (int) y / 50 + 1;
    cout << "pozycja 1: " << pozycja << endl;
  }
  else {
    pozycja2 = (int) y / 50 + 1;
    cout << "pozycja 2: " << pozycja2 << endl;
  }
}

void czysc_ruch1(GtkWidget *widget, GdkDragContext *context, gpointer user_data) {
  cout << "czysc ruch 1" << endl;
  rodzaj_akcji = 'X';
}

void czysc_ruch2(GtkWidget *widget, GdkDragContext *context, gpointer user_data) {
  cout << "czysc ruch 2" << endl;
  rodzaj_akcji2 = 'X';
}

void zmien_pionek31(GtkWidget *widget, const char* rodzaj) {
  znak_pionka = *rodzaj;
  kto = 0;
}

void zmien_pionek32(GtkWidget *widget, const char* rodzaj) {
  znak_pionka2 = *rodzaj;
  kto = 1;
}

void zmien_pozycje3(GtkWidget *widget, GdkEventMotion *event) {
  if (kto == 0) {
    rodzaj_akcji = 'P';
    pozycja = (int) event->y / 50 + 1;
  } else {
    rodzaj_akcji2 = 'P';
    pozycja2 = (int) event->y / 50 + 1;
  }
}

void aktywuj_produkcje(GtkWidget *widget, GtkWidget *button) {
  bool blokada = zenek->blokada();
  gtk_widget_set_sensitive (button, !blokada);
}

void aktywuj_zatrzymanie(GtkWidget *widget, GtkWidget *button) {
  bool blokada = zenek->blokada();
  gtk_widget_set_sensitive (button, blokada);
}

void aktywuj_produkcje2(GtkWidget *widget, GtkWidget *button) {
  bool blokada = wrog->blokada();
  gtk_widget_set_sensitive (button, !blokada);
}

void aktywuj_zatrzymanie2(GtkWidget *widget, GtkWidget *button) {
  bool blokada = wrog->blokada();
  gtk_widget_set_sensitive (button, blokada);
}

void zablokuj_nastepny(GtkWidget *widget, GtkWidget *button) {
  bool koniec = sedzia->koniec_gry(zenek, wrog);
  if (koniec)
    gtk_widget_set_sensitive (button, !koniec);
}

void aktywuj_box(GtkWidget *widget, GtkWidget *box) {
  gtk_widget_set_sensitive (box, TRUE);
}

void deaktywuj_box(GtkWidget *widget, GtkWidget *box) {
  gtk_widget_set_sensitive (box, FALSE);
}



int main(int argc, char *argv[]) {
  srand(time(0));

  GtkWidget *window, *vbox, *mbar, *oitem, *menu, *gaitem, *aaitem, *ggitem, *zitem, *evbox, *darea,
  *hbox, *wbox, *wlabel, *pbutt, *rspbox, *rsbox, *rscombo, *pbox, *pcombo, *zbutt, *nbutt,*aanbutt,
  *ggvbox, *gghbox, *ggrbutt, *ggsbutt, *ggzbutt, *gghbox2, *ggrbutt2, *ggsbutt2, *ggzbutt2, *ggnbutt,
  *label, *separator, *im;

  GList *galist = NULL, *aalist = NULL, *gglist = NULL;

  gtk_init (&argc, &argv);

  // window

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Krwawa gra");
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (window), 5);
  g_signal_connect (window, "delete_event", G_CALLBACK  (gtk_main_quit), NULL);
  g_signal_connect (window, "destroy", G_CALLBACK  (gtk_main_quit), NULL);
  gtk_widget_show (window);

  // vbox

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  // mbar

  mbar = gtk_menu_bar_new ();
  gtk_container_add (GTK_CONTAINER (vbox), mbar);
  gtk_widget_show (mbar);
  oitem = gtk_menu_item_new_with_mnemonic ("_Opcje");
  gtk_menu_bar_append (GTK_MENU_BAR (mbar), oitem);
  gtk_widget_show (oitem);
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (oitem), menu);
  gtk_widget_show (menu);

  gaitem = gtk_menu_item_new_with_label ("Gracz vs AI");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), gaitem);
  gtk_widget_show (gaitem);
  aaitem = gtk_menu_item_new_with_label ("AI vs AI");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), aaitem);
  gtk_widget_show (aaitem);
  ggitem = gtk_menu_item_new_with_label ("Gracz vs Gracz");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), ggitem);
  gtk_widget_show (ggitem);

  zitem = gtk_menu_item_new_with_label ("Zakoncz gre");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), zitem);
  g_signal_connect (G_OBJECT (zitem), "activate", G_CALLBACK (gtk_main_quit), NULL);
  gtk_widget_show (zitem);

  // evbox

  evbox = gtk_event_box_new ();
  gtk_widget_set_size_request (evbox, rozmiar_x + 10, rozmiar_y + 10);
  gtk_container_set_border_width (GTK_CONTAINER (evbox), 5);
  gtk_container_add (GTK_CONTAINER (vbox), evbox);
  g_signal_connect (evbox, "button_press_event", G_CALLBACK (zmien_pozycje3), NULL);
  gtk_widget_add_events (evbox, GDK_BUTTON_PRESS_MASK);

  galist = g_list_append (galist, evbox);
  aalist = g_list_append (aalist, evbox);
  gglist = g_list_append (gglist, evbox);

  gtk_drag_dest_set (evbox, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
  g_signal_connect (evbox, "drag-drop", G_CALLBACK (zmien_pozycje12), NULL);

  // darea

  darea = gtk_drawing_area_new();
  gtk_drawing_area_size (GTK_DRAWING_AREA (darea), rozmiar_x, rozmiar_y);
  gtk_container_add (GTK_CONTAINER (evbox), darea);
  g_signal_connect (darea, "expose-event", G_CALLBACK (wyswietl_plansze), NULL);

  galist = g_list_append (galist, darea);
  aalist = g_list_append (aalist, darea);
  gglist = g_list_append (gglist, darea);

  // hbox

  hbox = gtk_hbox_new (FALSE, 3);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  galist = g_list_append (galist, hbox);
  aalist = g_list_append (aalist, hbox);
  gglist = g_list_append (gglist, hbox);

  // wbox

  wbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), wbox, TRUE, TRUE, 0);

  label = gtk_label_new ("Wynik");
  gtk_box_pack_start (GTK_BOX (wbox), label, TRUE, TRUE, 0);
  wlabel = gtk_label_new ("0 : 0");
  gtk_box_pack_start (GTK_BOX (wbox), wlabel, TRUE, TRUE, 0);

  separator = gtk_vseparator_new ();
  gtk_box_pack_start (GTK_BOX (hbox), separator, TRUE, TRUE, 0);

  galist = g_list_append (galist, wbox);
  galist = g_list_append (galist, label);
  galist = g_list_append (galist, wlabel);
  galist = g_list_append (galist, separator);

  aalist = g_list_append (aalist, wbox);
  aalist = g_list_append (aalist, label);
  aalist = g_list_append (aalist, wlabel);
  aalist = g_list_append (aalist, separator);

  gglist = g_list_append (gglist, wbox);
  gglist = g_list_append (gglist, label);
  gglist = g_list_append (gglist, wlabel);
  gglist = g_list_append (gglist, separator);

  // pbutt

  pbutt = gtk_button_new_with_mnemonic("_Produkcja");
  gtk_box_pack_start (GTK_BOX (hbox), pbutt, TRUE, TRUE, 0);
  g_signal_connect (pbutt, "clicked", G_CALLBACK (zmien_akcje), (gpointer) "P");

  galist = g_list_append (galist, pbutt);

  // rspbox

  rspbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), rspbox, TRUE, TRUE, 0);

  galist = g_list_append (galist, rspbox);

  // rsbox

  rsbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_sensitive (rsbox, FALSE);
  gtk_box_pack_start (GTK_BOX (rspbox), rsbox, TRUE, TRUE, 0);
  g_signal_connect (pbutt, "clicked", G_CALLBACK (aktywuj_box), rsbox);

  label = gtk_label_new ("Rodzaj pionka");
  gtk_box_pack_start (GTK_BOX (rsbox), label, TRUE, TRUE, 0);

  rscombo = gtk_combo_box_new_text();
  gtk_box_pack_start (GTK_BOX (rsbox), rscombo, TRUE, TRUE, 0);
  gtk_combo_box_append_text(GTK_COMBO_BOX(rscombo), "Rycerz");
  gtk_combo_box_append_text(GTK_COMBO_BOX(rscombo), "Smok");
  g_signal_connect (rscombo, "changed", G_CALLBACK(zmien_pionek), NULL);

  galist = g_list_append (galist, rsbox);
  galist = g_list_append (galist, label);
  galist = g_list_append (galist, rscombo);

  // pbox

  pbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_sensitive (pbox, FALSE);
  gtk_box_pack_start (GTK_BOX (rspbox), pbox, TRUE, TRUE, 0);
  g_signal_connect (rscombo, "changed", G_CALLBACK(aktywuj_box), pbox);

  label = gtk_label_new ("Pozycja");
  gtk_box_pack_start (GTK_BOX (pbox), label, TRUE, TRUE, 0);

  pcombo = gtk_combo_box_new_text();
  gtk_box_pack_start (GTK_BOX (pbox), pcombo, TRUE, TRUE, 0);
  char *str = (char*) malloc(3 * sizeof(char));
  for (int i = 1; i <= rozmiar_planszy; i++) {
    sprintf(str, "%d", i);
    gtk_combo_box_append_text(GTK_COMBO_BOX(pcombo), (gchar*) str);
  }
  g_signal_connect (pcombo, "changed", G_CALLBACK(zmien_pozycje), NULL);

  galist = g_list_append (galist, pbox);
  galist = g_list_append (galist, label);
  galist = g_list_append (galist, pcombo);

  // zbutt

  zbutt = gtk_button_new_with_mnemonic("_Zatrzymanie");
  gtk_widget_set_sensitive (zbutt, FALSE);
  gtk_box_pack_start (GTK_BOX (hbox), zbutt, TRUE, TRUE, 0);
  g_signal_connect (zbutt, "clicked", G_CALLBACK (zmien_akcje), (gpointer) "Z");

  separator = gtk_vseparator_new ();
  gtk_box_pack_start (GTK_BOX (hbox), separator, TRUE, TRUE, 0);

  galist = g_list_append (galist, zbutt);
  galist = g_list_append (galist, separator);

  // nbutt

  nbutt = gtk_button_new_with_mnemonic ("_Nastepna runda");
  //gtk_widget_set_sensitive (nbutt, FALSE);
  gtk_box_pack_start (GTK_BOX (hbox), nbutt, TRUE, TRUE, 0);

  galist = g_list_append (galist, nbutt);

  g_signal_connect (pcombo, "changed", G_CALLBACK (aktywuj_box), nbutt);
  g_signal_connect (zbutt, "clicked", G_CALLBACK (aktywuj_box), nbutt);
  g_signal_connect (pbutt, "clicked", G_CALLBACK (deaktywuj_box), nbutt);

  g_signal_connect (nbutt, "clicked", G_CALLBACK (wczytaj_ruch), NULL);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (przesun_pionki), NULL);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (aktualizuj_wynik), wlabel);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (rozegraj_bitwe), NULL);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);

  g_signal_connect (nbutt, "clicked", G_CALLBACK (aktywuj_produkcje), pbutt);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (aktywuj_zatrzymanie), zbutt);

  g_signal_connect (nbutt, "clicked", G_CALLBACK (deaktywuj_box), rsbox);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (deaktywuj_box), pbox);

  g_signal_connect (nbutt, "clicked", G_CALLBACK (zablokuj_nastepny), nbutt);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (zablokuj_nastepny), pbutt);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (zablokuj_nastepny), zbutt);

  // aanbutt

  aanbutt = gtk_button_new_with_mnemonic ("_Nastepna runda");
  gtk_box_pack_start (GTK_BOX (hbox), aanbutt, TRUE, TRUE, 0);

  aalist = g_list_append (aalist, aanbutt);

  g_signal_connect (aanbutt, "clicked", G_CALLBACK (wczytaj_ruch), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (przesun_pionki), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_wynik), wlabel);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (rozegraj_bitwe), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);

  g_signal_connect (aanbutt, "clicked", G_CALLBACK (zablokuj_nastepny), aanbutt);

  // ggvbox

  ggvbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (hbox), ggvbox);

  gglist = g_list_append (gglist, ggvbox);

  // gghbox

  gghbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (ggvbox), gghbox);

  gglist = g_list_append (gglist, gghbox);

  // ggrbutt

  ggrbutt = gtk_button_new();
  im = gtk_image_new_from_file("png/zw.png");
  gtk_container_add (GTK_CONTAINER (ggrbutt), im);
  gtk_box_pack_start (GTK_BOX (gghbox), ggrbutt, TRUE, TRUE, 0);
  g_signal_connect (ggrbutt, "clicked", G_CALLBACK (zmien_pionek31), (gpointer) "R");

  gglist = g_list_append (gglist, ggrbutt);
  gglist = g_list_append (gglist, im);

  gtk_drag_source_set (ggrbutt, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY);
  g_signal_connect (ggrbutt, "drag-begin", G_CALLBACK (zmien_pionek1), (gpointer) "R");
  g_signal_connect (ggrbutt, "drag-end", G_CALLBACK (czysc_ruch1), NULL);

  // ggsbutt

  ggsbutt = gtk_button_new();
  im = gtk_image_new_from_file("png/zd.png");
  gtk_container_add (GTK_CONTAINER (ggsbutt), im);
  gtk_box_pack_start (GTK_BOX (gghbox), ggsbutt, TRUE, TRUE, 0);
  g_signal_connect (ggsbutt, "clicked", G_CALLBACK (zmien_pionek31), (gpointer) "S");

  gglist = g_list_append (gglist, ggsbutt);
  gglist = g_list_append (gglist, im);

  gtk_drag_source_set (ggsbutt, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY);
  g_signal_connect (ggsbutt, "drag-begin", G_CALLBACK (zmien_pionek1), (gpointer) "S");
  g_signal_connect (ggsbutt, "drag-end", G_CALLBACK (czysc_ruch1), NULL);

  // ggzbutt

  ggzbutt = gtk_button_new_with_mnemonic("_Zatrzymanie");
  gtk_widget_set_sensitive (ggzbutt, FALSE);
  gtk_box_pack_start (GTK_BOX (gghbox), ggzbutt, TRUE, TRUE, 0);
  g_signal_connect (ggzbutt, "clicked", G_CALLBACK (zmien_akcje), (gpointer) "Z");

  gglist = g_list_append (gglist, ggzbutt);

  // gghbox2

  gghbox2 = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (ggvbox), gghbox2);

  gglist = g_list_append (gglist, gghbox2);

  // ggrbutt2

  ggrbutt2 = gtk_button_new();
  im = gtk_image_new_from_file("png/ww.png");
  gtk_container_add (GTK_CONTAINER (ggrbutt2), im);
  gtk_box_pack_start (GTK_BOX (gghbox2), ggrbutt2, TRUE, TRUE, 0);
  g_signal_connect (ggrbutt2, "clicked", G_CALLBACK (zmien_pionek32), (gpointer) "R");

  gglist = g_list_append (gglist, ggrbutt2);
  gglist = g_list_append (gglist, im);

  gtk_drag_source_set (ggrbutt2, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY);
  g_signal_connect (ggrbutt2, "drag-begin", G_CALLBACK (zmien_pionek2), (gpointer) "R");
  g_signal_connect (ggrbutt2, "drag-end", G_CALLBACK (czysc_ruch2), NULL);

  // ggsbutt2

  ggsbutt2 = gtk_button_new();
  im = gtk_image_new_from_file("png/wd.png");
  gtk_container_add (GTK_CONTAINER (ggsbutt2), im);
  gtk_box_pack_start (GTK_BOX (gghbox2), ggsbutt2, TRUE, TRUE, 0);
  g_signal_connect (ggsbutt2, "clicked", G_CALLBACK (zmien_pionek32), (gpointer) "S");

  gglist = g_list_append (gglist, ggsbutt2);
  gglist = g_list_append (gglist, im);

  gtk_drag_source_set (ggsbutt2, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY);
  g_signal_connect (ggsbutt2, "drag-begin", G_CALLBACK (zmien_pionek2), (gpointer) "S");
  g_signal_connect (ggsbutt2, "drag-end", G_CALLBACK (czysc_ruch2), NULL);

  // ggzbutt2

  ggzbutt2 = gtk_button_new_with_mnemonic("_Zatrzymanie");
  gtk_widget_set_sensitive (ggzbutt2, FALSE);
  gtk_box_pack_start (GTK_BOX (gghbox2), ggzbutt2, TRUE, TRUE, 0);
  g_signal_connect (ggzbutt2, "clicked", G_CALLBACK (zmien_akcje2), (gpointer) "Z");

  gglist = g_list_append (gglist, ggzbutt2);

  // separator

  separator = gtk_vseparator_new ();
  gtk_box_pack_start (GTK_BOX (hbox), separator, TRUE, TRUE, 0);

  gglist = g_list_append (gglist, separator);

  // ggnbutt

  ggnbutt = gtk_button_new_with_mnemonic ("_Nastepna runda");
  gtk_box_pack_start (GTK_BOX (hbox), ggnbutt, TRUE, TRUE, 0);

  gglist = g_list_append (gglist, ggnbutt);

  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (wczytaj_ruch), NULL);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (przesun_pionki), NULL);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktualizuj_wynik), wlabel);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (rozegraj_bitwe), NULL);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);

  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktywuj_produkcje), ggrbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktywuj_produkcje2), ggrbutt2);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktywuj_produkcje), ggsbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktywuj_produkcje2), ggsbutt2);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktywuj_zatrzymanie), ggzbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (aktywuj_zatrzymanie2), ggzbutt2);

  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggnbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggrbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggrbutt2);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggsbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggsbutt2);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggzbutt);
  g_signal_connect (ggnbutt, "clicked", G_CALLBACK (zablokuj_nastepny), ggzbutt2);

  // galist

  g_signal_connect (G_OBJECT (gaitem), "activate", G_CALLBACK (nowa_gra), (gpointer) galist);
  g_signal_connect (G_OBJECT (gaitem), "activate", G_CALLBACK (nowa_gra), (gpointer) aalist);
  g_signal_connect (G_OBJECT (gaitem), "activate", G_CALLBACK (nowa_gra), (gpointer) gglist);
  g_signal_connect (G_OBJECT (gaitem), "activate", G_CALLBACK (nowa_gra_ga), (gpointer) galist);
  g_signal_connect (G_OBJECT (gaitem), "activate", G_CALLBACK (zeruj_wynik), wlabel);

  // aalist

  g_signal_connect (G_OBJECT (aaitem), "activate", G_CALLBACK (nowa_gra), (gpointer) galist);
  g_signal_connect (G_OBJECT (aaitem), "activate", G_CALLBACK (nowa_gra), (gpointer) aalist);
  g_signal_connect (G_OBJECT (aaitem), "activate", G_CALLBACK (nowa_gra), (gpointer) gglist);
  g_signal_connect (G_OBJECT (aaitem), "activate", G_CALLBACK (nowa_gra_aa), (gpointer) aalist);
  g_signal_connect (G_OBJECT (aaitem), "activate", G_CALLBACK (zeruj_wynik), wlabel);

  // gglist

  g_signal_connect (G_OBJECT (ggitem), "activate", G_CALLBACK (nowa_gra), (gpointer) aalist);
  g_signal_connect (G_OBJECT (ggitem), "activate", G_CALLBACK (nowa_gra), (gpointer) galist);
  g_signal_connect (G_OBJECT (ggitem), "activate", G_CALLBACK (nowa_gra), (gpointer) gglist);
  g_signal_connect (G_OBJECT (ggitem), "activate", G_CALLBACK (nowa_gra_gg), (gpointer) gglist);
  g_signal_connect (G_OBJECT (ggitem), "activate", G_CALLBACK (zeruj_wynik), wlabel);

  gtk_main();

  return 0;
}
