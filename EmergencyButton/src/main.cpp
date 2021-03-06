#include <Arduino.h>
#include <base64.h>
#include <ESP8266WiFi.h>

#include "config.hpp"

const int buzzerPin = D0;
const int batteryPin = A0;
const int beepCycle = 500;

int milliesBeepOn = 0;
bool doBeeping = false;
bool beepState = false;

void turnBeepOn()
{
  doBeeping = true;
  milliesBeepOn = millis();
}

void turnBeepOff()
{
  digitalWrite(buzzerPin, LOW);
  doBeeping = false;
  beepState = false;
}

void checkBeep()
{
  int cycleStatus = (millis() - milliesBeepOn) % beepCycle;
  if (cycleStatus < beepCycle / 2 && !beepState)
  {
    if (doBeeping)
    {
      digitalWrite(buzzerPin, HIGH);
      beepState = true;
    }
  }
  else if (cycleStatus >= beepCycle / 2 && beepState)
  {
    digitalWrite(buzzerPin, LOW);
    beepState = false;
  }
}

void checkTimer()
{
  if (millis() > 300000)
  {
    turnBeepOff();

    // Go to sleep
    ESP.deepSleep(0, WAKE_RF_DEFAULT);
    delay(500);
  }
}

void checkBeepAndTurnOffTimer()
{
  checkBeep();
  checkTimer();
}

void setupWifi()
{
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(100);

    // If Wi-Fi is not available, the beep will continue for 5 minutes which at least might be heard by people...
    checkBeepAndTurnOffTimer();
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

  // Read battery level
  float batteryLevelVolts = ((analogRead(batteryPin) / (float)1024) * 4.2);
  float batteryLevelPercentage = ((batteryLevelVolts - 3.4) / (4.2 - 3.4)) * 100;

  // Start beep
  pinMode(buzzerPin, OUTPUT);
  turnBeepOn();

  setupWifi();

  // Sending e-mail
  String text = String(EMAIL_BODY_PREFIX) + String(batteryLevelPercentage) + String("% (") + String(batteryLevelVolts) + String("V)");
  Serial.print("Body: ");
  Serial.println(text.c_str());
  if (sendEmail(EMAIL_RECEIVER_1, EMAIL_RECEIVER_2, EMAIL_RECEIVER_3, EMAIL_SUBJECT, text.c_str()))
  {
    Serial.println("Mail sent!");

    // Stop beep
    turnBeepOff();

    // Go to sleep
    ESP.deepSleep(0, WAKE_RF_DEFAULT);
    delay(500);
  }
  else
  {
    Serial.println("Error!");

    // If mail could not be sent, at least the beep continues for 5 minutes which might be heard by people. It can be restarted easily by another button press.
  }
}

void loop()
{
  delay(1);
  checkBeepAndTurnOffTimer();
}