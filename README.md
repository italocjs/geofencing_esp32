# Easy geofencing on ESP32 and arduino: How C++ and Python can be your best friends
For more detail, please check this article: https://www.linkedin.com/pulse/easy-geofencing-esp32-arduino-how-c-python-can-your-best-italo-soares/

## What is Geofencing? 📍

Geofencing is a technology that defines a virtual boundary around a real-world geographical area. Think of it like an invisible fence you can set up around a specific location. 🌍 When a device like a ESP32 with a GPS-enabled sensor enters or leaves this boundary, an action is triggered. 🚨 This could be anything from blinking an LED to fancy satellite communication. Geofencing is super useful in various applications, like tracking vehicle fleets, ensuring child safety, and even in marketing to send promotional messages when you're near a store. 

## Quick Steps to Use the Python Script 🗺️

1️⃣ Copy your chosen group in Google Earth.
2️⃣ Paste it into the google_earth.xml file. 
3️⃣ Run the google_earth_polygon_parser.py script. 
4️⃣ Copy and paste the output C++ code into your ESP32 setup. 
🚀

## How to Use Google Earth for Your Geofencing Project 🌍

Setting up your geofence in Google Earth is straightforward, but there are some pro tips to make the process even smoother. First off, it's a good idea to create a folder in Google Earth, name it something meaningful so you know exactly what it's for.

While you can use markers to define your geofence, using polygons is highly recommended for greater accuracy and ease. If you do go the polygon route, make sure to place the markers in a counter-clockwise direction. The Python script will automatically recognize these as the shape of your geofence.

However, if you still want to use markers, make sure to name them like p1, p2, p3, and so on. The Python script will look for markers with names starting with "p" to set the geofence shape. 

Tags: ESP32, Arduino, Geofencing, Embedded Software, Python, Google Earth