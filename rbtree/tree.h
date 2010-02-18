#ifndef _tree_h_
#define _tree_h_

#include <gtk/gtk.h>


enum rbtree_node_color { RED, BLACK };
typedef enum rbtree_node_color color;


typedef struct rbtree_node_t {
    int key;
    struct rbtree_node_t *left, *right, *parent;
    enum rbtree_node_color color;
    int x, y, r;
} *rbtree_node;
typedef rbtree_node node;


typedef struct rbtree_t {
    rbtree_node root;
} *rbtree;


color node_color(node n);

node grandparent(node n);
node sibling(node n);
node uncle(node n);

rbtree rbtree_create();
node new_node(int key, color node_color, node left, node right);

void repair(node n);
void replace_node(rbtree t, node oldn, node newn);

void insert_case5(GtkWidget *darea, rbtree t, node n);
void insert_case4(GtkWidget *darea, rbtree t, node n);
void insert_case3(GtkWidget *darea, rbtree t, node n);
void insert_case2(GtkWidget *darea, rbtree t, node n);
void insert_case1(GtkWidget *darea, rbtree t, node n);

void delete_case6(GtkWidget *darea, rbtree t, node n);
void delete_case5(GtkWidget *darea, rbtree t, node n);
void delete_case4(GtkWidget *darea, rbtree t, node n);
void delete_case3(GtkWidget *darea, rbtree t, node n);
void delete_case2(GtkWidget *darea, rbtree t, node n);
void delete_case1(GtkWidget *darea, rbtree t, node n);

node maximum_node(node n);
void rbtree_delete(GtkWidget *darea, rbtree t, int key);

int compare(int a, int b);

#endif
