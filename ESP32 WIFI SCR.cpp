/*
  ESP32 Wi-Fi Scanner (Safe)
  - Finds nearby Wi-Fi networks
  - Shows security type (Open/WEP/WPA/WPA2 etc.)
  - Marks if the Wi-Fi is risky or safe
  - Does NOT connect to any Wi-Fi
  - Prints info to Serial Monitor + JSON for parsing
*/

#include <WiFi.h>

const unsigned long SCAN_INTERVAL_MS = 5000; // time gap between scans
const int LED_PIN = 2;                       // LED pin (built-in LED)

// Your safe Wi-Fi names (won’t be marked risky)
const char* whitelist[] = {
  "HomeNetwork",
  "OfficeWiFi",
  "MyTrustedAP"
};
const int WHITELIST_COUNT = sizeof(whitelist) / sizeof(whitelist[0]);

// Words often seen in fake/open Wi-Fi names
const char* suspiciousPatterns[] = {
  "Free",
  "FreeWiFi",
  "Guest",
  "Open",
  "Hotspot",
  "Public",
  "WiFi"
};
const int SUSPICIOUS_COUNT = sizeof(suspiciousPatterns) / sizeof(suspiciousPatterns[0]);

// Turn MAC (BSSID) into text "AA:BB:CC:DD:EE:FF"
String macToString(uint8_t* mac) {
  char buf[18];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

// Check if SSID is safe (in whitelist)
bool isWhitelisted(const String &ssid) {
  for (int i = 0; i < WHITELIST_COUNT; ++i) {
    if (ssid == String(whitelist[i])) return true;
  }
  return false;
}

// Show Wi-Fi encryption as text
String encTypeToString(wifi_auth_mode_t t) {
  switch (t) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-E";
    default: return "Unknown";
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("ESP32 Wi-Fi Risk Scanner");
  Serial.println("Scanning every " + String(SCAN_INTERVAL_MS / 1000) + "s");
  Serial.println("-------------------------------------------------------");
}

void loop() {
  // Scan nearby Wi-Fi
  int n = WiFi.scanNetworks(false, true);
  unsigned long ts = millis();

  if (n <= 0) {
    Serial.println("No Wi-Fi found.");
  } else {
    Serial.printf("Found %d Wi-Fi networks:\n", n);
    for (int i = 0; i < n; ++i) {
      String ssid = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);                // signal strength
      wifi_auth_mode_t auth = WiFi.encryptionType(i);
      String enc = encTypeToString(auth);
      String bssidStr = macToString(WiFi.BSSID(i));

      // Decide risk
      String risk = "Low";
      if (auth == WIFI_AUTH_OPEN) risk = "High";  // Open = risky
      else if (auth == WIFI_AUTH_WEP) risk = "Medium"; // WEP = weak

      if (isWhitelisted(ssid)) {
        risk = "Low"; // Safe network
      } else {
        for (int p = 0; p < SUSPICIOUS_COUNT; ++p) {
          if (ssid.indexOf(suspiciousPatterns[p]) >= 0) {
            if (auth == WIFI_AUTH_OPEN || auth == WIFI_AUTH_WEP) {
              risk = "High"; // Open + suspicious = very risky
            } else if (risk != "High") {
              risk = "Medium";
            }
            break;
          }
        }
      }

      // Print readable info
      Serial.printf("%d) %s | RSSI: %d dBm | %s | BSSID: %s\n", 
                    i + 1, ssid.c_str(), rssi, enc.c_str(), bssidStr.c_str());
      Serial.printf("   -> Risk: %s\n", risk.c_str());

      // Print JSON for tools
      String json = String("{\"ts\":") + String(ts) +
                    String(",\"ssid\":\"") + ssid + "\"" +
                    String(",\"bssid\":\"") + bssidStr + "\"" +
                    String(",\"rssi\":") + String(rssi) +
                    String(",\"enc\":\"") + enc + "\"" +
                    String(",\"risk\":\"") + risk + "\"}";
      Serial.print("JSON:");
      Serial.println(json);
    }
  }

  Serial.println("-------------------------------------------------------");

  // Blink LED if risky Wi-Fi is around
  bool anyHigh = false;
  for (int i = 0; i < n; ++i) {
    if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
      anyHigh = true;
      break;
    }
  }
  if (anyHigh) {
    for (int k = 0; k < 3; ++k) {
      digitalWrite(LED_PIN, HIGH); delay(120);
      digitalWrite(LED_PIN, LOW); delay(100);
    }
  } else {
    digitalWrite(LED_PIN, HIGH); delay(80);
    digitalWrite(LED_PIN, LOW);
  }

  WiFi.scanDelete();              // clear memory
  delay(SCAN_INTERVAL_MS);        // wait before next scan
}
