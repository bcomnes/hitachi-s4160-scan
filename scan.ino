//#####################################################################
/*
A quick program to to posotion the stage of the SEM, take data, and 
output it to the serial line to the computer.
 */
 
 // Pin assignments
int pin_sensor                  = A0; // This is the pin we will read out signal voltage with
int pin_scan_x                  = 9;  // Sets out x position pin
int pin_scan_y                  = 10; // This sets up out y position pin

      int pin_blanking          = 1; 
const int pin_blanking_on       = LOW;
const int pin_blanking_off      = HIGH;

// Variables
volatile int sensorValue        = 0; // Initializing our sensor value variable 
volatile int settle_x_ms        = 1;  // This is the settle time after each xy pos. change
volatile int settle_y_ms        = 1;  // This is the settle time after each xy pos. change
volatile int settle_initial_ms  = 0; // This allows us to delay out program from running
volatile int settle_retrace_x_ms = 1; // retrace settle time x
volatile int settle_retrace_y_ms = 1; // retrace settle time y

volatile int scan_x_start       = 0; // The PWM x initial value
volatile int scan_y_start       = 0; // The PWM y initial value
volatile int scan_x_stop        = 255; // The PWM x final value
volatile int scan_y_stop        = 10; // The PWM y final value
volatile int scan_x_step        = 1; // Our x step size for our position stage
volatile int scan_y_step        = 1; // Our y step size for our position stage

//volatile int blanking = HIGH;

//#####################################################################

void setup() 
{// This is treated as out main() function for now
	pinMode(pin_scan_x, OUTPUT); // declare the xPin as an PWM OUTPUT
	pinMode(pin_scan_y, OUTPUT); // declare the yPin as an PWM OUTPUT:
//	pinMode(pin_sensor, INPUT);  // Might be importaint %TODO%


	pinMode(      pin_blanking, OUTPUT );	
	digitalWrite( pin_blanking, pin_blanking_on   );
	
	Serial.begin(9600);  //Sets up a serial output to our computer at 9600 bps
	
	delay(settle_initial_ms); // Wait an initial ammount of time before starting
	
	// This puts usinto the correct position, and settles before beginig our loop
	analogWrite(pin_scan_x, scan_x_start);
	analogWrite(pin_scan_y, scan_y_start);
	delay(settle_retrace_x_ms);
	delay(settle_retrace_y_ms);
	
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n"); 
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Scan_Start, \r\n");
	
	
	for (int y = scan_y_start ; y <= scan_y_stop ; y +=scan_y_step)
		{  // Outerloop(y)
			
		Serial.print("Seconds, "); 
		Serial.print("Hexfrac, "); 
		Serial.print("Board_ID, ");
		Serial.print("Rec_Format, ");
		Serial.print(y);
		Serial.print(", Bloop"); // Send the value to the computer
		
		analogWrite(pin_scan_y, y); // Move the state to t
		delay(settle_y_ms);
		
		digitalWrite(pin_blanking, pin_blanking_off);
		for (int x = scan_x_start ; x <= scan_x_stop ; x +=scan_x_step)
			{ // Innerloop(x)
			analogWrite(pin_scan_x, x); // Move the stage to x
			delay(settle_x_ms); // Wait for the motion to settle
			sensorValue = analogRead(pin_sensor); // Read the image signal
			Serial.print(", "); // Send the value to the computer
			Serial.print(sensorValue); // Send the value to the computer
			}
		digitalWrite(pin_blanking, pin_blanking_on);
		
		Serial.print("\r\n");  // Prints a new line when we move y
		
		analogWrite(pin_scan_x, scan_x_start);
		delay(settle_retrace_x_ms);
		}



	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Scan_Complete, \r\n");
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n"); 
	Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
	analogWrite(pin_scan_y, scan_y_start);
	delay(settle_retrace_y_ms);
}
//#####################################################################
void loop() {
	// When the data is collected, stop everything. 
	// Press reset on the computer if you want to run again.	
}

//#####################################################################
//####################### End of File ######################
