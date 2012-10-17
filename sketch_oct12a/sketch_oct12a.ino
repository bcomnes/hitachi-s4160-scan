/*
A quick program to to posotion the stage of the SEM, take data, and 
output it to the serial line to the computer.
 */

int sensorPin = A0; // This is the pin we will read out signal voltage with
int xPin = 9;   // Sets out x position pin
int yPin = 10;   // This sets up out y position pin
int sensorValue = 0; // Initializing our sensor value variable 
int settle_ms = 10;  //This is the settle time after each xy pos. change
int x0 = 0; // Our initial x position
int y0 = 0; // Our initial y position
int stageStep = 1; //Our step size for our position stage 

void setup() {
	pinMode(xPin, OUTPUT); // declare the xPin as an PWM OUTPUT
	pinMode(yPin, OUTPUT); // declare the yPin as an PWM OUTPUT:
	Serial.begin(9600);  //Sets up a serial output to our computer at 9600 bps
	delay(settle_ms);
	
	for (int y = y0 ; y <= 255; y +=stageStep) {  //For y = 0 to 255
		analogWrite(yPin, y); // Move the state to t
		for (int x = x0 ; x <= 255; x +=stageStep) { //For x = to 255
		    analogWrite(xPin, x); // Move the stage to x
			delay(settle_ms); // Wait for the motion to settle
			sensorValue = analogRead(sensorPin); // Read the image signal
		    Serial.print(sensorValue; // Send the value to the computer
		}
		Serial.print("\t");  // Prints a new line when we move y
	}
}

void loop() {
	// When the data is collected, stop everything. 
	// Press reset on the computer if you want to run again.	
}