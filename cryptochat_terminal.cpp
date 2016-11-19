#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <string>

#include <sys/timerfd.h>
#include <unistd.h>

#include "AfterCharStdioFlusher.hpp"

int main(int argc, char **argv)
{
    AfterCharStdioFlusher flusher;

    int timer = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timer < 0) {
        perror("timer");
        return 1;
    }

    struct itimerspec ts = {
        .it_interval = {
            .tv_sec = 3,
            .tv_nsec = 0
        },
        .it_value = {
            .tv_sec = 3,
            .tv_nsec = 0
        }
    };
    timerfd_settime(timer, 0, &ts, NULL);

    std::string input;

    write(1, ">> ", sizeof(">> "));
    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(timer, &fds);
        const int max_fd = timer;
        select(max_fd + 1, &fds, NULL, NULL, NULL);
        if (FD_ISSET(0, &fds)) {
            char c;
            read(0, &c, sizeof(c));

            switch (c) {
                case '.':
                    goto end;
                case '\n':
                    write(1, &c, sizeof(c));
                    write(1, ">> ", sizeof(">> "));
                    input.clear();
                    break;
                default:
                    write(1, &c, sizeof(c));
                    input += c;
            }
        } else if (FD_ISSET(timer, &fds)) {
            uint64_t x;
            read(timer, &x, sizeof(x));
            write(1, "\r", 1);
            for (int i = 0; i < input.size() + 3; ++i) {
                write(1, " ", 1);
            }
            write(1, "\r>> ", sizeof("\r>> "));
            write(1, "yo\n>> ", sizeof("yo\n>> "));
            write(1, input.c_str(), input.size());
        }
    }

end:
    close(timer);
    return 0;
}

