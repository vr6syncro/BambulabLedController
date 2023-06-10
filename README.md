## Wemos D1 Bambulab Neopixel Controller

<p align="left">
Click for Youtube Video<br>
	<a href="https://youtu.be/bh6ZwIvbDFg"><img src="https://github.com/vr6syncro/BambulabLedController-Neopixel/blob/main/Projekt%20Files/Media/Pictures/Desktop%20Stand/20230608_003318.jpg?raw=true" width="350"></a>
</p>


## Setup - Flashing

For easy setup use my Flash Homepage: [https://vr6syncro.de/programmer/flash.html](https://vr6syncro.de/programmer/flash.html)
There are compiled Version for installation.

Otherwise clone repository and flash with Arduino IDE.

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

3D Print related files are uploaded to: [https://www.printables.com/de/model/499286-bambulab-neopixel-status-indicator](https://www.printables.com/de/model/499286-bambulab-neopixel-status-indicator)


There are a few things that must be solved to get it stable but is is already working.



### License

The BL Led Controller is released under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license. See the [LICENSE](https://github.com/DutchDevelop/BambulabLedController/blob/main/LICENSE) file for more details.

### Original Firmware Author
[DutchDeveloper](https://dutchdevelop.com/).
