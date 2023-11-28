# 🚧 ECU1 Entrance Gate Functionality - Garage Management System

![1](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%203%20-%20Exit%20Unit/Documents/ECU3%20Includes.png)

## *Story*
> The main goal of ECU3 is to control the exit gate by reading the ID of the Driver using an RFID card based on UART and sending the driver data ID to the admin dashboard ECU2 through SPI when ECU2 Responds with a valid ID then ECU3 starts to open the exit gate using Fast PWM Mode. While the PIR sensor reads there is a vehicle under the exit gate, the exit gate will never close until the vehicle exits the area under the exit gate and the PIR sensor read LOW. when the exit gate is open the RFID reader can't read any data until the gate is closed. In case of an invalid ID, the exit gate never opens until the driver enters a valid ID.


## 🛠 Technologies & Hardware

- **Architecture**: Developed on the robust STM32F103C6 microcontroller platform
- **RFID**: Enables efficient, contactless ID scanning.
- **USART Protocol**: Integrated with STM32F103C6 for seamless RFID-ECU interactions.
- **SPI Protocol**: Facilitated by STM32F103C6 for inter-ECU communication.
- **Motion Sensor**: Connected to STM32F103C6 for real-time vehicle detection.
  
## *System Statistical Analysis Charts*
![System Charts](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%203%20-%20Exit%20Unit/Documents/ECU3%20Charts.png)
### *System Calls & Includes*
![2](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%203%20-%20Exit%20Unit/Documents/ECU3%20Calls.png)
### *System Depends*
![3](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%203%20-%20Exit%20Unit/Documents/ECU3%20DependsOn.png)
### *System Simple UML Sequence Diagram*
![4](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%203%20-%20Exit%20Unit/Documents/ECU3%20UML%20Simple%20Sequence%20Diagram.png)
> [!NOTE]
> For the full UML Sequence Diagram, please click [here](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%201%20-%20Entrance%20Unit/Documents/ECU1%20UML%20Simple%20SequenceDiagram.png).

## 📖 Detailed Overview

### Step 1: Vehicle Prepares to Exit
As a vehicle is poised to exit the garage, the user must present their ID to the RFID scanner at the exit gate. This identification process is expertly handled by the USART module integrated within the ATmega32.

### Step 2: Exit ECU Communicates with Main ECU
Upon ID capture, the Exit ECU initiates a query process with the Main ECU. This communication is executed over the SPI protocol, embedded in the ATmega32, ensuring rapid and error-free data exchanges.

### Step 3: ID Validation and Exit Execution

- **Valid ID Scenario**:
  - Recognizing an authenticated ID, the gate automatically swings open, authorizing the vehicle to exit.
  - During this time, a notification or alert is dispatched to the admin dashboard, ensuring real-time monitoring and awareness.

- **Invalid ID Scenario**:
  - Should the ID prove inauthentic, the gate remains firmly shut.
  - While the vehicle is not granted permission to exit, an urgent notification is routed to the admin dashboard, highlighting the unauthorized exit attempt.
  
Regardless of the ID's validity, the ATmega32 ensures that every exit attempt is duly recorded and the admin dashboard is updated promptly.
