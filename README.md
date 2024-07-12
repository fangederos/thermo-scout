# thermo-scout 1.3.0

`thermo-scout` calls the [`sensors`](https://www.commandlinux.com/man-page/man1/sensors.1.html) command (part of the [`lm-sensors`](https://archlinux.org/packages/?name=lm_sensors) package) to access and display real-time temperature sensor data retrieved from hardware monitoring sensors integrated into computer components such as CPU cores, motherboard, GPUs, and other peripheral devices. The aim of `thermo-scout` is to provide a convenient interface to monitor and visualize these temperature readings in real-time within the terminal environment.

### Prerequisites

- The [`lm-sensors`](https://archlinux.org/packages/?name=lm_sensors) package installed to enable temperature monitoring.
- `sensors-detect` is a `lm-sensors` command that must be run beforehand, as it will scan to find the sensors in your hardware. **Go through the execution of `sensors-detect` very carefully, as some of the scanning options have been known to cause issues in some computers. For more, refer to the Arch Wiki's `lm-sensors` page.

### Installation and Usage

1. **Clone the repository**:
   ```sh
   git clone https://github.com/matheuslazzarotto/thermo-scout.git
   ```

2. **Navigate to the directory, compile the program, and run it**:
   ```sh
   cd thermo-scout/
   make
   ./ts
   ```

The program will continuously display temperature readings in your terminal window. Use `Ctrl+C` to terminate the process. You can serve an integer as an argument to define the interval of display.

### Logging Functionality

- A log file is created upon execution, with the name format `[YYYY-MM-DD_HH-MM-SS]_sensor.log`.
- The log entries include local date and time (adhering to ISO 8601), levels of severity (INFO, WARNING, ERROR), the name of the device, the sensors of that device, and the value the sensors display, formatted as:
  ```
  [timestamp]::[SEVERITY]::[device]-[sensor]: [value]
  ```
- Log rotation involves periodically archiving and deleting old log files.

### Customization

- You can adjust the refresh interval in the `main()` function of the `main.c` script by changing the value passed to the `nanosleep()` function.

### Troubleshooting

- If you encounter any issues with the script, ensure that the `sensors` command is working correctly on your system.
- Ensure that your system has necessary permissions for creating and writing log files.

### Version History

- **1.3.0**:
  - Added logging functionality with log rotation.
  - Created a new logger.c file for logging-related operations.

- **1.2.0**:
  - Entire codebase converted to C.

- **1.1.0**:
  - Reduced sleep value for faster screen refresh.
  - Fixed unintended flickering screen behavior by adding the curses library.
  - Fixed issue where the program crashed upon resizing the window.
  - Added a launcher for easier execution.

### Useful Links

- [lm_sensors - ArchWiki](https://wiki.archlinux.org/title/lm_sensors)
- [lm_sensors - HWMon Wiki](https://hwmon.wiki.kernel.org/lm_sensors)
- [lm-sensors - GitHub](https://github.com/lm-sensors/lm-sensors)

### To-do
- Fix and fully implement logging.
- Proper formatting of the information displayed, instead of merely printing the output of `sensors`, which is not exactly very human-readable.
- Add graphs to the log file.
- Implement TUI and GUI for smoother user experience.

