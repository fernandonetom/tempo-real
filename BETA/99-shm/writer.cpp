#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;

int main() {
	cout << "Opening shared memory... " << flush;
	int fd = shm_open("/my_test_shm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		cout << "Failed." << endl;
		return 1;
	}
	cout << "Ok." << endl;

	cout << "Type your messages in..." << endl;
	
	char str[128];
	int n=0;
	do {
		write(fd, str, n);
		n = read(STDIN_FILENO, str, sizeof(str));
	} while (n >= 0);

	return 0;
}


