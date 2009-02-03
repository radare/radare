#if __linux__ || __NetBSD__ || __FreeBSD__ || __OpenBSD__

#include <r_io.h>
#include <r_lib.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

static int __waitpid(int pid)
{
	int st = 0;
	if (waitpid(pid, &st, 0) == -1)
		return R_FALSE;
	if (WIFEXITED(st)) {
	//if ((WEXITSTATUS(wait_val)) != 0) {
		perror("==> Process has exited\n");
		//debug_exit();
		return -1;
	}
	return R_TRUE;
}

// TODO: move to common os/ directory
/* 
 * Creates a new process and returns the result:
 * -1 : error
 *  0 : ok 
 * TODO: should be pid number?
 * TODO: should accept argv and so as arguments
 */
#include <linux/user.h>
#define MAGIC_EXIT 31337
static int fork_and_attach(const char *cmd)
{
	int pid = -1;

	pid = vfork();
	switch(pid) {
	case -1:
		fprintf(stderr, "Cannot fork.\n");
		break;
	case 0:
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) != 0) {
			fprintf(stderr, "ptrace-traceme failed\n");
			exit(MAGIC_EXIT);
		}
#if 0
		eprintf("argv = [ ");
		for(i=0;ps.argv[i];i++)
			eprintf("'%s', ", ps.argv[i]);
		eprintf("]\n");
#endif

		// TODO: USE TM IF POSSIBLE TO ATTACH IT FROM ANOTHER CONSOLE!!!
		// TODO: 
		//debug_environment();
fprintf(stderr, "execv: %s\n", cmd);
{
	char buf[128];
	char *argv[2];
		strcpy(buf, cmd);
	argv[0]= buf;
	argv[1] = NULL;
		//execv(cmd, argv); //ps.argv[0], ps.argv);
execl("/bin/ls", "ls", NULL);
}
		perror("fork_and_attach: execv");
		//printf(stderr, "[%d] %s execv failed.\n", getpid(), ps.filename);
		exit(MAGIC_EXIT); /* error */
		break;
	default:
		fprintf(stderr, "Waitpid\n");
		__waitpid(pid); //, NULL, WUNTRACED); //, &wait_val);
		fprintf(stderr, "Waitpid-end\n");

#if 0
		if (!is_alive(ps.pid)) {
			fprintf(stderr, "Oops the process is not alive?!?\n");
			return -1;
		}

		/* restore breakpoints */
		debug_bp_reload_all();

#endif
		/* required for some BSDs */
		kill(pid, SIGSTOP);
//		ptrace(PTRACE_ATTACH, pid, 0, 0);
//perror("ptrace:");
		break;
	}
	printf("PID = %d\n", pid);

	return pid;
}

static int __handle_open(const char *file)
{
	if (!memcmp(file, "dbg://", 6))
		return R_TRUE;
	return R_FALSE;
}

static int __open(const char *file, int rw, int mode)
{
	if (__handle_open(file)) {
		int pid = atoi(file+6);
		if (pid == 0)
			pid = fork_and_attach(file+6);
		if (pid > 0) {
			char foo[1024];
			sprintf(foo, "ptrace://%d", pid);
			// TODO: We need to implement to io redirect method here
			//return r_io_redirect(foo, rw, mode);
		}
	}
	return -1;
}

static int __handle_fd(int fd)
{
	return R_FALSE;
}

static int __init()
{
	printf("dbg init\n");
	return R_TRUE;
}

static struct r_io_handle_t r_io_plugin_dbg = {
        //void *handle;
	.name = "dbg",
        .desc = "Debug a program or pid. dbg:///bin/ls, dbg://1388",
        .open = __open,
        .handle_open = __handle_open,
        .handle_fd = __handle_fd,
	.lseek = NULL,
	.system = NULL,
	.init = __init,
        //void *widget;
/*
        struct debug_t *debug;
        u32 (*write)(int fd, const u8 *buf, u32 count);
	int fds[R_IO_NFDS];
*/
};

struct r_lib_struct_t radare_plugin = {
	.type = R_LIB_TYPE_IO,
	.data = &r_io_plugin_dbg
};

#endif
