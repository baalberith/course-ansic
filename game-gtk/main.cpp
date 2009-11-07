#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>

#include <cairo.h>
#include <gtk/gtk.h>

#include <cassert>

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
    static const int czas_tworzenia = 0,
      dlugosc_kroku = 2,
      dlugosc_zycia = 10;
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
    int max_zycie() { return dlugosc_zycia; }
};

class Smok : public Pionek {
  private:
    static const int czas_tworzenia = 1,
      dlugosc_kroku = 1,
      dlugosc_zycia = 20;
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
            nowy_pionek = new Rycerz(Pozycja(0, pozycja_y));
          else
            nowy_pionek = new Smok(Pozycja(0, pozycja_y));
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
            nowy_pionek = new Rycerz(Pozycja(0, pozycja));
          else
            nowy_pionek = new Smok(Pozycja(0, pozycja));
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
          (*it)->przesun(nowa_pozycja.x - pozycja.x, rozmiar_planszy);
          plansza_gracza.dodaj_na_plansze(*it);
          it++;
        }
      }
      return punkty;
    }
};

class Wrog : public Gracz {
  public:
    Wrog(int _r, Mode _m) : Gracz(_r, _m) {}

    void wczytaj_ruch(Akcja*) {
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
            nowy_pionek = new Rycerz(Pozycja(rozmiar_planszy + 1, pozycja));
          else
            nowy_pionek = new Smok(Pozycja(rozmiar_planszy + 1, pozycja));
          czas_blokady = nowy_pionek->czas();
        }
        if (czas_blokady == 0 && nowy_pionek) {
          dodaj_pionek(nowy_pionek);
          nowy_pionek = 0;
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
          (*it)->przesun(nowa_pozycja.x - pozycja.x, rozmiar_planszy);
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

const int rozmiar_planszy = 12,
  ilosc_rund = 20;
Gracz *zenek,
  *wrog;
Sedzia *sedzia = new Sedzia(rozmiar_planszy);

const int rozmiar_pola = 50,
  rozmiar_x = (rozmiar_planszy + 2) * rozmiar_pola,
  rozmiar_y = rozmiar_planszy * rozmiar_pola;

char rodzaj_akcji,
  znak_pionka;
int pozycja;

bool koniec_gry() {
  return sedzia->koniec_gry(zenek, wrog);
}

void wyswietl_widget(void *widget, void* data) {
  cout << "wyswietl widget " << endl;

  gtk_widget_show ((GtkWidget*) widget);
}

void nowa_gra_ga(GtkWidget *widget, GList *list) {
  cout << "nowa gra ga " << endl;

  zenek = new Zenek(rozmiar_planszy, GRACZ);
  wrog = new Wrog(rozmiar_planszy, AI);

  g_list_foreach (list, wyswietl_widget, NULL);
  g_list_free (list);
}

void nowa_gra_aa(GtkWidget *widget, GList *list) {
  cout << "nowa gra aa " << endl;

  zenek = new Zenek(rozmiar_planszy, AI);
  wrog = new Wrog(rozmiar_planszy, AI);

  g_list_foreach (list, wyswietl_widget, NULL);
  g_list_free (list);
}

void wyswietl_plansze(GtkWidget *widget, GdkEventExpose *event, gpointer data) {
  cout << "wyswietl plansze" << endl;

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
    for (int j = 0; j < rozmiar_planszy + 2; j++) {
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

  for (int i = 0; i < rozmiar_planszy + 3; i++) {
    cairo_move_to (cr, i * rozmiar_pola, 0);
    cairo_line_to (cr, i * rozmiar_pola, rozmiar_planszy * rozmiar_pola);
    cairo_move_to (cr, 0, i * rozmiar_pola);
    cairo_line_to (cr, (rozmiar_planszy + 2) * rozmiar_pola, i  * rozmiar_pola);
  }

  cairo_stroke (cr);

  cairo_destroy (cr);
  cairo_destroy (cr2);
}

void aktualizuj_plansze(GtkWidget *widget, gpointer data) {
  cout << "aktualizuj plansze " << endl;

  GdkRegion *region = gdk_drawable_get_clip_region (widget->window);

  gdk_window_invalidate_region (widget->window, region, TRUE);
  gdk_window_process_all_updates();

  usleep(200000);
}

void zmien_akcje(GtkWidget *widget, const char *rodzaj) {
  cout << "zmien akcje " << endl;

  rodzaj_akcji = *rodzaj;
}

void zmien_pionek(GtkWidget *widget, gpointer data) {
  cout << "zmien pionek " << endl;

  znak_pionka = *gtk_combo_box_get_active_text (GTK_COMBO_BOX (widget));
}

void zmien_pozycje(GtkWidget *widget, gpointer data) {
  cout << "zmien pozycje " << endl;

  pozycja = atoi(gtk_combo_box_get_active_text (GTK_COMBO_BOX (widget)));
}

void wczytaj_ruch(GtkWidget *widget, gpointer data) {
  cout << "wczytaj ruch " << endl;
  cout << "rodzaj akcji: " << rodzaj_akcji << " znak pionka: " << znak_pionka << " pozycja: " << pozycja << endl;

  Akcja *akcja = new Akcja(rodzaj_akcji, znak_pionka, pozycja);

  zenek->wczytaj_ruch(akcja);
  wrog->wczytaj_ruch(akcja);

  rodzaj_akcji = 'X';
}

void przesun_pionki(GtkWidget *widget, gpointer data) {
  //cout << "przesun pionki " << endl;

  zenek->przesun_pionki(wrog->plansza());
  wrog->przesun_pionki(zenek->plansza());
}

void rozegraj_bitwe(GtkWidget *widget, gpointer data) {
  //cout << "pozegraj bitwe " << endl;

  sedzia->rozegraj_bitwe(zenek->plansza(), wrog->plansza());
}

void aktualizuj_wynik(GtkWidget *widget, GtkLabel *label) {
  //cout << "aktualizuj wynik " << endl;

  char *str = (char*) malloc(6 * sizeof(char));
  sprintf(str, "%d : %d", zenek->wynik(), wrog->wynik());
  gtk_label_set_text (label, (gchar*) str);
}

void aktywuj_produkcje(GtkWidget *widget, GtkWidget *button) {
  //cout << "aktywuj produkcje" << endl;

  bool blokada = zenek->blokada();
  gtk_widget_set_sensitive (button, !blokada);
}

void aktywuj_zatrzymanie(GtkWidget *widget, GtkWidget *button) {
  //cout << "aktywuj zatrzymanie " << endl;

  bool blokada = zenek->blokada();
  gtk_widget_set_sensitive (button, blokada);
}

void aktywuj_box(GtkWidget *widget, GtkWidget *box) {
  //cout << "aktywuj box " << endl;

  gtk_widget_set_sensitive (box, TRUE);
}

void deaktywuj_box(GtkWidget *widget, GtkWidget *box) {
  //cout << "deaktywuj box " << endl;

  gtk_widget_set_sensitive (box, FALSE);
}



int main(int argc, char *argv[]) {
  srand(time(0));

  GtkWidget *window, *vbox, *darea, *hbox, *wbox, *label, *wlabel, *separator, *pbutt, *rsbox, *pbox, *zbutt, *nbutt,
    *mbar, *oitem, *menu, *gaitem, *aaitem, *ggitem, *zitem, *rscombo, *pcombo, *rspbox, *bbutt, *aanbutt;

  GList *galist = NULL, *aalist = NULL;

  gtk_init (&argc, &argv);

  // window

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Krwawa gra");
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

  zitem = gtk_menu_item_new_with_label ("Zakoncz gre");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), zitem);
  g_signal_connect (G_OBJECT (zitem), "activate", G_CALLBACK (gtk_main_quit), NULL);
  gtk_widget_show (zitem);

  // darea

  darea = gtk_drawing_area_new();
  gtk_drawing_area_size (GTK_DRAWING_AREA (darea), rozmiar_x, rozmiar_y);
  gtk_box_pack_start (GTK_BOX (vbox), darea, FALSE, FALSE, 5);
  g_signal_connect (darea, "expose-event", G_CALLBACK (wyswietl_plansze), NULL);

  galist = g_list_append (galist, darea);
  aalist = g_list_append (aalist, darea);

  // hbox

  hbox = gtk_hbox_new (FALSE, 3);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  galist = g_list_append (galist, hbox);
  aalist = g_list_append (aalist, hbox);

  // wbox

  wbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), wbox, TRUE, TRUE, 0);

  label = gtk_label_new ("Wynik");
  gtk_box_pack_start (GTK_BOX (wbox), label, TRUE, TRUE, 0);
  wlabel = gtk_label_new ("0:0");
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

  label = gtk_label_new ("Pionek");
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

  bbutt = gtk_button_new_with_mnemonic("_Brak");
  gtk_box_pack_start (GTK_BOX (hbox), bbutt, TRUE, TRUE, 0);
  g_signal_connect (bbutt, "clicked", G_CALLBACK (zmien_akcje), (gpointer) "X");

  separator = gtk_vseparator_new ();
  gtk_box_pack_start (GTK_BOX (hbox), separator, TRUE, TRUE, 0);

  galist = g_list_append (galist, zbutt);
  galist = g_list_append (galist, bbutt);
  galist = g_list_append (galist, separator);

  // nbutt

  nbutt = gtk_button_new_with_mnemonic ("_Nastepna runda");
  gtk_widget_set_sensitive (nbutt, FALSE);
  gtk_box_pack_start (GTK_BOX (hbox), nbutt, TRUE, TRUE, 0);

  g_signal_connect (pcombo, "changed", G_CALLBACK (aktywuj_box), nbutt);
  g_signal_connect (zbutt, "clicked", G_CALLBACK (aktywuj_box), nbutt);
  g_signal_connect (bbutt, "clicked", G_CALLBACK (aktywuj_box), nbutt);
  g_signal_connect (nbutt, "clicked", G_CALLBACK (deaktywuj_box), nbutt);

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

  galist = g_list_append (galist, nbutt);
  g_signal_connect (G_OBJECT (gaitem), "activate", G_CALLBACK (nowa_gra_ga), (gpointer) galist);

  // aanbutt

  aanbutt = gtk_button_new_with_mnemonic ("_Nastepna runda");
  gtk_box_pack_start (GTK_BOX (hbox), aanbutt, TRUE, TRUE, 0);

  g_signal_connect (aanbutt, "clicked", G_CALLBACK (wczytaj_ruch), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (przesun_pionki), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_wynik), wlabel);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (rozegraj_bitwe), NULL);
  g_signal_connect (aanbutt, "clicked", G_CALLBACK (aktualizuj_plansze), NULL);

  aalist = g_list_append (aalist, aanbutt);
  g_signal_connect (G_OBJECT (aaitem), "activate", G_CALLBACK (nowa_gra_aa), (gpointer) aalist);

  //gtk_widget_show_all (window);

  gtk_main();

  return 0;
}
