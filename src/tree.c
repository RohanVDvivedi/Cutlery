#include<tree.h>

tree* get_tree(unsigned long long int children_default_size, unsigned long long int size_of_data_element_on_node)
{
	tree* tree_p = ((tree*)calloc(1, sizeof(tree)));
	tree_p->size_of_data_on_node = size_of_data_element_on_node;
	tree_p->children_default_size = children_default_size;
	tree_p->increment_factor = 2;
	tree_p->increment_amount = 1;
	tree_p->root_node = NULL;
	return tree_p;
}

node* get_node(tree* tree_p, const void* data_p)
{
	node* node_p = ((node*)calloc(1, sizeof(node)));
	node_p->parent = NULL;
	node_p->children_occupied_size = 0;
	if(data_p != NULL)
	{
		memcpy(node_p->data_p, data_p, tree_p->size_of_data_on_node);
	}
	node_p->children_total_size = tree_p->children_default_size;
	node_p->children = (node**) calloc(tree_p->children_default_size ,sizeof(node*));
	return node_p;
}

void add_child(tree* tree_p ,node* parent_p, const void* data_p)
{
	node* child_p = get_node(tree_p, data_p);
	connect(tree_p, parent_p, child_p);
}

void connect(tree* tree_p, node* parent_p, node* child_p)
{
	// set the parent in the child node
	if(child_p != NULL)
	{
		child_p->parent = parent_p;
	}

	// add child in the list of parent node
	if(parent_p != NULL)
	{
		if(parent_p->children_occupied_size == parent_p->children_total_size)
		{
			// compute the new size required for this purpose
			unsigned long long int new_children_total_size = parent_p->children_total_size * tree_p->increment_factor + tree_p->increment_amount;
		
			// allocate a new node pointer array to hold data
			node** new_children = ((node**)calloc(new_children_total_size, sizeof(node*)));

			// copy pointers from old array to new expanded array
			memcpy(new_children, parent_p->children, parent_p->children_occupied_size);

			// free the older smaller node* array
			free(parent_p->children);

			// update children and children total_size attributed in parent node
			parent_p->children = new_children;
			parent_p->children_total_size = new_children_total_size;
		}
		// add child_p as a child in the parent's list of children
		parent_p->children[parent_p->children_occupied_size++] = child_p;
	}
}

void delete_tree(tree* tree_p)
{
	if(tree_p != NULL)
	{
		delete_nodes_from(tree_p->root_node);
		free(tree_p);
	}
}

void delete_nodes_from(node* node_p)
{
	for(unsigned long long int i = 0; i < node_p->children_occupied_size; i++)
	{
		delete_nodes_from(node_p->children[i]);
	}
	if(node_p->data_p != NULL)
	{
		free(node_p->data_p);
	}
	free(node_p);
}

int is_leaf(node* node_p)
{
	return node_p->children_occupied_size == 0;
}

int is_root(node* node_p)
{
	return node_p->parent == NULL;
}