
#include "usr_wireless.h"
#include "wireless_config.h"

#define MAX 50 //Constante para definir el payload maximo

bool coordinador = true; //TRUE=Cordinador y FALSE=Nodo cambiar segun el nodo a programar
						  //Cambiar tambien la direccion de origen en wireless_config.h
						  
char message[MAX];// cadena para enviar el payload de la primera trama
char message2[MAX];// cadena para enviar el payload de la segunda trama

trama trama_rx;//Estructura para recibir una trama. Se define en usr_wireless.h

bool comparadorPayload = false;//Valor que me permite comparar el payload
bool comparadorDir = false;//Valor que me permite comparar la direccion origen del nodo2 con la direccion destino del nodo1
bool transmitir = true; // Para bloquear la tx y esperar para transmitir de nuevo
bool tipoTx = true; // Para ver que tipo de mensaje enviar

int contTramas = 0;

/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.
	// This code block will be called only if the transmission is enabled.
	
	#ifdef TRANSMITTER_ENABLED		
		
		if(coordinador){
			if(contTramas < 2 && transmitir == true){//Solo si es coordinador puede transmitir
				if(tipoTx){//Para saber si enviar la primera trama 
					memset(&message,'0',sizeof(message));//Relleno mi cadena de ceros para enviarlos en el payload
					transmit_sample_frame((uint8_t*)message,MAX);
				}
				else {
					memset(&message2,'1',sizeof(message2));
					transmit_sample_frame((uint8_t*)message2,MAX);
					tipoTx = true;
				}
				transmitir = false;
				contTramas++;
				delay_ms(10);//Retardo para que no se envie otra trama y esperar a procesar la siguiente trama
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
		//Lo que recibe y responde el nodo 2
		if (!coordinador)
		{
			memset(&trama_rx,0,sizeof(trama_rx));//Se vacia la trama 
			memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));//Se copia la trama que llega en la estructura
			bmm_buffer_free(frame->buffer_header);
			char *payload=trama_rx.carga;//Se obtiene el payload recibido 
			transmit_sample_frame((uint8_t*)payload,MAX);//Se vuelve a transmitir el mensaje recibido
			
		}
		if(coordinador){
			//Lo que recibe y procesa el nodo1(Coordinador)
			comparadorPayload = true;//Para comparar la igualdad del payload
			memset(&trama_rx,0,sizeof(trama_rx));
			memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));
			bmm_buffer_free(frame->buffer_header);
			char *payload=trama_rx.carga;
			
			/*Se comprueba si la direccion de origen de la trama que llega es igual
			a la direccion de origen con la que se envió la trama mas uno*/
			if( (SRC_ADDR+1) == trama_rx.add_origen ){
				comparadorDir = true;
			}
			/*Se comprueba el valor del payload comparando el mensaje que se envió
			con la trama que se recibió del nodo 2*/
			for(int i = 0; i<MAX; i++){ 
				if(payload[i] != message[i]){
					comparadorPayload = false;
					break;
				}
			}
			COMMUTAR_LED(LED1R);
		}
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is transmitted
* \status Status of frame transmission i.e MAC_SUCCESS,MAC_NO_ACK,CHANNEL_ACCESS_FAILURE etc
* \param frame pointer to the transmitted frame
*/
void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{
	if(comparadorPayload && comparadorDir){
		transmitir = true;//Para volver a enviar la siguiente trama en usr_wireless_app_task
		ENCENDER_LED(LED2G);
		tipoTx = false; //Booleano que sirve para mandar el message2 en usr_wireless_app_task
		comparadorPayload = false;//Para poder seguir comparando el payload en usr_frame_received_cb()
		comparadorDir = false;//Para poder seguir comparando la dirección en usr_frame_received_cb()
	}
}
