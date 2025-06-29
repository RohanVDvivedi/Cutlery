#include<cutlery/pheap.h>

#include<cutlery/binary_tree_util.h>

#include<cutlery/cutlery_node.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/cutlery_math.h>

/*
	INTERNAL FUNCTIONS THESE ARE THE ONLY ONES THAT RELY ON THE PHEAPTYPE OF THE PHEAP
*/

static void swap_children_for_phpnode(phpnode* node_p)
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
static void restore_leftist_pheap_node_property_up_until_root(phpnode* node_p)
{
	// iterate over all the node from parent to the original root, in the original tree
	for(phpnode* temp = node_p; temp != NULL; temp = temp->parent)
	{
		cy_uint left_child_node_property = get_node_property_for_phpnode(temp->left);
		cy_uint right_child_node_property = get_node_property_for_phpnode(temp->right);

		if(left_child_node_property < right_child_node_property) // we must reinstate the tree property of left_tree_node_property >= right_tree_node_property
		{
			swap_children_for_phpnode(temp);

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

// the below 3 meld function does not modify the pheap_p in any way and only uses it for pheap_p->info, pheap_p->type and the pheap_p->node_offset

// not to be used except in meld() function
// remember the parent pointer of the returned phpnode must be reassigned to a correct value
static phpnode* meld_for_skew_pheap(const pheap* pheap_p, phpnode* a, phpnode* b)
{
	// if a is null return b
	if(a == NULL)
		return b;

	// if b is null return a
	if(b == NULL)
		return a;

	// we need to make one of them parent of the other
	phpnode* parent = a;
	phpnode* child = b;
	// make sure this assignment of parent and child satisfies the heap property, else if reordering is required then reverse it
	{
		const void* data_a = get_data(a, pheap_p);
		const void* data_b = get_data(b, pheap_p);
		if(is_reordering_required(data_a, data_b, &(pheap_p->info)))
		{
			parent = b;
			child = a;
		}
	}

	parent->right = meld_for_skew_pheap(pheap_p, parent->right, child);

	// make sure the parent pointers are correct, for the right child of the parent
	if(parent->right != NULL)
		parent->right->parent = parent;

	// force swap the children, so that the next meld happens at a different child
	swap_children_for_phpnode(parent);

	return parent;
}

// not to be used except in meld() function
// remember the parent pointer of the returned phpnode must be reassigned to a correct value
static phpnode* meld_for_leftist_pheap(const pheap* pheap_p, phpnode* a, phpnode* b)
{
	// if a is null return b
	if(a == NULL)
		return b;

	// if b is null return a
	if(b == NULL)
		return a;

	// we need to make one of them parent of the other
	phpnode* parent = a;
	phpnode* child = b;
	// make sure this assignment of parent and child satisfies the heap property, else if reordering is required then reverse it
	{
		const void* data_a = get_data(a, pheap_p);
		const void* data_b = get_data(b, pheap_p);
		if(is_reordering_required(data_a, data_b, &(pheap_p->info)))
		{
			parent = b;
			child = a;
		}
	}

	parent->right = meld_for_leftist_pheap(pheap_p, parent->right, child);

	// make sure the parent pointers are correct, for the right child of the parent
	if(parent->right != NULL)
		parent->right->parent = parent;

	// restore the leftist heap node property of being deeper on the left child
	{
		cy_uint left_child_node_property = get_node_property_for_phpnode(parent->left);
		cy_uint right_child_node_property = get_node_property_for_phpnode(parent->right);

		if(left_child_node_property < right_child_node_property) // we must reinstate the tree property of left_tree_node_property >= right_tree_node_property
		{
			swap_children_for_phpnode(parent);

			// fix the local variables, even though it is not necessary to be done
			left_child_node_property = get_node_property_for_phpnode(parent->left);
			right_child_node_property = get_node_property_for_phpnode(parent->right);
		}

		parent->node_property = min(left_child_node_property, right_child_node_property) + 1;
	}

	return parent;
}

// can be used by other functions
// remember the parent pointer of the returned phpnode must be reassigned to a correct value
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

int merge_pheaps(pheap* dest, pheap* src)
{
	if(dest->info.type != src->info.type)
		return 0;

	if(dest->info.comparator.context != src->info.comparator.context)
		return 0;

	if(dest->info.comparator.compare1 != src->info.comparator.compare1) // this ensures that even if the comparator uses compare2, they will checked for equality
		return 0;

	if(dest->type != src->type)
		return 0;

	if(dest->node_offset != src->node_offset)
		return 0;

	dest->root = meld(dest, dest->root, src->root);
	src->root = NULL;

	if(dest->root != NULL)
		dest->root->parent = NULL;

	return 1;
}

int push_to_pheap(pheap* pheap_p, const void* data)
{
	phpnode* node_p = get_node(data, pheap_p);

	if(!is_free_floating_phpnode(node_p))	// insert only a free floating node, i.e. a node that does not exist in any pheap
		return 0;

	// make the new node a tree of itself
	node_p->node_property = 1;

	// merge root with this new node
	pheap_p->root = meld(pheap_p, pheap_p->root, node_p);

	// reset the parent pointer of the root node
	if(pheap_p->root != NULL)
		pheap_p->root->parent = NULL;

	return 1;
}

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

void heapify_for_in_pheap(pheap* pheap_p, const void* data)
{
	phpnode* node_p = get_node(data, pheap_p);

	if(is_free_floating_phpnode(node_p))	// for attempting to heapify_for the node, it must be present in the pheap, i.e. must not be a free floating node
		return 0;

	// if the heapify is needed for he parent, the children need not be touched
	if(node_p->parent != NULL && is_reordering_required(get_data(node_p->parent), data, &(pheap_p->info)))
	{
		// disconnect node_p from its parent

		// if LEFTIST, restore node_property for the parent

		// meld root with node_p

		return;
	}
	else
	{
		phpnode* children_to_meld[2];
		cy_uint chidlren_to_meld_count = 0;

		if(node_p->left != NULL && is_reordering_required(data, get_data(node_p->left), &(pheap_p->info)))
		{
			children_to_meld[chidlren_to_meld_count++] = node_p->left;

			// disconnect left child from node_p
		}

		if(node_p->right != NULL && is_reordering_required(data, get_data(node_p->right), &(pheap_p->info)))
		{
			children_to_meld[chidlren_to_meld_count++] = node_p->right;

			// disconnect right child from node_p
		}

		// if no melding required early exit
		if(chidlren_to_meld_count == 0)
			return;

		// if LEFTIST, restore node_property for node_p

		// meld for all entries in children_to_meld

		return;
	}
}

int remove_from_pheap(pheap* pheap_p, const void* data)
{
	phpnode* node_p = get_node(data, pheap_p);

	if(is_free_floating_phpnode(node_p))	// for attempting to remove the node, it must be present in the pheap, i.e. must not be a free floating node
		return 0;

	// actual removal happens here
	{
		// meld left and right child of node_p into only_child
		phpnode* only_child = meld(pheap_p, node_p->left, node_p->right);
		if(only_child != NULL)
			only_child->parent = NULL;

		// connect only_child to node_p's parent
		phpnode* parent = node_p->parent;
		if(parent != NULL)
		{
			if(is_left_of_its_parent(node_p))
				parent->left = only_child;
			else
				parent->right = only_child;
		}
		else
			pheap_p->root = only_child;
		if(only_child != NULL)
			only_child->parent = parent;

		// now node_p is disconnected from the tree, but with it's left, right and parent pointers still intact

		// if it is a leftist pheap, then we need to fix the node_property of the parent, because it now has a new child
		if(pheap_p->type == LEFTIST)
		{
			if(parent != NULL)
				restore_leftist_pheap_node_property_up_until_root(parent);
		}
	}

	// NULL all references of the removed node
	initialize_phpnode(node_p);	// you must reinitialize the node before final removal

	return 1;
}

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
