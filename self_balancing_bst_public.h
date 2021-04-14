#ifndef __SELF_BALANCING_BIN_TREE_PRIVATE_COMMON_H__
#define __SELF_BALANCING_BIN_TREE_PRIVATE_COMMON_H__

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ENUMS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//Choose the self balancing bin tree
enum iada_self_balancing_bin_tree {
    IADA_TREE_AVL = 0,	//Lookup more frequent than Insert/Delete
    IADA_TREE_RED_BLACK, //Insert/Delete are more frequent than lookup
    IADA_TREE_SPLAY_TREE //Fast access to recently accessed items, used when N is huge but small subset is repeatedly searched
};

#endif
