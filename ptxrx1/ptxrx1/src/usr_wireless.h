
#ifndef USR_WIRELESS_H_
#define USR_WIRELESS_H_

#include "wireless_api.h"

#define maximo 101

typedef struct
{
	uint8_t longitud;
	uint16_t FCF;
	uint8_t nSec;
	uint16_t PAN_dest;
	uint16_t add_dest;
	uint16_t add_origen;
	char carga[maximo];
	uint16_t FCS;
}trama_tx;



/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void);

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is received
* \param frame pointer to the received frame
*/
void usr_frame_received_cb(frame_info_t *frame);

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is transmitted
* \param status  Status of frame transmission i.e MAC_SUCCESS,MAC_NO_ACK,CHANNEL_ACCESS_FAILURE etc
* \param frame pointer to the transmitted frame
*/
void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame);

#endif /* USER_WIRELESS_H_ */