import subprocess
import curses
import time

def read_sensors():
    try:
        # Execute the 'sensors' command and capture its output
        sensors_output = subprocess.check_output(['sensors'], encoding='utf-8')
        return sensors_output
    except subprocess.CalledProcessError:
        return "Error: Failed to run 'sensors' command."

def main(stdscr):
    # Initialize curses
    curses.curs_set(0)
    stdscr.nodelay(1)  # Make getch() non-blocking

    while True:
        # Get terminal dimensions
        height, width = stdscr.getmaxyx()

        # Read sensor output
        sensors_output = read_sensors()

        # Clear the screen
        stdscr.clear()

        # Ensure the output fits in the screen
        lines = sensors_output.splitlines()
        for i, line in enumerate(lines):
            if i < height - 1:  # Leave a line for potential user input
                stdscr.addstr(i, 0, line[:width - 1])  # Ensure line fits in width

        # Refresh the screen
        stdscr.refresh()

        # Wait for a short interval before refreshing
        time.sleep(1)  # Adjust the refresh interval as needed

        # Break the loop if a key is pressed
        if stdscr.getch() != -1:
            break

if __name__ == "__main__":
    curses.wrapper(main)
