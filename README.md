# thermo-scout 1.1.0
Thermo Scout utilizes the `sensors` command, which is part of the [`lm-sensors`](https://github.com/lm-sensors/lm-sensors) package, to access and display real-time temperature sensor data retrieved from hardware monitoring sensors integrated into computer components. This command gathers temperature readings from sensors embedded in the CPU, motherboard, graphics card, and other peripheral devices. Thermo Scout provides a convenient interface to monitor and visualize these temperature readings in real-time within the terminal environment. You can more information on `lm-sensors` [here](https://hwmon.wiki.kernel.org/lm_sensors?s[]=lm-sensors).

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
