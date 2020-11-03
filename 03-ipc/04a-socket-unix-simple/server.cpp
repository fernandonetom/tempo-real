#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

using namespace std;

int main() {
    // Creating a UNIX-domain socket:
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Check for error:
    if (sfd == -1) {
        cerr << "socket(...) failed." << endl;
        return -1;
    }

    // Config-struct for UNIX-domain sockets:
    sockaddr_un sa;
    // Common field specfies address-family.
    // Must be the same as the one used on socket(...).
    sa.sun_family = AF_UNIX;
    // Specialized field is the path, as a C-style string:
    strcpy(sa.sun_path, "/tmp/my_server");

    // Bind the server socket to the address.
    // C requires pointer type casting to sockaddr,
    int br = bind(sfd, (sockaddr*)&sa, sizeof(sa));

    // Check for error:
    if (br == -1) {
        cerr << "bind(...) failed." << endl;
        return -1;
    }

    // Listen starts the server, and sets queue length.
    int lr = listen(sfd, 10);

    // Check for error:
    if (lr == -1) {
        cerr << "listen(...) failed." << endl;
        return -1;
    }

    // Client loop:
    while (true) {
        // accept(...) creates new socket for the first client on the queue.
        // It may also return information about the client address, but we 
        // don't care about it for now so we pass 0 to the remaining args.
        int cfd = accept(sfd, 0, 0);

        // Now cfd can be used to talk to the client.
        // UPPERCASE echo, for example:
        char buffer[1024];
        int n = read(cfd, buffer, sizeof(buffer));
        if (n>0) {
            for (int i=0; i<n; i++)
                buffer[i] = toupper(buffer[i]);
            write(cfd, buffer, n);
        }

        // Communication done, close.
        close(cfd);

        // Loop reads next client on the queue.
    }
}
