# Car Monitor by Paul Horvath

License project developed by Paul Horvath Serban (AC CTI RO) - Polytechnic University of Timisoara.

🎓 **paul.horvath@student.upt.ro** 🎓

The proposed project is a system that effectively scans the cars' real-time parameters (such as instant velocity, engine revelations per minute, engine consumption, oil level, coolant level etc), retrieves the data through an OBD-II reader which communicates via Bluetooth to the ESP-32 board. The ESP-32 board then deploys the data to a web server hosted by itself, where a user with internet access can view the data and analyze it accordingly. This project is suitable for multiple fleets owned by ride-sharing companies, which need an app to monitor their own cars and reduce maintenance cost.

The project is developed on ESP-32 microcontroller on a NodeMCU-32S development board. This specific development board has the Wifi and Bluetooth implemented by default, modules which are used at their full capabilities in this project.


 # [RO] Rularea proiectului

 # Adresa repository-ului:
 
 https://github.com/paulhth/CarMonitor          - GitHub
 
 https://gitlab.upt.ro/paul.horvath/CarMonitor  - GitLab
 
 # Rularea proiectului

1. Se identifica portul OBD-II al masinii si se conecteaza cititorul (Vgate iCar Pro V2) la masina.
> Este necesar ca masina sa aiba minim contactul pus dar se recomanda sa se porneasca motorul masinii pentru a accesa parametrii.

2. Se seteaza atat numele dispozitivului OBD-II la care se va conecta ESP-32 cat si SSID si parola retelei din fisierul de configurare, retea in care se va realiza afisarea interfetei.

3. Se conecteaza ESP-32 la o sursa de alimentare. 
> Este recomandat sa se conecteze prin cablu de date USB-A to microUSB la laptopul dotat cu extensia PlatformIO in Visual Studio Code, Arduino IDE, sau orice aplicatie care are integrat un Serial Monitor pentru a urmari statusul lui ESP-32.
> 
- :warning: In cazul in care placa nu are codul incarcat in memoria sa, este necesara urcarea prin apasarea butonului PlatformIO: Upload. Acesta este un script care compileaza sistemul, iar daca nu sunt erori, va sterge memoria placii si o va incarca cu noul cod dorit.
Dupa terminarea incarcarii, codul se va rula automat si terminalul de monitorizare se va deschide. De asemenea, pentru montarea fisierelor statice prin SPIFFS (daca nu au fost deja montate) se va accesa meniul PlatformIO: Upload Filesystem Image.

4. ESP-32 se va conecta atat la reteaua setata, cat si la cititorul OBD-II. Acesta va incepe prin setarea modului de coexistenta, conectarea la retea, furnizarea de IP local pentru accesarea interfetei, initializarea canalului de comunicare cu OBD-II, apoi va interoga parametrii setati. Progresul va fi afisat in Serial Monitor, iar parametrii vor fi afisati pe serverul web in timp real.

> ESP-32 indica faptul ca s-a reusit conectarea la retea (si furnizearea de IP) prin aprinderea unui LED integrat de culoarea albastra 

# [EN] Running the project
# Repository Address:

[GitHub](https://github.com/paulhth/CarMonitor)  
[GitLab](https://gitlab.upt.ro/paul.horvath/CarMonitor)

# Running the Project

1. Identify the OBD-II port of the car and connect the reader (Vgate iCar Pro V2) to the car.
    > The car needs to at least have the ignition on, but it is recommended to start the car's engine to access the parameters.

2. Set both the name of the OBD-II device to which the ESP-32 will connect and the SSID and password of the network in the configuration file, the network where the interface will be displayed.

3. Connect the ESP-32 to a power source.
    > It is recommended to connect it via a USB-A to microUSB data cable to a laptop equipped with the PlatformIO extension in Visual Studio Code, Arduino IDE, or any application that has an integrated Serial Monitor to monitor the status of the ESP-32.
    > 
    > :warning: If the board does not have the code loaded in its memory, it is necessary to upload it by pressing the PlatformIO: Upload button. This is a script that compiles the system, and if there are no errors, it will erase the board's memory and load it with the new desired code. After the upload is complete, the code will run automatically, and the monitoring terminal will open. Also, to mount the static files through SPIFFS (if they have not already been mounted), access the PlatformIO: Upload Filesystem Image menu.

4. The ESP-32 will connect to both the set network and the OBD-II reader. It will start by setting the coexistence mode, connecting to the network, providing a local IP for accessing the interface, initializing the communication channel with OBD-II, and then querying the set parameters. The progress will be displayed in the Serial Monitor, and the parameters will be displayed on the web server in real time.
    > The ESP-32 indicates that it has successfully connected to the network (and provided an IP) by lighting up an integrated blue LED.


# Organizing the project

The project is organized in two phases: ![Phase 1](https://img.shields.io/badge/Phase-1-blue) and ![Phase 2](https://img.shields.io/badge/Phase-2-green).

![Phase 1](https://img.shields.io/badge/Phase-1-blue) is represented by the board's capability to successfully deploy and sustain a web server, where the real-time data will be prompted and displayed to the final user.

![Phase 2](https://img.shields.io/badge/Phase-2-green) is represented by the board's capability to successfully pair itself with the OBD-II device through the Bluetooth module and successfully read and temporarily store the real-time data.

# Understanding the version control system

The project is organized into branches, where the branch **main** is the official release. The branches which derive from main and where the work is effectively done, have a nomenclature that must be obeyed at all times. A typical branch has the following template:

**Px-Bx** where **Px** refers to the phase (which can be either one or two) and **Bx** refers to the branch itself. Example:

**P1-B3** - Phase 1, Branch 3.

> **Note:** The branch management system may be updated at any time, expect the README.md file to be updated when this happens.

# Dependencies

There are several dependencies which are necessary for the project to work:

**ESP Async WebServer**          - Used to locally host and maintain the frontend webserver.
**powerbroker2/ELMDuino@^3.2.5** - Top-level library used to send/receive commands and information through the OBDII adapter.

> **Note:** At the moment, the ESP-32 board has the following memory layout:
> RAM:   [==       ] 17.7% (used 57428 bytes from 327680 bytes)\
> Flash: [======== ] 79.5% (used 1543173 bytes from 1966080 bytes)

# Known problems and their fixes

- When connecting to a hotspot: The network must be on 2.4GHz, the ESP-32 board cannot connect to 5GHz networks
- Use the FreeRTOS coexistence logic to manage the access to the RF antenna between the WiFi and Bluetooth modules.

# Upcoming code and documentation aspects

- When documenting, don't forget to add Sequence Diagrams in PlantUML. [Add the extension to VS Code](https://marketplace.visualstudio.com/items?itemName=jebbs.plantuml)
- For code: -> Check if the parameters show up fine inside the local network
            -> Possibility of remote host using AWS
            -> Extension of ELMduino API

