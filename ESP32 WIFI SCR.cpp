#include <WiFi.h>

const unsigned long SCAN_INTERVAL = 5000;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop() {
  int n = WiFi.scanNetworks(false, true);

  if (n <= 0) {
    Serial.println("No Wi-Fi found.");
    delay(SCAN_INTERVAL);
    return;
  }

  for (int i = 0; i < n; i++) {
    String ssid = WiFi.SSID(i);
    String bssid = WiFi.BSSIDstr(i);
    int rssi = WiFi.RSSI(i);
    wifi_auth_mode_t enc = WiFi.encryptionType(i);
    String encStr = (enc == WIFI_AUTH_OPEN) ? "Open" : "Secure";

    Serial.print("SSID: "); Serial.print(ssid);
    Serial.print(" | BSSID: "); Serial.print(bssid);
    Serial.print(" | Signal: "); Serial.print(rssi);
    Serial.print(" dBm | Enc: "); Serial.println(encStr);

    if (enc == WIFI_AUTH_OPEN) {
      Serial.println(" -> ALERT: Open network (Vulnerable)");
    }

    // Check for Evil Twin only once
    bool evilTwinFound = false;
    for (int j = i + 1; j < n; j++) {
      String otherSSID = WiFi.SSID(j);
      String otherBSSID = WiFi.BSSIDstr(j);
      if (ssid == otherSSID && bssid != otherBSSID) {
        evilTwinFound = true;
        break; // stop checking further for this SSID
      }
    }

    if (evilTwinFound) {
      Serial.print(" -> ALERT: Possible Evil Twin detected for SSID: ");
      Serial.println(ssid);
    }
  }

  Serial.println("-----------------------");
  WiFi.scanDelete();
  delay(SCAN_INTERVAL);
}
