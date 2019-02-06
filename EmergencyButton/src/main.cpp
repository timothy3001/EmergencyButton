#include <Arduino.h>
#include <base64.h>
#include <ESP8266WiFi.h>

#include "config.hpp"

const int buzzerPin = D0;
const int batteryPin = A0;

void setupWifi()
{
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(500);

    // If Wi-Fi is not available, the beep will continue which at least might be heard by people...
  }

  Serial.println("\nConnected!");
}

byte eRcv(WiFiClientSecure client)
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client.available())
  {
    delay(1);
    loopCount++;
    if (loopCount > 10000)
    {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }
  if (respCode >= '4')
    return 0;
  return 1;
}

byte sendEmail(const char *receiver1, const char *receiver2, const char *receiver3, const char *subject, const char *body)
{
  WiFiClientSecure client;
  if (client.connect("smtp.gmail.com", 465) == 1)
    Serial.println(F("connected"));
  else
  {
    Serial.println(F("connection failed"));
    return 0;
  }

  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending EHLO"));
  client.println("EHLO 1.2.3.4");
  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending login"));
  client.println("AUTH LOGIN");
  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending User base64"));
  client.println(base64::encode(EMAIL_SENDER));
  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending Password base64"));
  client.println(base64::encode(EMAIL_PASSWORD));
  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending From"));
  client.print("MAIL From: <");
  client.print(EMAIL_SENDER);
  client.println(">");
  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending To"));
  client.print("RCPT TO: <");
  client.print(receiver1);
  client.println(">");
  if (!eRcv(client))
    return 0;
  if (strlen(receiver2) > 0)
  {
    Serial.println(F("--- Sending To"));
    client.print("RCPT TO: <");
    client.print(receiver2);
    client.println(">");
    if (!eRcv(client))
      return 0;
  }
  if (strlen(receiver3) > 0)
  {
    Serial.println(F("--- Sending To"));
    client.print("RCPT TO: <");
    client.print(receiver3);
    client.println(">");
    if (!eRcv(client))
      return 0;
  }
  Serial.println(F("--- Sending DATA"));
  client.println(F("DATA"));
  if (!eRcv(client))
    return 0;
  client.print("From: ");
  client.println(EMAIL_SENDER);
  client.print("Subject: ");
  client.println(subject);
  client.print("\n");
  client.println(body);
  client.println(".");
  if (!eRcv(client))
    return 0;
  Serial.println(F("--- Sending QUIT"));
  client.println(F("QUIT"));
  if (!eRcv(client))
    return 0;
  client.stop();
  return 1;
}

void setup()
{
  Serial.begin(115200);

  setupWifi();

  // Read battery level
  float batteryLevel = ((analogRead(batteryPin) / (float)1024) * 4.2) - 0.1;

  // Start beep
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);

  // Sending e-mail
  String text = String(EMAIL_BODY_PREFIX) + String(batteryLevel);
  Serial.print("Body: ");
  Serial.println(text.c_str());
  if (sendEmail(EMAIL_RECEIVER_1, EMAIL_RECEIVER_2, EMAIL_RECEIVER_3, EMAIL_SUBJECT, text.c_str()))
  {
    Serial.println("Mail sent!");

    // Stop beep
    digitalWrite(buzzerPin, LOW);

    // Go to sleep
    ESP.deepSleep(0, WAKE_RF_DEFAULT);
  }
  else
  {
    Serial.println("Error!");

    // If mail could not be sent, at least the beep continues which might be heard by people
  }
}

void loop()
{
  delay(1);
}