import esp8266
wifi = esp8266.ESP8266() # Create an ESP8266 object
wifi.startAccessPoint("MyHotspot", "MyPassword") # Start an access point
