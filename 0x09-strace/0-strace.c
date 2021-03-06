#include "strace.h"

/**
 * trace_sysnum - trace specified process and print out system all number
 * @pid: id of process to trace
 */
void trace_sysnum(pid_t pid)
{
	int wstatus;

	setbuf(stdout, NULL);
	waitpid(pid, &wstatus, 0);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		if (!step_syscall(pid))
			break;
		printf("%li\n", ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * ORIG_RAX));
		if (!step_syscall(pid))
			break;
	}
}

/**
 * main - entry point
 * @argc: number of command-line arguments
 * @argv: pointer to an array of character strings that contain the arguments
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
	pid_t pid;

	if (parse_args(argc, argv))
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (!pid)
		return (attach(argv + 1) == -1);
	trace_sysnum(pid);
	return (0);
}
