
#include "usr_wireless.h"
#include "wireless_config.h"
#include "periodic_timer.h"


/**
* \brief This function needs to be edited by the user for adding application tasks
*/
bool x = true;	

void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.

	#ifdef TRANSMITTER_ENABLED		
		// This code block will be called only if the transmission is enabled.
		transmit_sample_frame((uint8_t*)"Hello World!", 12);	
		if(x){
			start_timer1();
			x = false;		
		}
		delay_ms(1000);
		
	#endif
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is received
* \param frame pointer to the received frame
*/
void usr_frame_received_cb(frame_info_t *frame)
{
		//TODO (Project Wizard) - Add application task when the frame is received

		/* Toggle an LED in when frame is received */
		/* led_toggle(); */
		
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is transmitted
* \status Status of frame transmission i.e MAC_SUCCESS,MAC_NO_ACK,CHANNEL_ACCESS_FAILURE etc
* \param frame pointer to the transmitted frame
*/
void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{
	//TODO (Project Wizard) - Add application tasks when the frame is transmitted

	/* Toggle an LED in user-interface */
	/* led_toggle(); */
}

void usr_app_timer_cb(void *parameter){
	
	ENCENDER_LED(LED1R);
	stop_timer1();
}