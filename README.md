Passive Wi‑Fi Scanner (ESP32)

A simple ESP32 sketch that scans nearby Wi‑Fi networks and prints basic info to the Serial Monitor. It does not connect to any networks.

Features

Shows SSID, BSSID, signal strength (RSSI), and encryption type.

Flags open networks as potentially vulnerable.

Alerts on possible evil twins (same SSID, different BSSID).

How to Use

Open in Arduino IDE.

Select ESP32 board and upload.

Open Serial Monitor at 115200 baud.

Watch the scan output every 5 seconds.

Notes

Open network ≠ always malicious; alerts are heuristic.

Same SSID on multiple BSSIDs can be normal (mesh/APs).

For awareness, testing, and educational purposes only.
