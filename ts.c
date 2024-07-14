#include <ncurses.h>     // ncurses library for handling terminal screen output
#include <stdlib.h>      // Standard library for memory allocation, conversion, etc.
#include <string.h>      // Library for handling string operations
#include <unistd.h>      // POSIX API for handling sleep, pipe, and other system calls
#include <signal.h>      // Signal handling library
#include <errno.h>       // Library to provide error numbers
#include <time.h>        // Library for handling date and time operations
#include "logger.h"      // Custom logging library header

// Global variable to indicate if the terminal has been resized
volatile sig_atomic_t resize_flag = 0;

/**
 * Function: read_sensors
 * ----------------------
 * Executes the `sensors` command, which reads the hardware sensors data,
 * and stores the output in the provided buffer.
 *
 * buffer: Pointer to the buffer where the sensors' output will be stored.
 * size: Pointer to the size of the buffer.
 */
void read_sensors(char **buffer, size_t *size) {
    // Execute the `sensors` command using popen to read its output
    FILE *fp = popen("sensors", "r");
    if (fp == NULL) {
        // If the command fails, log the error into the buffer
        snprintf(*buffer, *size, "Error: Failed to run 'sensors' command: %s", strerror(errno));
        return;
    }

    // Initialize the capacity of the buffer and the current length of the output read
    size_t capacity = *size;
    size_t length = 0;
    char temp_buffer[1024];  // Temporary buffer to read chunks of the command's output

    // Read the command output line by line
    while (fgets(temp_buffer, sizeof(temp_buffer), fp) != NULL) {
        size_t temp_length = strlen(temp_buffer);
        // Check if the current buffer needs to be expanded
        if (length + temp_length >= capacity) {
            capacity *= 2;  // Double the capacity for the new buffer size
            *buffer = realloc(*buffer, capacity);  // Reallocate buffer with new size
            if (*buffer == NULL) {
                // If memory allocation fails, print an error and close the file pointer
                fprintf(stderr, "Memory allocation failed\n");
                pclose(fp);
                return;
            }
        }
        // Copy the temporary buffer content to the main buffer
        strcpy(*buffer + length, temp_buffer);
        length += temp_length;  // Update the length with the new data added
    }
    pclose(fp);  // Close the file pointer after reading is complete
    *size = capacity;  // Update the size pointer to the new capacity
}

/**
 * Signal handler for window resize (SIGWINCH).
 *
 * signum: Signal number (unused in this handler)
 */
void resize_handler(int signum) {
    resize_flag = 1;  // Set the resize flag indicating that the terminal size has changed
}

/**
 * Initializes ncurses library and terminal settings.
 * Sets up ncurses mode, disables cursor visibility, and sets non-blocking input mode.
 * Also sets up the signal handler for window resize.
 */
void initialize_ncurses() {
    initscr();  // Initialize ncurses mode
    curs_set(0);  // Hide the cursor
    nodelay(stdscr, TRUE);  // Set getch() to non-blocking mode
    signal(SIGWINCH, resize_handler);  // Set up signal handler for window resize
}

/**
 * Handles the terminal resize event.
 * If the terminal has been resized, it refreshes and clears the screen.
 */
void handle_resize() {
    if (resize_flag) {
        endwin();  // End ncurses mode to reinitialize
        refresh();  // Refresh the screen
        clear();  // Clear the screen content
        resize_flag = 0;  // Reset the resize flag
    }
}

/**
 * Displays the sensors' output on the terminal screen.
 *
 * sensors_output: The string containing the sensors' output to be displayed.
 * height: The height of the terminal window.
 * width: The width of the terminal window.
 */
void display_sensors_output(char *sensors_output, int height, int width) {
    clear();  // Clear the screen content
    int line_count = 0;  // Counter for the number of lines displayed
    char *line = strtok(sensors_output, "\n");  // Split the output by newline

    // Loop through each line and display it on the screen
    while (line != NULL && line_count < height - 2) {
        // Print the line on the screen, limiting to the terminal width
        mvprintw(line_count + 1, 0, "%.*s", width - 1, line);
        line = strtok(NULL, "\n");  // Get the next line
        line_count++;  // Increment the line count
    }
    refresh();  // Refresh the screen to display the changes
}

/**
 * Compares two strings of sensor data to determine if there have been changes.
 *
 * new_data: The new sensors' output data.
 * old_data: The previous sensors' output data.
 *
 * Returns 1 if the data has changed, 0 otherwise.
 */
int has_sensor_data_changed(const char *new_data, const char *old_data) {
    return strcmp(new_data, old_data) != 0;  // Compare the strings and return the result
}

int main(int argc, char *argv[]) {
    // Default update interval for reading sensors data
    int update_interval = 1;

    // Check if an update interval was provided as a command-line argument
    if (argc > 1) {
        update_interval = atoi(argv[1]);
        // Validate the update interval
        if (update_interval <= 0) {
            fprintf(stderr, "Invalid update interval: %d\n", update_interval);
            return 1;  // Exit with an error code if the interval is invalid
        }
    }

    initialize_ncurses();  // Initialize ncurses and terminal settings

    size_t buffer_size = 4096;  // Initial size for the sensors data buffer
    // Allocate memory for sensors output and previous output buffers
    char *sensors_output = malloc(buffer_size);
    char *previous_output = malloc(buffer_size);

    // Check if memory allocation was successful
    if (sensors_output == NULL || previous_output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        endwin();  // End ncurses mode
        return 1;  // Exit with an error code if memory allocation failed
    }

    memset(previous_output, 0, buffer_size);  // Initialize previous output buffer to zeros

    // Initialize the logger with a specified log file
    init_logger("sensor.log");

    while (1) {
        handle_resize();  // Check and handle terminal resize events

        int height, width;
        // Get the current dimensions of the terminal window
        getmaxyx(stdscr, height, width);

        // Read the sensors data and store it in the sensors_output buffer
        read_sensors(&sensors_output, &buffer_size);

        // Check if the sensors data has changed compared to the previous output
        if (has_sensor_data_changed(sensors_output, previous_output)) {
            // If data has changed, update the previous output buffer
            strcpy(previous_output, sensors_output);
            // Display the updated sensors output on the terminal screen
            display_sensors_output(sensors_output, height, width);

            // Example log entry (modify as per actual sensor output parsing)
            write_log(LOG_INFO, "device_name", "sensor_name", "sensor_value");
        }

        // Define the sleep interval for the main loop
        struct timespec req = {update_interval, 0};
        nanosleep(&req, NULL);  // Sleep for the defined interval

        // Check for user input to exit the loop
        if (getch() != ERR) {
            break;  // Exit the loop if any key is pressed
        }

    }

    free(sensors_output);  // Free the allocated memory for sensors output buffer
    free(previous_output);  // Free the allocated memory for previous output buffer
    endwin();  // End ncurses mode
    return 0;  // Exit the program successfully
}
