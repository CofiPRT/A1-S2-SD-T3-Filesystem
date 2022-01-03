#include "main_header.h"

void add_separator(char *string)
{
	if (string[strlen(string) - 1] != '/')
	{
		// make sure it ends in an '/'
		concat(string, '/');
	}
}

void print_string(node *curr_child, char *string, bool classify)
{
	if (curr_child->isValid)
	{
		// this is a file
		if (classify)
		{
			fprintf(stdout, "%s%c ", string, '*');
		}
		else
		{
			fprintf(stdout, "%s ", string);
		}
	}
	else if (curr_child->child && curr_child->child->data == '/')
	{
		// this is a directory
		if (classify)
		{
			fprintf(stdout, "%s%c ", string, '/');
		}
		else
		{
			fprintf(stdout, "%s ", string);
		}
	}
}

void previous_dir(main_params *params, char *string, char *path)
{
	if (!strcmp(params->pwd_string, "/"))
	{
		// root has no parents, return invalid path
		strcpy(path, "");
		return;
	}

	unsigned int depth = 0;
	while (depth*3 + 1 < strlen(string) && 
			string[depth*3] == '.' && string[depth*3 + 1] == '.')
	{
		// how many pairs of '..' are there
		depth++;
	}

	int aux_depth = depth;

	char *point;
	strcpy(path, params->pwd_string);
	while (depth)
	{
		// for each depth level, jump a directory ('/')
		point = strrchr(path, '/');
		path[point - path] = '\0';

		depth--;

		if (!strcmp(path, ""))
		{
			if (depth)
			{
				// a level beyond root is searched, nonsense
				return;
			}

			// the last level ended on root
			strcpy(path, "/");
		}			
	}

	if (string[aux_depth*3 - 1] == '/')
	{
		// further down than simply '..'
		add_separator(path);

		strcat(path, string + aux_depth*3);
	}
}

char *string_to_path(main_params *params, char *string)
{
	char *path = malloc_c(MAXCHARS);
	strcpy(path, "");

	if (string[0] == '/')
	{
		// already an absolute path
		strcpy(path, string);
	}
	else if (string[0] == '.' && string[1] == '.')
	{
		previous_dir(params, string, path);
	}
	else if (string[0] == '.' && string[1] != '.')
	{
		strcpy(path, params->pwd_string);

		if (string[1] == '/')
		{
			// further down than simply '.'
			add_separator(path);

			strcat(path, string + 2);
		}
	}
	else
	{
		strcpy(path, params->pwd_string);

		add_separator(path);

		strcat(path, string);
	}

	return path;
}

node *find_node(main_params *params, char *path, bool descend)
{
	if (!strcmp(path, ""))
	{
		// path could not be constructed
		return NULL;
	}

	unsigned int cursor = 0; // move through each char of 'path'
	node *curr_node = params->root;
	node *prev_node = NULL;
	while (cursor < strlen(path))
	{
		while (curr_node)
		{
			// move through the children nodes
			if (curr_node->data == path[cursor])
			{
				cursor++;
				prev_node = curr_node;
				break;
			}
			curr_node = curr_node->next;
		}

		if (!curr_node)
		{
			// the path has not been found through these children nodes
			break;
		}
		curr_node = curr_node->child;
	}

	if (cursor == strlen(path))
	{
		// it has successfully found all the chars in 'path'

		if (descend && prev_node->child && prev_node->child->data == '/')
		{
			// it's a dir path, descend into the '/' node
			return prev_node->child;
		}

		return prev_node;
	}

	return NULL;
}

node *split_path(main_params *params, char *path, char *file_name)
{
	char *point = strrchr(path, '/');

	// up until the '/', this is the directory
	char *dir_name = malloc_c(MAXCHARS);
	strncpy_c(dir_name, path, point - path);
	// after the '/', this is the file
	strcpy(file_name, point + 1);

	if (!strcmp(dir_name, ""))
	{
		// the directory is actually root
		free(dir_name);
		return params->root;
	}

	node *work_node = find_node(params, dir_name, true);
	free(dir_name);

	return work_node;
}

node *add_to_trie(node *work_node, char *name, bool isDir)
{
	unsigned int cursor = 0;

	// add one character at a time, checking whether or not it already exists
	while (cursor < strlen(name))
	{
		if (!work_node->child)
		{
			// no children, add this one
			node *new_node = init_node();
			new_node->data = name[cursor];

			work_node->child = new_node;
			work_node = new_node;
		}
		else
		{
			node *curr_node = work_node->child;
			node *prev_node = NULL;
			bool already_here = false;

			// add this node so the children are sorted by ASCII table
			while (curr_node)
			{
				if (curr_node->data == name[cursor])
				{
					// already here, do not add
					already_here = true;
					break;
				}
				if (curr_node->data > name[cursor])
				{
					// add right here
					break;
				}

				prev_node = curr_node;
				curr_node = curr_node->next;
			}

			if (already_here)
			{
				// don't have to do anything, move to the next character
				cursor++;
				work_node = curr_node;
				continue;
			}

			node *new_node = init_node();
			new_node->data = name[cursor];

			if (!prev_node)
			{
				// add at the beginning
				new_node->next = curr_node;
				work_node->child = new_node;
			}
			else
			{	
				// anywhere else
				prev_node->next = new_node;
				new_node->next = curr_node;
			}

			work_node = new_node;
		}

		cursor++;
	}

	work_node->isValid = !isDir;
	/* 'work_node' is the last node added;
		isDir = false means this is a valid FILE path */

	return work_node;
}

node *find_deletion_node(main_params *params, char *path, int *cursor)
{
	char *partial_path = malloc_c(MAXCHARS);
	node *work_node = find_node(params, path, false);

	while (work_node->data != '/' && (*cursor))
	{
		// go up towards root one character at a time
		strncpy_c(partial_path, path, (*cursor) + 1);

		// get this node
		work_node = find_node(params, partial_path, false);

		if ((work_node->child && work_node->child->next) || work_node->isValid)
		{
			// parents are part of other files, don't remove
			break;
		}

		(*cursor)--;
	}

	free(partial_path);
	return work_node;
}

node *remove_child_link(node *work_node, char character)
{
	node *curr_node = work_node->child;
	node *prev_node = NULL;

	while (curr_node)
	{
		if (curr_node->data == character)
		{
			// remove this one
			break;
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}

	if (!prev_node)
	{
		// update the first element of the list
		work_node->child = curr_node->next;
	}
	else
	{
		// link
		prev_node->next = curr_node->next;
	}

	// return the unlinked child
	return curr_node;
}
