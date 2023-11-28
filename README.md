# 🚗 Simple Garage System

Welcome to the Simple Garage System repository. This project showcases an innovative solution designed to revolutionize vehicle access management within a garage setting. Built with the Agile Scrum methodology using Jira software, the system ensures smooth operations, user convenience, and efficient administration.

![Project Schematic](https://github.com/Hesham-14/Private_Garage_System/blob/main/Documents/Project%20Schematic.png)


## 🌟 Overview

The Private Garage System operates with a clear workflow:

Users who wish to enter the garage present their ID through an RFID system. This ID is captured by the **Entrance Gate ECU** using the USART module. The Entrance ECU then communicates with the **System Control ECU** via the SPI protocol to verify the validity of the presented ID.

#### Upon successful ID validation:
- The garage gate opens.
- LCD Welcoming message, signaling authorized access.
- The system waits for a signal from the motion sensor, confirming the vehicle's passage.
- Once the vehicle has passed, the gate automatically closes.
- During this process, the admin dashboard receives an update on the recent activity.

#### Should the ID be deemed invalid:
- The system displays an "ID not valid" message.
- - A warning alarm gets turned on using a buzzer.
- The admin dashboard is immediately updated with this failed access attempt.

Exiting the garage follows a similar pattern. The client presents their ID via RFID to the **Exit Gate ECU**. The ID is then verified with the System Control ECU. If the ID is valid, the exit gate opens, allowing the vehicle to leave. If not, the admin is notified without the gate opening.

Furthermore, administrators have access to a dedicated dashboard. Using a keypad, UART Terminal, and LCD interface, they can add new IDs, granting authorized access to new users.

## 🛠 Development

The system was developed in adherence to the Agile Scrum methodology and managed using Jira software. Over one release until now, the solution has been refined to its current state:

- **Release 1 (3 Sprints)**
    1. Design and implement the MCAL (Microcontroller Abstraction Layer) & HAL (Hardware Abstraction Layer) for the three MCUs.
    2. Implementation of the refinement of various system functionalities and the finalization of the application layer.
    3. Testing the system through some unit & system tests.
 
![Jira Timeline](https://github.com/Hesham-14/Private_Garage_System/blob/main/Documents/JIRA%20Timeline.png)

This Agile approach ensured iterative improvement and effective incorporation of feedback, leading to a reliable and efficient garage management system.

## ⚙️ Features

- **RFID Integration:** Fast and efficient ID scanning for vehicle access.
- **SPI Communication:** Seamless and swift communication between ECUs using the SPI protocol in a single-master multiple-slave configuration.
- **Admin Dashboard:** Enables administrative oversight and control, from monitoring access attempts to adding new authorized IDs.

## 🎥 User Story Exploration Video
#### 🛠 This project's Video is currently being prepared for uploading.

The video will provide a comprehensive walk-through of a typical user story, showcasing the ease of use and functionalities of our system. From scanning an RFID at the entrance to seamless communication between ECUs, witness firsthand how our system transforms garage operations.

## 🚀 Future Improvements
![System Releases](https://github.com/Hesham-14/Private_Garage_System/blob/main/Documents/JIRA%20System%20Releases.png)
- **Release 2**
    Including an EEPROM to the admin dashboard unit to contain all the database.
- **Release 3**
    Integrating STM32 with Atmega32 to establish communication between 2 different architectures.


Thank you for exploring the Simple Garage System. Your feedback and contributions are always welcome!
