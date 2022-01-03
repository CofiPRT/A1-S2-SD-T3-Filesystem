#include "main_header.h"

node *init_node()
{
	node *new_node = malloc_c(sizeof(node));

	new_node->next = NULL;
	new_node->child = NULL;
	new_node->isValid = false;

	return new_node;
}

void free_all_nodes(node *curr_node)
{
	if (!curr_node)
	{
		return;
	}

	// recursively free all children [from their suffering]
	free_all_nodes(curr_node->next);
	free_all_nodes(curr_node->child);

	free(curr_node);
}

main_params *init_params()
{
	main_params *params = malloc_c(sizeof(main_params));

	if (fscanf(stdin, "%d ", &params->useless_parameter) != 1)
	{
		fprintf(stderr, "Error when reading 'useless_parameter' from stdin!\n");
		exit(-1);
	}

	params->root = init_node();
	params->root->data = '/';

	params->pwd_string = malloc_c(MAXCHARS);
	strcpy(params->pwd_string, "/");

	params->buffer = malloc_c(MAXCHARS);

	return params;
}

void free_params(main_params *params)
{
	free_all_nodes(params->root);
	free(params->pwd_string);
	free(params->buffer);

	free(params);
}
