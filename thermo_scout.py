import subprocess
import time

def read_sensors():
    try:
        # Execute the 'sensors' command and capture its output
        sensors_output = subprocess.check_output(['sensors']).decode('utf-8')
        return sensors_output
    except subprocess.CalledProcessError:
        return "Error: Failed to run 'sensors' command."

def main():
    while True:
        # Clear the screen
        print("\033c", end="")

        # Read and print sensor output
        print(read_sensors())

        # Wait for a short interval before refreshing
        time.sleep(0.1)  # Adjust the refresh interval as needed

if __name__ == "__main__":
    main()

