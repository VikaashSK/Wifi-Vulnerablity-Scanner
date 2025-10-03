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

    for (int j = i + 1; j < n; j++) {
      if (ssid == WiFi.SSID(j) && bssid != WiFi.BSSIDstr(j)) {
        Serial.println(" -> ALERT: Possible Evil Twin (Same SSID, different BSSID)");
      }
    }
  }

  Serial.println("-----------------------");
  WiFi.scanDelete();
  delay(SCAN_INTERVAL);
}
