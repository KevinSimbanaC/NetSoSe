
#include "usr_wireless.h"
#include "wireless_config.h"

bool coordinador = true;
bool primera = false;
char* A = "Hola";
char* B = "Pepe";

trama_tx trama_rx;

/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.

	#ifdef TRANSMITTER_ENABLED	
	
	if((!ioport_get_pin_level(GPIO_PUSH_BUTTON_0)) || primera ){
		//retardo para conocer la presion del boton
		delay_ms(200);
		if (coordinador)
		{
			//ENCENDER_LED(LED2G);
			//ENCENDER_LED(LED1R);
			ENCENDER_LED(LED3Y);
			// This code block will be called only if the transmission is enabled.
			transmit_sample_frame((uint8_t*)A, strlen(A));	
			delay_ms(1000);
			primera=true;
		
		}
	}
	
	#endif

	/* Examples : */

	/* Toggle an LED in when frame is received */
    /* led_toggle(); */

    /* Check for button press */
    /* if( button_pressed() )
    {
        // Add application specific code here
    } */
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is received
* \param frame pointer to the received frame
*/
void usr_frame_received_cb(frame_info_t *frame)
{		
		if (!coordinador)
		{
			//delay_ms(2000);
			memset(&trama_rx,0,sizeof(trama_rx));
			memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));
			bmm_buffer_free(frame->buffer_header);
			//COMMUTAR_LED()
			//if(trama_rx.add_origen==0xAA02){
				char *payload=trama_rx.carga;
				//memcpy(&payload,trama_rx.carga,11);
				transmit_sample_frame((uint8_t*)B,4);
			//}
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

	/* Toggle an LED in user-interface */
	//ENCENDER_LED(LED2G);
	/*if(! ioport_get_pin_level(GPIO_PUSH_BUTTON_0)){
		//delay_ms(200);
		COMMUTAR_LED(LED2G);
	}*/
	
}
