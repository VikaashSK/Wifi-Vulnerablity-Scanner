ESP32 Wi-Fi Risk Analyzer

This project uses an ESP32 to scan nearby Wi-Fi networks and provide a basic security analysis. The program does not connect to any Wi-Fi networks. It only scans and reports information through the serial monitor.

Features

Scans for all nearby Wi-Fi networks.

Shows:

SSID (network name)

BSSID (MAC address of the access point)

RSSI (signal strength)

Encryption type (Open, WEP, WPA, WPA2, WPA3)

Classifies networks as Low, Medium, or High risk.

Prints results in two formats:

Human-readable output.

JSON output for tools or further processing.

Supports a whitelist of trusted networks and a list of suspicious name patterns.

Requirements

ESP32 development board.

Arduino IDE with ESP32 board package installed.

USB cable for flashing and monitoring.

Setup

Clone or download this repository.

Open the code in Arduino IDE.

Select your ESP32 board in Tools > Board.

Upload the code.

Open Serial Monitor at 115200 baud to see results.

Example Output
Found 3 Wi-Fi networks:
1) Free_WiFi_Airport | RSSI: -60 dBm | Open | BSSID: 62:22:32:9B:35:2A
   -> Risk: High
2) MyHomeNetwork | RSSI: -40 dBm | WPA2 | BSSID: 82:14:67:AC:11:4B
   -> Risk: Low


JSON output:

{"ts":241172,"ssid":"Free_WiFi_Airport","bssid":"62:22:32:9B:35:2A","rssi":-60,"enc":"Open","risk":"High"}

Limitations

The project only uses passive scanning.

It cannot detect captive portals (for example, airport Wi-Fi that shows a login page after connecting).

Encryption type is only an indicator. A WPA2 network may still be unsafe if it is a fake hotspot.

BSSID helps distinguish real from fake networks, but advanced attackers can still spoof MAC addresses.

Future Work

Add captive portal detection by attempting active connection tests.

Create a dashboard or app to display results more clearly.

Add advanced risk detection techniques.
