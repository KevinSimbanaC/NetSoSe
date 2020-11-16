
#include "usr_wireless.h"
#include "wireless_config.h"
#include "periodic_timer.h"

#define MAX 12

/**
* \brief This function needs to be edited by the user for adding application tasks
*/
bool coordinador = false;//Valor que permite diferenciar entre un coordinador y un nodo
bool transmitir = true;//Valor que permite bloquear la transmisión
bool comparadorPayload = false;//Valor que me permite comparar el payload
bool comparadorDir = false;//Valor que me permite comparar la direccion origen del nodo2 con la direccion destino del nodo1
bool tipoTx = true; // Para ver que tipo de mensaje enviar

char message[MAX];
char message2[MAX];

trama trama_rx;//Para obtener la trama recibida y manipularla

int contRetransmision = 0;//Para limitar un máximo de retransmisiones
int contTramas = 0;//Para limitar el numero de tramas enviadas por el coordinador

void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.

	#ifdef TRANSMITTER_ENABLED		
		// This code block will be called only if the transmission is enabled.
	if(coordinador){	
	
		if(transmitir && contRetransmision<4 && contTramas<2){
			if(tipoTx){//Se envia el primer tipo de mensaje
				memset(&message,'0',sizeof(message));//Relleno mi cadena de ceros para enviarlos en el payload
				transmit_sample_frame((uint8_t*)message,MAX);
			}
			else {//Se envia el segundo tipo de mensaje
				memset(&message2,'1',sizeof(message2));
				transmit_sample_frame((uint8_t*)message2,MAX);
				tipoTx = true;
			}
			transmitir = false;	
			delay_ms(20);
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
			if(trama_rx.nSec != 0x00){ //Si es la primera trama con numero de secuencia 0 no responde
				char *payload=trama_rx.carga;//Se obtiene el payload recibido
				transmit_sample_frame((uint8_t*)payload,MAX);//Se vuelve a transmitir el mensaje recibido
			}
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
		}		
}

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is transmitted
* \status Status of frame transmission i.e MAC_SUCCESS,MAC_NO_ACK,CHANNEL_ACCESS_FAILURE etc
* \param frame pointer to the transmitted frame
*/
void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{
	if(comparadorPayload && comparadorDir){//Booleanos que son modificados tambien en usr_frame_received_cb()
		COMMUTAR_LED(LED1R);
		transmitir = true;//Se vuelve a habilitar la transmision para enviar otra trama en usr_wireless_app_task
		tipoTx = false; //Booleano que sirve para mandar el message2 en usr_wireless_app_task
		contTramas++;//Se aumeta el contador para limitar las tramas enviadas en usr_wireless_app_task(void)
		contRetransmision=0;//En caso de que si exista respuesta y la comparacion sea exitosa entonce el contador de retransmision se resetea
		comparadorPayload = false;//Para poder seguir comparando el payload
		comparadorDir = false;//Para poder seguir comparando la dirección
	}else{
		//Se inicia el timer para transmitir otra trama en caso de que no exista respuesta por parte del nodo
		start_timer1(); 
	}
}

void usr_app_timer_cb(void *parameter){
	
	COMMUTAR_LED(LED3Y);
	transmitir=true;//Se habilita la transmision de nuevo para transmitir otra trama
	contRetransmision++;//Se aumenta el contador de retransmisión (se admite máximo 4 retransmisiones)
	stop_timer1();//Se detiene el timer
	
}