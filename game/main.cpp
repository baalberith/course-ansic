#include "game_txt.h"
#include "game_gtk.h"

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
