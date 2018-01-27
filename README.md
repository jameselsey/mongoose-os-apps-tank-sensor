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


AWS IoT
====
This project will upload the sensor readings to AWS IoT.

When you run `mos build && mos flash` for the first time, there will be no AWS config, run `mos ls`, you shouldn't see
and `aws-*.pem` files, thats because I haven't committed them as they're private. You'll need to initialise by running 
`mos aws-iot-setup`.

Run `mos ls` again and you should see those AWS files on the device. Next thing you need to do is configure the device to use those keys. Run `mos call Config.Get` to see the config that the device is actually using, you can either set config using `mos config-set` or you can alter the `fs/conf9.json` file as that file will survive any OTA updates.

Login to your AWS IoT console and you should see data being published!


Next steps
=====
1. Figure out solar/battery solution