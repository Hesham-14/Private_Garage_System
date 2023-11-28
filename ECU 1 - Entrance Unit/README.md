# 🚧 ECU1 Entrance Gate Functionality - Garage Management System

![1](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20Includes.png)

## *Story*
> The main goal of ECU1 is to control the entrance gate by reading the ID of the Driver using an RFID card based on UART and sending the driver ID to the admin dashboard ECU2 through SPI when ECU2 Responds with a valid ID then ECU1 starts to open the entrance gate using Fast PWM Mode. While the PIR sensor reads there is a vehicle under the entrance gate, the entrance gate will never close until the vehicle exits the area under the entrance gate and the PIR sensor reads LOW. when the entrance gate is open the RFID reader can't read any data until the gate is closed.
> In case of an invalid ID, the entrance gate never opens and a warning alarm runs for a limited time.


## 🛠 Technologies & Hardware

- **Architecture**: Developed on the robust STM32F103C6 microcontroller platform
- **RFID**: Enables efficient, contactless ID scanning.
- **USART Protocol**: Integrated with STM32F103C6 for seamless RFID-ECU interactions.
- **SPI Protocol**: Facilitated by STM32F103C6 for inter-ECU communication.
- **Motion Sensor**: Connected to STM32F103C6 for real-time vehicle detection.
  
## *System Statistical Analysis Results*
![System Calls](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20Charts.png)
### *System Calls & Includes*
![2](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20Calls.png)
### *System Depends*
![3](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20DependsON.png)
### *System Simple UML Sequence Diagram*
![4](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20UML%20Simple%20SequenceDiagram.png)
> [!NOTE]
> For the full UML Sequence Diagram, please click [here](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20UML%20Simple%20SequenceDiagram.png).


## 📖 Detailed Overview

### Step 1: User Approach and ID Scanning
When a user seeks to access the garage, they present their ID to the RFID scanner. This ID scanning action is driven by the USART module integrated with the ECU1. 

### Step 2: Entrance ECU to Main ECU Communication
On capturing the ID, the Entrance ECU1 starts communicating with ECU2 **"The main one"**. This communication relies on the SPI protocol, facilitated by the ECU1, to ensure a swift and error-free data transmission.

### Step 3: ID Validation and Action Execution

- **Valid ID Scenario**:
  - A Welcoming message appears on the LCD, connected to the ECU1, and illuminates to indicate successful validation.
  - The gate automatically opens, granting vehicle access.
  - The system enters a standby mode, with the ECU1 continuously reading the motion sensor.
  - As the motion sensor detects the vehicle's movement, the gate begins its closing sequence.
  
- **Invalid ID Scenario**:
  - The LCD screen, driven by the ECU1, promptly flashes an "ID not valid access" message.
  - A buzzer, interfaced with the ECU1, gets triggered, notifying the user of an access denial.
 
In both scenarios **(valid or invalid)** the ECU2 ensures that the admin dashboard gets timely notifications for real-time system monitoring.
