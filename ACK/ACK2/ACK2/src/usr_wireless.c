/**
 * \file usr_wireless.c
 *
 * Created: 26/10/2020 16:54:13
 * Author: cisco2-pc
 *
 * \brief  User level API file of Wireless Project generated by Project Wizard
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \page license License
 * Copyright(c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


#include "usr_wireless.h"
#include "wireless_config.h"

//Constante para definir el tama�o del payload
#define MAX 114

//Variable para manejar cuando es coordinador o nodo
bool coord = true;

//Variable para validar que se haya recibido el ACK.
bool envio = true;

//Variable para manejar el n�mero de tramas que ser�n enviadas por el coordinador
int cont = 0;

//Variables para envio y recepcion de un mensaje en el payload 
char mensajetx [MAX];
char mensajerx [MAX];

//Variable para receptar la trama que fue declarada en usr_wireless.h
tramaData trama_rx;


/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void)
{
	// TODO (Project Wizard) - Add application tasks here.
	// This function will be called repeatedly from main.c. (Refer to function app_task(), WirelessTask() in main.c)
	// The following code demonstrates transmission of a sample packet frame every 1 second.
	
	//Transmision habilitada
	#ifdef TRANSMITTER_ENABLED		
		// This code block will be called only if the transmission is enabled.
		//Validacion de que sea coordinador
		if (coord)
		{
			//Validacion del contador para que se envien solamente dos tramas 
			//y que envie la siguiente una vez recibido el ACK
			if( cont<2 && envio)
			{
				//Delay de 50 ms necesario para que contin�e la transmisi�n caso contrario se interrumpe
				delay_ms(50);
				//Uso de un LED para comprobar si se cumple o no la validaci�n mediante el if
				cambiar(LEDA);
				//Llenar con el caracter "a" la variable mensajetx para su env�o
				memset(&mensajetx,'a',sizeof(mensajetx));
				//Transmisi�n de la trama, dentro del payload se encuentra el contenido de mensajetx
				transmit_sample_frame((uint8_t*)mensajetx,MAX);
				//Aumento el contador
				cont ++;
				//Seteo envio en false esto con la intenci�n de que al cumplirse una validaci�n
				//que se har� despu�s cambie a true y logre enviarse la siguiente trama
				envio = false;
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
		//TODO (Project Wizard) - Add application task when the frame is received

		/* Toggle an LED in when frame is received */
		/* led_toggle(); */
		
		//Validaci�n de que no sea coordinador, es decir, es nodo.
		if (!coord)
		{
			//Blanqueo la trama de recepcion
			memset(&trama_rx,0,sizeof(trama_rx));
			//Copio la mpdu de la trama recibida en la trama creada trama_rx
			memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));
			//Vac�o el buffer para la siguiente recepci�n
			bmm_buffer_free(frame->buffer_header);
			//Llenar con el caracter "b" la variable mensajetx para su env�o
			memset(&mensajerx,'b',sizeof(mensajerx));
			//Uso de un LED para comprobar el funcionamiento del programa
			cambiar(LEDA);
			//Validacion de que la trama de origen sea del coordinador
			if(trama_rx.add_origen == 0x0001){
				//Validaci�n para que el tipo de trama enviada sea de datos 
				if(FCF_GET_FRAMETYPE(trama_rx.FCF & FCF_FRAMETYPE_MASK) == 0x01){ //Comprobacion tipo de trama (datos)
					//Uso de un LED para comprobar que se est� cumpliendo o no la validaci�n del if
					cambiar(LEDR);
					//Transmisi�n de la trama, dentro del payload se encuentra el contenido de mensajerx
					transmit_sample_frame((uint8_t*)mensajerx,MAX);
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
	//TODO (Project Wizard) - Add application tasks when the frame is transmitted

	/* Toggle an LED in user-interface */
	/* led_toggle(); */
	
	//Validaci�n de que sea coordinador 
	if (coord)
	{
		//Condici�n si es que no se recibe el ACK
		if (status == 0xE9)
		{
			//Uso de LED para comprobar funcionamiento del c�digo
			cambiar(LEDV);
			
		}
		//Condici�n si es que se recibi� el ACK
		else
		{
			//Uso de LED para comprobar funcionamiento del c�digo
			cambiar(LEDR);
			//Cambio de la variable envio a true dado que se recibi� el ACK 
			//Con este cambio se asegura que el coordinador enviar� la siguiente trama
			//caso contrario no puede transmitir nuevamente
			envio = true;

		}
	}
}