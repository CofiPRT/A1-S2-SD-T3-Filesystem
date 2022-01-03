#include "main_header.h"

void print_dir(main_params *params)
{
	fprintf(stdout, "%s\n", params->pwd_string);
}

void list(main_params *params)
{
	// default parameters
	bool classify = false; // -F
	node *work_node = NULL;

	if (!list_validation(params, &work_node, &classify))
	{
		/* if it is NULL, it either hasn't been found
		or something else went wrong */
		return;
	}

	char *string = malloc_c(MAXCHARS);
	strcpy(string, "");

	list_support(work_node, classify, string);
	fprintf(stdout, "\n");

	free(string);
}

void change_dir(main_params *params)
{
	char *string = malloc_c(MAXCHARS);
	
	if (!change_dir_validation(params, string))
	{
		return;
	}

	change_dir_support(params, string);

	free(string);
}

void make_dir(main_params *params)
{
	char *curr_token = strtok(params->buffer, " ");
	// this is 'mkdir' command

	curr_token = strtok(NULL, " ");

	if (!curr_token)
	{
		// at least one more argument required
		fprintf(stderr, "mkdir: missing operand\n");
		return;
	}

	for(; curr_token; curr_token = strtok(NULL, " "))
	{
		create_file_support(params, curr_token, true);
	}
}

void create_file(main_params *params)
{
	char *curr_token = strtok(params->buffer, " ");
	// this is 'touch' command

	curr_token = strtok(NULL, " ");

	if (!curr_token)
	{
		// at least one more argument required
		fprintf(stderr, "touch: missing operand\n");
		return;
	}

	for(; curr_token; curr_token = strtok(NULL, " "))
	{
		create_file_support(params, curr_token, false);
	}
}

void remove_empty_dir(main_params *params)
{
	char *curr_token = strtok(params->buffer, " ");
	// this is 'rmdir' command

	curr_token = strtok(NULL, " ");

	if (!curr_token)
	{
		fprintf(stderr, "rmdir: missing operand\n");
		return;
	}

	for (; curr_token; curr_token = strtok(NULL, " "))
	{
		remove_path_support(params, curr_token, true);
	}
}

void remove_path(main_params *params)
{
	char *curr_token = strtok(params->buffer, " ");
	// this is 'rm' command

	curr_token = strtok(NULL, " ");

	if (!curr_token)
	{
		fprintf(stderr, "rm: missing operand\n");
		return;
	}

	for (; curr_token; curr_token = strtok(NULL, " "))
	{
		remove_path_support(params, curr_token, false);
	}
}

void move(main_params *params)
{
	char *curr_token = strrchr(params->buffer, ' ');
	char *destination = malloc_c(MAXCHARS);
	strcpy(destination, curr_token + 1);

	curr_token = strtok(params->buffer, " ");
	// this is the 'mv' command
	curr_token = strtok(NULL, " ");

	if (!strcmp(curr_token, destination))
	{
		// only one parameter for 'mv' function?
		fprintf(stderr, "mv %s: missing operand\n", curr_token);
		free(destination);
		return;
	}

	for (; strcmp(curr_token, destination); curr_token = strtok(NULL, " "))
	{
		move_copy_support(params, curr_token, destination, true);
	}

	free(destination);
}

void copy(main_params *params)
{
	char *curr_token = strrchr(params->buffer, ' ');
	char *destination = malloc_c(MAXCHARS);
	strcpy(destination, curr_token + 1);

	curr_token = strtok(params->buffer, " ");
	// this is the 'cp' command
	curr_token = strtok(NULL, " ");

	if (!strcmp(curr_token, destination))
	{
		// only one parameter for 'mv' function?
		fprintf(stderr, "cp %s: missing operand\n", curr_token);
		free(destination);
		return;
	}

	for (; strcmp(curr_token, destination); curr_token = strtok(NULL, " "))
	{
		move_copy_support(params, curr_token, destination, false);
	}

	free(destination);
}

void function_not_found(main_params *params)
{
	(void)params;
}