#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// Server configuration
#define SERVER_HOST "127.0.0.1"  // Server IP address
#define SERVER_PORT 12345  // Server port number

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];

    // Create a TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        cerr << "Error creating socket\n";
        return -1;
    }

    // Set server address details
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_HOST);
    server_address.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cerr << "Error connecting to server\n";
        return -1;
    }

    // Authentication process
    while (true) {
        bzero(buffer, sizeof(buffer));
        
        recv(client_socket, buffer, sizeof(buffer), 0);  // Receive server prompts

        if (strstr(buffer, "Enter username: ")) {
            string username;
            cout << "Enter username: ";
            getline(cin, username);  // Get username input
            send(client_socket, username.c_str(), username.size(), 0);  // Send username to server
        } else if (strstr(buffer, "Enter password: ")) {
            string password;
            cout << "Enter password: ";
            getline(cin, password);  // Get password input
            send(client_socket, password.c_str(), password.size(), 0);  // Send password to server
        } else if (strstr(buffer, "Authentication successful")) {
            break;  // Exit loop if authentication is successful
        }
    }

    // Main loop to send commands to the server
    while (true) {
        bzero(buffer, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);  // Receive server prompts
        cout << buffer;  // Display the prompt
        bzero(buffer, sizeof(buffer));

        string command;
        getline(cin, command);  // Get command input
        send(client_socket, command.c_str(), command.size(), 0);  // Send command to server

        if (command == "exit") {
            break;  // Exit loop if the command is "exit"
        }

        // Receive and display the command output from the server
        while (true) {
            recv(client_socket, buffer, sizeof(buffer), 0);
            
            if (strstr(buffer, "<end>")) {
                break;  // Break loop if end of output is received
            }
            
            cout << buffer;  // Display command output
            bzero(buffer, sizeof(buffer));
        }
    }

    close(client_socket);  // Close the client socket

    return 0;
}
