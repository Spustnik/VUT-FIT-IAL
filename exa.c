/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu)
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 *
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v
 * uložen ve stromu.
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
 */
void letter_count(bst_node_t **tree, char *input) {
  bst_init(tree);
  while (*input != '\0') { // iterate through input
    char currentChar = *input;
    if ((currentChar >= 'A' && currentChar <= 'Z')) {
      currentChar += 32; // convert to lowercase
    }

    if ((currentChar >= 'a' && currentChar <= 'z')) {
      int value;
      if (bst_search(*tree, currentChar, &value)) {
        bst_insert(tree, currentChar, value + 1);
      } else {
        bst_insert(tree, currentChar, 1);
      }
    } else if (currentChar == ' ') { // space
      int value;
      if (bst_search(*tree, currentChar, &value)) {
        bst_insert(tree, currentChar, value + 1);
      } else {
        bst_insert(tree, currentChar, 1);
      }
    } else { // other
      int value;
      if (bst_search(*tree, '_', &value)) {
        bst_insert(tree, '_', value + 1);
      } else {
        bst_insert(tree, '_', 1);
      }
    }
    input++; // next char
  }
}

/**
 * Vyvážení stromu.
 *
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 *
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
 */
void bst_items_init(bst_items_t *items) {
  // inicialize struct
  items->capacity = 0;
  items->nodes = NULL;
  items->size = 0;
}

bst_node_t *help_balance_bst(bst_node_t **nodes, int start, int end) {
  if (start > end) { // no nodes
    return NULL;
  }
  // get middle node
  int mid = (start + end) / 2;
  bst_node_t *root = nodes[mid];
  // build left and right subtree
  root->left = help_balance_bst(nodes, start, mid - 1);
  root->right = help_balance_bst(nodes, mid + 1, end);
  return root;
}

void bst_balance(bst_node_t **tree) {
  if (*tree == NULL) {
    return;
  }

  bst_items_t items;
  bst_items_init(&items);
  bst_inorder(*tree, &items);

  *tree = help_balance_bst(items.nodes, 0, items.size - 1); // build balanced tree
  free(items.nodes);                                        // free memory
}
