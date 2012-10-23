
//#[ first_scan_capture_ard.ino ] kwp, 2012-10-20-1346

//##############################################################
/*
A quick program to to XY scan the SEM, take data, and 
output it to the serial line to the computer.
 */

//------------------------------
// Pin assignments

// analog inputs
int pin_video_read_raw              = A0; // console processed video
int pin_se_preamp_read_raw          = A1; // Secondary Electron preamp
int pin_beam_mon_read_raw           = A2; // Beam Monitor preamp
void setup_analog_inputs()
{
    pinMode( pin_video_read_raw,     INPUT );  // Might be importaint %TODO%
    pinMode( pin_se_preamp_read_raw, INPUT );  // Might be importaint %TODO%
    pinMode( pin_beam_mon_read_raw,  INPUT );  // Might be importaint %TODO%
}

// scan control outputs
int pin_scan_x                       =  9; // Sets out x position pin
int pin_scan_y                       = 10; // This sets up out y position pin

// blanking signal out
      int pin_blanking               = 1; 
const int pin_blanking_set_on_const  = LOW;
const int pin_blanking_set_off_const = HIGH;
void      pin_blanking_setup(   void ); 
void      sig_blanking_set_on(  void );
void      sig_blanking_set_off( void );

//------------------------------
// Variables

volatile int video_raw           =   0; // Initializing our sensor value variable

// scan control
volatile int scan_x_start        =   0; // PWM x initial value
volatile int scan_y_start        =   0; // PWM y initial value
volatile int scan_x_stop         = 255; // PWM x final value
volatile int scan_y_stop         = 255; // PWM y final value
volatile int scan_x_step         =  8; // x step size
volatile int scan_y_step         =  8; // y step size
//
volatile int settle_initial_ms   =   1; // This allows us to delay out program from running
volatile int settle_x_ms         =   1; // This is the settle time after each xy pos. change
volatile int settle_y_ms         =   1; // This is the settle time after each xy pos. change
volatile int settle_retrace_x_ms =   1; // retrace settle time x
volatile int settle_retrace_y_ms =   1; // retrace settle time y


//##############################################################

unsigned long   g_since_boot_seconds        = 0;
float           g_since_boot_sec_float      = 0.0;
float           g_since_boot_sec_frac       = 0.0;
char          * g_since_boot_sec_hexfrac_sz = "00";

void do_calc_seconds_hexfrac_globals();
void do_calc_seconds_hexfrac_globals()   //TODO:kwp: make this work
{
g_since_boot_seconds        = 0;
g_since_boot_sec_float      = 0.0;
g_since_boot_sec_frac       = 0.0;
g_since_boot_sec_hexfrac_sz = "00";
}
//==============================================================

void do_print_csv_front( char * board_id_sz, char * rec_format_sz, int finish_record );			
void do_print_csv_front( char * board_id_sz, char * rec_format_sz, int finish_record )
{			
    do_calc_seconds_hexfrac_globals();

    Serial.print( g_since_boot_seconds );
    Serial.print(", ");
    Serial.print( g_since_boot_sec_hexfrac_sz );
    Serial.print(", " );
    Serial.print( rec_format_sz );
    if( finish_record )
        {
        Serial.print( "\r\n");
        }
}
//##############################################################
// 
// int pin_video_read_raw              = A0; // console processed video
// int pin_se_preamp_read_raw          = A1; // Secondary Electron preamp
// int pin_beam_mon_read_raw           = A2; // Beam Monitor preamp
// 
//--------------------------------------------------------------
// video_raw = video_read_raw(); // analogRead(pin_video_read_raw);

int video_read_raw( void )
{
    return analogRead( pin_video_read_raw /* A0 */ ); // Arduino
}
//--------------------------------------------------------------

int se_preamp_read_raw( void )
{
    return analogRead( pin_se_preamp_read_raw ); // Arduino
}
//--------------------------------------------------------------

int beam_mon_read_raw( void )
{
    return analogRead( pin_beam_mon_read_raw ); // Arduino
}
//##############################################################

//    pin_blanking_setup();  // pinMode(      pin_blanking, OUTPUT );

void  pin_blanking_setup(   void ) 
{
	pinMode(      pin_blanking, OUTPUT ); // Arduino
}
//------------------------------

void  sig_blanking_set_on(  void )
{
	digitalWrite( pin_blanking , pin_blanking_set_on_const ); // Arduino
}
//------------------------------

void  sig_blanking_set_off( void )
{
	digitalWrite( pin_blanking , pin_blanking_set_off_const ); // Arduino
}
//##############################################################
// scan_x_set( x ); //analogWrite(pin_scan_x, x); // Move the stage to x

void scan_x_set( int x )
{
	analogWrite( pin_scan_x, x ); // Arduino
}
//------------------------------
// scan_y_set( y ); //analogWrite(pin_scan_y, y); // Move the stage to y

void scan_y_set( int y)
{
	analogWrite( pin_scan_y, y ); // Arduino
}
//##############################################################
// delay_ms( settle_y_ms ); // delay(settle_y_ms);

void delay_ms( int ms )
{
	delay( ms ); // Arduino
}
//##############################################################
//##############################################################

#define USE_SCAN_X 1

#ifdef  USE_SCAN_X

void do_scan_x( void )
{
    sig_blanking_set_off();

    for (int x = scan_x_start ; x <= scan_x_stop ; x +=scan_x_step)
        { // Inner loop (x)
        scan_x_set( x );              // analogWrite(pin_scan_x, x); // Move the stage to x
        delay_ms( settle_x_ms );      // delay(settle_x_ms); // Wait for the motion to settle
        video_raw = video_read_raw();   // analogRead(pin_video_read_raw);
        Serial.print(", ");           // Send the value to the computer
        Serial.print( video_raw );  // Send the value to the computer
        }

    sig_blanking_set_on();
}
#endif // #ifdef USE_SCAN_X

//==============================================================

#define USE_SCAN_XY 1

#ifdef  USE_SCAN_XY

void do_scan_xy( void )
{

    // This puts usinto the correct position, and settles before beginig our loop
    scan_x_set( scan_x_start ); // analogWrite(pin_scan_x, scan_x_start);
    scan_y_set( scan_y_start ); // analogWrite(pin_scan_y, scan_y_start);

    delay_ms( settle_retrace_x_ms ); // delay(settle_retrace_x_ms);
    delay_ms( settle_retrace_y_ms ); // delay(settle_retrace_y_ms);

#if 0
    do_calc_seconds_hexfrac_globals();
    Serial.print( g_since_boot_seconds );
    Serial.print(", ");
    Serial.print( g_since_boot_sec_hexfrac_sz );
    Serial.print(", Board_ID, Rec_Null, \r\n");
#else
    do_print_csv_front(            "Board_ID-x", "Rec_Null-x", 1 );			
//  Serial.print("Seconds, Hexfrac, Board_ID-x,   Rec_Null-x, \r\n");
#endif
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n"); 
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Scan_Start, \r\n");
	
    for( int y = scan_y_start ; y <= scan_y_stop ; y +=scan_y_step )
        {  // Outer loop (y)

#if 1
        do_print_csv_front( "temp_board_ID", "temp_rec_format", 0 );			
   //     Serial.print("Seconds, Hexfrac, Board_ID, ");
   //     Serial.print("Rec_Format, ");
        Serial.print(y);
        Serial.print(", Bloop"); // Send the value to the computer
#endif

        scan_y_set( y );         // analogWrite(pin_scan_y, y); // Move the state to t
        delay_ms( settle_y_ms ); // delay(settle_y_ms);

#ifdef         USE_SCAN_X
        do_scan_x();
#else
        sig_blanking_set_off();
        for (int x = scan_x_start ; x <= scan_x_stop ; x +=scan_x_step)
            { // Inner loop (x)
            scan_x_set( x );                    // analogWrite(pin_scan_x, x); // Move the stage to x
            delay_ms( settle_x_ms );            // delay(settle_x_ms); // Wait for the motion to settle
            video_raw = video_read_raw();         // analogRead(pin_video_read_raw);
            Serial.print(", ");                 // Send the value to the computer
            Serial.print( video_raw );        // Send the value to the computer
            }
        sig_blanking_set_on();
#endif // #ifdef #else USE_SCAN_X
	
        Serial.print("\r\n");                   // Prints a new line when we move y
		
        scan_x_set( scan_x_start );             // analogWrite(pin_scan_x, scan_x_start);
        delay_ms( settle_retrace_x_ms );        // delay(settle_retrace_x_ms);
        }

}
#endif // #ifdef USE_SCAN_XY

//##############################################################
//##############################################################

void setup() 
{// This is treated as out main() function for now
    pinMode(pin_scan_x, OUTPUT); // declare the xPin as an PWM OUTPUT
    pinMode(pin_scan_y, OUTPUT); // declare the yPin as an PWM OUTPUT:

    setup_analog_inputs();
//  pinMode(pin_video_read_raw, INPUT);  // Might be importaint %TODO%
//int pin_se_preamp_read_raw               = A1; // Secondary Electron preamp
//int pin_beam_mon_read_raw                = A2; // Beam Monitor preamp

    pin_blanking_setup();  // pinMode(      pin_blanking, OUTPUT );
    sig_blanking_set_on(); // digitalWrite( pin_blanking, pin_blanking_set_on_const   );

    Serial.begin(9600);  //Sets up a serial output to our computer at 9600 bps

    delay_ms( settle_initial_ms ); // delay(settle_initial_ms); // Wait an initial ammount of time before starting

#ifdef USE_SCAN_XY
    do_scan_xy();
#else
    // This puts usinto the correct position, and settles before beginig our loop
    scan_x_set( scan_x_start ); // analogWrite(pin_scan_x, scan_x_start);
    scan_y_set( scan_y_start ); // analogWrite(pin_scan_y, scan_y_start);

    delay_ms( settle_retrace_x_ms ); // delay(settle_retrace_x_ms);
    delay_ms( settle_retrace_y_ms ); // delay(settle_retrace_y_ms);
	
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n"); 
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Scan_Start, \r\n");

    for( int y = scan_y_start ; y <= scan_y_stop ; y +=scan_y_step )
        {  // Outer loop (y)
			
        Serial.print("Seconds, "); 
        Serial.print("Hexfrac, "); 
        Serial.print("Board_ID, ");
        Serial.print("Rec_Format, ");
        Serial.print(y);
        Serial.print(", Bloop"); // Send the value to the computer
		
        scan_y_set( y );         // analogWrite(pin_scan_y, y); // Move the state to t
        delay_ms( settle_y_ms ); // delay(settle_y_ms);
		
        sig_blanking_set_off();
        for (int x = scan_x_start ; x <= scan_x_stop ; x +=scan_x_step)
            { // Inner loop (x)
            scan_x_set( x );            // analogWrite(pin_scan_x, x); // Move the stage to x
            delay_ms( settle_x_ms );    // delay(settle_x_ms); // Wait for the motion to settle
            video_raw = video_read_raw(); // analogRead(pin_video_read_raw);
            Serial.print(", ");         // Send the value to the computer
            Serial.print(video_raw);  // Send the value to the computer
            }
        sig_blanking_set_on();
		
        Serial.print("\r\n");  // Prints a new line when we move y
		
        scan_x_set( scan_x_start );      // analogWrite(pin_scan_x, scan_x_start);
        delay_ms( settle_retrace_x_ms ); // delay(settle_retrace_x_ms);
        }
#endif // #ifndef #else USE_SCAN_XY

    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Scan_Complete, \r\n");
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n"); 
    Serial.print("Seconds, Hexfrac, Board_ID, Rec_Null, \r\n");

    scan_y_set( scan_y_start );      // analogWrite(pin_scan_y, scan_y_start);
    delay_ms( settle_retrace_y_ms ); // delay_ms( settle_retrace_y_ms ); // delay(settle_retrace_y_ms);
}
//##############################################################
void loop() 
{
// When the data is collected, stop everything.
// Press reset on the computer if you want to run again.

}

//##############################################################
//######################## End of File #########################

