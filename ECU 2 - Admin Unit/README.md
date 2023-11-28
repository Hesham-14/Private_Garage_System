# 🚧 ECU2 Admin & Main ECU Functionality

![1](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%202%20-%20Admin%20Unit/Documents/ECU2%20Includes.png)

## *Story*
> The main goal of ECU2 is controlling the admin privileges to add, delete, and edit driver data the system may have more than one admin each one of them has its username and password. ECU2 is connected to an LCD to display the different statuses of the garage and control driver data and shows the number of available slots in the garage, an existing Terminal for admins to help them navigate on LCD and edit any driver information.


## 🛠 Technologies & Hardware

- **Architecture**: Developed on the robust STM32F103C6 microcontroller platform
- **LCD Scress**: Working in harmony with the STM32F103C6, it ensures administrators experience a lucid and dynamic interaction.
- **USART Protocol**: Integrated with STM32F103C6 for seamless RFID-ECU interactions.
- **SPI Protocol**: Facilitated by STM32F103C6 for inter-ECU communication.
  
## *System Statistical Analysis Charts*
![System Calls](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%202%20-%20Admin%20Unit/Documents/ECU2%20Charts.png)
### *System Calls & Includes*
![2](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%202%20-%20Admin%20Unit/Documents/ECU2%20Calls.png)
### *System Depends*
![3](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%202%20-%20Admin%20Unit/Documents/ECU2%20DependsON.png)
### *System Simple UML Sequence Diagram*
![4](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%202%20-%20Admin%20Unit/Documents/ECU2%20Simple%20UML%20Sequence%20Diagram.png)
> [!NOTE]
> For the full UML Sequence Diagram, please click [here](https://github.com/Hesham-14/Private_Garage_System/blob/main/ECU%202%20-%20Admin%20Unit/Documents/ECU2%20UML%20SequenceDiagram.png).


## 📖 Detailed Overview

### Step 1: Admin Dashboard Authentication
Accessing the specialized administrative dashboard on the Main ECU requires validation through a secure password. The STM32F103C6 ensures this access mechanism remains impenetrable, allowing only authorized personnel.

### Step 2: Step 2: ID Management
Upon successful authentication, the admin can seamlessly navigate the system using the UART terminal, enabling the addition of new, authorized IDs. The LCD, empowered by the STM32F103C6, offers an interactive experience, promptly displaying feedback and confirmation of ID inclusions.

### Step 3: Inter-ECU Communication
As vehicles aim to traverse the garage boundaries, the associated ECUs (Entrance or Exit) engage the Main ECU to validate the presented ID. The SPI communication, an integral feature of the STM32F103C6, facilitates this interaction:

- **Valid ID Scenario**:
  - Post verification, the Main ECU affirms the ID's legitimacy and transmits a "valid" flag to the inquiring ECU. This acknowledgment induces the desired reaction at the respective boundary – primarily, the initiation of the gate mechanism.

- **Invalid ID Scenario**:
  - In case of ID discrepancies, the Main ECU promptly relays an "invalid" signal to the querying ECU. Such instances ensure the gate's sustained closure and activation of associated alert systems, such as buzzers or LEDs, indicating an unauthorized endeavor.

Every ID verification, be it valid or otherwise, is diligently recorded and available for review from the admin dashboard, embodying a layer of transparency and control.
