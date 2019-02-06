# EmergencyButton

This is a simple button, that sends an e-mail when it's pressed. It is easy and
quick to build and can be used by older people to send a notification in case
they have had an accident (e.g. a fall on the ground). Alternatively you could
use it for any other case.

## Features

- Sends an e-mail to up to 3 recipients (can easily be extended)
- Uses Google Mail
- Battery level monitor
- Customizable e-mail subject

## Things you need

- Wemod D1 mini (see 'Other ESP8266 variants' for more information)
- LiPo or Li-Ion, single cell (Make sure it fits into the 3d printed case
  though, the case's inner size is 5cm x 5cm)
- 100k resistor
- Another resistor (can be 100k as well)
- Tactile push button (The standard ones you usually get in those Arduino kits)
- Cable
- Buzzer or LED (optional)

## Electronics

## Other ESP8266 variants

You can use other ESP8266 variants as well but to be able to monitor the battery
without damaging the board, you need to add a voltage divider on A0.
