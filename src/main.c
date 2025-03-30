#include "commons.h"

static int	exec_routine(t_conf *conf, int argc, char **argv, int *unrecoverable_error)
{
	memset(conf, 0, sizeof(t_conf));
	for (int i = 0; HANDLERS[i].name != NULL; i++)
	{
		if (strcmp(argv[0], HANDLERS[i].name) != 0)
			continue;
		if (HANDLERS[i].parser(argc, argv, conf))
		{
			if (unrecoverable_error)
				*unrecoverable_error = HANDLERS[i].error(conf, 0);
			return (HANDLERS[i].cleaner(conf), 1);
		}
		if (HANDLERS[i].executor(conf))
		{
			if (unrecoverable_error)
				*unrecoverable_error = HANDLERS[i].error(conf, 0);
			return (HANDLERS[i].cleaner(conf), 1);
		}
		HANDLERS[i].cleaner(conf);
		return (HANDLERS[i].error(conf, 1));
	}
	print_commands(argv[0], 1);
	return (1);
}

static int	process_command(t_conf *conf, char *command)
{
	pid_t 		pid;
	int			status = 0;
	wordexp_t	p = {0};
	int			wordexp_result;
	size_t		len;

	len = strlen(command);
	if (len > 0 && command[len - 1] == '\n')
		command[len - 1] = '\0';
	wordexp_result = wordexp(command, &p, WRDE_NOCMD | WRDE_REUSE);
	if (wordexp_result != 0)
		return (wordfree(&p), 1);
	pid = fork();
	if (pid == -1)
		return (wordfree(&p), perror_int());
	else if (pid == 0)
	{
		int test = 0;
		status = exec_routine(conf, p.we_wordc, p.we_wordv, &test);
		wordfree(&p);
		free(command);
		exit(status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status))
				fprintf(stderr, "error in %s\n", p.we_wordv[0]);
			return (wordfree(&p), 0);
		}
		else
			return (wordfree(&p), 1);
	}
}

static char	*get_command(void)
{
	char	*command;

	write(STDOUT_FILENO, "ft_ssl> ", 8);
	command = get_next_line(STDIN_FILENO);
	if (!command)
		return (NULL);
	return (command);
}

static int	loop_commands(t_conf *conf)
{
	while (1)
	{
		char *command = get_command();
		if (!command)
			return (0);
		else if (!strcmp(command, "exit\n") || !strcmp(command, "exit"))
			return (free(command), 0);
		else if (!strcmp(command, "help\n") || !strcmp(command, "exit"))
			print_commands(NULL, 0);
		else if (process_command(conf, command))
			return (free(command), 1);
		free(command);
	}
	return (130);
}

int main(int argc, char **argv)
{
	t_conf conf;

	if (argc >= 2)
		return (exec_routine(&conf, argc - 1, argv + 1, NULL));
	else
		return (loop_commands(&conf));
}