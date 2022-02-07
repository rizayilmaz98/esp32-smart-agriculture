# AKTARSIS Device: IoT based Smart Agriculture System for ESP32
- In the Smart Agriculture System (AKTARSIS) Device, it is aimed to feed the planted crop at an optimum level, to minimize human-induced agricultural errors and to produce the crop in the most effective way, thanks to agricultural control algorithms.

</br > ![System Schema](https://github.com/tamersum11/esp32-smart-agriculture/blob/master/schema/images/Aktarsis%20Sistem%20%C5%9Eemas%C4%B1.png)

## AKTARSIS Device

- AKTARSIS Device is an embedded system and uses the WiFi-BLE module ESP32-Solo-1, which is located on the ESP32 Devkit C V4 development board, as a microcontroller unit.
- With FreeRTOS, the microcontroller works as a real-time operating system and performs multitasking at the same time with specified task structures.
- WiFi connection is made via the ESP32-Solo-1 microcontroller.
- The device connects to the server port where the Aktarsis Database is located and sets the parameters to be used in the agricultural algorithms and feeding control required for the crop planted on the device, by selecting it from the necessary tables with a MySQL query. Parameters set with database connection; These are the parameters that the device will use in feeding control and pH control.
- In the desktop application, the user can see the temperature, humidity, soil moisture, pH, produced power, consumed power data obtained from the sensors according to date and time; With the SSD1306 OLED screen on the device, you can see the instant values of this data.
- The temperature and humidity data from the Si7021 temperature and relative humidity sensor are transferred to the database, no temperature and humidity control is performed on the device, but is required for the Crop State algorithm in the Aktarsis desktop application.

</br > ![Device Schema](https://github.com/tamersum11/esp32-smart-agriculture/blob/master/schema/images/cihaz-%C5%9Fema.png)

## AKTARSIS Database EER Diagram
</br > ![Aktarsis Database](/schema/images/aktarsisdb-eerdiagram.PNG)

## Queries to AKTARSIS Database in Google Cloud Platform
</br > ![Aktarsis GCP](/schema/images/gcp.png)

## Circuit Schema
You can see circuit schema of the system in [here](/schema/aktarsis_device.fzz).

## AKTARSIS Desktop App
You can see AKTARSIS Desktop App in [here](https://github.com/tamersum11/aktarsis-desktop-app).

## Graduation Project
[Istanbul Medeniyet University](https://www.medeniyet.edu.tr/tr), Faculty Of Engineering And Natural Sciences, Electrical And Electronics Engineering Program Graduation Project.
