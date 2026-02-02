#define BLYNK_TEMPLATE_ID "TMPL34Wh926kz"
#define BLYNK_TEMPLATE_NAME "smart classroom"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

/* -------- WIFI AND BLYNK SETTINGS -------- */
char auth[] = "B8kkS8CkwuwGIvimt13ARy2qsAti2jbP"; 
char ssid[] = "Wokwi-GUEST";     
char pass[] = "";                

/* -------- PIN CONNECTIONS -------- */
#define DHT_PIN      18   // Temp & Humidity sensor
#define PIR_PIN      7    // Motion sensor
#define SMOKE_PIN    4    // Smoke sensor
#define RELAY_PIN    12   // Fan control
#define BUZZER_PIN   13   // Alarm sound
#define LED_PIN      15   // Classroom light

#define DHT_TYPE     DHT22
#define TEMP_LIMIT   25
#define HUM_LIMIT    50
#define SMOKE_LIMIT  1000

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;
int occupancyCount = 0;

/* -------- REMOTE BUTTONS (FROM APP) -------- */

// Button to turn Light ON/OFF manually
BLYNK_WRITE(V7) {
  int pinValue = param.asInt();
  digitalWrite(LED_PIN, pinValue);
  Serial.print("Remote Light Toggle: ");
  Serial.println(pinValue ? "ON" : "OFF");
}

// Manual Fan Control from Dashboard (V8)
BLYNK_WRITE(V8) {
  int pinValue = param.asInt();
  digitalWrite(RELAY_PIN, pinValue);
  Serial.print("Remote Fan Toggle: ");
  Serial.println(pinValue ? "ON" : "OFF");
}

/* -------- SEND DATA TO BLYNK -------- */
void pushTelemetry() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int s = analogRead(SMOKE_PIN);

  if (!isnan(t) && !isnan(h)) {
    Blynk.virtualWrite(V0, t); // Send Temp to V0
    Blynk.virtualWrite(V1, h); // Send Humidity to V1
  }
  Blynk.virtualWrite(V2, s);   // Send Smoke value to V2
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Blynk.begin(auth, ssid, pass);

  // Send sensor data to the app every 2 seconds
  timer.setInterval(2000L, pushTelemetry);
}

void loop() {
  Blynk.run(); // Keep Blynk connected
  timer.run(); // Keep the timer running

  // 1. AUTOMATIC FAN CONTROL
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  if (!isnan(t) && (t > TEMP_LIMIT || h > HUM_LIMIT)) {
    digitalWrite(RELAY_PIN, HIGH); // Turn Fan ON
    Serial.println("Fan ON");
    delay(5000); 
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Turn Fan OFF
    Serial.println("Fan OFF");
    delay(1000); 
  }

  // 2. MOTION AND LIGHT CONTROL
  int motion = digitalRead(PIR_PIN);
  
  if (motion == HIGH) {
    Serial.println("Motion Detected - Light ON");
    digitalWrite(LED_PIN, HIGH);
    delay(3000);   // Maintain light for visibility
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("No motion - Energy Saving Mode");
    delay(100);
  }
    // Count how many times motion was detected
    occupancyCount++;
    Blynk.virtualWrite(V5, occupancyCount); // Update count on Blynk
    
    delay(10000); // Keep delay of 10 seconds

  // 3. SMOKE ALARM
  int smokeVal = analogRead(SMOKE_PIN);
  if (smokeVal > SMOKE_LIMIT) {
    tone(BUZZER_PIN, 1000); // Start alarm sound
    Serial.println("SMOKE ALERT!");
  } else {
    noTone(BUZZER_PIN);    // Stop alarm sound
  }
}
