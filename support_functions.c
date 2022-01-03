#include "main_header.h"

void list_support(node *work_node, bool classify, char *string)
{
	node *curr_child = work_node->child;

	char *curr_string = malloc_c(MAXCHARS);

	while (curr_child)
	{
		if (curr_child->data == '/')
		{
			// skip
			curr_child = curr_child->next;
			continue;
		}
		
		// save the current string
		strcpy(curr_string, string);

		// concatenate the char
		concat(string, curr_child->data);

		// decide whether or not to print
		print_string(curr_child, string, classify);

		// recursive call
		list_support(curr_child, classify, string);

		// get back the string from this step
		strcpy(string, curr_string);
		curr_child = curr_child->next;
	}

	free(curr_string);
}

void change_dir_support(main_params *params, char *string)
{
	char *path = string_to_path(params, string);
	node *work_node = find_node(params, path, true);

	if (!work_node)
	{
		fprintf(stderr, "%s: No such file or directory\n", string);
		free(path);
		return;
	}
	if (work_node->data != '/')
	{
		fprintf(stderr, "%s: Not a directory\n", string);
		free(path);
		return;
	}

	strcpy(params->pwd_string, path);
	free(path);
}

void create_file_support(main_params *params, char *curr_token, bool isDir)
{
	node *work_node = NULL;
	char *file_name = malloc_c(MAXCHARS);

	if (!create_file_validation(params, curr_token, &work_node, file_name))
	{
		free(file_name);
		return;
	}

	if (isDir)
	{
		add_separator(file_name);
	}
	add_to_trie(work_node, file_name, isDir);

	free(file_name);
}

void remove_path_support(main_params *params, char *curr_token, bool isDir)
{
	char *path = NULL;

	if (!remove_path_validation(params, &path, curr_token, isDir))
	{
		free(path);
		return;
	}
	
	int cursor = strlen(path) - 1; // end of string
	node *work_node = find_deletion_node(params, path, &cursor);
	
	node *remove_node = remove_child_link(work_node, path[cursor + 1]);
	free_all_nodes(remove_node->child);
	free(remove_node);

	free(path);
}

void move_copy_support(main_params *params, char *curr_token,
						char *destination, bool isMove)
{
	char *path = string_to_path(params, curr_token);
	node *work_node = find_node(params, path, true);

	if (!work_node || !work_node->isValid)
	{
		// node hasn't been found OR is a directory
		fprintf(stderr, "%s: No such file or directory\n", curr_token);
		free(path);
		return;
	}

	char *dest_path = string_to_path(params, destination);
	node *dest_node = find_node(params, dest_path, true);

	if (!dest_node)
	{
		// not found
		fprintf(stderr, "%s: No such file or directory\n", destination);
		free(dest_path);
		free(path);
		return;
	}
	if (dest_node->data != '/')
	{
		fprintf(stderr, "%s: Not a directory\n", destination);
		free(dest_path);
		free(path);
		return;
	}

	if (isMove)
	{
		// find and extract the node that does not need to be here
		if (work_node->isValid && work_node->child)
		{
			// it has children, simply remove the validation from this one
			work_node->isValid = false;
		}
		else
		{
			int cursor = strlen(path) - 1; // end of string
			work_node = find_deletion_node(params, path, &cursor);
			node *remove_node = remove_child_link(work_node, path[cursor + 1]);

			free_all_nodes(remove_node->child);
			free(remove_node);
		}
	}

	// add the file name to the destination
	char *file_name = malloc_c(MAXCHARS);
	split_path(params, path, file_name);
	add_to_trie(dest_node, file_name, false);

	free(file_name);
	free(dest_path);
	free(path);
}
