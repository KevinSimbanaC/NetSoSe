

#include "usr_wireless.h"
#include "wireless_config.h"

#define MAX 102 //Constante para definir el tamaño maximo del payload


bool coordinador = true;//cambiar a false en el caso del receptor

trama trama_rx;

/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.

	#ifdef TRANSMITTER_ENABLED		
		if(!ioport_get_pin_level(GPIO_PUSH_BUTTON_0)){
			if(coordinador){
				// This code block will be called only if the transmission is enabled.
				char A[MAX];
				//Lazo que llena de ceros segun el tamaño maximo definido del payload
				for(int i=0;i<MAX;i++){
					A[i]='0';
				}
				transmit_sample_frame((uint8_t*)A,MAX);
				delay_ms(1000);
			}
		}
		
	#endif
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is received
* \param frame pointer to the received frame
*/
void usr_frame_received_cb(frame_info_t *frame)
{
		//TODO (Project Wizard) - Add application task when the frame is received
		/* led_toggle(); */
	if (!coordinador)
	{
		memset(&trama_rx,0,sizeof(trama_rx));
		memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));
		bmm_buffer_free(frame->buffer_header);
		
		if(trama_rx.add_origen==0x0001){	
			//COMMUTAR_LED(LED1R);	
			char B[MAX];
			//Lazo que llena de unos segun el tamaño maximo definido del payload
			for(int i=0;i<MAX;i++){
				B[i]='1';
			}		
			transmit_sample_frame((uint8_t*)B,MAX);
		}
	}
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is transmitted
* \status Status of frame transmission i.e MAC_SUCCESS,MAC_NO_ACK,CHANNEL_ACCESS_FAILURE etc
* \param frame pointer to the transmitted frame
*/
void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{
	//TODO (Project Wizard) - Add application tasks when the frame is transmitted

}