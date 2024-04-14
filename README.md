# RFID Scanner Using FRDM K66, ESP32 and Flask(Python)

Our proposed solution integrates a Real-Time Clock Module to ensure impeccably precise timekeeping and reduce latency in timestamp acquisition, thereby eliminating dependence on secondary APIs and fortifying security. The backbone of our server is Flask, a Python-based micro web framework, chosen for its agility in handling dynamic data operations. Our data repositories will reside in a robust relational database fashioned using SQLite, ensuring efficient data manipulation. To enrich user interaction, we're incorporating auditory feedback through a buzzer system, affirming successful check-ins. Our objective is not only to streamline the attendance process but also to empower event organizers with actionable insights and a heightened level of control over their events.

## Tool/Libraries Used

- MFRC522 for RFID Scanner
- mbedLCD for parallel lcd
- DS1302 for RTC component
- MbedOS as the Real Time operating System for working with FRDM K66
- Arduino For Operating ESP32
- Python Server at the backend alonside SQLite3 and Flask

## System/Circuit Diagram

![Diagram](<./System%20Diagram_Circuit%20Diagram%20(1).jpeg>)

## How to run the code

We encountered various issues with the synchronization and we did implement the mechanism to clear the buffer periodically, but still some issue existed if different programmes for each component in this project ran differently, so the idea would be first run the code for ESP32 , then have the python server running and at last flash the FRDM k66 to prevent synchronization errors
