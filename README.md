# Sharpy
Arduino Nano microcontroller with two relays to control a sharpening machine's operation.

![Box Design](https://raw.githubusercontent.com/mdavid626/sharpy/main/docs/box-design.png)

## Operation
First the device needs to be switched on with the `ON/OFF` switch. Then the cycle is ready to be started with the `START` button. It can be stopped any time using the `STOP` button.

The cycle is the following: F1 -> P1 -> 3x (F2 -> P2)

During F1 relay R1 is turned on. During F2 relay R2 correspondingly. P1 and P2 are for pauses.

F1, P1, F2 and P2 duration can be controlled using the potentiometers. The range is from `0-10s`. The interval `0-1` is considered as `1`.
