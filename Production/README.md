# Production
This folder contains files used to have the hardware manufactured 

## Files

#### Version v0.2 - Release Version

* [Geber](0v2_Release/PrimaryLogger_2020-09-17_x274.zip)
* [Top Mount](0v2_Release/PrimaryLogger_v2.mnt)
* [BoM](0v2_Release/PrimaryLogger_v2_BoM.csv)

### File Conventions 
* `HardwareName.mnb` - These are mount files (pick and place) for the bottom of the board
* `HardwareName.mnt` - These are mount files (pick and place) for the top of the board
* `HardwareName_YYYY-MM-DD_x274.zip` - These are Gerber (PCB) files from a given date in x274 format, which is accepted by most board houses   

#### Notes:

* Take note of Solder Jumper Stencil issue under Errata 
* A [CR1220](https://www.digikey.com/en/products/detail/panasonic-bsg/CR1220/269740) coin cell battery is required 