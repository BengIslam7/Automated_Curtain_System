# Automated_Curtain_System
# 1. Introduction
The Automated Curtain System project aims to create a system capable of controlling
the opening and closing of curtains automatically based on various conditions such as
ambient light or manual commands. To achieve this, the system utilizes light sensors
(LDR), a motion sensor (PIR), and a servo motor to actuate the curtains.
Communication between the system and the user is facilitated through the MQTT
protocol, enabling remote control and scheduling of actions.
# 2. Hardware Used
- ESP32: Microcontroller used for control and communication.
- Motion Sensor (PIR): Detects motion presence.
- Light Sensor (LDR): Measures ambient light.
- Servo Motor: Actuates the curtains.
- RTC (Real-Time Clock) Module: Manages time and schedules.
- LEDs: Indicate the status of the curtains (open or closed).
# 3. Key Features
- Manual and Automatic Control: Users can choose between manual and automatic
modes to control the curtains.
- Motion Detection: Curtains automatically open when motion is detected (automatic
mode).
- Automatic Adjustment Based on Light: Curtains open or close automatically based
on ambient light.
- Scheduling: Users can schedule specific times for opening and closing the curtains.
- MQTT Communication: Utilizes MQTT protocol for communication between the
system and the mobile application.
# 4. MQTT Communication
The system uses MQTT for communication with a mobile application. The MQTT topics
used are as follows:
- rideaux/rtc: For scheduling curtain open/close times.
- rideaux/mode: To switch between manual and automatic operation modes.
- rideaux/commande: For sending manual open/close commands for the curtains.
# 5. Mobile Application
The mobile application serves as the primary interface for users to interact with the
Automated Curtain System. It offers the following features:
- User Authentication: Users are required to create an account and log in to access the
functionalities of the application. This ensures secure access to the system and
personalized settings.
- Manual Control: Allows the user to manually open or close the curtains.
- Automatic Control: Activates automatic mode for automatic curtain control.
- Planning: Allows the user to set specific schedules for curtain opening and closing.
- Sensors: Provides real-time information on:
- Light Percentage
- PIR Sensor Status
- LDR Sensor Status
- Curtain Status (Open/Closed)
# 6. Initial Setup
Before using the system, users need to perform the following initial setup steps:
- Account Creation: Users need to create an account by providing necessary
information such as username, email, and password.
- Authentication: After creating an account, users must log in using their credentials to
authenticate themselves and gain access to the application's features.
- Configuration: Users are prompted to configure Wi-Fi parameters (SSID and
password) for connecting the system to the internet. Additionally, users need to connect
the system to an MQTT server (e.g., "broker.hivemq.com") and configure MQTT topics as
needed.
# 7. Conclusion
The Automated Curtain System project offers a convenient and efficient solution for
curtain control, providing users with customizable options for manual or automatic
operation and scheduling. With the MQTT communication protocol and real-time sensor
data monitoring, users can remotely control their curtains and monitor environmental
conditions, enhancing comfort and energy efficiency in residential or commercial
settings.
