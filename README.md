# Internet-of-Things Lab 101

## Goal
This half-day workshop is designed to teach attendees with no prior experience with Internet-of-Things (commonly IoT) projects how to approach designing and implementing a common scenario end-to-end.

Perhaps the most common IoT use case is to collect sensor data from many devices in the field and report ...

## Inventory
The facilities should be able to provide the following:
* A method by which to present content from a computer to the class (projector, TV, etc.).
* Power outlets sufficient for all attendees to plug in a laptop.

Each attendee will need to bring following items to the event:
* A laptop (Windows, OS X, or Linux is fine) with a USB port.

The following software will be installed on each laptop (this will be done during the event):

|Software|Link|
|--------|----|
|Arduino IDE v1.6.8+|https://www.arduino.cc/en/Main/Software|
|ESP8266 Board Manager|http://arduino.esp8266.com/stable/package_esp8266com_index.json|
|Node.js|https://nodejs.org/en/download|

The following items will need to be purchased for each attendee:

|Cost|Item|Link|
|----|----|----|
|$35.96 ea. (in qty. of 25+)|Azure IoT Starter Kit – SparkFun Thing|https://www.sparkfun.com/products/13799|
|$0.90 ea (in qty. of 25+)|Trimpot 10K with Knob|https://www.sparkfun.com/products/9806|

## Agenda

1. Designing solutions (link to presentation)
  1. Introduce IoT and some specific scenarios and implementations
  2. Discuss the specific implementation we will try today

2. Working with the board (ESP8266 WiFi)
  1. Download and install the Arduino IDE (https://www.arduino.cc/en/Main/Software)
  2. Download and install the ESP8266 Board Manager (http://arduino.esp8266.com/stable/package_esp8266com_index.json)
  3. Configure the board per https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/setting-up-arduino

    ![board settings](/images/board-settings.png)

  4. Test connectivity to the board (use [verify_connectivity.ino](verify_connectivity.ino))
  5. Build a circuit with a dial for input and warning lamps (build [circuit.md](circuit.md))
    * You should be able to rotate the dial on the circuit and notice that the warning lamps come on as expected

3. Sending the data to Azure IoT Hub
  1. Download and install Node.js (https://nodejs.org/en/download)
  2. Install iothub-explorer: npm install -g iothub-explorer
  3. Each attendee should register their device with the IoT Hub
    1. iothub-explorer login "HostName=pelasne-iothub.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=<key>"
    2. iothub-explorer create <deviceId> --connection-string
  4. Apply the code to communicate to Azure (use [report_to_iothub.ino](report_to_iothub.ino))
  5. Change the values at the top of the file as appropriate (WiFi SSD, WiFi password, deviceId, connection string)
  6. Monitor the events: iothub-explorer "HostName=pelasne-iothub.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=/<key/>" monitor-events /<deviceId/>
    * You should be able to rotate the dial on the circuit and notice that the events are sent to the Azure IoT Hub

4. Handling the data in Azure
  1. Discuss Azure Event Hubs
  2. Discuss Azure Stream Analytics
    1. Discuss hot path/cold path
    2. Discuss inputs/outputs
    3. Show queries for Stream Analytics [stream_analytics.sql](stream_analytics.sql)
    4. Discuss windows [event_windows.md](event_windows.md)
  3. Discuss Azure SQL Database
  4. Discuss Power BI
    1. Talk about real-time vs. stored data

## References
You can find more information at these links:
* Azure IoT Hub https://azure.microsoft.com/en-us/services/iot-hub/
* ESP8266 Azure IoT Hub samples https://github.com/stefangordon/AzureIoT/tree/master/examples/esp8266
* Get Started with Azure IoT Sparkfun Kit https://azure.microsoft.com/en-us/documentation/samples/iot-hub-c-thingdev-getstartedkit/
* Azure IoT Hub Arduino Libraries https://github.com/arduino-libraries/AzureIoTHub
* Azure IoT Hub SDK for C http://azure.github.io/azure-iot-sdks/c/api_reference/iothub__client__ll_8h.html#aa7773cdda101288cfd805c34f726eb84
* Intro to IoT Hub SDK for C https://azure.microsoft.com/en-us/documentation/articles/iot-hub-device-sdk-c-intro/
* IoT Hub Client Source Code https://github.com/Azure/azure-iot-sdks/tree/master/c/iothub_client/src
