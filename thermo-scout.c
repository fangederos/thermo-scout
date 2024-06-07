#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t resize_flag = 0;

void read_sensors(char *buffer, size_t size) {
    FILE *fp;
    fp = popen("sensors", "r");
    if (fp == NULL) {
        snprintf(buffer, size, "Error: Failed to run 'sensors' command.");
        return;
    }
    size_t offset = 0;
    while (fgets(buffer + offset, size - offset, fp) != NULL) {
        offset = strlen(buffer);
    }
    pclose(fp);
}

void resize_handler(int signum) {
    resize_flag = 1;
}

int main() {
    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);
    signal(SIGWINCH, resize_handler);

    while (1) {
        if (resize_flag) {
            endwin();
            refresh();
            clear();
            resize_flag = 0;
        }

        int height, width;
        getmaxyx(stdscr, height, width);

        char sensors_output[2048];
        read_sensors(sensors_output, sizeof(sensors_output));

        clear();

        int line_count = 0;
        char *line = strtok(sensors_output, "\n");
        while (line != NULL && line_count < height - 2) {
            mvprintw(line_count + 1, 0, "%.*s", width - 1, line);
            line = strtok(NULL, "\n");
            line_count++;
        }

        refresh();
        sleep(1);

        if (getch() != ERR) {
            break;
        }
    }

    endwin();
    return 0;
}

