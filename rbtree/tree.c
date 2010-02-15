#include "tree.h"

#include <stdlib.h>


color node_color(node n) {
  return n == NULL ? BLACK : n->color;
}


node grandparent(node n) {
  return n->parent->parent;
}

node sibling(node n) {
  if (n == n->parent->left)
    return n->parent->right;
  else
    return n->parent->left;
}

node uncle(node n) {
  return sibling(n->parent);
}


rbtree rbtree_create() {
  rbtree t = (rbtree)malloc(sizeof(struct rbtree_t));

  t->root = NULL;

  return t;
}

node new_node(int key, color col, node left, node right) {
  node n = (node)malloc(sizeof(struct rbtree_node_t));

  n->key = key;
  n->color = col;
  n->left = left;
  n->right = right;

  if (left  != NULL)
    left->parent = n;
  if (right != NULL)
    right->parent = n;

  n->parent = NULL;

  return n;
}


void repair(node n) {
  if (n->left != NULL) {
    n->left->x = n->x - n->r / 2;
    n->left->y = n->y + 30;
    n->left->r = n->r / 2;

    repair(n->left);
  }

  if (n->right != NULL) {
    n->right->x = n->x + n->r / 2;
    n->right->y = n->y + 30;
    n->right->r = n->r / 2;

    repair(n->right);
  }
}

void replace_node(rbtree t, node oldn, node newn) {
  if (oldn->parent == NULL) {
    t->root = newn;
  }
  else {
    if (oldn == oldn->parent->left)
      oldn->parent->left = newn;
    else
      oldn->parent->right = newn;
  }

  if (newn != NULL) {
    newn->x = oldn->x;
    newn->y = oldn->y;
    newn->r = oldn->r;

    newn->parent = oldn->parent;
  }
}


void insert_case5(GtkWidget *darea, rbtree t, node n) {
  n->parent->color = BLACK;
  grandparent(n)->color = RED;

  if (n == n->parent->left && n->parent == grandparent(n)->left)
    rotate_right(darea, t, grandparent(n));
  else
    rotate_left(darea, t, grandparent(n));
}

void insert_case4(GtkWidget *darea, rbtree t, node n) {
  if (n == n->parent->right && n->parent == grandparent(n)->left) {
    rotate_left(darea, t, n->parent);

    n = n->left;
  }
  else if (n == n->parent->left && n->parent == grandparent(n)->right) {
    rotate_right(darea, t, n->parent);

    n = n->right;
  }

  insert_case5(darea, t, n);
}

void insert_case3(GtkWidget *darea, rbtree t, node n) {
  if (node_color(uncle(n)) == RED) {
    n->parent->color = BLACK;
    uncle(n)->color = BLACK;
    grandparent(n)->color = RED;

    insert_case1(darea, t, grandparent(n));
  }
  else {
    insert_case4(darea, t, n);
  }
}

void insert_case2(GtkWidget *darea, rbtree t, node n) {
  if (node_color(n->parent) == BLACK)
    return;
  else
    insert_case3(darea, t, n);
}

void insert_case1(GtkWidget *darea, rbtree t, node n) {
  if (n->parent == NULL)
    n->color = BLACK;
  else
    insert_case2(darea, t, n);
}


void delete_case6(GtkWidget *darea, rbtree t, node n) {
  sibling(n)->color = node_color(n->parent);
  n->parent->color = BLACK;

  if (n == n->parent->left) {
    sibling(n)->right->color = BLACK;

    rotate_left(darea, t, n->parent);
  }
  else {
    sibling(n)->left->color = BLACK;

    rotate_right(darea, t, n->parent);
  }
}

void delete_case5(GtkWidget *darea, rbtree t, node n) {
  if (n == n->parent->left &&
      node_color(sibling(n)) == BLACK &&
      node_color(sibling(n)->left) == RED &&
      node_color(sibling(n)->right) == BLACK)
    {
      sibling(n)->color = RED;
      sibling(n)->left->color = BLACK;

      rotate_right(darea, t, sibling(n));
    }
  else if (n == n->parent->right &&
           node_color(sibling(n)) == BLACK &&
           node_color(sibling(n)->right) == RED &&
           node_color(sibling(n)->left) == BLACK)
    {
      sibling(n)->color = RED;
      sibling(n)->right->color = BLACK;

      rotate_left(darea, t, sibling(n));
    }

  delete_case6(darea, t, n);
}

void delete_case4(GtkWidget *darea, rbtree t, node n) {
  if (node_color(n->parent) == RED &&
      node_color(sibling(n)) == BLACK &&
      node_color(sibling(n)->left) == BLACK &&
      node_color(sibling(n)->right) == BLACK)
    {
      sibling(n)->color = RED;
      n->parent->color = BLACK;
    }
  else
    delete_case5(darea, t, n);
}

void delete_case3(GtkWidget *darea, rbtree t, node n) {
  if (node_color(n->parent) == BLACK &&
      node_color(sibling(n)) == BLACK &&
      node_color(sibling(n)->left) == BLACK &&
      node_color(sibling(n)->right) == BLACK)
    {
      sibling(n)->color = RED;
      delete_case1(darea, t, n->parent);
    }
  else
    delete_case4(darea, t, n);
}

void delete_case2(GtkWidget *darea, rbtree t, node n) {
  if (node_color(sibling(n)) == RED) {
    n->parent->color = RED;
    sibling(n)->color = BLACK;

    if (n == n->parent->left)
      rotate_left(darea, t, n->parent);
    else
      rotate_right(darea, t, n->parent);
  }

  delete_case3(darea, t, n);
}

void delete_case1(GtkWidget *darea, rbtree t, node n) {
  if (n->parent == NULL)
    return;
  else
    delete_case2(darea, t, n);
}


node maximum_node(node n) {
  while (n->right != NULL)
    n = n->right;

  return n;
}

void rbtree_delete(GtkWidget *darea, rbtree t, int key) {
  node n = lookup_node(darea, t, key);
  node child;

  if (n == NULL) return;

  if (n->left != NULL && n->right != NULL) {
    node pred = maximum_node(n->left);

    n->key   = pred->key;
    n = pred;
  }

  child = n->right == NULL ? n->left : n->right;
  if (node_color(n) == BLACK) {
    n->color = node_color(child);

    delete_case1(darea, t, n);
  }

  replace_node(t, n, child);

  if (n->parent == NULL && child != NULL)
    child->color = BLACK;

  free(n);
}


int compare(int a, int b) {
  if (a < b)
    return -1;
  else if (a > b)
    return 1;
  else
    return 0;
}
