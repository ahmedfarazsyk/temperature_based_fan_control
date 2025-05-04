# Automatic Temperature-Based Fan Control System

## Table of Contents
- [Project Overview](#project-overview)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Technical Specifications](#technical-specifications)
- [Troubleshooting](#troubleshooting)
- [Future Enhancements](#future-enhancements)

## Project Overview
This project implements an intelligent temperature-controlled fan system using:
- **AT89C52 microcontroller** (8051 architecture)
- **LM35 precision temperature sensor**
- **PWM-controlled DC motor**
- **16x2 LCD display** for real-time feedback

The system automatically adjusts fan speed based on ambient temperature, demonstrating energy-efficient thermal management through software-controlled PWM.

## Prerequisites
### Software Requirements
- [Keil µVision 5](https://www.keil.com/download/product/) (C51 compiler)
- [Proteus 8 Professional](https://www.labcenter.com/downloads/)
- Git (for repository cloning)

### Hardware Requirements (for physical implementation)
- AT89C52 microcontroller
- LM35 temperature sensor
- 16x2 LCD module
- DC motor with transistor driver circuit

## Installation
### Clone the repository:
   ```bash
   git clone https://github.com/ahmedfarazsyk/temperature_based_fan_control.git
   cd temperature-based-fan-control
```

### For Keil µVision:
- Open firmware_keil_uvision/temperature_fan.uvproj
- Build the project (F7 or Project → Build Target)
- HEX file will be generated in firmware_keil_uvision/objects/

### For Proteus Simulation:
- Open schema_proteus/temperature_fan.pdsprj
- Right-click AT89C52 → Edit Properties
- Load the generated HEX file from firmware_keil_uvision/objects/
- Set clock frequency to 12 MHz

## Usage
### Simulation Mode
Run the Proteus simulation and adjust temperature using:
- + button to increase temperature
- - button to decrease temperature

### Observe:
- LCD display showing current temperature
- PWM waveform on oscilloscope
- Motor speed changes

### Temperature Control Logic
- Temperature Range |	Fan State |	PWM Duty Cycle
- < 25°C            |	OFF       |	0%
- 25-30°C           |	Medium	  | 50%
- 30-35°C           |	High	    | 75%
- > 35°C	          | Maximum   |	100%


## Technical Specifications
### Firmware Details
- Clock Frequency: 11.0592 MHz
- PWM Resolution: 8-bit (software-implemented)
- LCD Interface: 4-bit mode
- ADC Resolution: 8-bit (via ADC0804)

### Schematic Components
- Microcontroller	(AT89C52):	11.0592 MHz
- Temperature Sensor (LM35):	10mV/°C
- LCD Display	(LM016L):	16x2 characters
- Motor Driver (1N4004):	NPN Transistor


## Troubleshooting
- Issue                    |	Solution
- HEX file not loading     |	Verify Keil build succeeded
- LCD displays garbage     |	Check contrast potentiometer
- Motor not responding     |	Verify transistor connections
- PWM not visible on scope |	Check oscilloscope connections

## Future Enhancements
- Hardware implementation guide
- PID control implementation
- Wireless monitoring via Bluetooth
- Energy consumption metrics
