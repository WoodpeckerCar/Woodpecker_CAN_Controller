/**
 * @file teleop_cmd.h
 * @brief Command interface for Teleoperations application.
 *
 */


#ifndef TELEOP_CMD_H
#define TELEOP_CMD_H


/**
 * @brief Initialize teleop_cmd for use
 *
 * @param [in] channel to use to communicate with OSCC modules
 *
 * @return ERROR code:
 * \li \ref NOERR (1) if success.
 * \li \ref ERROR (0) if failure.
 *
 */
int teleop_cmd_init( int channel );

/**
 * @brief Close the teleop_cmd.  Releases and closes all modules
 *        under the teleop_cmd also.
 *
 * @param [in] channel used to communicate with OSCC modules
 *
 * @return void
 *
 */
void teleop_cmd_close( int channel );

/**
 * @brief Checks the receivd UDP messages and the the OSCC modules 
 * and updates the current values, including brakes, throttle and 
 * steering. Is expected to execute every 50ms.
 *
 * @param [void]
 *
 * @return ERROR code:
 * \li \ref NOERR (1) if success.
 * \li \ref ERROR (0) if failure.
 *
 */
int check_for_teleop_cmd_update( );


#endif /* TELEOP_CMD_H */