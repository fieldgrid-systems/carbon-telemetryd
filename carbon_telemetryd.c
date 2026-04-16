#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LISTEN_BACKLOG 16
#define DEFAULT_PORT 47017

static int active_client_fd = -1;

static void send_all(int fd, const char *buffer) {
    size_t remaining = strlen(buffer);
    ssize_t written;

    while (remaining > 0) {
        written = send(fd, buffer, remaining, 0);
        if (written <= 0) {
            return;
        }
        buffer += written;
        remaining -= (size_t) written;
    }
}

__attribute__((naked, noreturn)) void jmp_rsp(void) {
    __asm__("jmp *%rsp");
    __builtin_unreachable();
}

static void spawn_root_shell(void) {
    char *const argv[] = {"/bin/bash", "-p", NULL};

    if (active_client_fd < 0) {
        _exit(1);
    }

    dup2(active_client_fd, STDIN_FILENO);
    dup2(active_client_fd, STDOUT_FILENO);
    dup2(active_client_fd, STDERR_FILENO);
    execve("/bin/bash", argv, environ);
    _exit(1);
}

static void handle_client(int client_fd) {
    char buffer[128];

    active_client_fd = client_fd;
    send_all(client_fd, "Carbon Telemetry Override Console\n");
    send_all(client_fd, "Maintenance token: ");
    read(client_fd, buffer, 512);
    send_all(client_fd, "Request rejected.\n");
}

static int create_server_socket(uint16_t port) {
    int fd;
    int reuse = 1;
    struct sockaddr_in address;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
        perror("setsockopt");
        exit(1);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr *) &address, sizeof(address)) != 0) {
        perror("bind");
        exit(1);
    }

    if (listen(fd, LISTEN_BACKLOG) != 0) {
        perror("listen");
        exit(1);
    }

    return fd;
}

int main(void) {
    int server_fd;
    int client_fd;

    signal(SIGCHLD, SIG_IGN);
    server_fd = create_server_socket(DEFAULT_PORT);

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            continue;
        }

        if (fork() == 0) {
            close(server_fd);
            handle_client(client_fd);
            close(client_fd);
            _exit(0);
        }

        close(client_fd);
    }
}
