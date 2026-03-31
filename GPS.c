#include <SoftwareSerial.h>

// GPS Module
SoftwareSerial gpsSerial(4, 3); // RX, TX

// GSM Module
SoftwareSerial gsmSerial(10, 9); // RX, TX

String latitude = "";
String longitude = "";

int buttonPin = 2;

void setup()
{
    Serial.begin(9600);
    gpsSerial.begin(9600);
    gsmSerial.begin(9600);

    pinMode(buttonPin, INPUT_PULLUP);

    Serial.println("System Ready...");
}

void loop()
{
    // Read GPS Data
    while (gpsSerial.available())
    {
        String data = gpsSerial.readStringUntil('\n');

        // Check for GPGGA sentence (contains location)
        if (data.startsWith("$GPGGA"))
        {
            parseGPS(data);
        }
    }

    // Emergency Button Press
    if (digitalRead(buttonPin) == LOW)
    {
        sendSMS();
        delay(5000); // prevent multiple messages
    }
}

// Function to extract latitude & longitude
void parseGPS(String data)
{
    int commaIndex[15];
    int index = 0;

    for (int i = 0; i < data.length(); i++)
    {
        if (data[i] == ',')
        {
            commaIndex[index++] = i;
        }
    }

    // Extract raw values
    String lat = data.substring(commaIndex[1] + 1, commaIndex[2]);
    String latDir = data.substring(commaIndex[2] + 1, commaIndex[3]);

    String lon = data.substring(commaIndex[3] + 1, commaIndex[4]);
    String lonDir = data.substring(commaIndex[4] + 1, commaIndex[5]);

    latitude = lat;
    longitude = lon;

    Serial.print("Lat: ");
    Serial.print(latitude);
    Serial.print(" Lon: ");
    Serial.println(longitude);
}

// Function to send SMS
void sendSMS()
{
    Serial.println("Sending SMS...");

    gsmSerial.println("AT");
    delay(1000);

    gsmSerial.println("AT+CMGF=1"); // Text mode
    delay(1000);

    gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Replace with guardian number
    delay(1000);

    gsmSerial.print("EMERGENCY! I need help.\nLocation:\n");

    gsmSerial.print("https://maps.google.com/?q=");
    gsmSerial.print(latitude);
    gsmSerial.print(",");
    gsmSerial.print(longitude);

    gsmSerial.write(26); // CTRL+Z to send
    delay(5000);

    Serial.println("SMS Sent!");
}