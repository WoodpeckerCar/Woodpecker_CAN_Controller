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

#include "can_protocols/brake_can_protocol.h"
//#include "can_protocols/steering_can_protocol.h"
#include "can_protocols/steering_angle_can_protocol.h"
#include "can_protocols/throttle_can_protocol.h"
#include "can_protocols/fault_can_protocol.h"


// ToDo: Maybe add channel input arguments to ..init() and ..close() (see commander.c)


teleop_cmd_init(){
    int ret_open = OSCC_ERROR;
    ret_open = oscc_open( 1 );
	printf("Opening OSCC: %d\n", ret_open);

	// Is this necessary?
    int ret_enable = OSCC_ERROR;
    ret_enable = oscc_enable( );
	printf("Enabling OSCC: %d\n", ret_enable);

	
    // Register callback handlers (Activate these later)
/*
    oscc_subscribe_to_obd_messages(obd_callback);
    oscc_subscribe_to_brake_reports(brake_callback);
    oscc_subscribe_to_steering_reports(steering_callback);
    oscc_subscribe_to_throttle_reports(throttle_callback);
    oscc_subscribe_to_fault_reports(fault_callback);
*/	
	// Activate listening to UDP (Add)
/*
	....
*/

	return ( ret_open );

}

void teleop_cmd_close( )
{
    oscc_disable( );
    oscc_close( channel );
}


int check_for_teleop_cmd_update( ){
	return 0;
};