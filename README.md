# mongoose-os-apps-tank-sensor

Ultrasonic tank sensor
=======

Simple app running Mongoose OS on a Lolin32 ESP32 board to read distance via a waterproof ultrasonic sensor (JSN SR04T, also works with HC SR04 as they are same interface).

Checkout, then build and flash using `mos build && mos flash && mos console`

You should see output like 

```
[Jan 27 09:55:24.980] timer_cb             Distance - 162.18 cm
[Jan 27 09:55:26.980] timer_cb             Distance - 163.00 cm
[Jan 27 09:55:28.981] timer_cb             Distance - 162.72 cm
[Jan 27 09:55:30.980] timer_cb             Distance - 162.18 cm
[Jan 27 09:55:32.981] timer_cb             Distance - 162.71 cm
[Jan 27 09:55:34.980] timer_cb             Distance - 162.16 cm
```

Credit to "nliviu" on Mongoose OS forums for helping me get the pulseIn working
https://forum.mongoose-os.com/discussion/1928/arduino-compat-lib-implicit-declaration-of-function-pulsein#latest

Next steps
=====
1. Start sending data to AWS IoT
2. Figure out solar/battery solution