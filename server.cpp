#include <iostream>
#include <cstring>
#include <unistd.h>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// Server configuration
#define SERVER_PORT 12345  // Define the port number the server will listen on

// User credentials (username:password)
unordered_map<string, string> users = {
    {"user1", "password1"},
    {"user2", "password2"}
};

// Function to handle a connected client
void handle_client(int client_socket) {
    char buffer[1024];

    // Send a welcome message to the client
    send(client_socket, "Welcome to the SSH server\n", 26, 0);

    // Authentication process
    bool authenticated = false;
    while (!authenticated) {
        bzero(buffer, sizeof(buffer));
        
        // Prompt for username
        send(client_socket, "Enter username: ", 17, 0);
        recv(client_socket, buffer, sizeof(buffer), 0);
        string username(buffer);  // Get the username entered by the client
        cout << "Received username: " << username << endl;
        bzero(buffer, sizeof(buffer));

        // Prompt for password
        send(client_socket, "Enter password: ", 17, 0);
        recv(client_socket, buffer, sizeof(buffer), 0);
        string password(buffer);  // Get the password entered by the client
        cout << "Received password: " << password << endl;
        bzero(buffer, sizeof(buffer));

        // Validate credentials
        if (users[username] == password) {
            authenticated = true;
            send(client_socket, "Authentication successful!\n", 29, 0);
            cout << "Authentication successful for user: " << username << endl;
        } else {
            send(client_socket, "Authentication failed. Try again.\n", 36, 0);
        }
    }

    bzero(buffer, sizeof(buffer));

    // Main loop to handle commands
    while (true) {
        // Prompt for command
        send(client_socket, "$ ", 2, 0);
        recv(client_socket, buffer, sizeof(buffer), 0);

        string command(buffer);  // Get the command from the client
        bzero(buffer, sizeof(buffer));

        if (command == "exit\n") {  // Check if the client wants to exit
            break;
        }

        // Execute the command and send the output back to the client
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            send(client_socket, "Error executing command\n", 24, 0);
            continue;
        } else {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                send(client_socket, buffer, sizeof(buffer), 0);
                bzero(buffer, sizeof(buffer));
            }
            send(client_socket, "<end>", 5, 0);  // Signal end of command output
            usleep(5000);  // Short delay to ensure the client receives all data
            pclose(pipe);
        }
    }
    close(client_socket);  // Close the client socket
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "Error creating socket\n";
        return -1;
    }

    // Bind the socket to the specified address and port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cerr << "Error binding socket\n";
        return -1;
    }

    // Start listening for incoming connections
    if (listen(server_socket, 1) < 0) {
        cerr << "Error listening for connections\n";
        return -1;
    }
    cout << "[*] Listening on 127.0.0.1:" << SERVER_PORT << endl;

    // Accept and handle client connections
    while (true) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0) {
            cerr << "Error accepting connection\n";
            continue;
        }

        cout << "[*] Accepted connection from " << inet_ntoa(client_address.sin_addr)
             << ":" << ntohs(client_address.sin_port) << endl;

        handle_client(client_socket);  // Handle the connected client
    }

    close(server_socket);  // Close the server socket

    return 0;
}
