# ESP32-S3-LCD-1.9 Meteostation

This is a ESP32-S3 based meteostation with some additionals features.

For project documentation see the [docs](./docs/index.md).

## How to flash

This is an ESP-IDF project, so you can use it to flash, monitor or debug the device.

I'd recommend using `VSCode` and corresponding [extension](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension) to work with the code.

The project was tested and should work with no code modifications, everythig you need is to plug the device in and run:
`idf.py flash`.

Double check `CMakeLists.txt` for commented code for the assets. If you flash the device for the first time you need to flash the assets into the flash memory.

After flashing the device will run in a provisioning mode with instruction on how to set it up.

## Provisioning

When the device is in provisioning mode, you'll see the corresponding screen on the device, and will be able to connect to the device's WiFi acces point.

To provicion the device you need to type in the following information:

1. WiFi access point name to connect the device to. You'll all accessible APs in the list.
2. WiFi password.
3. A name that will prefix fortune telling messages.
4. [OpenWeatherMap](https://openweathermap.org/) account API.
5. [OpenWeatherMap](https://openweathermap.org/) city name.

## Button functions

The button has two functions:
1. Short press opens fortune prediction test. Second click returns to the main screen. Device switches back to the main screen if idles for 10 seconds.
2. Long press reopens provisionong screen. If pressed accicentally, unpulg the device and blug back in.

## Contrubution

If you found a bug, feel free to send a pull request.

