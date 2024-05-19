# Temperature Monitor 1.0.0

## Overview

Thermo Scout is a Python script designed to display real-time temperature readings from various parts of a computer using the `sensors` command. Currently, the script continuously refreshes the screen to show updated temperature information.

## Features

- Displays temperature readings from the `sensors` command.

### Prerequisites

- Python 3.x installed on your system.
- The `lm-sensors` package installed to enable temperature monitoring.

### Installation

1. Clone the repository:
   ```
   git clone https://github.com/matheuslazzarotto/thermo-scout.git
   ```
2. Navigate to the project directory:
   ```
   cd thermo-scout
   ```

### Usage

1. Run the script:
   ```
   python thermo-scout.py
   ```
2. The script will continuously display temperature readings in your terminal window.
3. Use Ctrl+C to terminate the process.

### Customization

- You can adjust the refresh interval in the `main()` function of the `temperature_monitor.py` script by changing the value passed to `time.sleep()`.
- For any additional customizations, you can modify the script as needed.

## Troubleshooting

- If you encounter any issues with the script, ensure that the `sensors` command is working correctly on your system.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
