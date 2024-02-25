# Car Monitor by Paul Horvath

License project developed by Paul Horvath Serban (AC CTI RO) - Polytechnic University of Timisoara.

🎓 **paul.horvath@student.upt.ro** 🎓

The proposed project is a system that effectively scans the cars' real-time parameters (such as instant velocity, engine revelations per minute, engine consumption, oil level, coolant level etc), retrieves the data through an OBD-II reader which communicates via Bluetooth to the ESP-32 board. The ESP-32 board then deploys the data to a web server hosted by itself, where a user with internet access can view the data and analyze it accordingly. This project is suitable for multiple fleets owned by ride-sharing companies, which need an app to monitor their own cars and reduce maintenance cost.

The project is developed on ESP-32 microcontroller on a NodeMCU-32S development board. This specific development board has the Wifi and Bluetooth implemented by default, modules which are used at their full capabilities in this project.

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
> RAM:   [==       ] 17.5% (used 57428 bytes from 327680 bytes)\
> Flash: [======== ] 78.5% (used 1543173 bytes from 1966080 bytes)
