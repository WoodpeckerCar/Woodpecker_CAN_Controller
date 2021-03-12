#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>

#include "oscc.h"

//#include "commander.h"
#include "teleop_cmd.h"

//#include "can_protocols/steering_can_protocol.h"
#include "can_protocols/steering_angle_can_protocol.h"

#define TELEOP_CMD_UPDATE_INTERVAL_MICRO (50000)
#define SLEEP_TICK_INTERVAL_MICRO (1000)


//ToDo - Remove (Used for Command line testing)
#define INCR_STEP_STEERING_ANGLE	0.05
#define INCR_STEP_THROTTLE_BRAKE	0.05


static int error_thrown = OSCC_OK;



/**
 *
 * Functions and types for Command line testing
 *
 */



// Modes of "A" and "D" functionality
typedef enum
{
	STEER_MODE_AXLE_1;
	STEER_MODE_AXLE_2;
	STEER_MODE_BOTH_AX;
} steering_mode_t;

increment_steer(steering_mode_t steer_mode, int inc_dir, teleop_cmd_input_s* commanded_values)
{
	switch (steer_mode){
		case (STEER_MODE_AXLE_1):
			if((commanded_values->steering_angle_axle_1 < 1.0) && 
				(commanded_values->steering_angle_axle_1 > -1.0))
			{ 
				commanded_values->steering_angle_axle_1 += ( inc_dir * INCR_STEP_STEERING_ANGLE ); 
			}	
			break;
			
		case (STEER_MODE_AXLE_2):
			if((commanded_values->steering_angle_axle_2 < 1.0) && 
				(commanded_values->steering_angle_axle_2 > -1.0))
			{ 
				commanded_values->steering_angle_axle_2 += ( inc_dir * INCR_STEP_STEERING_ANGLE ); 
			}
			break;
			
		default:
			break;
	}
}

// Modes of "W" and "S" functionality
typedef enum
{
	ACCEL_MODE_CONSEQ;
	ACCEL_MODE_BRAKE;
	ACCEL_MODE_ACCEL;
} accel_mode_t;

increment_accel(accel_mode_t accel_mode, int inc_dir, teleop_cmd_input_s* commanded_values){
	switch (accel_mode){
		case (ACCEL_MODE_CONSEQ):
			if((commanded_values->consequent_coefficient < 1.0) && 
				(commanded_values->consequent_coefficient > -1.0))
			{ 
				commanded_values->consequent_coefficient += ( inc_dir * INCR_STEP_THROTTLE_BRAKE ); 
			}	
			
			if (commanded_values->consequent_coefficient > 0.0){
				commanded_values->throttle_position = commanded_values->consequent_coefficient;
				commanded_values->brake_position = 0;
			}
			
			if (commanded_values->consequent_coefficient <= 0.0){
				commanded_values->brake_position = ( -1 * commanded_values->consequent_coefficient );
				commanded_values->throttle_position = 0;
			}
			
			break;
		
		case (ACEEL_MODE_BRAKE):
			break;
			
		case (ACEEL_MODE_ACCEL):
			break;
			
		default:
			break;
	}
	
}



/**
 *
 * Functions for main loop management
 *
 */

static unsigned long long get_timestamp_micro( )
{
    struct timeval time;

    gettimeofday( &time, NULL );

    return ( time.tv_usec );
}

static unsigned long long get_elapsed_time( unsigned long long timestamp )
{
    unsigned long long now = get_timestamp_micro( );
    unsigned long long elapsed_time = now - timestamp;

    return elapsed_time;
}

void signal_handler( int signal_number )
{
    if ( signal_number == SIGINT )
    {
        error_thrown = OSCC_ERROR;
    }
}






int main( int argc, char **argv )
{
    unsigned long long update_timestamp = get_timestamp_micro();
    unsigned long long elapsed_time = 0;
    oscc_result_t ret = OSCC_OK;

	// CONTROL VARIABLES
	
	steering_mode_t steer_mode = STEER_MODE_AXLE_1;
	accel_mode_t accel_mode = ACCEL_MODE_CONSEQ;
	
	
	// Structure of commanded values to be broadcasted on CAN
	teleop_cmd_input_s commanded_values_all =
	{
		.enable_request = CONTROLS_ENABLE;
		.brake_position = 0.0;
		.throttle_position = 0.0;
		.steering_angle_axle_1 = 0.0;
		.steering_angle_axle_2 = 0.0;	
	}

	// INITIALIZE 
	teleop_cmd_init();
	
	char input_char;
	
	if (ret == OSCC_OK)
	{
		while(ret == OSCC_OK && error_thrown == OSCC_OK)
		{
            
			
			// Command line blocking incremental input testing:
			while (1) 
			{
				printf("Input \'a\' to increment or  \'d\' to decrement:  ");
				scanf(" %c", &input_char);
				switch (input_char){
					// Steer
					case ('a'):	increment_steer(steer_mode, ( 1 ), &commanded_values_all); break;
					case ('d'):	increment_steer(steer_mode, (-1 ), &commanded_values_all); break;
					// Accelerate - Decelerate - Brake
					case ('w'):	increment_accel(accel_mode, ( 1 ), &commanded_values_all); break;
					case ('s'):	increment_accel(accel_mode, (-1 ), &commanded_values_all); break;
					
					// Change steer mode
					case ('1'):	steer_mode = STEER_MODE_AXLE_1; 	break;
					case ('2'):	steer_mode = STEER_MODE_AXLE_2; 	break;
					case ('3'):	steer_mode = STEER_MODE_BOTH_AX; 	break;
					// Change acceleration/braking mode
					case ('4'):	accel_mode = ACCEL_MODE_CONSEQ; 	break;
					case ('5'):	accel_mode = ACCEL_MODE_BRAKE;		break;
					case ('6'):	accel_mode = ACCEL_MODE_ACCEL;		break;
					
					default: 
						printf("Input single character - a/d");
						break;
				}	
				
				printf("\n COMMANDS - Angle 1: %lf, Angle 2: %lf, Throttle: %lf, Brake: %lf, ConseqCoef: %lf ", 	
						commanded_values_all.steering_angle_axle_1,
						commanded_values_all.steering_angle_axle_2,
						commanded_values_all.brake_position,
						commanded_values_all.throttle_position,
						commanded_values_all.consequent_coefficient );				
				
				printf("\n CONFIG - Enable Req: %d, Steering Mode: %d, Accel Mode: %d ", 	
						commanded_values_all.enable_request, 
						steer_mode,
						accel_mode );
				
				teleop_cmd_update_outputs( commanded_values_all );

			}
		
		
		
			// Command line blocking specific input testing:
/*				
			printf("Input steering angle: ");
			scanf("%lf", &angle_cmd);
			printf("Angle command: %lf \n\n", angle_cmd);
			oscc_publish_steering_angle(ANGLE_STEERING_AXLE_1, angle_cmd);
*/



			// Rapid cycle:
/*			
			elapsed_time = get_elapsed_time( update_timestamp );

            if ( elapsed_time > (TELEOP_CMD_UPDATE_INTERVAL_MICRO) )
            {
                update_timestamp = get_timestamp_micro();
				
				// CYCLIC ACTIONS
				
				// Implement later:
				// ret = check_for_teleop_cmd_update( );
				
            }

            // Delay 1 ms to avoid loading the CPU
            (void) usleep( SLEEP_TICK_INTERVAL_MICRO );
*/
		}
	}
	
	
	// DEINITIALIZE 
	teleop_cmd_close();

	// ORIGINAL MAIN FUNCTION:
/*
	
    oscc_result_t ret = OSCC_OK;
    unsigned long long update_timestamp = get_timestamp_micro();
    unsigned long long elapsed_time = 0;

    int channel;

    errno = 0;

    if ( argc != 2 || ( channel = atoi( argv[1] ), errno ) != 0 )
    {
        printf( "usage %s channel\n", argv[0] );
        exit( 1 );
    }

    struct sigaction sig;
    sig.sa_handler = signal_handler;
    sigaction( SIGINT, &sig, NULL );

    ret = commander_init( channel );



    if ( ret == OSCC_OK )
    {
        printf( "\nControl Ready:\n" );
        printf( "    START - Enable controls\n" );
        printf( "    BACK - Disable controls\n" );
        printf( "    LEFT TRIGGER - Brake\n" );
        printf( "    RIGHT TRIGGER - Throttle\n" );
        printf( "    LEFT STICK - Steering\n" );
        printf( "    Wakakakakakaakaka - Baka\n" );

        while ( ret == OSCC_OK && error_thrown == OSCC_OK )
        {
            elapsed_time = get_elapsed_time( update_timestamp );

            if ( elapsed_time > COMMANDER_UPDATE_INTERVAL_MICRO )
            {
                update_timestamp = get_timestamp_micro();

                ret = check_for_controller_update( );
            }

            // Delay 1 ms to avoid loading the CPU
            (void) usleep( SLEEP_TICK_INTERVAL_MICRO );
        }
        commander_close( channel );
    }

*/


    return 0;
}

