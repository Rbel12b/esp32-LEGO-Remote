# ESP32-LEGO-Remote

*Disclaimer*: LEGO® is a trademark of the LEGO Group of companies which does not sponsor, authorize or endorse this project.

**ESP32-LEGO-Remote** is a project for programming all kinds of *LEGO* Powered UP devices. From the two port hub, Move Hub (e.g. Boost), DUPLO train hub, Technic Hub. You can control almost everything with that library and your Arduino sketch. It uses the [Legoino](https://github.com/corneliusmunz/legoino) library to connect to the HUBs.

It is also possible to use the "old" Power Function IR Modules and control them via an IR LED connected to a PIN of your ESP32 device.

It needs an external device what sends data(e.g. button states) to the ESP32 via Serial connection.

## Arduino Hardware and dependent libraries

The library is implemented for **ESP32** boards and uses the ESP32 [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) library as dependency. This should be installed via the Arduino library manager before using Legoino.


# Setup and Usage

Just clone the project with git and open it in PlatformIO and upload it.
And have a look on the [Hardware Setup](doc/HARDWARESETUP.md).


## PowerFunction IR

To use the PowerFunction Infrared library you have to connect a IR-LED to the pin 4 of your controller.

# Connection to more than 3 hubs

It is possible to connect to up to 9 hubs in parallel with a common ESP32 board. To enable the connection to more than 3 hubs, you have to change a single configuration of the NimBLE library. Just open the ```nimconfig.h``` file located in your Arduino library folder in the directory ```NimBLE-Arduino/src```. Open the file with an editor and change the following settings to your demands:

```
/** @brief Sets the number of simultaneous connections (esp controller max is 9) */
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3
```

Then close the Arduino environment and open it again to force the rebuild of the library. Open your sketch build and upload it and be happy with multiple connections.

##### Note: 
The project will only allow to connect 7 HUBs


# Debug Messages

The standard `log_d`, `log_e`, `log_xx` messages are used. The log levels could be set via the Arduino environment and the messages are sent to the serial monitor.


# Credits

Thanks to [@Cornelius Munz](https://github.com/corneliusmunz) for creating the [Legoino](https://github.com/corneliusmunz/legoino) library

# Remarks

Prerequisite of that library is the NimBLE-Arduino library (https://github.com/h2zero/NimBLE-Arduino) with at least version 1.0.1. Otherwise it will not work.

Up to now the library is only tested for Control+ Hubs and only set motor speeds.


# ToDo
* Add sensor support
* Add more motor commands
