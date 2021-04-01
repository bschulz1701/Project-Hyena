#include <SPI.h>
#include <Wire.h>
#include <MCP7940.h>
#include <SD.h>
//FIX Make random function??

boolean Debug = false; //FIX!!
char ReadArray[25] = {0};

// namespace Pins { //Use for Boron
// 	constexpr uint16_t WD_HOLD  = 7;
// 	constexpr uint16_t EXT_EN   = 8;
// 	constexpr uint16_t SD_EN    = 9;
// 	constexpr uint16_t SD_CS    = 14;
// 	constexpr uint16_t SDA_CTRL = 2;
// 	constexpr uint16_t TIP      = 15;
// 	constexpr uint16_t GPIOXA 	= 16;
// 	constexpr uint16_t Clock_INT 	= 2;
// }

namespace Pins { //Use for Feather M0 Express
	constexpr uint16_t WD_HOLD  = 12;
	constexpr uint16_t EXT_EN   = 13;
	constexpr uint16_t SD_EN    = 1;
	constexpr uint16_t SD_CS    = 19;
	constexpr uint16_t SDA_CTRL = 5;
	constexpr uint16_t TIP      = 18;
	constexpr uint16_t GPIOXA 	= 17;
	constexpr uint16_t Clock_INT 	= 6;
}

//////////////////////////////////////I2C/////////////////////////////////////
uint8_t ExpectedAddresses[] = {0x20, 0x39, 0x6F, 0x70}; //Define the I2C devices which should be present on Hyena 
uint8_t NumDevices = 4; //This is the number of I2C devices expected 

////////////////////////////////////SD////////////////////////////////////////
char FileNameC[11]; //Used for file handling
boolean SD_Init = false;

////////////////////////////////////CLOCK//////////////////////////////////////
MCP7940 Clock; // Instantiate Clock

void setup() {
	Serial.begin(115200); //Begin fast serial 
	pinMode(Pins::EXT_EN, OUTPUT);
	digitalWrite(Pins::EXT_EN, HIGH); //Turn on external power 
	while(!Serial); //Wait for serial to start //DEBUG!
	Wire.begin(); //Start I2C module
  

}

void loop() {
	Serial.println("Begin Test...");
	Serial.print("SD:\t");
	Serial.println(BoolToString(TestSD()));
	Serial.print("I2C:\t");
	Serial.println(BoolToString(TestI2C()));
	Serial.print("Sense:\t");
	Serial.println(BoolToString(TestSensors()));
	Serial.print("RTC:\t");
	Serial.println(BoolToString(TestClock()));
	Serial.print("Current Time:\t");
	Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
	Serial.println("...End Test");
	while(1);
}

bool TestSD() 
{
	pinMode(Pins::SD_EN, OUTPUT); //Ensure EN pin is setup as output
	digitalWrite(Pins::SD_EN, HIGH); //Set enable pin to turn on power

	boolean SDError = false;
	pinMode(Pins::SD_CS, OUTPUT);
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	// SPI.setDataMode(SPI_MODE0);
	// SPI.setClockDivider(SPI_CLOCK_DIV2); //Sts SPI clock to 4 MHz for an 8 MHz system clock

	if(!SD_Init) {
		if (!SD.begin(Pins::SD_CS)) {
			if(Debug) Serial.println("Failed Init");
			SDError = true; 
			return SDError;
		}
		if(Debug) Serial.println("Initialized");
		SD_Init = true;
	}
	String FileName = "HWTest";

	(FileName + ".txt").toCharArray(FileNameC, 11);
	SD.remove(FileNameC); //Remove any previous files

	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	int RandVal = random(30557); //Generate a random number between 0 and 30557 (the number of words in Hamlet)
	char RandDigits[5] = {0};
	sprintf(RandDigits, "%d", RandVal); //Convert RandVal into a series of digits
	int RandLength = (int)((ceil(log10(RandVal))+1)*sizeof(char)); //Find the length of the values in the array
//	Serial.print("Random Digits:"); //DEBUG!
//	for(int p = 0; p < 5; p++) { //DEBUG!
//	Serial.print(RandDigits[p]);
//	}
//	Serial.println(""); //DEBUG!

	File DataWrite = SD.open(FileNameC, FILE_WRITE);
	if(DataWrite) {
		DataWrite.println(RandVal);
		DataWrite.println("\nHe was a man. Take him for all in all.");
		DataWrite.println("I shall not look upon his like again.");
		DataWrite.println("-Hamlet, Act 1, Scene 2");
	}
	DataWrite.close();

	if(Debug){
		Serial.print("Random Value = "); Serial.println(RandVal);
	}

	char TestDigits[5] = {0};
	File DataRead = SD.open(FileNameC, FILE_READ);
	if(DataRead) {
		DataRead.read(TestDigits, RandLength);

		if(Debug){
		  Serial.print("Recieved Value = ");
		}
		for(int i = 0; i < RandLength - 1; i++){ //Test random value string
			if(Debug) Serial.print(TestDigits[i]);
			if(TestDigits[i] != RandDigits[i]) {
				SDError = true;
				Serial.print("x"); //DEBUG!
				Serial.print(RandDigits[i]);
			}
		}
		if(Debug) Serial.println("");

		if(Debug) {
		//      byte Data = DataRead.read();
			Serial.println("SD Test Data:");
			while(DataRead.available()) {
				Serial.write(DataRead.read());
			//        Data = DataRead.read();
			}
		}
	}
	DataRead.close();

	if(Debug) Serial.println("");

	SD.remove(FileNameC); //Remove dummy file created 

	return SDError;
}

bool TestI2C() //Test against fixed list 
{
	pinMode(Pins::SDA_CTRL, OUTPUT); //Setup SDA control as an output
	digitalWrite(Pins::SDA_CTRL, HIGH); //Set turn on on board SDA

	for(int i = 0; i < NumDevices; i++) {
		Wire.beginTransmission(ExpectedAddresses[i]);
		Wire.write(0x00);
		if(Wire.endTransmission() != 0) {
			Serial.println(ExpectedAddresses[i]);
			return 1; //Return failure if any device fails
		}
	}

	return 0; //If all devices return 0, then return passing state 
}

bool TestClock()
{
	bool Clock_Error = 1; //Default error
  pinMode(Pins::SDA_CTRL, OUTPUT); //Setup SDA control as an output
  digitalWrite(Pins::SDA_CTRL, HIGH); //Set turn on on board SDA
	pinMode(Pins::Clock_INT, INPUT); //Setup Clock pin as input
	// Serial.print("Start Time:\t");
	// Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
  Clock.Begin();
  Clock.ClearAlarm(0);
  Clock.ClearAlarm(1);
  Clock.SetMode(MCP7940::Mode::Inverted); //Use dual alarm setup 
  Clock.SetTime(2021, 06, 10, 2, 12, 0, 0); //Set arbitrary time (incept date) //DEBUG!
	Clock.SetAlarm(5); //Set an alarm for 5 seconds from now

	unsigned long LocalTime = millis();
  // Serial.print("Alarm State = "); 
  // Serial.println(Clock.ReadAlarm());
	while(digitalRead(Pins::Clock_INT) != HIGH && (millis() - LocalTime < 7000)); //Wait for Clock to trigger, or 7 seconds to go by 
  // Serial.print("Alarm State = "); 
  // Serial.println(Clock.ReadAlarm());
	if(millis() - LocalTime < 7000 && (millis() - LocalTime > 3000)) { //Ensure time passed is greater than 3 seconds, less than 7 seconds 
		// Serial.print("End Time:\t");
		// Serial.println(return Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
		if(Clock.ReadAlarm(0) == true) Clock_Error = 0; //Clear error
		else Clock_Error = 1; //Set error flag if interrupt bit not setg
	}
	else {
		// Serial.println("ERROR: No Alarm!");
		Clock_Error = 1; //Set alarm flag
	}

	return Clock_Error; //Return the resulting state
}

bool TestSensors()
{
	return 0; //FIX! 
}

String BoolToString(bool State)
{
	String Vals[] = {"Pass", "Fail"};
	return Vals[State]; //Return appropriate value based on State
}
