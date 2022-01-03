#include "main_header.h"

void (*find_function(char *buffer))(main_params *)
{
	function_map map[] = {
		{"pwd", print_dir},
		{"ls", list},
		{"cd", change_dir},
		{"mkdir", make_dir},
		{"touch", create_file},
		{"rmdir", remove_empty_dir},
		{"rm", remove_path},
		{"mv", move},
		{"cp", copy}
	};

	char *operation = malloc_c(MAXCHARS);
	sscanf(buffer, "%s ", operation); // first string of the command

	int i;
	for (i = 0; i < MAXFUNCTIONS; i++)
	{
		if (!strcmp(map[i].fname, operation))
		{
			free(operation);
			return map[i].f;
		}
	}

	fprintf(stderr, "%s: command not found\n", operation);
	free(operation);
	return function_not_found;
}

int main()
{
	main_params *params = init_params();

	while (fgets(params->buffer, MAXCHARS, stdin))
	{
		if (params->buffer[strlen(params->buffer) - 1] == '\n')
		{
			// strtok handling
			params->buffer[strlen(params->buffer) - 1] = '\0';
		}
		find_function(params->buffer)(params);
	}

	free_params(params);
	return 0;
}