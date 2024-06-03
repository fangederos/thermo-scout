# thermo-scout 1.1.0

###   WARNING! *Be sure open it to run it on a maximized window.* Current build struggles with formatting due to variance in window sizes.

thermo scout calls the [`sensors`](https://www.commandlinux.com/man-page/man1/sensors.1.html) command (part of the [`lm-sensors`](https://archlinux.org/packages/?name=lm_sensors) package) to access and display real-time temperature sensor data retrieved from hardware monitoring sensors integrated into computer components such as CPU cores, motherboard, GPUs, and other peripheral devices. The aim of Thermo Scout is to provides a convenient interface to monitor and visualize these temperature readings in real-time within the terminal environment.

### Prerequisites

- Most (if not any) Linux distribution will do.
- Python 3.x installed on your system.
- The [`lm-sensors`](https://archlinux.org/packages/?name=lm_sensors) package installed to enable temperature monitoring.
- `sensors-detect` is a `lm-sensors` command that must be run beforehand, as it will scan to find the sensors in your hardware. **Go through the execution of `sensors-detect` very carefully, as some of the scanning options have been known to cause issues in some computers. For more, refer to the Arch Wiki's `lm-sensors` page.

### Installation and Usage

1. Clone the repository:
   ```
   git clone https://github.com/matheuslazzarotto/thermo-scout.git
   ```
1. Go to the executable's directory and run it:
   ```
   cd thermo-scout/dist
   ./thermo-scout
   ```
The script will continuously display temperature readings in your terminal window. Use `Ctrl+C` to terminate the process.

### Customization

- You can adjust the refresh interval in the `main()` function of the `thermo-scout.py` script by changing the value passed to `time.sleep()`.

### Troubleshooting

- If you encounter any issues with the script, ensure that the `sensors` command is working correctly on your system.

### Version History

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

1- In effort to keep thermo scout a lightweight program, I will convert the entire codebase (or as much as possible) to C. The current python build is so bloated that the executable is actually larger than the core program itself.
2- Proper formatting of the information displayed, instead of merely printing the output of `sensors`, which is not exactly very human-readable.
3- Implement TUI and GUI for smoother use-experience.

### [GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.en.html)
