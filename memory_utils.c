#include "shell.h"

/**
 * bfree - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */


int bfree(void **ptr)
{
	int result = 0;

	for (; ptr && *ptr;)
	{
		free(*ptr);
		*ptr = NULL;
		result = 1;
	}
	return (result);
}
