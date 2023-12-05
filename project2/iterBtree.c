/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) { *tree = NULL; }

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) {
    return false;
  }
  while (tree != NULL) {
    if (tree->key == key) {
      *value = tree->value;
      return true;

    } else if (tree->key > key) {
      tree = tree->left;
    } else {
      tree = tree->right;
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // initialize new node
  bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
  new_node->key = key;
  new_node->value = value;
  new_node->right = NULL;
  new_node->left = NULL;

  if (*tree == NULL) {
    *tree = new_node;
    return;
  }
  bst_node_t *current_node = *tree;
  while (1) {
    if (key == current_node->key) { // replace
      current_node->value = value;
      free(new_node);
      return;
    } else if (key < current_node->key) { // left
      if (current_node->left == NULL) {
        current_node->left = new_node;
        return;
      } else {
        current_node = current_node->left;
      }
    } else { // right
      if (current_node->right == NULL) {
        current_node->right = new_node;
        return;
      } else {
        current_node = current_node->right;
      }
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
//
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (*tree == NULL) {
    return;
  }

  bst_node_t *current_node = *tree;
  bst_node_t *parent_node = NULL;
  while (current_node->right != NULL) {
    parent_node = current_node;
    current_node = current_node->right;
  }

  target->key = current_node->key;
  target->value = current_node->value;

  if (parent_node == NULL) { // root
    *tree = current_node->left;
  } else { // not root
    parent_node->right = current_node->left;
  }
  free(current_node);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *current_node = *tree;
  bst_node_t *parent_node = NULL;
  while (current_node != NULL) {
    if (current_node->key == key) {
      if (current_node->left == NULL && current_node->right == NULL) { // leaf
        if (parent_node == NULL) {                                     // root
          *tree = NULL;                                                // set tree to null
        } else if (parent_node->left == current_node) {                // left child
          parent_node->left = NULL;                                    // set left child to null
        } else {
          parent_node->right = NULL; // set right child to null
        }
        free(current_node);
        return;
        // only right child
      } else if (current_node->left == NULL) {
        if (parent_node == NULL) {     // root
          *tree = current_node->right; // set tree to right child
        } else if (parent_node->left == current_node) {
          parent_node->left = current_node->right;
        } else {
          parent_node->right = current_node->right;
        }
        free(current_node);
        return;
        // only left child
      } else if (current_node->right == NULL) {
        if (parent_node == NULL) {
          *tree = current_node->left;
        } else if (parent_node->left == current_node) {
          parent_node->left = current_node->left;
        } else {
          parent_node->right = current_node->left;
        }
        free(current_node);
        return;
      } else {
        bst_replace_by_rightmost(current_node, &current_node->left);
      }
    } else if (key < current_node->key) {
      parent_node = current_node;
      current_node = current_node->left;
    } else {
      parent_node = current_node;
      current_node = current_node->right;
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {

  if (tree == NULL || *tree == NULL) {
    return;
  }

  int top = -1;
  bst_node_t *current = *tree;
  bst_node_t *previous = NULL;
  bst_node_t *stack[MAXSTACK];
  while (current != NULL || top != -1) {
    while (current != NULL) {
      stack[++top] = current;
      current = current->left; // go to left
    }
    current = stack[top];
    if (current->right != NULL && current->right != previous) { // right child exists and was not visited
      current = current->right;                                 // go to right
    } else {
      top--;
      previous = current;
      free(current);
      current = NULL;
    }
  }
  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while (tree != NULL || !stack_bst_empty(to_visit)) { // while tree is not null or stack is not empty
    if (tree != NULL) {
      bst_add_node_to_items(tree, items); // add node to items
      if (tree->right != NULL) {
        stack_bst_push(to_visit, tree->right);
      }
      tree = tree->left;
    } else {
      tree = stack_bst_pop(to_visit);
    }
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack, items);

  while (!stack_bst_empty(&stack)) {
    bst_node_t *current_node = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(current_node->right, &stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);

  while (!stack_bst_empty(&stack)) {
    bst_node_t *current_node = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_add_node_to_items(current_node, items);
    bst_leftmost_inorder(current_node->right, &stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_node_t *current = tree;
  bst_node_t *last_node = NULL;

  while (!stack_bst_empty(&stack) || current != NULL) {
    if (current != NULL) {
      stack_bst_push(&stack, current); // push current node
      current = current->left;
    } else {
      bst_node_t *child_node = stack_bst_top(&stack);                    // get top node
      if (child_node->right != NULL && child_node->right != last_node) { // right child exists and was not visited
        current = child_node->right;                                     // go to right
      } else {
        // right child does not exist or was visited
        bst_node_t *node = stack_bst_pop(&stack);
        bst_add_node_to_items(node, items);
        last_node = node;
      }
    }
  }
}
