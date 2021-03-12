#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_joystick.h>
//#include <SDL2/SDL_gamecontroller.h>
#include <sys/time.h>
#include <linux/can.h>

#include "oscc.h"
//#include "joystick.h"
#include "vehicles.h"
#include "teleop_cmd.h"

#include "can_protocols/brake_can_protocol.h"
//#include "can_protocols/steering_can_protocol.h"
#include "can_protocols/steering_angle_can_protocol.h"
#include "can_protocols/throttle_can_protocol.h"
#include "can_protocols/fault_can_protocol.h"


static bool control_enabled = false;		// Is an enable command sent to the OSCC modules?
static bool teleop_cmd_enabled = false;		// Has the initialization function been called?



// ToDo: Maybe add channel input arguments to ..init() and ..close() (see commander.c)

//// Prototypes of internal functions called by interface functions

static int teleop_cmd_disable_controls( );
static int teleop_cmd_enable_controls( );


////////////////////////////////
////
////    Interface functions
////
////////////////////////////////

teleop_cmd_init(){
	
 	int return_code = OSCC_ERROR;
 	
	if ( teleop_cmd_enabled == false ) 
	{
		teleop_cmd_enabled = true;
		
		return_code = oscc_open( 1 );
		printf("Opening OSCC: %d\n", reutrn_code);

	
		// Register callback handlers 
		//oscc_subscribe_to_obd_messages(obd_callback);
		oscc_subscribe_to_brake_reports(brake_callback);
		oscc_subscribe_to_angle_steering_1_reports(angle_steering_1_callback);
		oscc_subscribe_to_angle_steering_2_reports(angle_steering_2_callback);
		oscc_subscribe_to_throttle_reports(throttle_callback);
		//oscc_subscribe_to_fault_reports(fault_callback);
		
		// ToDo: Will UDP be initialized here?
		
	}
	
	return ( return_code );

}

// ToDo: add channel as argument?
void teleop_cmd_close( )
{
	if ( teleop_cmd_enabled == true )
	{
		teleop_cmd_enabled = false;
		
	    oscc_disable( );
	    oscc_close( 1 ); 
		
		// ToDo: Add UDP disable?
		
	}
}


oscc_result_t teleop_cmd_update_outputs( teleop_cmd_input_s* commanded_values ){
    oscc_result_t return_code = OSCC_OK;

    switch (commanded_values->enable_request) {
    	case CONTROLS_DISABLE:
			return_code = teleop_cmd_disable_controls();
			break;
		
		case CONTROLS_ENABLE;
			return_code = teleop_cmd_enable_controls();
			break;
		
		case CONTROLS_NO_REQ;
			break;
		
		default:
			return_code = OSCC_ERROR;
    		// ToDo: Print error message?
	}
	
	// If control enabled, begin updating actuator values
	
    if ( (return_code == OSCC_OK)
        && (control_enabled == true) )
    {
        return_code = oscc_publish_brake_position( 
				commanded_values->brake_position);

        if ( return_code == OSCC_OK )
        {
            return_code = oscc_publish_throttle_position(
				commanded_values->throttle_position );
        }

        if ( return_code == OSCC_OK )
        {
            return_code = oscc_publish_steering_angle( 
				ANGLE_STEERING_AXLE_1, 
				commanded_values->steering_angle_axle_1 );
        }

        if ( return_code == OSCC_OK )
        {
            return_code = oscc_publish_steering_angle( 
				ANGLE_STEERING_AXLE_2, 
				commanded_values->steering_angle_axle_2 );
        }
    }

    return return_code;
};


///////////////////////////////
////
////    Internal functions
////
///////////////////////////////


oscc_result_t teleop_cmd_enable_controls( )
{
    oscc_result_t return_code = OSCC_ERROR;

    if ( (teleop_cmd_enabled == true) && (control_enabled == false) )
    {
        printf( "Enable controls\n" );

        return_code = oscc_enable();

        if ( return_code == OSCC_OK )
        {
            control_enabled = true;
        }
    }
    else
    {
        return_code = OSCC_OK;
    }

    return ( return_code );
}

oscc_result_t teleop_cmd_disable_controls( )
{
    oscc_result_t return_code = OSCC_ERROR;

    if ( (commander_enabled == true) && (control_enabled == true) )
    {
        printf( "Disable controls\n" );

        return_code = oscc_disable();

        if ( return_code == OSCC_OK )
        {
            control_enabled = false;
        }
    }
    else
    {
        return_code = OSCC_OK;
    }

    return return_code;
}



// Callback functions performed upon receival of CAN messages


static void throttle_callback(oscc_throttle_report_s *report)
{
    if ( report->operator_override )
    {
         //commander_disable_controls();

        printf("Override: Throttle\n");
    }
}


static void angle_steering_1_callback(oscc_steering_report_s *report)
{
    //printf("angle_steering_1_callback\n");
    ++ global_incremental;
	
}
static void angle_steering_2_callback(oscc_steering_report_s *report)
{
    //printf("angle_steering_2_callback\n");
}


static void brake_callback(oscc_brake_report_s * report)
{
    if ( report->operator_override )
    {
        // commander_disable_controls();

        printf("Override: Brake\n");
    }
//    printf("Brake callback!\n");
}







