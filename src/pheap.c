#include<cutlery/pheap.h>

#include<cutlery/binary_tree_util.h>

#include<cutlery/cutlery_node.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/cutlery_math.h>

/*
	INTERNAL FUNCTIONS THESE ARE THE ONLY ONES THAT RELY ON THE PHEAPTYPE OF THE PHEAP
*/

static void swap_chidren_for_phpnode(phpnode* node_p)
{
	void* temp = node_p->left;
	node_p->left = node_p->right;
	node_p->right = temp;
}

// this function will mostly be used by LEFTIST pheap
static cy_uint get_node_property_for_phpnode(const phpnode* node_p)
{
	if(node_p == NULL) // NULL is 0 elements from the NULL, so its node_property is 0 for a LEFTIST pheap, for SKEW returning anything works
		return 0;

	return node_p->node_property;
}

// below function needs to be called only on some node_p in the leftist pheap, when you suspect the node_property of leftist heap is violated
// this includes leftist heap property of being left deep OR incorrectly set node_property
static void restore_leftist_heap_node_properties_up_until_root(phpnode* node_p)
{
	// iterate over all the node from parent to the original root, in the original tree
	for(phpnode* temp = node_p; temp != NULL; temp = temp->parent)
	{
		cy_uint left_child_node_property = get_node_property_for_phpnode(temp->left);
		cy_uint right_child_node_property = get_node_property_for_phpnode(temp->right);

		if(left_child_node_property < right_child_node_property) // we must reinstate the tree property of left_tree_node_property >= right_tree_node_property
		{
			swap_chidren_for_phpnode(temp);

			// fix the local variables, even though it is not necessary to be done
			left_child_node_property = get_node_property_for_phpnode(temp->left);
			right_child_node_property = get_node_property_for_phpnode(temp->right);
		}

		cy_uint new_node_property = min(left_child_node_property, right_child_node_property) + 1;

		// if the node_property does not need fixing, then break out
		if(new_node_property == get_node_property_for_phpnode(temp))
			break;

		// else set it
		temp->node_property = new_node_property;
	}
}

// the below function does not modify the pheap_p in any way and only uses it for pheap_p->type
// it severs all the three connection ->parent, ->left and ->right for the node_p, singling it out
static void sever_phpnode_from_tree(const pheap* pheap_p, phpnode* node_p)
{
	// sever the left child. if exists
	if(node_p->left != NULL)
	{
		node_p->left->parent = NULL;
		node_p->left = NULL;
	}

	// sever the right child, if exists
	if(node_p->right != NULL)
	{
		node_p->right->parent = NULL;
		node_p->right = NULL;
	}

	// cache parent pointer for future use
	phpnode* parent = node_p->parent;

	if(node_p->parent != NULL)
	{
		if(is_left_of_its_parent(node_p))
			node_p->parent->left = NULL;
		else
			node_p->parent->right = NULL;
		node_p->parent = NULL;
	}

	// now the node_p is itself a single node tree whose node_property is definitely 1 for all trees
	node_p->node_property = 1;

	switch(pheap_p->type)
	{
		case LEFTIST :
		{
			if(parent != NULL) // if the parent exists then, its node_property may be violated in the tree so fix it
				restore_leftist_heap_node_properties_up_until_root(parent);
			return;
		}
		default :
			return;
	}
}

// the below 3 meld function does not modify the pheap_p in any way and only uses it for pheap_p->info, pheap_p->type and the pheap_p->node_offset

// not to be used except in meld() function
static phpnode* meld_for_skew_pheap(const pheap* pheap_p, phpnode* a, phpnode* b)
{
	// TODO
	return NULL;
}

// not to be used except in meld() function
static phpnode* meld_for_leftist_pheap(const pheap* pheap_p, phpnode* a, phpnode* b)
{
	// TODO
	return NULL;
}

// can be used by other functions
static phpnode* meld(const pheap* pheap_p, phpnode* a, phpnode* b)
{
	switch(pheap_p->type)
	{
		case SKEW :
			return meld_for_skew_pheap(pheap_p, a, b);
		case LEFTIST :
			return meld_for_leftist_pheap(pheap_p, a, b);
		default :
			return NULL;
	}
}

void TO_BE_REMOVED()
{
	sever_phpnode_from_tree(NULL, NULL);
	meld(NULL, NULL, NULL);
}

/*
	INTERNAL FUNCTIONS END
*/

void initialize_pheap(pheap* pheap_p, heap_type type, pheaptype policy, const comparator_interface* comparator, cy_uint node_offset)
{
	pheap_p->info = (heap_info){.type = type, .comparator = (*comparator)};;
	pheap_p->type = policy;
	pheap_p->node_offset = node_offset;
	pheap_p->root = NULL;
}

void initialize_phpnode(phpnode* node_p)
{
	node_p->parent = NULL;
	node_p->left = NULL;
	node_p->right = NULL;

	// node property of the node that is not linked to any pheap is 0
	node_p->node_property = 0;
}

int is_free_floating_phpnode(const phpnode* node_p)
{
	// note: node_property of a new_phpnode is 0
	return ((node_p->parent == NULL) && (node_p->left == NULL) && (node_p->right == NULL) && (node_p->node_property == 0));
}

int is_empty_pheap(const pheap* pheap_p)
{
	return (pheap_p->root == NULL);
}

int push_to_pheap(pheap* pheap_p, const void* data);

int pop_from_pheap(pheap* pheap_p)
{
	// can not pop, if there are no elements in the pheap
	if(is_empty_pheap(pheap_p))
		return 0;

	// remove the top of the pheap tree
	return remove_from_pheap(pheap_p, get_data(pheap_p->root, pheap_p));
}

const void* get_top_of_pheap(const pheap* pheap_p)
{
	// there is no top element, if there are no elements in the pheap
	if(is_empty_pheap(pheap_p))
		return NULL;

	// return root of the tree
	return get_data(pheap_p->root, pheap_p);
}

void heapify_for_in_pheap(pheap* pheap_p, const void* data);

int remove_from_pheap(pheap* pheap_p, const void* data);

// only the below function needs this header file
#include<cutlery/singlylist.h>

void remove_all_from_pheap(pheap* pheap_p, const notifier_interface* ni_p)
{
	// stack data that needs to be removed, linked with their parent pointers of the nodes
	singlylist unremoved_stack;
	initialize_singlylist(&unremoved_stack, pheap_p->node_offset + offsetof(phpnode, parent));

	// insert first element of the stack
	if(pheap_p->root != NULL)
	{
		pheap_p->root->parent = NULL; // this is equivalent to initializing slnode for unremoved stack
		insert_head_in_singlylist(&unremoved_stack, get_data(pheap_p->root, pheap_p));

		// root now points to nothing
		pheap_p->root = NULL;
	}

	while(!is_empty_singlylist(&unremoved_stack))
	{
		// remove an element from the stack
		const void* data_p = get_head_of_singlylist(&unremoved_stack);
		remove_head_from_singlylist(&unremoved_stack);

		// get its node
		// since we just remove it from the unremoved_stack, it s parent is suppossed to be NULL
		phpnode* node_p = get_node(data_p, pheap_p);

		// if node_p has right node, then insert it to unremoved_stack
		if(node_p->right != NULL)
		{
			node_p->right->parent = NULL; // this is equivalent to initializing slnode for unremoved stack
			insert_head_in_singlylist(&unremoved_stack, get_data(node_p->right, pheap_p));
		}

		// if node_p has left node, then insert it to unremoved_stack
		if(node_p->left != NULL)
		{
			node_p->left->parent = NULL; // this is equivalent to initializing slnode for unremoved stack
			insert_head_in_singlylist(&unremoved_stack, get_data(node_p->left, pheap_p));
		}

		// re initialize phpnode of data_p
		// its parent was already NULL, the below call will reset its left, right and node_property to NULLs and 0s
		initialize_phpnode(node_p);

		// notify the notifier_interface ni_p (if any)
		if(ni_p != NULL)
			ni_p->notify(ni_p->resource_p, data_p);
	}
}

static void for_each_node_post_order(const pheap* pheap_p, const phpnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	for_each_node_post_order(pheap_p, node_p->left, operation, additional_params);
	for_each_node_post_order(pheap_p, node_p->right, operation, additional_params);
	operation(get_data(node_p, pheap_p), additional_params);
}

void for_each_in_pheap(const pheap* pheap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(is_empty_pheap(pheap_p))
		return;

	for_each_node_post_order(pheap_p, pheap_p->root, operation, additional_params);
}

static void print_node(dstring* append_str, const pheap* pheap_p, const phpnode* node_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
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
		sprint_element(append_str, get_data(node_p, pheap_p), tabs + 1);
		snprintf_dstring(append_str, " (%" PRIu_cy_uint ")\n", node_p->node_property);

		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "parent : [%p]\n", node_p->parent);

		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "left  :  [%p]\n", node_p->left);
			
		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "right :  [%p]\n\n", node_p->right);
	}
}

static void print_tree(dstring* append_str, const pheap* pheap_p, const phpnode* node_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	if(node_p != NULL)
	{
		print_tree(append_str, pheap_p, node_p->left, sprint_element, tabs);
		print_node(append_str, pheap_p, node_p, sprint_element, tabs);
		print_tree(append_str, pheap_p, node_p->right, sprint_element, tabs);
	}
}

void sprint_pheap(dstring* append_str, const pheap* pheap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++);
	switch(pheap_p->info.type)
	{
		case MIN_HEAP :
		{snprintf_dstring(append_str, "pheap (MIN_HEAP) :\n");break;}
		case MAX_HEAP :
		{snprintf_dstring(append_str, "pheap (MAX_HEAP) :\n");break;}
	}

	sprint_chars(append_str, '\t', tabs++);
	switch(pheap_p->type)
	{
		case SKEW :
		{snprintf_dstring(append_str, "pheap (SKEW_HEAP) :\n");break;}
		case LEFTIST :
		{snprintf_dstring(append_str, "pheap (LEFTIST_HEAP) :\n");break;}
	}

	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "node_offset : [%" PRIu_cy_uint "]\n", pheap_p->node_offset);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "root : [%p]\n", pheap_p->root);

	print_tree(append_str, pheap_p, pheap_p->root, sprint_element, tabs + 1);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "top : ");
	if(get_top_of_pheap(pheap_p) != NULL)
		sprint_element(append_str, get_top_of_pheap(pheap_p), 0);
	else
		snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}
