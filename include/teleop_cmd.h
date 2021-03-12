/**
 * @file teleop_cmd.h
 * @brief Command interface for Teleoperations application.
 *
 */


#ifndef TELEOP_CMD_H
#define TELEOP_CMD_H


//ToDo - remove this
volatile int global_incremental;

//ToDo - add F/R/N type when control module will be introduced

typedef enum
{
	CONTROLS_DISABLE;
	CONTROLS_ENABLE;
	CONTROLS_NO_REQ;
} teleop_oscc_enable_t;

typedef struct 
{
	teleop_oscc_enable_t enable_request;		
	double brake_position;
	double throttle_position;
	double steering_angle_axle_1;
	double steering_angle_axle_2;	
	
	
	// ToDo: Remove (For command line testing)
	double consequent_coefficient;	
	
	
} teleop_cmd_input_s;




/**
 * @brief Initialize teleop_cmd for use
 *
 * @param [in] (REMOVED) channel to use to communicate with OSCC modules
 *
 * @return ERROR code:
 * \li \ref NOERR (1) if success.
 * \li \ref ERROR (0) if failure.
 *
 */
int teleop_cmd_init( );

/**
 * @brief Close the teleop_cmd.  Releases and closes all modules
 *        under the teleop_cmd also.
 *
 * @param [in] (REMOVED) channel used to communicate with OSCC modules
 *
 * @return void
 *
 */
void teleop_cmd_close( );

/**
 * @brief Checks the receivd UDP messages and the the OSCC modules 
 * and updates the current values, including brakes, throttle and 
 * steering. Is expected to execute every 50ms.
 *
 * @param [in] commanded_values struct containging commands for all modules
 *
 * @return ERROR code:
 * \li \ref NOERR (1) if success.
 * \li \ref ERROR (0) if failure.
 *
 */
oscc_result_t teleop_cmd_update_outputs( teleop_cmd_input_s* commanded_values );



/**
 * @brief 	Impelment these callbacks to send out feedback 
 *			coming from components over CAN bus.
 *			
 *			These functions are executed asynchronously by sigaction.
 */


static void brake_callback(oscc_brake_report_s *report);
static void angle_steering_1_callback(oscc_steering_report_s *report);
static void angle_steering_2_callback(oscc_steering_report_s *report);
static void throttle_callback(oscc_throttle_report_s *report);







#endif /* TELEOP_CMD_H */
