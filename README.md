# EmergencyButton

This is a simple button, that sends an e-mail when it's pressed. It can be built
quickly and easily and is intended to be used by older and disabled people to
send an emergency notification in case they have had an accident. Alternatively
you could use it for any other case.

The e-mail body contains the current voltage of the battery so you can easily
monitor the battery status.

**DISCLAIMER**: This is a **private project** and I built it for my grandmother.
It is only a temporary solution until a real emergency button will be delivered.
Keep that in mind please and don't make me resposible in case it is not working
for you or has any other kind of faults.

## Features

- Sends an e-mail to up to 3 recipients (can easily be extended)
- Uses Google Mail
- Battery level monitor
- Customizable e-mail subject

## Things you need

- Wemos D1 mini (see 'Other ESP8266 variants' for more information)
- LiPo or Li-Ion, single cell: I used a 720mAh LiPo (Make sure it fits into the
  3d printed case though, the case's inner size is 5cm x 5cm)
- 100k resistor
- Another resistor (can be 100k as well)
- Tactile push button (The standard ones you usually get in those Arduino kits)
- Cable
- Buzzer or LED (optional, see 'Buzzer')

## Electronics

The circuit might not be 100% bullet proof but I'm not an electronics engineer,
so please bear with me.

The buzzer is not yet included but be careful: You need a mosfet or something
else to drive the buzzer.

![Schematics](https://github.com/timothy3001/EmergencyButton/blob/master/Other/EmergencyButton.png?raw=true)

## Other ESP8266 variants

You can use other ESP8266 variants as well but to be able to monitor the battery
without damaging the board, you need to add a voltage divider on A0.

## Buzzer

You can connect a buzzer to pin D0. While sending the e-mail it will beep. As
soon as the e-mail is sent, it will stop. In case the e-mail cannot be send, it
will continue beeping, so at least someone may hear the beeping.
