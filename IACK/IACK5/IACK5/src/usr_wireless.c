#include "usr_wireless.h"
#include "wireless_config.h"
#include "periodic_timer.h"

char tipoNodo = 'R'; // T = Txn ; I = Ixn ; R = Rxn
char *message = "Test"; //Payload de la trama a enviar

trama trama_rx;	//Estructura para recibir la trama
int cont = 0; // Contador para limitar el numero de tramas enviadas
/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.

	#ifdef TRANSMITTER_ENABLED		
	// This code block will be called only if the transmission is enabled.
		if(tipoNodo == 'T' && cont < 1){
			transmit_sample_frame((uint8_t*)message, strlen(message), 0xFFFF);
			delay_ms(20);
			cont++;
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
	if(tipoNodo != 'T'){
		memset(&trama_rx,0,sizeof(trama_rx));//Se vacia la trama
		memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));//Se copia la trama que llega en la estructura
		bmm_buffer_free(frame->buffer_header);
		//Si es el nodo Rxn y la trama que recibio proviene de la direccion origen del nodo-2 transmite de nuevo el payload
		if(tipoNodo == 'R' && (SRC_ADDR-2) == trama_rx.add_origen  ){
			char *payload=trama_rx.carga;//Se obtiene el payload recibido
			transmit_sample_frame((uint8_t*)payload,strlen(message), 0xFFFF);//Se vuelve a transmitir el mensaje recibido
		}
		if (tipoNodo == 'I' && (SRC_ADDR+1) == trama_rx.add_origen  )
		{
			char *payload=trama_rx.carga;//Se obtiene el payload recibido
			transmit_sample_frame((uint8_t*)payload,strlen(message), 0xFFFF);//Se vuelve a transmitir el mensaje recibido
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

	/* Toggle an LED in user-interface */
	/* led_toggle(); */
}

void usr_app_timer_cb(void *parameter)
{
	// INFO (Project Wizard) - This timer callback will be called back periodically based on the time-out value "TIMER_DURATION"
	// TODO (Project Wizard) - Timer Expiry Call back. Perform appropriate timer expiry tasks here.

	/* Toggle an LED in when timer expires */
	/* led_toggle(); */
}