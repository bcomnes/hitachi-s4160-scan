
//#[ first_scan_capture_ard.ino ] kwp, 2012-10-21-0825

#define SQUARE_ARRAY_SIDE 256

#define CSV_BOARD_ID     id_woof

//##############################################################
/*
A quick program to to XY scan the SEM, take data, and 
output it to the serial line to the computer.
 */

//------------------------------
// Pin assignments

// analog inputs
int     pin_video_read_raw              = A0; // console processed video
int     pin_se_preamp_read_raw          = A1; // Secondary Electron preamp
int     pin_beam_mon_read_raw           = A2; // Beam Monitor preamp
void    setup_analog_inputs( void );
void    setup_analog_inputs( void )
{
    pinMode( pin_video_read_raw,     INPUT );  // Might be importaint %TODO%
    pinMode( pin_se_preamp_read_raw, INPUT );  // Might be importaint %TODO%
    pinMode( pin_beam_mon_read_raw,  INPUT );  // Might be importaint %TODO%
}

// scan control outputs
int     pin_scan_x                      =  9; // Sets out x position pin
int     pin_scan_y                      = 10; // This sets up out y position pin
void    setup_scan_outputs( void );
void    setup_scan_outputs( void )
{
    pinMode( pin_scan_x, OUTPUT);       // declare the xPin as an PWM OUTPUT
    pinMode( pin_scan_y, OUTPUT);       // declare the yPin as an PWM OUTPUT:
}

// blanking signal out
      int pin_blanking                  = 1; 
const int pin_blanking_set_on_const     = LOW;
const int pin_blanking_set_off_const    = HIGH;
void      setup_blanking_outputs( void ); 
void      sig_blanking_set_on(    void );
void      sig_blanking_set_off(   void );

// relay signal out
      int pin_relay                     = 2; 
const int pin_relay_set_on_const     = HIGH;
const int pin_relay_set_off_const    = LOW;
void      setup_relay_outputs( void ); 
void      sig_relay_set_on(    void );
void      sig_relay_set_off(   void );

//------------------------------
// Variables

volatile int video_raw           =   0; // Initializing our sensor value variable

//------------------------------
// AWG -- Arbitrary Waveform Generation, for now, rotation
//        scan_x_set( awg_x[ x ] );j
#define        SIZE_AWG         SQUARE_ARRAY_SIDE
int     awg_x[ SIZE_AWG ];
int     awg_y[ SIZE_AWG ];
void setup_awg_default( void );
void setup_awg_default( void )
{
    for( int i=0 ; i<SIZE_AWG ; i++ )
        { // setup a default of data equals index
        awg_x[ i ] = i;
        awg_y[ i ] = i;
        }
}
void awg_calculate( int raster_y, float theta );
void awg_calculate( int raster_y, float theta ) //TODO:kwp: make work
{
    for( int i=0 ; i<SIZE_AWG ; i++ )
        { // setup a DUMMY of data equals index
        awg_x[ i ] = i;
        awg_y[ i ] = i;
        }
}
const float   theta_zero = 0.0;
//------------------------------

// scan control
//volatile int scan_x_start        =        0; // PWM x initial value
//volatile int scan_y_start        =        0; // PWM y initial value
//volatile int scan_x_stop         = SQUARE_ARRAY_SIDE; // PWM x final value
//volatile int scan_y_stop         = SQUARE_ARRAY_SIDE; // PWM y final value
//volatile int scan_x_step         =        1; // x step size
//volatile int scan_y_step         =        1; // y step size
//
volatile int settle_relay_ms     = 100; // This allows us to delay out program from running
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

int video_read_raw( void )
{
    return analogRead( pin_video_read_raw ); // Arduino
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

void  sig_blanking_set_off( void )
{
	digitalWrite( pin_blanking , pin_blanking_set_off_const ); // Arduino
}
//------------------------------

void  sig_blanking_set_on(  void )
{
	digitalWrite( pin_blanking , pin_blanking_set_on_const ); // Arduino
}
//------------------------------

void  setup_blanking_outputs(   void ) 
{
	pinMode( pin_blanking, OUTPUT ); // Arduino
    sig_blanking_set_on();
}
//##############################################################

void  sig_relay_set_off( void )
{
	digitalWrite( pin_relay , pin_relay_set_off_const ); // Arduino
}
//------------------------------

void  sig_relay_set_on(  void )
{
	digitalWrite( pin_relay , pin_relay_set_on_const ); // Arduino
}
//------------------------------

void  setup_relay_outputs(   void ) 
{
	pinMode( pin_relay, OUTPUT ); // Arduino
    sig_relay_set_on();
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

void setup_USB_serial( int bps );
void setup_USB_serial( int bps )
{
    Serial.begin( bps );
}
//##############################################################
//##############################################################

void do_raster_scan_x( int raster_y )
{
    sig_blanking_set_off();

    for( int raster_x = 0 ; raster_x < SQUARE_ARRAY_SIDE ; raster_x++ )
        { // Inner loop (x)
#if 1
        scan_x_set( awg_x[ raster_x ] );
        scan_y_set( awg_y[ raster_y ] );
#else
        scan_x_set( raster_x );
        scan_y_set( raster_y );
#endif

        delay_ms( settle_x_ms );

        // get data and add to horiz. scan record
        video_raw = video_read_raw();
        Serial.print(", ");
        Serial.print( video_raw );
        }

    sig_blanking_set_on();
}
//==============================================================

void do_raster_scan_xy( float theta )
{

    // This puts usinto the correct position, and settles before beginig our loop
//    scan_x_set( scan_x_start );
//    scan_y_set( scan_y_start );

    delay_ms( max( settle_retrace_x_ms, settle_retrace_y_ms ) );

#if 0
    do_calc_seconds_hexfrac_globals();
    Serial.print( g_since_boot_seconds );
    Serial.print(", ");
    Serial.print( g_since_boot_sec_hexfrac_sz );
    Serial.print(", Board_ID, Rec_Null, \r\n");
#endif

	
    for( int raster_y = 0 ; raster_y < SQUARE_ARRAY_SIDE ; raster_y++ )
        {  // Outer loop (y)
     // awg_calculate( int raster_y, float theta );
        awg_calculate(     raster_y, theta );

        // output front of the horizontal scan line CSV record
        do_print_csv_front( "CSV_BOARD_ID", "temp_rec_format", 0 );			
        Serial.print( raster_y );
        Serial.print(", ##");

        do_raster_scan_x( raster_y );
	
        // output end of the horizontal scan line CSV record
        Serial.print("\r\n");
		
//        scan_x_set( scan_x_start );             // analogWrite(pin_scan_x, scan_x_start);
        delay_ms( settle_retrace_x_ms );        // delay(settle_retrace_x_ms);
        }
}
//##############################################################
//##############################################################

void    do_it( float theta )
{
    sig_relay_set_on();

    delay_ms( settle_relay_ms );

    do_print_csv_front( "CSV_BOARD_ID", "Rec_Null-x", 1 );
    do_print_csv_front( "CSV_BOARD_ID", "Rec_Null-x", 1 );
    do_print_csv_front( "CSV_BOARD_ID", "Rec_Null-x", 1 );

    do_print_csv_front( "CSV_BOARD_ID", "Rec_Scan_Start",    1 );
    do_raster_scan_xy( theta );
    do_print_csv_front( "CSV_BOARD_ID", "Rec_Scan_Complete", 1 );

    do_print_csv_front( "CSV_BOARD_ID", "Rec_Null-x", 1 );
    do_print_csv_front( "CSV_BOARD_ID", "Rec_Null-x", 1 );
    do_print_csv_front( "CSV_BOARD_ID", "Rec_Null-x", 1 );

    sig_relay_set_off();
}
//##############################################################
//##############################################################

void our_setup() //TODO: move out of Arduino .ino file
{// This is treated as our main() function for now

    setup_USB_serial( 9600 );
    setup_scan_outputs();
    setup_analog_inputs();

    setup_blanking_outputs();
    setup_awg_default();

    setup_relay_outputs();

//    do_it( theta_zero );
}
//##############################################################

void our_loop() //TODO: move out of Arduino .ino file
{
// When the data is collected, stop everything.
// Press reset on the computer if you want to run again.

      do_it( theta_zero );
      delay_ms( 1000 );

}

//##############################################################
//##############################################################
//##############################################################

void setup() // Arduino MANDATED name function
{
    our_setup();
}
//--------------------------------------------------------------

void loop() // Arduino MANDATED name function
{
    our_loop();
}

//##############################################################
//######################## End of File #########################

