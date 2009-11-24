#include "game_gtk.h"

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

GtkTargetEntry target_list[] = { { "application/x-rootwindow-drop", 0, TARGET_ROOTWIN } };
guint n_targets = G_N_ELEMENTS (target_list);

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

  //usleep(150000);
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
