#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */

int is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;
	int result = 0;


	for (; j < strlen(buf) - 1; j++)
	{
		if (buf[j] == '|' && buf[j + 1] == '|')
		{
			buf[j] = 0;
			j++;
			info->cmd_buf_type = CMD_OR;
			result = 1;
			break;
		}
		else if (buf[j] == '&' && buf[j + 1] == '&')
		{
			buf[j] = 0;
			j++;
			info->cmd_buf_type = CMD_AND;
			result = 1;
			break;
		}
		else if (buf[j] == ';')
		{
			buf[j] = 0;
			info->cmd_buf_type = CMD_CHAIN;
			result = 1;
			break;
		}
	}

	*p = j;
	return (result);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */

void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;


	for (; j < len; j++)
	{
		if (info->cmd_buf_type == CMD_AND)
		{
			if (info->status)
			{
				buf[i] = 0;
				j = len;
			}
		}
		if (info->cmd_buf_type == CMD_OR)
		{
			if (!info->status)
			{
				buf[i] = 0;
				j = len;
			}
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */

int replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;


	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */

int replace_vars(info_t *info)
{
	int i;
	list_t *node;


	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			char *converted = convert_number(info->status, 10, 0);

			replace_string(&(info->argv[i]), _strdup(converted));
			free(converted);
			continue;
		}

		if (!_strcmp(info->argv[i], "$$"))
		{
			char *pid_str = convert_number(getpid(), 10, 0);

			replace_string(&(info->argv[i]), _strdup(pid_str));
			free(pid_str);
			continue;
		}

		node = node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			char *value = _strchr(node->str, '=') + 1;

			replace_string(&(info->argv[i]), _strdup(value));
			continue;
		}

		replace_string(&info->argv[i], _strdup(""));
	}

	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */

int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
