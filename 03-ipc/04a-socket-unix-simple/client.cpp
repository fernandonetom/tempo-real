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

    // Connect to the address:
    // C-style function requires cast to sockaddr*
    int cr = connect(sfd, (sockaddr*)&sa, sizeof(sa));

    // Check for error:
    if (cr == -1) {
        cerr << "connect(...) failed." << endl;
        return -1;
    }

    // Now s can be used like any file descriptor, and talks to the server.

    // Send a message, and get responsed:
    string message = "this is a test message.";
    int wr = write(sfd, message.data(), message.length());
    if (wr != message.length()) {
        cerr<<"write(...) failed." << endl;
        return -1;
    }
    cout << "Sent......: \"" << message << "\"" << endl;

    char buffer[1024];
    int rr = read(sfd, buffer, sizeof(buffer));
    if (rr <= 0) {
        cerr << "read(...) failed." << endl;
        return -1;
    }
    message = string(buffer, 0, rr);
    cout << "Received..: \"" << message << "\"" << endl;

    // Communication done, close.
    close(sfd);
}
