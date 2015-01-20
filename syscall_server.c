#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define SYSCALL_ARGS_P &args[0], &args[1], &args[2], &args[3], &args[4], &args[5], &args[6]
#define SYSCALL_ARGS   args[0], args[1], args[2], args[3], args[4], args[5], args[6]

size_t stdin2buf(char* buf, size_t count) {
	size_t size = count;
	while(true) {
		size_t rc = read(fileno(stdin), buf, count);
		if(((int)rc)<=0) {
			//fprintf(stderr, "ERROR: %d\n", (int)rc);
			break;
		}

		//fprintf(stderr, "read()=%d\n", (int)rc);

		count-=rc;
		buf+=rc;

		if(count<=0) break;
	}

	return size-count;
}

void fn_readmem(const char* line) {
	long addr;
	long count;

	// parse args
	sscanf(line, "%ld %ld", &addr, &count);
	//fprintf(stderr, "readmem(%p, %lu)\n", (void*)addr, count);

	size_t rc = write(fileno(stdout), (void*)addr, (size_t)count);
	fflush(stdout);

	// print result
	printf("%ld\n", (long)rc);
	fflush(stdout);

	//fprintf(stderr, "Done = %ld\n", rc);
}

void fn_writemem(const char* line) {
	long addr;
	long count;

	// parse args
	sscanf(line, "%ld %ld", &addr, &count);

	// signal start
	printf("\n");
	fflush(stdout);

	// read data
	size_t rc = stdin2buf((void*)addr, (size_t)count);

	// read the rest of data in error case
	if((long)rc<count) {
		size_t i;
		for(i=0; i<(count-rc); i++) {
			getchar();
		}
	}

	// print result
	printf("%ld\n", (long)rc);
	fflush(stdout);
}

void fn_syscall(const char* line) {
	// get args
	long args[7];
	sscanf(line, "%ld %ld %ld %ld %ld %ld %ld", SYSCALL_ARGS_P);

	// run syscall
	//fprintf(stderr, "syscall(%ld %ld %ld %ld %ld %ld %ld) = ", SYSCALL_ARGS);
	long rc = syscall(SYSCALL_ARGS);
	//fprintf(stderr, "%ld\n", rc);


	// print result
	printf("%ld\n", rc);
	fflush(stdout);
}

int main(void) {
	// open stdin
	FILE* f = fdopen(fileno(stdin), "rb");

	// read line by line
	char * line = NULL;
	size_t len = 0;
	ssize_t cnt;
	while ((cnt = getline(&line, &len, f)) != -1) {
		line[cnt-1]=0;
		//fprintf(stderr, "line=%s\n", line);

		// commands
		if(strstr(line, "readmem")==line) {
			fn_readmem(line+7);
		}
		else if(strstr(line, "writemem")==line) {
			fn_writemem(line+8);
		}
		else if(strstr(line, "syscall")==line) {
			fn_syscall(line+7);
		}
	}

	// close stdin
	fclose(f);

	return 0;
}
