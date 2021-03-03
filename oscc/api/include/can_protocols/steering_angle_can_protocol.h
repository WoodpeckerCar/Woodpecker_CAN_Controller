/**
 * @file steering_can_protocol.h
 * @brief Steering CAN Protocol 
 *		  for 2byte "EPAS" steering controller ("Chinese" version)
 */


#ifndef _OSCC_ANGLE_STEERING_CAN_PROTOCOL_H_
#define _OSCC_ANGLE_STEERING_CAN_PROTOCOL_H_


#include <stdint.h>

/*
 * @brief Steering angle command message (CAN frame) ID's.
 * 		  
 */
#define OSCC_STEERING_CMD_ANGLE_1_CAN_ID (0x469) // Axle 1
#define OSCC_STEERING_CMD_ANGLE_2_CAN_ID (0x369) // Axle 2


/*
 * @brief Defines for specifying which AXLE is commanded
 * 		  by oscc_publish_steering_angle()
 *		  
 */
#define ANGLE_STEER_AXLE_1		(0x01)
#define ANGLE_STEER_AXLE_2		(0x02)

/*
 * @brief Constants for steering command scaling and shifting
 * 		  
 */
#define ANGLE_STEER_AMPLITUDE	(520)
#define ANGLE_STEER_CENTER		(1024)


#pragma pack(push)
#pragma pack(1)

typedef struct
{
	// 0x00	Mechanical mode
	// 0x10	Assistance mode;
	// 0x20	Angle control mode;
	// 0x22	Angle control mode (angle mode is disabled after manual intervention is cancelled);
    uint8_t control_mode;
	
	uint8_t reserved[2];
    
	// steering angle command
	uint8_t steer_angle[2];
	
	// 0x55 - angle should be set
	// 0x00 - no alignment required 
	uint8_t angle_instruction;
	
	// Default 0x96
    uint8_t angular_velocity;
	
	// XOR(B3, XOR(B2, XOR(B0,B1))) etc. for ALL 7 bytes
	uint8_t xor_check;
} oscc_steering_cmd_angle_s;


#pragma pack(pop)

#endif /* _OSCC_ANGLE_STEERING_CAN_PROTOCOL_H_ */