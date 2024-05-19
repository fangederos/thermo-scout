import subprocess
import curses
import time
import signal

resize_flag = False

def read_sensors():
    try:
        # Execute the 'sensors' command and capture its output
        sensors_output = subprocess.check_output(['sensors'], encoding='utf-8')
        return sensors_output
    except subprocess.CalledProcessError:
        return "Error: Failed to run 'sensors' command."

def resize_handler(signum, frame):
    global resize_flag
    resize_flag = True

def main(stdscr):
    global resize_flag

    # Initialize curses
    curses.curs_set(0)
    stdscr.nodelay(1)  # Make getch() non-blocking

    # Set up signal handler for window resize
    signal.signal(signal.SIGWINCH, resize_handler)

    while True:
        if resize_flag:
            stdscr.clear()
            stdscr.refresh()
            resize_flag = False

        # Get terminal dimensions
        height, width = stdscr.getmaxyx()

        # Read sensor output
        sensors_output = read_sensors()

        # Clear the screen
        stdscr.clear()

        # Debug: Print the terminal dimensions and number of lines
        #stdscr.addstr(0, 0, f"Height: {height}, Width: {width}, Lines: {len(sensors_output.splitlines())}")

        # Ensure the output fits in the screen
        lines = sensors_output.splitlines()
        for i, line in enumerate(lines):
            if i < height - 2:  # Leave two lines for potential user input and debugging info
                stdscr.addstr(i + 1, 0, line[:width - 1])  # Ensure line fits in width

        # Refresh the screen
        stdscr.refresh()

        # Wait for a short interval before refreshing
        time.sleep(1)  # Adjust the refresh interval as needed

        # Break the loop if a key is pressed
        if stdscr.getch() != -1:
            break

if __name__ == "__main__":
    curses.wrapper(main)
