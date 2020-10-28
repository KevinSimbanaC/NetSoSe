/**
 * \file usr_wireless.c
 *
 * Created: 26/10/2020 10:50:30
 * Author: kYDke
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

bool coordinador = true;
char *message = "Hola";
char *message2 = "h0la";
trama trama_rx;
bool comparador = false;
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
				transmit_sample_frame((uint8_t*)message,strlen(message));
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
		//Lo que responde el nodo 2
		if (!coordinador)
		{
			memset(&trama_rx,0,sizeof(trama_rx));
			memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));
			bmm_buffer_free(frame->buffer_header);
			char *payload=trama_rx.carga;
			transmit_sample_frame((uint8_t*)payload,strlen(message));
			
		}
		if(coordinador){
			//Lo que responde el nodo1
			comparador = true;
			memset(&trama_rx,0,sizeof(trama_rx));
			memcpy(&trama_rx,frame->mpdu,sizeof(trama_rx));
			bmm_buffer_free(frame->buffer_header);
			char *payload=trama_rx.carga;
			
			for(int i = 0; i<strlen(message);i++){
				if(payload[i] != message[i]){
					comparador = false;
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
	if(comparador){
		transmit_sample_frame((uint8_t*)message2,strlen(message2));
		comparador = false;
	}
}