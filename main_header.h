#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef _FILESYSTEM_HEADER_
#define _FILESYSTEM_HEADER_

#define MAXCHARS 100
#define MAXFUNCTIONS 9

typedef struct node
{
	char data;
	struct node *next, *child;
	bool isValid;
} node;

typedef struct functions_map
{
	char *fname; // function mapping
	void (*f)();
} function_map;

typedef struct main_params
{
	// pass arguments to functions more easily
	int useless_parameter;
	node *root;

	char *pwd_string;

	FILE *in_file, *out_file;
	char *buffer;
} main_params;

// custom_functions
void *malloc_c(size_t size);
FILE *fopen_c(char *filename, char *mode);
void strncpy_c(char *string1, char *string2, int number);
void concat(char *string, char character);

// init_free_functions
main_params *init_params();
void free_params(main_params *params);
node *init_node();
void free_all_nodes(node *curr_node);

// secondary_functions
void add_separator(char *string);
void print_string(node *curr_child, char *string, bool classify);
void previous_dir(main_params *params, char *string, char *path);
char *string_to_path(main_params *params, char *string);
node *find_node(main_params *params, char *path, bool descend);
node *split_path(main_params *params, char *path, char *filename);
node *add_to_trie(node *work_node, char *filename, bool isValid);
node *find_deletion_node(main_params *params, char *path, int *cursor);
node *remove_child_link(node *work_node, char character);

// support_functions
void list_support(node *work_node, bool classify, char *string);
void change_dir_support(main_params *params, char *string);
void create_file_support(main_params *params, char *curr_token, bool isDir);
void remove_path_support(main_params *params, char *curr_token, bool isDir);
void move_copy_support(main_params *params, char *curr_token,
						char *destination, bool isMove);

// validation_functions
bool list_validation(main_params *params, node **work_node, bool *classify);
bool change_dir_validation(main_params *params, char *string);
bool create_file_validation(main_params *params, char *curr_token,
							node **work_node, char *file_name);
bool remove_path_validation(main_params *params, char **path,
							char *curr_token, bool isDir);

// primary_functions
void print_dir(main_params *params);
void list(main_params *params);
void change_dir(main_params *params);
void make_dir(main_params *params);
void create_file(main_params *params);
void remove_empty_dir(main_params *params);
void remove_path(main_params *params);
void move(main_params *params);
void copy(main_params *params);
void function_not_found(main_params *params);

#endif