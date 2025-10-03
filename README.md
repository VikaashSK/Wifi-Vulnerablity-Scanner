Passive Wi‑Fi Scanner (ESP32)

This is a small, passive Wi‑Fi scanner for the ESP32.
It listens for nearby Wi‑Fi access points and prints basic details to the Serial Monitor. It does not connect to any networks.

What it does

Scans nearby Wi‑Fi networks (passive scan).

For each network it shows:

SSID (network name)

BSSID (access point MAC address)

RSSI (signal strength in dBm)

Basic encryption label (Open / Secure)

Flags and prints simple alerts:

Open network (Vulnerable) — link‑layer is open (no Wi‑Fi encryption). This is a possible issue; an open network might be a captive portal or intentionally open.

Possible Evil Twin (Same SSID, different BSSID) — same network name seen from different access point MACs in the same scan.

Why this is passive and safe

The program only listens to Wi‑Fi beacons. It does not attempt to join or send data to any Wi‑Fi network.

Alerts are heuristic indicators for manual follow-up, not definitive proof that a network is malicious.

Requirements

ESP32 development board

Arduino IDE (or PlatformIO) with ESP32 board support installed

USB cable to flash the board and open the serial monitor

How to run

Open Arduino IDE.

Select your ESP32 board under Tools > Board.

Copy the sketch into the IDE (or open the provided .ino file).

Upload to your ESP32.

Open the Serial Monitor at 115200 baud.

Walk around or place the board in the area you want to scan. Results will print every few seconds.

Serial monitor example
SSID: MyHomeNetwork | BSSID: AA:BB:CC:11:22:33 | Signal: -42 dBm | Enc: Secure
SSID: Free_Cafe_WiFi | BSSID: 44:55:66:77:88:99 | Signal: -68 dBm | Enc: Open
 -> ALERT: Open network (Vulnerable)
SSID: Free_Cafe_WiFi | BSSID: 66:55:44:33:22:11 | Signal: -80 dBm | Enc: Open
 -> ALERT: Possible Evil Twin (Same SSID, different BSSID)
-----------------------

Notes and limitations

Open ≠ always malicious. An open AP may be a captive portal (requires a web login) or intentionally open. The scanner reports potential issues for manual checking.

Evil twin detection is heuristic. Many legitimate networks use multiple APs with the same SSID (mesh or enterprise deployments). Alerts can be false positives.

This tool is for awareness, testing, education, and field surveys. Do not use it to perform active probing or automatic connections to unknown networks without consent.

Suggested improvements (optional)

Add a small history buffer and only alert after the condition appears in consecutive scans (reduces false positives).

Save logs as CSV/JSON for later analysis.

Add channel information or basic statistics about channel congestion.
