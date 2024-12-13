#include <WiFi.h>
#include <HTTPClient.h>

#define green_led 41 // Pin used to control the green LED
#define red_led 40 // Pin used to control the red LED
#define yellow_led 9 // Pin used to control the yellow LED

const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

const int ldrPin = 4;  // the number of the pushbutton pin
int threshold = 600;

void setup() {

  // Initial configuration of pins for controlling the LEDs as OUTPUTs of the ESP32
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);

  // Initialization of inputs
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input 
  pinMode(ldrPin, INPUT);

  digitalWrite(yellow_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);

  Serial.begin(9600); // Configuration for debugging via serial interface between ESP and computer with baud rate of 9600

  WiFi.begin("Wokwi-GUEST", ""); // Connection to the open WiFi network with SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi successfully!"); // Considering that it exited the above loop, the ESP32 is now connected to WiFi (another option is to place this command inside the if below)

  // Checks the button state
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Button pressed!");
  } else {
    Serial.println("Button not pressed!");
  }

  if (WiFi.status() == WL_CONNECTED) { // If the ESP32 is connected to the Internet
    HTTPClient http; 

    String serverPath = "http://www.google.com.br/"; // HTTP request endpoint

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Result code of the HTTP request

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus = analogRead(ldrPin); 

  if (ldrstatus <= threshold) { 
    Serial.println("it's dark turn on led");
    nightMode(); 

  } else {
    Serial.println("it's bright turn off light");
    digitalWrite(yellow_led, LOW); 
  }
}

void nightMode() {
  digitalWrite(yellow_led, HIGH); // Blink the yellow LED
  delay(500); // LED on for 500ms
  digitalWrite(yellow_led, LOW);
  delay(500); // LED off for 500ms
}