## Wemos D1 Bambulab Neopixel Controller

Important information: Please only use with one connected AMS, otherwise there will be problems with the large MQTT packages.

<p align="left">
Click for Youtube Video<br>
	<a href="https://youtu.be/bh6ZwIvbDFg"><img src="https://github.com/vr6syncro/BambulabLedController-Neopixel/blob/main/Projekt%20Files/Media/Pictures/Desktop%20Stand/20230608_003318.jpg?raw=true" width="150"></a>
</p>

### Firmware changelog:

v3 - added debug options, updated the code, removed some effects
for more informations see: https://github.com/vr6syncro/BambulabLedController_Neopixel/releases/tag/testing_v3

Also updated the Programmer Homepage to v3 (see below)



# Setup - Flashing and Configure Controller

For easy setup use my Flash Homepage: [https://vr6syncro.de/programmer/flash.html](https://vr6syncro.de/programmer/flash.html)
There are compiled Version for installation.

Otherwise clone repository and flash with Arduino IDE.

After you have flashed the firmware look for the access point that the controller should now open.

Connect to the access point. It can happen that an error is displayed because there is no internet connection, just ignore it.

Now switch to the browser and enter the IP 192.168.4.1.

Here you connect the controller to your WiFi.

After you have saved you have to switch to the console of the controller.
This can be done via the IDE or via the website programmer under the Logs and Console menu.

The IP, the username and the password for the controller are now displayed here.
(Please make a note, will not be displayed later)

Now connect by entering the displayed IP and log in.

Configure your printer by entering and saving the IP of the printer, the secret and the serial number.

In the console where we previously received the user data, messages like: Message arrived in topic: ...
turn up.

Now the controller is set up.

## Wiring

Up to 6 Led is supported with internal USB Power on the Wemos D1 mini if u are not modify the brightness in the firmware otherwise there ther might be some issues.

- 5V Power pin to LED + / 5V Pin
- G / Ground pin to G / Ground Pin
- D2 pin to DIN on the first LED

There is maybe also a arrow on the led. This must face away from the wemos side. like: wemos d2 -> din -> dout -> din .....

If you use a Led Strip thats it otherwise you continue soldering from the dout side of the led to the next led din side.

## Configuration

You can find out how the compiled images were configured in the release notes.

#####  If you want to configure it yourself you can find some information here:
Wich effect is played on wich action is stored in handle.h
The effects are stored in effects.h, i would advise against changing anything there.

For more infos and help see my discord at: [http://discord.vr6syncro.de](http://discord.vr6syncro.de)

3D Print related files are uploaded to Printables: 
<br><a href="https://www.printables.com/de/model/501355-bambulab-status-indicator-table-stand">Table Stand</a> 
<br><a href="https://www.printables.com/de/model/499286-bambulab-neopixel-status-indicator">Traffic Light</a> <br>


There are a few things that must be solved to get it stable but is is already working.



### License

The BL Led Controller is released under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license. See the [LICENSE](https://github.com/DutchDevelop/BambulabLedController/blob/main/LICENSE) file for more details.

### Original Firmware Author
[DutchDeveloper](https://dutchdevelop.com/).