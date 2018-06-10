# Christmas lights control

The project contains instructions on how to setup christmas lights control using the ESP8266. The ESP8266 is flashed with an
SPIFFS file system where the HTML, CSS and JS files are uploaded. Connectivity to the ESP is established via WiFi and after opening
the browser and loading the IP of the ESP a connection via websockets is established through which the christmas lights can be 
controlled.

# Hardware

Components used for the project are:
1. ESP8266 5V Wifi Relay Module;
2. Sonoff Waterproof IP66 Plastic Cable Wire Connector Gland Electrical 2 Way ABS Material Junction Box;
3. 5V AC-DC power supply;

Link to places where these can be bought can be found below in the Appendix. Of course there may be cheaper places as well or 
more suitable parts.

# Software

In order to flash a customized program to the ESP8266, which in this case is (ESP8266-01) 3.3V need to be provided, the RX and TX
pins need to be connected appropriately and in addition the EN pin has be connected to high voltage (3.3V) and the GPIO0 pin needs
to be connected to GND.

In order to use the files in the "data" folder a file system needs to be created. In this case the SPIFFS is used. So prior to writing the image to the ESP make sure you have the SPIFFS library. If you have it then from Tools choose Flash size and what best suites your needs as size for the file system. In this case "512K (128K SPIFFS)" is used. After you've selected that option
proceed with writing the image to the ESP.

In order to upload the files to the ESP from the data folder a plug-in is needed: arduino-esp8266fs-plugin. Install if from the 
repository and after the installation click on "Tools->ESP8266 Sketch Data Upload". This will upload the files in the "data" folder
to the filesystem of the ESP.

After programming the ESP assemble the project by providing 220V AC to the power supply which has to be also connected to the 5V
of the module in order to power it. Attach the desired christmas lights to the relay and after boot test whether the lights can be 
turned on or off.
  


