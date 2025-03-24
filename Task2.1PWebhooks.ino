#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Wi-Fi Credentials
const char ssid[] = "SS"; // Wi-Fi Network Name
const char pass[] = "Barry_Belgrave"; // Wi-Fi Password (Keep Confidential)

WiFiClient client;

// ThingSpeak Channel Information
const unsigned long channelID = 2880425;
const char *writeAPIKey = "MTC9P96FQLD2BVOG";

// DHT Sensor Configuration
#define DHT_PIN 2        // Pin connected to the DHT sensor
#define DHT_TYPE DHT11   // Type of DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();

    // Establish Wi-Fi connection
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Connected!");

    // Initialize ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Validate sensor readings
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Error: Unable to read from DHT sensor.");
        return;
    }

    // Display sensor values
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");

    // Send data to ThingSpeak
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    
    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (response == 200) {
        Serial.println("Data successfully sent to ThingSpeak.");
    } else {
        Serial.println("Error: Failed to send data to ThingSpeak.");
    }

    // Wait for the next update
    delay(60000); // Send data every 60 seconds
}