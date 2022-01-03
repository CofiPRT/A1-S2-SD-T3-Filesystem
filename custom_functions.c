#include "main_header.h"

void *malloc_c(size_t size)
{
	void *p = malloc(size);
	if (!p)
	{
		fprintf(stderr, "Allocation error!\n");
		exit(-1);
	}

	return p;
}

FILE *fopen_c(char *filename, char *mode)
{
	FILE *file = fopen(filename, mode);
	if (!file)
	{
		fprintf(stderr, "File %s could not be opened!\n", filename);
		exit(-1);
	}

	return file;
}

void strncpy_c(char *string1, char *string2, int number)
{
	strncpy(string1, string2, number);
	string1[number] = '\0';
}

void concat(char *string, char character)
{
	string[strlen(string) + 1] = '\0';
	string[strlen(string)] = character;
}