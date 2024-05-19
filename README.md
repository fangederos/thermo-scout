# thermo-scout 1.1.0
thermo-scout displays real-time temperature readings from various parts of a computer using the `sensors` command.
### Features

- Displays the output of the `sensors` command.

### Prerequisites

- Python 3.x installed on your system.
- The [`lm-sensors`](https://github.com/lm-sensors/lm-sensors) package installed to enable temperature monitoring.

### Installation

1. Clone the repository:
   ```
   git clone https://github.com/matheuslazzarotto/thermo-scout.git
   ```
### Usage

1. Go to the executable's directory and run it:
   ```
   cd thermo-scout/dist
   ./thermo-scout
   ```
The script will continuously display temperature readings in your terminal window. Use Ctrl+C to terminate the process.

### Customization

- You can adjust the refresh interval in the `main()` function of the `thermo_scout.py` script by changing the value passed to `time.sleep()`.
- For any additional customizations, you can modify the script as needed.

### Troubleshooting

- If you encounter any issues with the script, ensure that the `sensors` command is working correctly on your system.

### Version History

- **1.1.0**:
- Reduced sleep value for faster screen refresh.
- Fixed unintended flickering screen behavior by adding the curses library.
- Fixed issue where the program crashed upon resizing the window.
- Added a launcher for easier execution.
