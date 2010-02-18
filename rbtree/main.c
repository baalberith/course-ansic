#include "tree.h"

#include <cairo.h>
#include <gtk/gtk.h>

#include <stdlib.h>
#include <math.h>


#define WIDTH 1000
#define HEIGHT 225


rbtree tree;
int randnum;
int path = 0, rotation = 0;


void print_circle(GtkWidget *widget, const char* s, color c, int x, int y, int d, int h) {
  cairo_t *cr = gdk_cairo_create(widget->window);

  cairo_set_line_width(cr, 2);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_arc(cr, x, y, d, 0, 2 * M_PI);
  cairo_stroke_preserve(cr);

  if (c == RED)
    cairo_set_source_rgb(cr, 1, 0, 0);
  else
    cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_fill(cr);

  int d2 = 5, d1 = strlen(s) < 2 ? 4 : 9;

  cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
  cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, h);
  cairo_move_to(cr, x - d1, y + d2);
  cairo_show_text(cr, s);

  cairo_destroy(cr);
}

void print_left_arrow(GtkWidget *widget, int x, int y, int d) {
  cairo_t *cr = gdk_cairo_create(widget->window);

  cairo_set_line_width(cr, 2);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_arc(cr, x, y, d, M_PI + 0.6, 2 * M_PI - 0.6);
  cairo_stroke(cr);

  cairo_move_to (cr, x - 16, y - 12);
  cairo_line_to (cr, x - 14, y - 19);
  cairo_move_to (cr, x - 16, y - 12);
  cairo_line_to (cr, x - 10, y - 14);
  cairo_stroke (cr);

  cairo_destroy(cr);
}

void print_right_arrow(GtkWidget *widget, int x, int y, int d) {
  cairo_t *cr = gdk_cairo_create(widget->window);

  cairo_set_line_width(cr, 2);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_arc(cr, x, y, d, M_PI + 0.6, 2 * M_PI - 0.6);
  cairo_stroke(cr);

  cairo_move_to (cr, x + 16, y - 12);
  cairo_line_to (cr, x + 14, y - 19);
  cairo_move_to (cr, x + 16, y - 12);
  cairo_line_to (cr, x + 10, y - 14);
  cairo_stroke (cr);

  cairo_destroy(cr);
}

void print_lines(GtkWidget *widget, int left, int right, int x, int y, int r) {
  cairo_t *cr = gdk_cairo_create(widget->window);

  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to (cr, x, y);
  cairo_line_to (cr, x - (r / 2), y + 30);
  cairo_move_to (cr, x, y);
  cairo_line_to (cr, x + (r / 2), y + 30);
  cairo_stroke (cr);

  if (!left) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, x - (r / 2) - 2, y + 28, 10, 5);
    cairo_set_line_width(cr, 10);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 8);
    cairo_move_to(cr, x - (r / 2) - 4, y + 33);
    cairo_show_text(cr, "NIL");
  }

  if (!right) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, x + (r / 2) - 4, y + 28, 10, 5);
    cairo_set_line_width(cr, 10);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 8);
    cairo_move_to(cr, x + (r / 2) - 6, y + 33);
    cairo_show_text(cr, "NIL");
  }

  cairo_destroy(cr);
}


void print_tree(GtkWidget *widget, rbtree_node node) {
  if (node != NULL) {
    char *s = (char*) malloc(3 * sizeof(char));
    sprintf(s, "%d", node->key);

    print_lines(widget, node->left != NULL, node->right != NULL, node->x, node->y, node->r);

    print_tree(widget, node->left);
    print_circle(widget, s, node_color(node), node->x, node->y, 15, 16);
    print_tree(widget, node->right);
  }
}


void rotate_left(GtkWidget *darea, rbtree t, node n) {
  if (rotation)
    print_left_arrow(darea, n->right->x, n->right->y, 20);

  node R = n->right;
  replace_node(t, n, R);
  n->right = R->left;

  if (R->left != NULL)
    R->left->parent = n;

  R->left = n;
  n->parent = R;

  repair(R);
}

void rotate_right(GtkWidget *darea, rbtree t, node n) {
  if (rotation)
    print_right_arrow(darea, n->left->x, n->left->y, 20);

  node L = n->left;
  replace_node(t, n, L);
  n->left = L->right;

  if (L->right != NULL)
    L->right->parent = n;

  L->right = n;
  n->parent = L;

  repair(L);
}


node lookup_node(GtkWidget *darea, rbtree t, int key) {
  char *s = (char*) malloc(3 * sizeof(char));
  sprintf(s, "%d", key);

  node n = t->root;

  while (n != NULL) {
    if (path)
      print_circle(darea, s, RED, n->x - 20, n->y, 10, 12);

    int comp = compare(key, n->key);

    if (comp == 0)
      return n;
    else if (comp < 0)
      n = n->left;
    else
      n = n->right;
  }

  return n;
}


void rbtree_insert(GtkWidget *darea, rbtree t, int key) {
  int x = WIDTH / 2, y = 22, r = (WIDTH - 20) / 2;

  char *s = (char*) malloc(3 * sizeof(char));
  sprintf(s, "%d", key);

  node inserted_node = new_node(key, RED, NULL, NULL);

  if (t->root == NULL) {
    inserted_node->x = x;
    inserted_node->y = y;
    inserted_node->r = r;

    t->root = inserted_node;
  }
  else {
    node n = t->root;

    while (1) {
      int comp = compare(key, n->key);

      if (comp == 0) {
        if (path)
          print_circle(darea, s, RED, x - 20, y, 10, 12);

        free (inserted_node);

        return;
      }
      else if (comp < 0) {
        if (path)
          print_circle(darea, s, RED, x - 20, y, 10, 12);

        if (n->left == NULL) {
          inserted_node->x = x - r / 2;
          inserted_node->y = y + 30;
          inserted_node->r = r / 2;

          n->left = inserted_node;

          print_circle(darea, s, RED, inserted_node->x, inserted_node->y, 15, 16);

          break;
        }
        else {
          x -= r / 2;
          y += 30;
          r /= 2;

          n = n->left;
        }
      }
      else {
        if (path)
          print_circle(darea, s, RED, x - 20, y, 10, 12);

        if (n->right == NULL) {
          inserted_node->x = x + r / 2;
          inserted_node->y = y + 30;
          inserted_node->r = r / 2;

          n->right = inserted_node;

          print_circle(darea, s, RED, inserted_node->x, inserted_node->y, 15, 16);

          break;
        }
        else {
          x += r / 2;
          y += 30;
          r /= 2;

          n = n->right;
        }
      }
    }

    inserted_node->parent = n;
  }

  insert_case1(darea, t, inserted_node);
}


void on_expose_event(GtkWidget *widget) {
  print_tree(widget, tree->root);
}

void on_insert_event(GtkWidget *widget, GdkEventExpose *event, const char *s) {
  print_circle(widget, s, RED, 16, 16, 15, 16);
}

void on_delete_event(GtkWidget *widget, GdkEventExpose *event, const char *s) {
  print_circle(widget, s, BLACK, 16, 16, 15, 16);
}


void insert(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  int n = atoi(gtk_entry_get_text(widget));

  if (n > 0 && n < 100)
    rbtree_insert(darea, tree, n);
}

void insert_random(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  randnum = 1 + rand() % 99;

  rbtree_insert(darea, tree, randnum);
}


void ins(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  int s = gtk_entry_get_text(widget);
  int n = atoi(s);

  if (n > 0 && n < 100)
    print_circle(darea, s, RED, 16, 16, 15, 16);
  else
    print_circle(darea, "?", RED, 16, 16, 15, 16);
}

void ins_r(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  char *s = (char*) malloc(3 * sizeof(char));
  sprintf(s, "%d", randnum);

  print_circle(darea, s, RED, 16, 16, 15, 16);
}


void delete(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  int n = atoi(gtk_entry_get_text(widget));

  if (n > 0 && n < 100)
    rbtree_delete(darea, tree, n);
}

void delete_random(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  randnum = 1 + rand() % 99;

  rbtree_delete(darea, tree, randnum);
}

void del(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  int s = gtk_entry_get_text(widget);
  int n = atoi(s);

  if (n > 0 && n < 100)
    print_circle(darea, s, BLACK, 16, 16, 15, 16);
  else
    print_circle(darea, "?", BLACK, 16, 16, 15, 16);
}

void del_r(GtkWidget *widget, gpointer data) {
  GtkWidget *darea = (GtkWidget *) data;

  char *s = (char*) malloc(3 * sizeof(char));
  sprintf(s, "%d", randnum);

  print_circle(darea, s, BLACK, 16, 16, 15, 16);
}


static void clear(GtkWidget *widget, gpointer data) {
  tree = rbtree_create();
}


static void set_path(GtkWidget *widget) {
  if (gtk_toggle_button_get_active(widget))
    path = 1;
  else path = 0;
}

static void set_rotation(GtkWidget *widget) {
  if (gtk_toggle_button_get_active(widget))
    rotation = 1;
  else rotation = 0;
}


void actualize(GtkWidget *widget) {
  gtk_widget_queue_draw(widget);
}


int main (int argc, char *argv[]) {
  srand(time(0));

  GtkWidget *window, *vbox, *frame, *darea, *hbox, *frame1, *hbox1, *darea1, *entry1, *butt1,
    *frame2, *hbox2, *darea2, *entry2, *butt2, *butt3, *frame3, *hbox3, *check1, *check2;

  tree = rbtree_create();

  gtk_init(&argc, &argv);


  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW (window), "Drzewo czerono-czarne");
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (window), 5);


  vbox = gtk_vbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (window), vbox);


  frame = gtk_frame_new("Wizualizacja");

  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);

  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 5);


  darea = gtk_drawing_area_new();

  g_signal_connect (darea, "expose-event", G_CALLBACK (on_expose_event), NULL);

  gtk_drawing_area_size (GTK_DRAWING_AREA (darea), WIDTH, HEIGHT);

  gtk_container_add (GTK_CONTAINER (frame), darea);


  hbox = gtk_hbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (vbox), hbox);


  frame1 = gtk_frame_new("Wstawianie");

  gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_OUT);

  gtk_box_pack_start (GTK_BOX (hbox), frame1, FALSE, FALSE, 5);


  hbox1 = gtk_hbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (frame1), hbox1);


  darea1 = gtk_drawing_area_new();

  g_signal_connect (darea1, "expose-event", G_CALLBACK (on_insert_event), (gpointer) "?");

  gtk_drawing_area_size (GTK_DRAWING_AREA (darea1), 35, 35);

  gtk_box_pack_start (GTK_BOX (hbox1), darea1, FALSE, FALSE, 5);


  entry1 = gtk_entry_new();

  g_signal_connect(entry1, "activate", G_CALLBACK (insert), (gpointer) darea);
  g_signal_connect(entry1, "activate", G_CALLBACK (ins), (gpointer) darea1);

  gtk_entry_set_text(GTK_ENTRY (entry1), " Liczba od 1 do 99");
  gtk_entry_set_width_chars(GTK_ENTRY (entry1), 15);

  gtk_box_pack_start (GTK_BOX (hbox1), entry1, TRUE, TRUE, 0);


  butt1 = gtk_button_new_with_mnemonic(" _Wstaw losową ");

  g_signal_connect (butt1, "clicked", G_CALLBACK (insert_random), (gpointer) darea);
  g_signal_connect(butt1, "clicked", G_CALLBACK (ins_r), (gpointer) darea1);

  gtk_box_pack_start (GTK_BOX (hbox1), butt1, TRUE, TRUE, 0);


  frame2 = gtk_frame_new("Usuwanie");

  gtk_frame_set_shadow_type(GTK_FRAME(frame2), GTK_SHADOW_OUT);

  gtk_box_pack_start (GTK_BOX (hbox), frame2, FALSE, FALSE, 5);


  hbox2 = gtk_hbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (frame2), hbox2);


  darea2 = gtk_drawing_area_new();

  g_signal_connect (darea2, "expose-event", G_CALLBACK (on_delete_event), (gpointer) "?");

  gtk_drawing_area_size (GTK_DRAWING_AREA (darea2), 35, 35);

  gtk_box_pack_start (GTK_BOX (hbox2), darea2, FALSE, FALSE, 5);


  entry2 = gtk_entry_new();

  g_signal_connect(entry2, "activate", G_CALLBACK (delete), (gpointer) darea);
  g_signal_connect(entry2, "activate", G_CALLBACK (del), (gpointer) darea2);

  gtk_entry_set_text(GTK_ENTRY (entry2), " Liczba od 1 do 99");
  gtk_entry_set_width_chars(GTK_ENTRY (entry2), 15);

  gtk_box_pack_start (GTK_BOX (hbox2), entry2, TRUE, TRUE, 0);


  butt2 = gtk_button_new_with_mnemonic(" _Usuń losową ");

  g_signal_connect (butt2, "clicked", G_CALLBACK (delete_random), (gpointer) darea);
  g_signal_connect(butt2, "clicked", G_CALLBACK (del_r), (gpointer) darea2);

  gtk_box_pack_start (GTK_BOX (hbox2), butt2, TRUE, TRUE, 0);


  butt3 = gtk_button_new_with_label(" Usuń wszystkie ");

  g_signal_connect (butt3, "clicked", G_CALLBACK (clear), NULL);

  gtk_box_pack_start (GTK_BOX (hbox2), butt3, TRUE, TRUE, 0);


  frame3 = gtk_frame_new("Ustawienia");

  gtk_frame_set_shadow_type(GTK_FRAME(frame3), GTK_SHADOW_OUT);

  gtk_box_pack_start (GTK_BOX (hbox), frame3, FALSE, FALSE, 5);


  hbox3 = gtk_hbox_new (FALSE, 0);

  gtk_container_add (GTK_CONTAINER (frame3), hbox3);


  check1 = gtk_check_button_new_with_label("Pokazuj scieżkę");

  g_signal_connect (check1, "toggled", G_CALLBACK (set_path), NULL);

  gtk_box_pack_start (GTK_BOX (hbox3), check1, FALSE, FALSE, 5);


  check2 = gtk_check_button_new_with_label("Pokazuj rotacje");

  g_signal_connect (check2, "toggled", G_CALLBACK (set_rotation), NULL);

  gtk_box_pack_start (GTK_BOX (hbox3), check2, FALSE, FALSE, 5);


  gtk_widget_show_all(window);

  g_timeout_add(2000, (GSourceFunc) actualize, (gpointer) darea);

  gtk_main();

  return 0;
}
