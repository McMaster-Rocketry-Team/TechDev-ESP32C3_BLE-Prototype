# TechDev-ESP32C3_BLE-Prototype

Prototype ESP32C3 BLE server and client communication with Ultrasonic Sensor

How it works:
- BLE is a power-conserving variant of Bluetooth designed for short-distance transmission of small amounts of data.
- BLE devices can act as either a server or a client.
- The server advertises its existence and contains data that the client can read.
- The client scans for nearby devices and, when it finds the server it is looking for, establishes a connection and listens for incoming data.
- Communication between the server and client is based on the Generic Attribute Profile (GATT), which defines a hierarchical data structure for sending and receiving standard messages.
- The GATT hierarchy consists of profiles, services, characteristics, and descriptors.
- A profile is a standard collection of services for a specific use case.
- A service is a collection of related information, such as sensor readings or battery level.
- A characteristic is where the actual data is stored within the hierarchy.
- A descriptor describes how the characteristic value can be interacted with, such as read, write, notify, or indicate.
- Each service, characteristic, and descriptor has a unique identifier called a UUID (Universally Unique Identifier).
- Data is exchanged between the server and client by reading and writing characteristic values using their UUIDs.

![image](https://user-images.githubusercontent.com/84754292/229200301-0d9ee1ae-2586-4ca0-b8dd-1c25901ea899.png)


![image](https://user-images.githubusercontent.com/84754292/229018864-e0279f6b-5b4f-4f99-a51b-9ecc2e103660.png)

Client and Server:

![b4cd47e7-ac7f-4af7-b639-9b92c936b590](https://user-images.githubusercontent.com/84754292/229019603-89a01226-86bf-4d00-8a3a-31f74ce3822e.jpg)



