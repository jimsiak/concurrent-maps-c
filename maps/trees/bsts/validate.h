#ifndef _VALIDATE_H_
#define _VALIDATE_H_

#include "bst.h"

/**
 * The following definitions can be used:
 *   - BST_INTERNAL
 *   - BST_EXTERNAL
 **/
#if !defined(BST_INTERNAL) && !defined(BST_EXTERNAL)
#	error "Tree type should be defined"
#endif

static int key_in_max_path, key_in_min_path;
static int total_paths, total_nodes, bst_violations;
static int min_path_len, max_path_len;
static void _bst_validate_rec(bst_node_t *root, int _th)
{
	if (!root) return;

	bst_node_t *left = root->left;
	bst_node_t *right = root->right;

	total_nodes++;
	_th++;

	/* BST violation? */
#	if defined(BST_INTERNAL)
	if (left && left->key >= root->key)   bst_violations++;
#	elif defined(BST_EXTERNAL)
	if (left && left->key > root->key)    bst_violations++;
#	endif
	if (right && right->key <= root->key) bst_violations++;

	/* We found a path (a node with at least one NULL child). */
	if (!left || !right) {
		total_paths++;

		if (_th <= min_path_len){
			min_path_len = _th;
			key_in_min_path = root->key;
		}
		if (_th >= max_path_len){
			max_path_len = _th;
			key_in_max_path = root->key;
		}
	}

	/* Check subtrees. */
	if (left)  _bst_validate_rec(left, _th);
	if (right) _bst_validate_rec(right, _th);
}

static int bst_validate(bst_t *bst)
{
	int check_bst = 0;
	total_paths = 0;
	min_path_len = 99999999;
	max_path_len = -1;
	total_nodes = 0;
	bst_violations = 0;

	_bst_validate_rec(bst->root, 0);

	check_bst = (bst_violations == 0);

	printf("Validation:\n");
	printf("=======================\n");
	printf("  BST Violation: %s\n",
	       check_bst ? "No [OK]" : "Yes [ERROR]");
	printf("  Tree size: %8d\n", total_nodes);
	printf("  Total paths: %d\n", total_paths);
	printf("  Min/max paths length: %d/%d\n", min_path_len, max_path_len);
	printf("  Key of min path: %d\n", key_in_min_path);
	printf("  Key of max path: %d\n", key_in_max_path);
	printf("\n");

	return check_bst;
}

#endif /* _VALIDATE_H_ */
