#include "main_header.h"

bool list_validation(main_params *params, node **work_node, bool *classify)
{
	*work_node = find_node(params, params->pwd_string, true);
	char *string1 = malloc_c(MAXCHARS);
	char *string2 = malloc_c(MAXCHARS);
	char *string3 = malloc_c(MAXCHARS);

	bool return_value = true;

	switch (sscanf(params->buffer, "%*s %s %s %s ", string1, string2, string3))
	{
		case EOF:
		{
			break;
		}
		case 1:
		{
			if (!strcmp(string1, "-F"))
			{
				*classify = true;
			}
			else
			{
				char *path = string_to_path(params, string1);
				*work_node = find_node(params, path, true);
				free(path);
			}
			break;
		}
		case 2:
		{
			*classify = true;

			char *path = string_to_path(params, string2);
			*work_node = find_node(params, path, true);
			free(path);

			break;
		}
		default:
		{
			fprintf(stderr, "%s: too many arguments\n", params->buffer);
			return_value = false;
		}
	}
	free(string1);
	free(string2);
	free(string3);

	if (!work_node)
	{
		// hasn't been found
		return_value = false;
	}

	return return_value;
}

bool change_dir_validation(main_params *params, char *string)
{
	switch (sscanf(params->buffer, "%*s %s ", string))
	{
		case EOF:
		{
			fprintf(stderr, "cd: missing operand");
			return false;
		}
		case 1:
		{
			return true;
		}
		default:
		{
			fprintf(stderr, "cd: too many arguments\n");
			return false;
		}
	}

	return false;
}

bool create_file_validation(main_params *params, char *curr_token,
							node **work_node, char *file_name)
{
	char *path = string_to_path(params, curr_token);
	*work_node = find_node(params, path, 1);

	if (*work_node && ((*work_node)->data == '/' || (*work_node)->isValid))
	{
		// exists AND (isDir OR isFile)
		fprintf(stderr, "%s: already exists\n", curr_token);
		free(path);
		return false;
	}

	*work_node = split_path(params, path, file_name);
	free(path);

	if (!*work_node)
	{
		// not found
		fprintf(stderr, "%s: No such file or directory\n", curr_token);
		return false;
	}

	return true;
}

bool remove_path_validation(main_params *params, char **path,
							char *curr_token, bool isDir)
{
	*path = string_to_path(params, curr_token);
	node *work_node = find_node(params, *path, true);

	if (!strcmp(*path, "/"))
	{
		// delete root?
		return false;
	}
	if (isDir)
	{
		// it's a directory
		if (!work_node)
		{
			fprintf(stderr, "%s: No such file or directory\n", curr_token);
			return false;
		}
		if (work_node->data != '/')
		{
			fprintf(stderr, "%s: Not a directory\n", curr_token);
			return false;
		}
		if (work_node->child)
		{
			fprintf(stderr, "%s: Directory not empty\n", curr_token);
			return false;
		}
	}
	else
	{
		// it's a file
		if (!work_node || !work_node->isValid)
		{
			fprintf(stderr, "%s: No such file or directory\n", curr_token);
			return false;
		}
		if (work_node->isValid && work_node->child)
		{
			// it has children, simply remove the validation from this one
			work_node->isValid = false;
			return false;
		}
		work_node->isValid = false;
	}

	return true;
}