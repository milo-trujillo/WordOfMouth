#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "log.h"

//
// This is based on the textbook example from Advanced Programming in the UNIX
// Environment, Stevens, Chapter 13
//

// Returns whether daemonization succeeded
bool daemonize()
{
	log(LOG_INFO, "Daemonizing...");
	int fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	// Clear file creation mask
	umask(0);

	// Get max number of file descriptors
	if( getrlimit(RLIMIT_NOFILE, &rl) < 0 )
	{
		log(LOG_ERROR, "Cannot get file limit");
		return false;
	}
	
	// Become session leader to lose the TTY
	if( (pid = fork()) < 0 )
	{
		log(LOG_ERROR, "Unable to fork");
		return false;
	}
	else if( pid != 0 ) // Parent
		exit(0);
	setsid(); // Child becomes the leader

	// Ensure opening files will not allocate TTYs
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	
	if( sigaction(SIGHUP, &sa, NULL) < 0 )
	{
		log(LOG_ERROR, "Unable to ignore SIGHUP");
		return false;
	}
	if( (pid = fork()) < 0 )
	{
		log(LOG_ERROR, "Unable to fork");
		return false;
	}
	else if( pid != 0 ) // Parent
		exit(0);

	// Change CWD to root so we won't be a problem if the config was on a CD
	// or flash drive or something the user wants to unmount
	if( chdir("/") < 0 )
	{
		log(LOG_ERROR, "Unable to move to root directory");
		return false;
	}

	// Close all open file descriptors
	if( rl.rlim_max == RLIM_INFINITY )
		rl.rlim_max = 1024;
	for( unsigned int i = 0; i < rl.rlim_max; i++ )
		close(i);

	// Attach file descriptors 0, 1, and 2 to /dev/null
	// This is of course, stdin, stdout, and stderr
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	if( fd0 != 0 || fd1 != 1 || fd2 != 2 )
	{
		log(LOG_ERROR, "Unexpected file handles after closing standard I/O");
		exit(1);
	}
	return true;
}
