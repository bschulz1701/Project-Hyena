# Project-Hyena
Daughterboard for use with Feather form factor controller, designed to interface with multiple, flexible, low cost sensors at the core of a low cost meteorological sensing system. 

# Repo Roadmap
#### [Documents](Documents/) 

General document and image storage for repo and README

#### [Hardware](Hardware/)

Current board files (Eagle), Bill of Materials (BoM), and other design documents

#### [Production](Production/)

Contains the various Gerber and pick and place files required to have the Printed Circuit Boards (PCBs) manufactured or populated 

#### [Software](Software/)

The software associated with the piece of hardware, this is usually diagnostic software used for verifying or investigating the hardware

#### [Testing](Testing/)

Scripts and results from the testing process and development process. Contains more detailed information about documented issues among other testing. 



## Overview
* **Type:** Data Logger/Daughterboard 
* **Interfaces:** I<sup>2</sup>C, GPIO, Pulse
* **Release Version:** v0.2 - 2020/07/15

![Hyena v0.2 - Top](Documents/Images/Hyena_0v2_Top_Cropped.jpg)

![Hyena v0.2 - Bottom](Documents/Images/Hyena_0v2_Bottom_Cropped.jpg)

## Features
### On-Board
* Micro SD Card 
* RTC w/Battery Backup
* Temperature/Pressure/RH Sensor
* RGB Ambient Light Sensor
* Reverse Polarity Power Protected Inputs 

### Interface
* Tipping Bucket Input (SPST switch compatible)
* Dedicated Potentiometric Input (x1) 
* Flexible Sensor Inputs (x3)
* Dedicated I<sup>2</sup>C Input (x1) 
* Supports Particle and other Feather based controllers 


## Specifications
* **Sensor Voltage:** 3.3V
* **Sensor Current (Max):** 500mA (Combined)

## Jumper Settings 


**Port Select Jumpers**
| Jumper | Purpose | Position 1 | Position 2 | 
| ------ | ------- | ---------- | ---------- |
| `JP1`	| GPIO vs I<sup>2</sup>C - Port 1 Digital | Select I2C | Select GPIO | 
| `JP2` | GPIO vs I<sup>2</sup>C - Port 1 Analog | Select I2C | Select GPIO | 
| `JP3` | GPIO vs I<sup>2</sup>C - Port 2 Digital | Select I2C | Select GPIO | 
| `JP4` | GPIO vs I<sup>2</sup>C - Port 2 Analog | Select I2C | Select GPIO | 
| `JP5` | GPIO vs I<sup>2</sup>C - Port 3 Digital | Select I2C | Select GPIO | 
| `JP6` | GPIO vs I<sup>2</sup>C - Port 3 Analog | Select I2C | Select GPIO | 
| `JP8` | Power Select (Core vs Ext) - Port 1 | Core Power for Port 1 | External Switched Power for Port 1 |

**Configuration Jumpers**
| Jumper | Purpose | Open | Closed | Default | 
| ------ | ------- | ---------- | ---------- | ----- | 
| `JP7` | Enable Hardware WDT | WDT Disabled | WDT Enabled | Closed |  
| `JP9` | Pullup for Port 1 Digital GPIO | Pullup Disabled | Pullup Enabled | Open |
| `JP10` | Gang On-Board and External I2C Buses | I2C Combined | I2C Isolated | Open | 

## Known Issues/Errata

#### WDT Disable

**Issue:** If using Particle Boron as controller and if you have the WDT disabled (leave `JP7` open), the system fails to run properly, or has sporadic resets. This is likely due to parasitic current on the reset line of the Boron.

**Workaround:** Make sure WDT is enabled by closing `JP7`

#### Solder Jumper Stencil

**Issue:** Depending on the thickness of the solder stencil used for populating the board, the solder jumpers may not bridge when desired. This is due to lack of stencil thickness specification and pad geometry.

**Workaround:** Make a note on the board assembly (if working with a board house) as to which jumpers should be connected (see jumper tables). Otherwise, if assembling manually, check the jumpers are connected, and bridge them using solder wire and a soldering iron if they have failed to jump where desired. 