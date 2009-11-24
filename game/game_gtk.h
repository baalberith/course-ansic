#ifndef _game_gtk_h_
#define _game_gtk_h_

#include "game_txt.h"

#include <cairo.h>
#include <gtk/gtk.h>

extern const int rozmiar_planszy,
  ilosc_rund;
extern Gracz *zenek,
  *wrog;
extern Sedzia *sedzia;

extern const int rozmiar_pola,
  rozmiar_x,
  rozmiar_y;

extern char rodzaj_akcji, rodzaj_akcji2,
  znak_pionka, znak_pionka2;
extern int pozycja, pozycja2;
extern bool kto;

enum { TARGET_ROOTWIN };

extern GtkTargetEntry target_list[];
extern guint n_targets;

void wyswietl_widget(void *widget, void* data);
void schowaj_widget(void *widget, void* data);
void nowa_gra(GtkWidget *widget, GList *list);
void nowa_gra_ga(GtkWidget *widget, GList *list);
void nowa_gra_aa(GtkWidget *widget, GList *list);
void nowa_gra_gg(GtkWidget *widget, GList *list);
void wyswietl_plansze(GtkWidget *widget, GdkEventExpose *event, gpointer data);
void aktualizuj_plansze(GtkWidget *widget, gpointer data);
void wczytaj_ruch(GtkWidget *widget, gpointer data);
void przesun_pionki(GtkWidget *widget, gpointer data);
void rozegraj_bitwe(GtkWidget *widget, gpointer data);
void aktualizuj_wynik(GtkWidget *widget, GtkLabel *label);
void zeruj_wynik(GtkWidget *widget, GtkLabel *label);
void zmien_akcje(GtkWidget *widget, const char *rodzaj);
void zmien_akcje2(GtkWidget *widget, const char *rodzaj);
void zmien_pionek(GtkWidget *widget, gpointer data);
void zmien_pozycje(GtkWidget *widget, gpointer data);
void zmien_pionek1(GtkWidget *widget, GdkDragContext *context, const char* rodzaj);
void zmien_pionek2(GtkWidget *widget, GdkDragContext *context, const char* rodzaj);
void zmien_pozycje12(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data);
void czysc_ruch1(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
void czysc_ruch2(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
void zmien_pionek31(GtkWidget *widget, const char* rodzaj);
void zmien_pionek32(GtkWidget *widget, const char* rodzaj);
void zmien_pozycje3(GtkWidget *widget, GdkEventMotion *event);
void aktywuj_produkcje(GtkWidget *widget, GtkWidget *button);
void aktywuj_zatrzymanie(GtkWidget *widget, GtkWidget *button);
void aktywuj_produkcje2(GtkWidget *widget, GtkWidget *button);
void aktywuj_zatrzymanie2(GtkWidget *widget, GtkWidget *button);
void zablokuj_nastepny(GtkWidget *widget, GtkWidget *button);
void aktywuj_box(GtkWidget *widget, GtkWidget *box);
void deaktywuj_box(GtkWidget *widget, GtkWidget *box);

#endif
