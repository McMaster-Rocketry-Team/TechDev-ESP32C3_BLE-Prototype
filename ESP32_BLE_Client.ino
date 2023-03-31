#include "BLEDevice.h"  

// Define UUIDs for the service and characteristic of the BLE server we want to connect to
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

// Define boolean flags to keep track of the state of the BLE client
static boolean doConnect = false;   
static boolean connected = false;   
static boolean doScan = false;      

// Declare pointers to the remote characteristic and advertised device of the BLE server
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

// This function is called when the ESP32 receives a notification or indication from the server
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                            uint8_t* pData, size_t length, bool isNotify)
{
  // Print the characteristic UUID, data length, and data received
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.println((char*)pData);
}

// This class defines the callbacks for the BLE client
class MyClientCallback : public BLEClientCallbacks
{
  //client successfully connects to the server
  void onConnect(BLEClient* pclient){}

  //client is disconnected from the server
  void onDisconnect(BLEClient* pclient)
  {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer()
{
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());
  
  // Create a BLE client and set its callbacks
  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");
  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remote BLE server
  pClient->connect(myDevice);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    // If the service cannot be found, print an error message and disconnect
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    // If the characteristic cannot be found, print an error message and disconnect
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;

  }
  Serial.println(" - Found our characteristic");

  if(pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value is: ");
    Serial.println(value.c_str());
  }

  if(pRemoteCharacteristic->canNotify())
  {
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  }
    connected = true;
    return true;
}
// Scan for BLE servers and find the first one that advertises the service we are looking for
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    /* We have found a device, let us now see if it contains the service we are looking for. */
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
    {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    }
  }
};


void setup()
{
  Serial.begin(115200);  
  Serial.println("Starting Arduino BLE Client application..."); 
  BLEDevice::init("ESP32-BLE-Client"); 

  BLEScan* pBLEScan = BLEDevice::getScan(); // create an instance of BLEScan object
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // set the callback for advertised devices
  pBLEScan->setInterval(1349); // set the scan interval in microseconds
  pBLEScan->setWindow(449); // set the scan window in microseconds
  pBLEScan->setActiveScan(true); // set active scanning
  pBLEScan->start(5, false); // start scanning for BLE devices
}

void loop()
{
  if (doConnect == true) // if we need to connect to a BLE server
  {
    if (connectToServer()) // try to connect to the server
    {
      Serial.println("We are now connected to the BLE Server."); 
    } 
    else
    {
      Serial.println("We have failed to connect to the server; there is nothin more we will do."); 
    }
    doConnect = false; // reset the flag
  }

  if (connected) // if we are connected to a BLE server
  {
    std::string value1 = pRemoteCharacteristic->readValue(); // read the value of the remote characteristic
    Serial.print("The characteristic value was: "); 
    Serial.println(value1.c_str()); 
  }
  else if(doScan) // if we need to start scanning for BLE devices
  {
    BLEDevice::getScan()->start(0);  // start scanning for BLE devices (0 means indefinite scan duration)
  }
  
  delay(2000); 
}
