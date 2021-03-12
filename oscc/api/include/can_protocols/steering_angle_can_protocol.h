/**
 * @file steering_can_protocol.h
 * @brief Steering CAN Protocol 
 *		  for 16 bit "EPAS" steering controller ("Chinese" version)
 */


#ifndef _OSCC_ANGLE_STEERING_CAN_PROTOCOL_H_
#define _OSCC_ANGLE_STEERING_CAN_PROTOCOL_H_


#include <stdint.h>

/*
 * @brief Steering angle command message (CAN frame) ID's.
 * 		  
 */
#define OSCC_ANGLE_STEERING_1_CMD_CAN_ID (0x469) // Axle 1
#define OSCC_ANGLE_STEERING_2_CMD_CAN_ID (0x369) // Axle 2

/*
 * @brief Steering angle report message (CAN frame) ID.
 *
 */
#define OSCC_ANGLE_STEERING_1_REPORT_CAN_ID (0x401)
#define OSCC_ANGLE_STEERING_2_REPORT_CAN_ID (0x301)

/*
 * @brief Defines for specifying which AXLE is commanded
 * 		  by oscc_publish_steering_angle()
 *		  
 */
#define ANGLE_STEERING_AXLE_1		(0x01)
#define ANGLE_STEERING_AXLE_2		(0x02)

/*
 * @brief Constants for steering command scaling and shifting
 * 		  
 */
#define ANGLE_STEERING_AMPLITUDE	(520)  // How many degrees to each side
#define ANGLE_STEERING_CENTER		(1024) // Equals 0 degrees command


#pragma pack(push)
#pragma pack(1)


/**
 * @brief Steering command message data.
 *
 * CAN frame ID: \ref OSCC_ANGLE_STEERING_CMD_1_CAN_ID
 * 				OR:   OSCC_ANGLE_STEERING_CMD_2_CAN_ID
 *
 */

typedef struct
{
	// 0x00	Mechanical mode
	// 0x10	Assistance mode;
	// 0x20	Angle control mode;
	// 0x22	Angle control mode (angle mode is disabled after manual intervention is cancelled);
    uint8_t control_mode;
	
	uint8_t reserved[2];
    
	// steering angle command
	uint8_t steer_angle_H;
	uint8_t steer_angle_L;
	
	// 0x55 - angle should be set
	// 0x00 - no alignment required 
	uint8_t angle_instruction;
	
	// Default 0x96
    uint8_t angular_velocity;
	
	// XOR(B3, XOR(B2, XOR(B0,B1))) etc. for ALL 7 bytes
	uint8_t xor_check;
} oscc_steering_cmd_angle_s;


/**
 * @brief Angle steering report message data.
 *
 * CAN frame ID: \ref OSCC_ANGLE_STEERING_REPORT_1_CAN_ID
 * 				OR:   OSCC_ANGLE_STEERING_REPORT_2_CAN_ID
 *
 */

typedef struct
{
    // See manual for content interpretation
	uint8_t working_mode;
	uint8_t	steering_wheel_torq;
	uint8_t fault_code_1;
	uint8_t steer_angle_H;	
	uint8_t steer_angle_L;
	uint8_t feedback_on_execution;
	uint8_t fault_code_2;
	uint8_t	xor_check;
	
} oscc_angle_steering_report_s;

#pragma pack(pop)

#endif /* _OSCC_ANGLE_STEERING_CAN_PROTOCOL_H_ */
