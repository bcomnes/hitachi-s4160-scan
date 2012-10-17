//#####################################################################
/*
A quick program to to posotion the stage of the SEM, take data, and 
output it to the serial line to the computer.
 */
 
 // Pin assignments
int pin_sensor = A0; // This is the pin we will read out signal voltage with
int pin_scan_x = 9;   // Sets out x position pin
int pin_scan_y = 10;   // This sets up out y position pin

// Variables
volatile int sensorValue = 0; // Initializing our sensor value variable 
volatile int settle_ms = 10;  //This is the settle time after each xy pos. change
volatile int initial_settle_ms = 100;

volatile int scan_x_start = 0;
volatile int scan_y_start = 0;
volatile int scan_x_stop = 255;
volatile int scan_y_stop = 255;
volatile int scan_x_step = 1; //Our step size for our position stage
volatile int scan_y_step = 1; 

//#####################################################################

void setup() 
{
	pinMode(pin_scan_x, OUTPUT); // declare the xPin as an PWM OUTPUT
	pinMode(pin_scan_y, OUTPUT); // declare the yPin as an PWM OUTPUT:
//	pinMode(pin_sensor, INPUT); 
	
	Serial.begin(9600);  //Sets up a serial output to our computer at 9600 bps
	
	delay(initial_settle_ms);
	
	for (int y = scan_y_start ; y <= scan_y_stop ; y +=scan_y_step)
		{  // Outerloop(y)
	    
		Serial.print("Seconds, "); 
		Serial.print("Hexfrac, "); 
		Serial.print("Board_ID, ");
		Serial.print("Rec_Format, "); 				
	    Serial.print(y);
		Serial.print(", Bloop"); // Send the value to the computer			
		
		analogWrite(pin_scan_y, y); // Move the state to t
		
		for (int x = scan_x_start ; x <= scan_x_stop ; x +=scan_x_step)
			{ // Innerloop(x)
		    analogWrite(pin_scan_x, x); // Move the stage to x
			delay(settle_ms); // Wait for the motion to settle
			sensorValue = analogRead(pin_sensor); // Read the image signal
		    Serial.print(", "); // Send the value to the computer			
		    Serial.print(sensorValue); // Send the value to the computer
			}
		Serial.print("\n");  // Prints a new line when we move y
		}
}
//#####################################################################
void loop() {
	// When the data is collected, stop everything. 
	// Press reset on the computer if you want to run again.	
}

//#####################################################################
//####################### End of File ######################