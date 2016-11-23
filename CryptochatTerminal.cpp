#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <string>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "AfterCharStdioFlusher.hpp"

int main(int argc, char **argv)
{
//     AfterCharStdioFlusher flusher;

//     int timer = timerfd_create(CLOCK_MONOTONIC, 0);
//     if (timer < 0) {
//         perror("timer");
//         return 1;
//     }

//     struct itimerspec ts = {
//         .it_interval = {
//             .tv_sec = 3,
//             .tv_nsec = 0
//         },
//         .it_value = {
//             .tv_sec = 3,
//             .tv_nsec = 0
//         }
//     };
//     timerfd_settime(timer, 0, &ts, NULL);

//     std::string input;

//     write(1, ">> ", sizeof(">> "));
//     while (1) {
//         fd_set fds;
//         FD_ZERO(&fds);
//         FD_SET(0, &fds);
//         FD_SET(timer, &fds);
//         const int max_fd = timer;
//         select(max_fd + 1, &fds, NULL, NULL, NULL);
//         if (FD_ISSET(0, &fds)) {
//             char c;
//             read(0, &c, sizeof(c));

//             switch (c) {
//                 case '.':
//                     goto end;
//                 case '\n':
//                     write(1, &c, sizeof(c));
//                     write(1, ">> ", sizeof(">> "));
//                     input.clear();
//                     break;
//                 default:
//                     write(1, &c, sizeof(c));
//                     input += c;
//             }
//         } else if (FD_ISSET(timer, &fds)) {
//             uint64_t x;
//             read(timer, &x, sizeof(x));
//             write(1, "\r", 1);
//             for (int i = 0; i < input.size() + 3; ++i) {
//                 write(1, " ", 1);
//             }
//             write(1, "\r>> ", sizeof("\r>> "));
//             write(1, "yo\n>> ", sizeof("yo\n>> "));
//             write(1, input.c_str(), input.size());
//         }
//     }

// end:
//     close(timer);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("socket server");
        return 1;
    }

    struct sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 0;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind server");
        return 2;
    }

    if (listen(server, 5) < 0) {
        perror("lister server");
        return 3;
    }

    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        perror("socket client");
        return 4;
    }
    struct sockaddr_in client_addr = { 0 };
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = 0;
    client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(client, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind client");
        return 5;
    }

    socklen_t len;
    if (getsockname(server, (struct sockaddr *)&server_addr, &len) < 0) {
        perror("getsockname server");
        return 7;
    }

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect client");
        return 6;
    }

    while (1) {
        if (server == -1 && client == -1) {
            break;
        }
        fd_set fds;
        FD_ZERO(&fds);
        if (server != -1) {
            FD_SET(server, &fds);
        }

        if (client != -1) {
            FD_SET(client, &fds);
        }
        const int max_fd = server > client ? server : client;
        select(max_fd + 1, &fds, NULL, NULL, NULL);
        if (FD_ISSET(server, &fds)) {
            struct sockaddr_in addr;
            socklen_t len;
            int cl = accept(server, (struct sockaddr *)&addr, &len);
            write(cl, "sup1", sizeof("sup1") - 1);
            sleep(1);
            write(cl, "sup2", sizeof("sup2"));
            close(cl);
            //close(server);
            //server == -1;
        } else if (FD_ISSET(client, &fds)) {
            static int state = 0;
            char buf[100];
            read(client, buf, sizeof(buf));
            printf("%s\n", buf);
            close(client);
            client == -1;
        }
    }

    return 0;
}
