# EmergencyButton

This is a simple button, that sends an e-mail when it's pressed. It can be built
quickly and easily and is intended to be used by older or disabled people to
send an emergency notification in case they have had an accident at home.

The e-mail body contains the current voltage of the battery so you can monitor
the battery status.

Of course this button could be easily repurposed.

**DISCLAIMER**: This is a **private project** and I built it for my grandmother.
It is only a temporary solution until the real emergency button will be
delivered. Keep that in mind please and don't make me resposible in case it is
not working for you or has any other kind of faults.

## Features

- Sends an e-mail to up to 3 recipients (can easily be extended)
- Uses Google Mail
- Battery level monitor
- Customizable e-mail subject

## Things you need

Hardware:
- Wemos D1 mini (see 'Other ESP8266 variants' for more information)
- LiPo or Li-Ion, single cell: I used a 720mAh LiPo (Make sure it fits into the
  3d printed case though, the case's inner size is 5cm x 5cm)
- 100k resistor
- Another resistor (can be 100k as well)
- Tactile push button (The standard ones you usually get in those Arduino kits)
- Cable
- 4x M3 screws (20mm at least) with sink head
- Buzzer or LED (optional, see 'Buzzer or LED')

Software:
- Microsoft Visual Studio Code
- PlatformIO as VS Code extension

## Electronics

The circuit might not be 100% bullet proof but I am not an electronics engineer,
so please bear with me.

The buzzer is not yet included but be careful: You need a mosfet or something
else to drive the buzzer.

I recommend desoldering the pins off of the wemos board to reduce some height of
the board, so that it will fit better into the button case.

![Schematics](https://github.com/timothy3001/EmergencyButton/blob/master/Other/EmergencyButton.png?raw=true)

## Other ESP8266 variants

You can use other ESP8266 variants as well but to be able to monitor the battery
without damaging the board, you need to add a voltage divider on A0.

## Buzzer or LED

You can connect a buzzer or a LED to pin D0. While sending the e-mail it will
beep or / and blink. As soon as the e-mail is sent, it will stop. In case the
e-mail cannot be send, the buzzer will continue beeping, so at least someone may
hear the beeping.

## CAD File

You can find the CAD file in the 'Others' directory. I designed the case in
Fusion 360. I tried to use as many parameters as possible so that it can be
easily customized.

## Sources

The code for sending the e-mail was mostly copied from here:
https://www.hackster.io/MakerRobotics/send-e-mail-nodemcu-with-mikroelektronika-clicks-395002
