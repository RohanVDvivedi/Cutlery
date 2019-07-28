#include<tree.h>

node* get_new_node(const tree* tree_p, const void* data_p)
{
	node* node_p = ((node*)calloc(1, sizeof(node)));
	node_p->parent = NULL;
	node_p->data_p = data_p;
	node_p->children = (node**) calloc(tree_p->children_default_size ,sizeof(node*));
	return node_p;
}

tree* get_tree(unsigned long long int children_default_size, const void* root_data_p)
{
	tree* tree_p = ((tree*)calloc(1, sizeof(tree)));
	tree_p->children_default_size = children_default_size;
	tree_p->root_node = get_new_node(tree_p, root_data_p);
	return tree_p;
}

int set_child(const tree* tree_p ,node* parent_p, const void* data_p, unsigned long long int child_index)
{
	if(tree_p->children_default_size > child_index)
	{
		if(parent_p->children[child_index] == NULL)
		{
			node* child_p = get_new_node(tree_p, data_p);
			connect(tree_p, parent_p, child_p, child_index);
		}
		else
		{
			parent_p->children[child_index]->data_p = data_p;
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

const node* get_child(const tree* tree_p, node* parent_p, unsigned long long int child_index)
{
	if(tree_p->children_default_size > child_index)
	{
		return parent_p->children[child_index];
	}
	else
	{
		return NULL;
	}
}

const void* get_data(const tree* tree_p, node* parent_p, unsigned long long int child_index)
{
	const node* child_node = get_child(tree_p, parent_p, child_index);
	return child_node == NULL ? NULL : child_node->data_p;
}

void connect(const tree* tree_p, node* parent_p, node* child_p, unsigned long long int child_index)
{
	// set the parent in the child node
	if(child_p != NULL)
	{
		child_p->parent = parent_p;
	}

	// add child in the list of parent node
	if(parent_p != NULL)
	{
		// add child_p as a child in the parent's list of children
		parent_p->children[child_index] = child_p;
	}
}

void delete_node(node* node_p)
{
	if(node_p->children != NULL)
	{
		free(node_p->children);
		node_p->children = NULL;
	}
	free(node_p);
}

void delete_nodes_from(const tree* tree_p, node* node_p)
{
	if(node_p->children != NULL && tree_p->children_default_size > 0)
	{	
		for(unsigned long long int i = 0; i < tree_p->children_default_size; i++)
		{
			if(node_p->children[i] != NULL)
			{
				delete_nodes_from(tree_p, node_p->children[i]);
				node_p->children[i] = NULL;
			}
		}
	}
	delete_node(node_p);
}

void remove_child(const tree* tree_p , node* parent, unsigned long long int child_index)
{
	if(parent->children[child_index] != NULL)
	{
		delete_nodes_from(tree_p, parent->children[child_index]);
	}
	parent->children[child_index] = NULL;
}

void delete_tree(tree* tree_p)
{
	if(tree_p->root_node != NULL)
	{
		delete_nodes_from(tree_p ,tree_p->root_node);
	}
	free(tree_p);
}

// prints n tabs
void print_tabs(unsigned long long int tabs_count)
{
	for(unsigned long long int i = 0; i < tabs_count; i++)
	{
		printf("\t");
	}
}

// prints only the node pointed to by node_p
void print_node(const node* node_p, void (*print_data)(const void* data_p), unsigned long long int tabs_count)
{
	if(node_p != NULL)
	{
		print_tabs(tabs_count); printf("data : "); print_data(node_p->data_p); printf("\n");
	}
	else
	{
		print_tabs(tabs_count); printf("NULL NODE"); printf("\n");
	}
}

// prints node and all of its children aswell recursively
void print_nodes_from(const tree* tree_p, const node* node_p, void (*print_data)(const void* data_p), unsigned long long int tabs_count)
{
	if(node_p != NULL)
	{
		print_node(node_p, print_data, tabs_count);
		for(unsigned long long int i = 0; i < tree_p->children_default_size; i++)
		{
			print_tabs(tabs_count); printf("child_node : %llu\n", i);
			print_nodes_from(tree_p, node_p->children[i], print_data, tabs_count + 1);
		}
	}
	else
	{
		print_tabs(tabs_count); printf("NULL NODE"); printf("\n");
	}
}

// print tree
void print_tree(const tree* tree_p, void (*print_data)(const void* data_p))
{
	printf("printing tree : \n");
	if(tree_p != NULL)
	{
		printf("children_default_size : %llu\n", tree_p->children_default_size);
		printf("nodes_from_root : "); printf("\n");
		print_nodes_from(tree_p, tree_p->root_node, print_data, 1);
	}
	else
	{
		printf("NULL TREE"); printf("\n");
	}
	printf("\n");
}