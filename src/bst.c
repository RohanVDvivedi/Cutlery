#include<cutlery/bst.h>

#include<cutlery/bst_core.h>
#include<cutlery/bst_nonbalancing.h>
#include<cutlery/bst_avl.h>
#include<cutlery/bst_redblack.h>
#include<cutlery/bst_util.h>

#include<cutlery/cutlery_node.h>
#include<cutlery/cutlery_stds.h>

void initialize_bst(bst* bst_p, bsttype type, const comparator_interface* comparator, cy_uint node_offset)
{
	bst_p->type = type;
	bst_p->comparator = (*comparator);
	bst_p->node_offset = node_offset;
	bst_p->root = NULL;
	bst_p->order_stat_node_offset = NO_ORDER_STAT_BST_NODE_OFFSET;
}

void initialize_order_stat_bst(bst* bst_p, bsttype type, const comparator_interface* comparator, cy_uint node_offset, cy_uint order_stat_node_offset)
{
	bst_p->type = type;
	bst_p->comparator = (*comparator);
	bst_p->node_offset = node_offset;
	bst_p->root = NULL;
	bst_p->order_stat_node_offset = order_stat_node_offset;
}

void initialize_bstnode(bstnode* node_p)
{
	node_p->parent = NULL;
	node_p->left = NULL;
	node_p->right = NULL;

	// node property of the node that is not linked to any bst is 0
	node_p->node_property = 0;
}

int is_free_floating_bstnode(const bstnode* node_p)
{
	// note: node_property of a new_bstnode is 0
	return ((node_p->parent == NULL) && (node_p->left == NULL) && (node_p->right == NULL) && (node_p->node_property == 0));
}

int is_empty_bst(const bst* bst_p)
{
	return (bst_p->root == NULL);
}


#define is_greater(compare_A_with_B) 	((compare_A_with_B) > 0)
#define is_lesser(compare_A_with_B)		((compare_A_with_B) < 0)
#define is_equal(compare_A_with_B)		((compare_A_with_B) == 0)

// searches for bstnode that holds data equal to the given data
static const bstnode* find_node(const bst* bst_p, const void* data, search_occurence occurence_type)
{
	const bstnode* result = NULL;
	const bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = compare_with_comparator(&(bst_p->comparator), data, get_data(node_p, bst_p));
		if(is_equal(compared_data_with_current_node))
		{
			result = node_p;
			switch(occurence_type)
			{
				case FIRST_OCCURENCE :
				{
					node_p = node_p->left;
					break;
				}
				case LAST_OCCURENCE :
				{
					node_p = node_p->right;
					break;
				}
			}
		}
		else if(is_lesser(compared_data_with_current_node))
			node_p = node_p->left;
		else if(is_greater(compared_data_with_current_node))
			node_p = node_p->right;
	}
	return result;
}

static const bstnode* find_node_preceding(const bst* bst_p, const void* data)
{
	const bstnode* result = NULL;
	const bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = compare_with_comparator(&(bst_p->comparator), data, get_data(node_p, bst_p));
		if(is_equal(compared_data_with_current_node) || is_lesser(compared_data_with_current_node))
			node_p = node_p->left;
		else if(is_greater(compared_data_with_current_node))
		{
			result = node_p;
			node_p = node_p->right;
		}
	}
	return result;
}

static const bstnode* find_node_preceding_or_equals(const bst* bst_p, const void* data)
{
	const bstnode* result = NULL;
	const bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = compare_with_comparator(&(bst_p->comparator), data, get_data(node_p, bst_p));
		if(is_lesser(compared_data_with_current_node))
			node_p = node_p->left;
		else if(is_equal(compared_data_with_current_node) || is_greater(compared_data_with_current_node))
		{
			result = node_p;
			node_p = node_p->right;
		}
	}
	return result;
}

static const bstnode* find_node_succeeding(const bst* bst_p, const void* data)
{
	const bstnode* result = NULL;
	const bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = compare_with_comparator(&(bst_p->comparator), data, get_data(node_p, bst_p));
		if(is_equal(compared_data_with_current_node) || is_greater(compared_data_with_current_node))
			node_p = node_p->right;
		else if(is_lesser(compared_data_with_current_node))
		{
			result = node_p;
			node_p = node_p->left;
		}
	}
	return result;
}

static const bstnode* find_node_succeeding_or_equals(const bst* bst_p, const void* data)
{
	const bstnode* result = NULL;
	const bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = compare_with_comparator(&(bst_p->comparator), data, get_data(node_p, bst_p));
		if(is_equal(compared_data_with_current_node) || is_lesser(compared_data_with_current_node))
		{
			result = node_p;
			node_p = node_p->left;
		}
		else if(is_greater(compared_data_with_current_node))
			node_p = node_p->right;
	}
	return result;
}

const void* find_equals_in_bst(const bst* bst_p, const void* data, search_occurence occurence_type)
{
	const bstnode* node_p = find_node(bst_p, data, occurence_type);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

const void* find_preceding_in_bst(const bst* bst_p, const void* data)
{
	const bstnode* node_p = find_node_preceding(bst_p, data);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

const void* find_preceding_or_equals_in_bst(const bst* bst_p, const void* data)
{
	const bstnode* node_p = find_node_preceding_or_equals(bst_p, data);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

const void* find_succeeding_in_bst(const bst* bst_p, const void* data)
{
	const bstnode* node_p = find_node_succeeding(bst_p, data);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

const void* find_succeeding_or_equals_in_bst(const bst* bst_p, const void* data)
{
	const bstnode* node_p = find_node_succeeding_or_equals(bst_p, data);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

const void* find_smallest_in_bst(const bst* bst_p)
{
	const bstnode* node_p = get_smallest_node_from_node(bst_p->root);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

const void* find_largest_in_bst(const bst* bst_p)
{
	const bstnode* node_p = get_largest_node_from_node(bst_p->root);
	return (node_p != NULL) ? get_data(node_p, bst_p) : NULL;
}

static const bstnode* get_inorder_next_node(const bstnode* node_p)
{
	const bstnode* node_next = NULL;

	if(node_p->right != NULL)
		node_next = get_smallest_node_from_node(node_p->right);
	else if(!is_root_node(node_p))
	{
		const bstnode* temp = node_p;
		while(is_right_of_its_parent(temp))
			temp = temp->parent;
		node_next = temp->parent;
	}

	return node_next;
}

static const bstnode* get_inorder_prev_node(const bstnode* node_p)
{
	const bstnode* node_prev = NULL;

	if(node_p->left != NULL)
		node_prev = get_largest_node_from_node(node_p->left);
	else if(!is_root_node(node_p))
	{
		const bstnode* temp = node_p;
		while(is_left_of_its_parent(temp))
			temp = temp->parent;
		node_prev = temp->parent;
	}

	return node_prev;
}

const void* get_inorder_next_of_in_bst(const bst* bst_p, const void* data_xist)
{
	const bstnode* node_xist = get_node(data_xist, bst_p);
	// node_xist must not be free floating
	if(is_free_floating_bstnode(node_xist))
		return NULL;
	const bstnode* node_next = get_inorder_next_node(node_xist);
	return (node_next != NULL) ? get_data(node_next, bst_p) : NULL;
}

const void* get_inorder_prev_of_in_bst(const bst* bst_p, const void* data_xist)
{
	const bstnode* node_xist = get_node(data_xist, bst_p);
	// node_xist must not be free floating
	if(is_free_floating_bstnode(node_xist))
		return NULL;
	const bstnode* node_prev = get_inorder_prev_node(node_xist);;
	return (node_prev != NULL) ? get_data(node_prev, bst_p) : NULL;
}

static cy_uint find_all_in_range_recursive(const bst* bst_p, const bstnode* node_p, const void* lower_bound, const void* upper_bound, sort_direction sort_dirctn, cy_uint max_result_count, int* accumulator_stop, int (*result_accumulator)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return 0;

	cy_uint results_accumulated = 0;

	const void* data_p = get_data(node_p, bst_p);

	int lower_bound_check = (lower_bound != NULL) ? (compare_with_comparator(&(bst_p->comparator), lower_bound, data_p) <= 0) : 1;
	int upper_bound_check = (upper_bound != NULL) ? (compare_with_comparator(&(bst_p->comparator), upper_bound, data_p) >= 0) : 1;

	switch(sort_dirctn)
	{
		case ASCENDING_ORDERED :
		{
			// node_p is greater than the lower_bound, i.e. left sub tree may have nodes lesser than the lower bound
			if(!(*accumulator_stop) && lower_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->left, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, accumulator_stop, result_accumulator, additional_params);

			// node_p is in range [lower_bound, upper_bound], consider this node aswell
			if(!(*accumulator_stop) && lower_bound_check && upper_bound_check && (results_accumulated < max_result_count))
			{
				// a 0 return from the result_accumulator means a stop condition
				(*accumulator_stop) = (result_accumulator(data_p, additional_params) == 0);
				results_accumulated += 1;
			}

			// node_p is lesser than the upper_bound, i.e. right sub tree may have nodes greater than the lower bound
			if(!(*accumulator_stop) && upper_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->right, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, accumulator_stop, result_accumulator, additional_params);
		
			break;
		}
		case DESCENDING_ORDERED :
		{
			// node_p is lesser than the upper_bound, i.e. right sub tree may have nodes greater than the lower bound
			if(!(*accumulator_stop) && upper_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->right, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, accumulator_stop, result_accumulator, additional_params);

			// node_p is in range [lower_bound, upper_bound], consider this node aswell
			if(!(*accumulator_stop) && lower_bound_check && upper_bound_check && (results_accumulated < max_result_count))
			{
				// a 0 return from the result_accumulator means a stop condition
				(*accumulator_stop) = (result_accumulator(data_p, additional_params) == 0);
				results_accumulated += 1;
			}

			// node_p is greater than the lower_bound, i.e. left sub tree may have nodes lesser than the lower bound
			if(!(*accumulator_stop) && lower_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->left, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, accumulator_stop, result_accumulator, additional_params);

			break;
		}
	}

	return results_accumulated;
}

cy_uint find_all_in_range_in_bst(const bst* bst_p, const void* lower_bound, const void* upper_bound, sort_direction sort_dirctn, cy_uint max_result_count, int (*result_accumulator)(const void* data, const void* additional_params), const void* additional_params)
{
	// errror in providing values 
	// if both of the lower and upper bounds are provided, then lower_bound must not be greater than upper bound
	if(((lower_bound != NULL) && (upper_bound != NULL)) && (compare_with_comparator(&(bst_p->comparator), lower_bound, upper_bound) > 0))
		return 0;

	int accumulator_stop = 0;
	return find_all_in_range_recursive(bst_p, bst_p->root, lower_bound, upper_bound, sort_dirctn, max_result_count, &accumulator_stop, result_accumulator, additional_params);
}

int insert_in_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data, bst_p);
	
	if(!is_free_floating_bstnode(node_p))	// insert only a free floating node, i.e. a node that does not exist in any bst
		return 0;

	// else insert node as per the balancing that this tree uses
	switch(bst_p->type)
	{
		case NON_SELF_BALANCING:
		{
			insert_node_in_non_self_balancing_tree(bst_p, node_p);
			break;
		}
		case RED_BLACK_TREE :
		{
			insert_node_in_red_black_tree(bst_p, node_p);
			break;
		}
		case AVL_TREE :
		{
			insert_node_in_avl_tree(bst_p, node_p);
			break;
		}
	}

	return 1;
}

int remove_from_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data, bst_p);

	if(is_free_floating_bstnode(node_p))	// for attempting to remove the node, it must be present in the bst, i.e. must not be a free floating node
		return 0;

	switch(bst_p->type)
	{
		case NON_SELF_BALANCING:
		{
			remove_node_from_non_self_balancing_tree(bst_p, node_p);
			break;
		}
		case RED_BLACK_TREE :
		{
			remove_node_from_red_black_tree(bst_p, node_p);
			break;
		}
		case AVL_TREE :
		{
			remove_node_from_avl_tree(bst_p, node_p);
			break;
		}
	}

	// NULL all references of the removed node
	initialize_bstnode(node_p);	// you must reinitialize the node before final removal

	return 1;
}

// only the below function needs this header file
#include<cutlery/singlylist.h>

void remove_all_from_bst(bst* bst_p, notifier_interface* ni_p)
{
	// stack data that needs to be removed, linked with their parent pointers of the nodes
	singlylist unremoved_stack;
	initialize_singlylist(&unremoved_stack, bst_p->node_offset + offsetof(bstnode, parent));

	// insert first element of the stack
	if(bst_p->root != NULL)
	{
		bst_p->root->parent = NULL; // this is equivalent to initializing slnode for unremoved stack
		insert_head_in_singlylist(&unremoved_stack, get_data(bst_p->root, bst_p));

		// root now points to nothing
		bst_p->root = NULL;
	}

	while(!is_empty_singlylist(&unremoved_stack))
	{
		// remove an element from the stack
		const void* data_p = get_head_of_singlylist(&unremoved_stack);
		remove_head_from_singlylist(&unremoved_stack);

		// get its node
		// since we just remove it from the unremoved_stack, it s parent is suppossed to be NULL
		bstnode* node_p = get_node(data_p, bst_p);

		// if node_p has right node, then insert it to unremoved_stack
		if(node_p->right != NULL)
		{
			node_p->right->parent = NULL; // this is equivalent to initializing slnode for unremoved stack
			insert_head_in_singlylist(&unremoved_stack, get_data(node_p->right, bst_p));
		}

		// if node_p has left node, then insert it to unremoved_stack
		if(node_p->left != NULL)
		{
			node_p->left->parent = NULL; // this is equivalent to initializing slnode for unremoved stack
			insert_head_in_singlylist(&unremoved_stack, get_data(node_p->left, bst_p));
		}

		// re initialize bstnode of data_p
		// its parent was already NULL, the below call will reset its left, right and node_property to NULLs and 0s
		initialize_bstnode(node_p);

		// notify the notifier_interface ni_p (if any)
		if(ni_p != NULL)
			ni_p->notify(ni_p->resource_p, data_p);
	}
}

int is_order_stat_bst(const bst* bst_p)
{
	return bst_p->order_stat_node_offset != NO_ORDER_STAT_BST_NODE_OFFSET;
}

static void for_each_node_pre_order(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	operation(get_data(node_p, bst_p), additional_params);
	for_each_node_pre_order(bst_p, node_p->left, operation, additional_params);
	for_each_node_pre_order(bst_p, node_p->right, operation, additional_params);
}

static void for_each_node_in_order(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	for_each_node_in_order(bst_p, node_p->left, operation, additional_params);
	operation(get_data(node_p, bst_p), additional_params);
	for_each_node_in_order(bst_p, node_p->right, operation, additional_params);
}

static void for_each_node_post_order(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	for_each_node_post_order(bst_p, node_p->left, operation, additional_params);
	for_each_node_post_order(bst_p, node_p->right, operation, additional_params);
	operation(get_data(node_p, bst_p), additional_params);
}

void for_each_in_bst(const bst* bst_p, bsttraversal traversal, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(is_empty_bst(bst_p))
		return;
	switch(traversal)
	{
		case PRE_ORDER :
			for_each_node_pre_order(bst_p, bst_p->root, operation, additional_params); return;
		case IN_ORDER :
			for_each_node_in_order(bst_p, bst_p->root, operation, additional_params); return;
		case POST_ORDER :
			for_each_node_post_order(bst_p, bst_p->root, operation, additional_params); return;
	}
}

static void print_node(dstring* append_str, const bst* bst_p, const bstnode* node_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	if(node_p != NULL)
	{
		sprint_chars(append_str, '\t', tabs++);
		if(is_root_node(node_p))
			snprintf_dstring(append_str, "node ROOT  : [%p]\n", node_p);
		else if(is_leaf_node(node_p))
			snprintf_dstring(append_str, "node LEAF  : [%p]\n", node_p);
		else if(is_internal_node(node_p))
			snprintf_dstring(append_str, "node INTER : [%p]\n", node_p);

		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "data :\n");
		sprint_element(append_str, get_data(node_p, bst_p), tabs + 1);
		snprintf_dstring(append_str, " (%" PRIu_cy_uint ")\n", node_p->node_property);

		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "parent : [%p]\n", node_p->parent);

		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "left  :  [%p]\n", node_p->left);
			
		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "right :  [%p]\n\n", node_p->right);
	}
}

static void print_tree(dstring* append_str, const bst* bst_p, const bstnode* node_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	if(node_p != NULL)
	{
		print_tree(append_str, bst_p, node_p->left, sprint_element, tabs);
		print_node(append_str, bst_p, node_p, sprint_element, tabs);
		print_tree(append_str, bst_p, node_p->right, sprint_element, tabs);
	}
}

void sprint_bst(dstring* append_str, const bst* bst_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++);
	switch(bst_p->type)
	{
		case NON_SELF_BALANCING :
		{snprintf_dstring(append_str, "bst (NON_SELF_BALANCING) :\n"); break; }
		case AVL_TREE :
		{snprintf_dstring(append_str, "bst (AVL_TREE) :\n"); break; }
		case RED_BLACK_TREE :
		{snprintf_dstring(append_str, "bst (RED_BLACK_TREE) :\n"); break; }
	}

	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "node_offset : [%" PRIu_cy_uint "]\n", bst_p->node_offset);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "root : [%p]\n", bst_p->root);

	print_tree(append_str, bst_p, bst_p->root, sprint_element, tabs + 1);
}