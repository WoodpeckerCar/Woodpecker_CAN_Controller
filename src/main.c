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

static int error_thrown = OSCC_OK;

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


	// INITIALIZE 
	teleop_cmd_init();

	double angle_cmd = 0;
	char input_char;
	
	if (ret == OSCC_OK)
	{
		while(ret == OSCC_OK && error_thrown == OSCC_OK)
		{
            
			
			// Command line blocking incremental input testing:
			while (1) {
				printf("Input \'a\' to increment or  \'d\' to decrement:  ");
				scanf(" %c", &input_char);
				switch (input_char){
					case ('a'):
						(angle_cmd < 1.0) ? (angle_cmd += 0.05) : 1;
						break;
					
					case ('d'):
						(angle_cmd > -1.0) ? (angle_cmd -= 0.05) : 1;
						break;

					default: 
						printf("Input single character - a/d");
						break;
				}	
				
				printf("\nAngle command: %lf, Angle in degrees: %lf \n\n", angle_cmd, (angle_cmd*520.0) );

				oscc_publish_steering_angle(ANGLE_STEERING_AXLE_1, angle_cmd);
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

