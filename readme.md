# SSH Server-Client Project

DIY By - [Abhishek Singh Kushwaha](https://ask-03-portfolio.vercel.app/)

## Overview

This project implements a basic SSH server-client setup in C++. The server listens for incoming connections and authenticates clients based on predefined credentials. Once authenticated, clients can execute shell commands on the server remotely.

**(Note: This project is a learning tool designed to demonstrate the basics of socket programming and TCP connections. It is not intended to function as a fully-featured SSH server for deployment.)**

## Features

- **TCP-based Communication:** Server and client communicate over TCP.
- **User Authentication:** Users must provide a valid username and password.
- **Command Execution:** Clients can execute shell commands on the server.
- **Secure Communication:** Supports basic authentication and secure command execution.

## Prerequisites

- **C++ Compiler:** Ensure you have a C++ compiler (e.g., g++) installed. Use these blogs to download C++ compiler [[Windows]](https://www.freecodecamp.org/news/how-to-install-c-and-cpp-compiler-on-windows/) [[Linux]](https://data-flair.training/blogs/install-cpp/)

## Setup Instructions

### 1. Run the server

1.  Compile the server

    ```
    g++ server.cpp -o server
    ```

2.  Start the server

    ```
    ./server
    ```

**(The server will now listen for incoming connections on port `12345`)**

### 2. Run the Client

1.  Compile the client

    ```
    g++ client.cpp -o client
    ```

2.  Start the server

    ```
    ./client
    ```

**(The client will connect to the server running on `127.0.0.1:12345`)**

### 3. Authentication Process

- **Enter Username:** The server will prompt for a username. Example: user1
- **Enter Password:** The server will prompt for a password. Example: password1

Upon successful authentication, you can execute commands on the server.

### 4. Commands

- **Run Commands:** Type any shell command to execute it on the server.
- **Exit:** Type `exit` to disconnect from the server.

## Troubleshooting

- **Port Conflicts:** If port **12345** is in use, modify the `SERVER_PORT` in both `server.cpp` and `client.cpp`.
- **Connection Issues:** Ensure that the server is running and reachable from the client.

## Future Enhancements

- **Encryption:** Implement SSL/TLS for secure communication.
- **Dynamic User Management:** Allow runtime addition/removal of users.
- **Multi-threading:** Handle multiple clients simultaneously using threading.

## Contributions

Contributions to SSH Server-Client Project are welcome! If you encounter any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request on the GitHub repository.

## Author

[Abhishek Singh Kushwaha](https://ask-03-portfolio.vercel.app/)
