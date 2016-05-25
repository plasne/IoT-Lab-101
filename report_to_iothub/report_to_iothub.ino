#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <String.h>
#include "AzureIoTHub.h"

// wifi config
const char* SSID = "?????";
const char* PASSWORD = "?????";

// IoT Hub config
static const char* deviceId = "device01";
static const char* connectionString = "HostName=pelasne-iothub.azure-devices.net;DeviceId=device01;SharedAccessKey=????????";

// forward declarations
class widget;

// define the data model
BEGIN_NAMESPACE(wfi);
  DECLARE_MODEL(widget,
    WITH_DATA(ascii_char_ptr, DeviceId),
    WITH_DATA(int, Value)
  );
END_NAMESPACE(wfi);

DEFINE_ENUM_STRINGS(IOTHUB_CLIENT_CONFIRMATION_RESULT, IOTHUB_CLIENT_CONFIRMATION_RESULT_VALUES)

// define global variables
widget* myWidget;
IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
WiFiClientSecure sslClient;
AzureIoTHubClient iotHubClient(sslClient);
int messageId = 0;

// connect to an NTP server
void initTime() {
  time_t epochTime;
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (true) {
    epochTime = time(NULL);
    if (epochTime == 0) {
      Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
      delay(2000);
    } else {
      Serial.print("Fetched NTP epoch time is: ");
      Serial.println(epochTime);
      break;
    }
  }
}

// connect to WiFi
void initWiFi() {
  Serial.print("ATTEMPT: Connecting to ");
  Serial.print(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("SUCCESS: WiFi connected with IP ");
  Serial.print(WiFi.localIP());
  Serial.println(".");
}

// connect to an Azure IoT Hub
void initIotHub() {
  if (serializer_init(NULL) != SERIALIZER_OK) {
    Serial.println("Failed on serializer_init.");
  } else {
    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, HTTP_Protocol);
    srand((unsigned int)time(NULL));
    if (iotHubClientHandle == NULL) {
      Serial.println("FAIL: Failed on IoTHubClient_LL_Create.");
    } else {
      unsigned int minimumPollingTime = 9; /*because it can poll "after 9 seconds" polls will happen effectively at ~10 seconds*/
      if (IoTHubClient_LL_SetOption(iotHubClientHandle, "MinimumPollingTime", &minimumPollingTime) != IOTHUB_CLIENT_OK) {
        Serial.println("FAIL: Failure to set option MinimumPollingTime.");
      }
      myWidget = CREATE_MODEL_INSTANCE(wfi, widget);
      if (myWidget == NULL) {
        Serial.println("FAIL: Failed on CREATE_MODEL_INSTANCE.");
      } else {
        Serial.println("SUCCESS: Connected to IoT Hub.");
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  initWiFi();
  initTime();
  initIotHub();
  iotHubClient.begin();
}

void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    int messageTrackingId = (intptr_t)userContextCallback;
    if (result == IOTHUB_CLIENT_CONFIRMATION_OK) {
      Serial.print("SUCCESS: Message ID ");
      Serial.print(messageTrackingId);
      Serial.println(" receieved.");
    } else {
      Serial.print("FAIL: Message ID ");
      Serial.print(messageTrackingId);
      Serial.print(" receieved with error: ");
      Serial.println(ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
    }
}

void sendReading(int value) {
  myWidget->DeviceId = (char*)deviceId;
  myWidget->Value = value;
  {
    unsigned char* destination;
    size_t destinationSize;
    if (SERIALIZE(&destination, &destinationSize, myWidget->DeviceId, myWidget->Value) != IOT_AGENT_OK) {
      Serial.println("FAIL: Failed to serialize.");
    } else {
      IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray(destination, destinationSize);
      if (messageHandle == NULL) {
        Serial.println("FAIL: Unable to create a new IoTHubMessage.");
      } else {
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendCallback, (void*)messageId) != IOTHUB_CLIENT_OK) {
          Serial.println("FAIL: Failed to hand over the message to IoTHubClient.");
        } else {
          Serial.println("SUCCESS: IoTHubClient accepted the message for delivery.");
        }
      }
      IoTHubMessage_Destroy(messageHandle);
      free(destination);
    }
  }
  messageId++;
}

void loop() {

  int sensorValue = analogRead(A0);
  Serial.println("value: ");
  Serial.println(sensorValue);
  delay(500);

  if (sensorValue > 800) {
    digitalWrite(4, HIGH);
    digitalWrite(13, LOW);
    Serial.println("alert high");
  } else if (sensorValue < 200) {
    digitalWrite(4, LOW);
    digitalWrite(13, HIGH);
    Serial.println("alert low");
  } else {
    digitalWrite(4, LOW);
    digitalWrite(13, LOW);
  }

  sendReading(sensorValue);
  IoTHubClient_LL_DoWork(iotHubClientHandle);

}
