// При загрузке системы создаётся 1 (init) процесс

// pid_t fork(); //системный вызов, копия текущего процесса

// exec__(file_name, argv, envp) передаётся имя исполняемого файла. Этот elf-файл заменяет процесс + переменная окружения
// execve(file_name, argv, envp)
// execl(name, arg1, arg2, ...) // ends with NULL pointer
// execle(name, arg1, arg2, ..., envp)
// execv()
// execlp()
// execvp
// Если невалидный эльф, то либо вернётся -1, либо всё плохо.
// Или не очень. exec__ может запускать скрипты (#!/bin/sh - для shell-скриптов)

// pid_t wait(&status) возвращает pid умершего процесса

#include <unistd.h>
#include <sys/types.h>

int main() {
	int pid = fork();

	if (fork()) {
		foo();
		wait(0);
		// Если не зомби, то всё ОК
		// 
	} else {
		exit(0);
		// После того, как ребёнок умирает, он превращается в зомби. Нам остаётся лишь код возврата
		// Зомби нужно убивать. Иначе заполнится таблица процессов.
	}

	if (pid > 0) {
		//parent
	} else {
		//child
	}
}

//*********************************************************
if (pid = fork() == 0) {
	exec("/bin/sh");
} else {
	while (1) {
		wait(0);
	}
}

// wait(int *status)
// WIFEEXITED(status)
// WEXITSTATUS()
// ...

// Группы процессов
// pid_t getpgid() // for current process
//		setpgid(pid_t, pid_t pgid)

// int dup(int) // сделать копию файлового дескриптора
// int dup2(int old, int new) // закрыть old и подставить туды new, вернуть его

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		int pid = fork();
		if (pid < 0) {
			fprintf(stderr, "Fork fail!\n");
		} else if (pid == 0) {
			execlp(argv[i], argv[0], (char *)0);
			fprintf(stderr, "Exec fail!\n");
			return 0;
		} else {
			int w_status;
			if (wait(&w_status) < 0) {
				fprintf(stderr, "Error %d\n", errno);
			}
		}
	}
}

// Нужно, чтобы два процесса по очереди записывали в файл 1 и 2

int main(int argc, char *argv[]) {
	for (int i = 1; i <= 2; i++) {
		int pid = fork();
		if (pid < 0) {
			fprintf(stderr, "Fork fail!\n");
		} else if (pid == 0) {
//		execlp(argv[i], argv[0], (char *)0);
//		fprintf(stderr, "Exec fail!\n");
			while (1)
				printf("1\n");
			return 0;
		}
	}
	wait(0);
	wait(0);
}
