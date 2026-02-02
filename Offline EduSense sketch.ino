// https://wokwi.com/projects/454308317695279105
// demo link for online version 


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

/* -------- SYSTEM CONFIGURATION -------- */
#define I2C_SDA     8    
#define I2C_SCL     9    
#define DHT_PIN     18   
#define PIR_PIN     7    
#define SMOKE_PIN   4    
#define RELAY_PIN   12   // Controls fans (relay used for simulation)
#define BUZZER_PIN  13   // Audible alarm for safety critical alerts
#define LED_PIN     15   // Visual feedback for occupancy

#define DHT_TYPE    DHT22
#define SMOKE_LIMIT 1000  // Threshold for automated smoke control
#define TEMP_LIMIT  25   // Threshold for automated temperature control
#define HUM_LIMIT   50  // Threshold for automated humidity control

// Initialize Hardware Interfaces

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C communication for LCD

  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  dht.begin();

  // Define I/O for sensors 

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // System Health Check
  lcd.print("Initializing...");
  delay(2000); // Sensor stabilisation 
  lcd.clear();
}

void loop() {
  /* -------- DATA ACQUISITION -------- */

  int smokeValue = analogRead(SMOKE_PIN);
  int motion = digitalRead(PIR_PIN);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  /* -------- MOTION & LIGHTING LOGIC -------- */
  // Implements occupancy-based energy saving

  if (motion == HIGH) {
    Serial.println("Motion Detected - Light ON");
    digitalWrite(LED_PIN, HIGH);
    delay(3000);   // Maintain light for visibility
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("No motion - Energy Saving Mode");
    delay(100);
  }

  /* -------- CLIMATE CONTROL LOGIC -------- */
  // Validation check: Ensure sensor data is valid before processing

  if (!isnan(temp) && !isnan(hum)) {
    Serial.print("Temp: "); Serial.print(temp);
    Serial.print(" | Hum: "); Serial.println(hum);

    // Automated Cooling Trigger (Using Relay): Activates if Temp OR Humidity exceeds safety limits

    if (temp > TEMP_LIMIT || hum > HUM_LIMIT) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("FANS - ON ");
      delay(5000);  // Prevents rapid relay cycling (chatter)
    } else {
      digitalWrite(RELAY_PIN, LOW);   
    }
  } 

  /* -------- USER INTERFACE & SAFETY ALERTS -------- */
  
  // Row 0: Environmental Telemetry

  lcd.setCursor(0, 0);
  if (isnan(temp) || isnan(hum)) {
    lcd.print("Sensor Error   "); 
  } else {
    lcd.print("T:"); lcd.print(temp, 1);
    lcd.print("C  H:"); lcd.print(hum, 0);
    lcd.print("% ");
  }

  // Row 1: Safety Critical Status
  lcd.setCursor(0, 1);
  if (smokeValue > SMOKE_LIMIT) {
    lcd.print("ALERT: SMOKE!   ");
    tone(BUZZER_PIN, 1000); // High-frequency emergency tone
  } else {
    lcd.print("Status: Safe    ");
    noTone(BUZZER_PIN);
  }

  // Frequency control for DHT22 stability
  delay(2000); 
}


