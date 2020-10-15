/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Lucas Vinicius Hartmann
 *
 * Descrição:
 * introdução à manipulação de file-descriptors
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;

void standard_output() {
	cout << "Hello world!" << endl;
	
	const char *str = "Hello again!\n";
	write(STDOUT_FILENO, str, strlen(str));
	
	cout << "STDOUT_FILENO is " << STDOUT_FILENO << "." << endl;
}

void standard_input() {
	char buff[1024];

	cout << "STDIN_FILENO is " << STDIN_FILENO << "." << endl;
	
	while (true) {
		cout << "Type something them ENTER. CTRL+D to end nicely. CTRL+C to cancel." << endl;
		int n = read(STDIN_FILENO, buff, 1024);
		if (n <= 0) {
			cout << "Bad: n=" << n << endl;
			break;
		}
		
		cout << "Got " << n << "bytes: \"" << flush;
		write(STDOUT_FILENO, buff, n);
		cout << "\"" << endl;
	}
}

void my_cat(const char *name) {
	int fd = open(name, O_RDONLY);
	
	if (fd == -1)
		return;
	
	while (true) {
		char buff[1024];
		int n = read(fd, buff, 1024);
		
		if (n <= 0)
			break;
		
		write(STDOUT_FILENO, buff, n);
	}
	
	close(fd);
}

void my_copy(const char *from, const char *to) {
	int fdf = open(from, O_RDONLY);
	if (fdf == -1)
		return;
	
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int fdt = open(to, O_WRONLY | O_CREAT | O_EXCL, mode);
	if (fdt == -1)
		return;
	
	while (true) {
		char buff[1024];
		int n = read(fdf, buff, 1024);
		
		if (n <= 0)
			break;
		
		write(fdt, buff, n);
	}
	
	close(fdf);	
	close(fdt);
}

int main(int argc, char *argv[]) {
	// Test 1: Standard output
	standard_output();
	
	// Test 2: Standard input
//	standard_input();
	
	// Test 3: My CAT
//	if (argc == 2)
//		my_cat(argv[1]);
	
	// Test 3: My Copy
//	if (argc == 3)
//		my_copy(argv[1], argv[2]);
	
	return 0;
}
