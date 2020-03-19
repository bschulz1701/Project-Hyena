
volatile bool Pulse = false; //Global flag to set on a pulse event 
const uint8_t PulsePin = 2; //Pin to measure pulse on

void setup() {
	Serial.begin(9600);
	Serial.println("Welcome to The Machine...");
	attachInterrupt(digitalPinToInterrupt(PulsePin), PulseEvent, FALLING); //Configure interrupt
}

void loop() {
	static unsigned long EventTime = millis(); //Used to keep track of time between events
	if(Pulse == true) {
		unsigned long EventDelta = millis() - EventTime; //Capture event time period immediately 
		EventTime = millis(); //Reset timing delta 
		Serial.print("Pulse Delta = ");
		Serial.print(EventDelta);
		Serial.println(" ms");
		Pulse = false; //Clear pulse flag
	}
}

void PulseEvent() 
{
	Pulse = true; //Set pulse flag
}