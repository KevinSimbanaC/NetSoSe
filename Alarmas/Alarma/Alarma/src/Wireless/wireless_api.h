/**
 * \file wireless_api.h
 *
 * Created: 29/03/2021 11:33:03
 * Author: cisco2-pc
 *
 * \brief  API file of Wireless Project generated by Project Wizard
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
 * Copyright(c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


#ifndef WIRELESS_API_H_
#define WIRELESS_API_H_

#include <asf.h>
#include "tal.h"
#include "string.h"
#include "ieee_const.h"
#include "tal_helper.h"
#include "usr_wireless.h"

/**
 * \page wl_tal_quick_start Quick Start Guide for Wireless API Functions for performing IEEE 802.15.4 TAL Functionalities
 *
 * This is the quick start guide for the usage of the wireless api layer generated by the project wizard with step by step
 * procedure for creating a simple IEEE 802.15.4 wireless project using the api's provided by this layer
 *
 * \section wl_api_init Wireless Module Initialization:
 * To get started with an wireless application the Wireless-API Layer provides a function wireless_init() which initializes the 
 * Board components used by the project i.e SPI,USART etc, initializes the clock,SPI for Transceiver communication and enables the IRQ
 * Once the basic board initialization is done the same function initializes the selected TAL Layer,if there are any issues with this initialization 
 * the app_alert() function is called to indicate an issue with the initialization.
 * The Channel,Channel page,and other options selcted in the wizard are initialized here.
 * Once the initalization is done all the transceiver related functionalities can be performed using various api's provided
 *
 * \section wl_api_tx Frame Transmission Unit:
 * The transmit_sample_frame() function can be used to transmit a frame in the selected channel and channel page with the selected Transmit Power
 * This function requires two arguments ,the address to the payload and the length of the payload,once both of them are provided 
 * the frame will be transmitted.
 * Once a frame is transmitted the tal_tx_frame_done_cb() is called which hooks the user callback usr_frame_transmitted_cb()
 *  which provides the pointer to the transmitted frame and status of the transmission which can be MAC_SUCCESS or MAC_NO_ACK etc,
 * so that user can perform his tasks based on the status of the transmitted frame.
 *
 * \section wl_api_rx Frame Reception Unit::
 * The init_data_reception() function initializes the frame reception unit,if Promiscuous mode is enabled the receiver is put in RX_ON Mode
 * to not send an ACK ,else it is started in RX_AACK_ON Mode.In order to enable PROMISCUOUS MODE and send ack ,set mode = false in the function.
 * Once a frame is received by the transcmiever the TAL Layer calls a callback function tal_rx_end_cb() which hooks to user callback usr_frame_received_cb
 * with the pointer to the received frame ,the user can add his tasks to be performed when a frame is received. 
 * 
 * \section wl_api_ed ED Scan Feature
 * The TAL Layer provides the energy in the current channel,to get this tal_ed_start() function of the tal needs to be called
 * On completion the tal_ed_end_cb will be called,The following cb code needs to be added .
 * \note FFD switch needs to be added for this functionality
 * Add this to the main file to get ed level of the channel ,Invoked by giving tal_ed_start in the application
 * \code
 * void tal_ed_end_cb(uint8_t energy_level)
 * {
 * 	energy_level = energy_level; 
 * } 
 * \endcode
 *
 */
 
/**
 * \brief This Function initializes the Wireless Module and board components i.e clock,irq etc
 */
void wireless_init(void);

/** 
 * \brief The Receiver is switched on Using this function,
  * When PROMISCUOUS_MODE is enabled the receiver is put in RX_ON mode ,else it is switched on in RX_AACK_ON Mode
 */
void init_data_reception(void);

/*
 * \brief Function to transmit frames as per 802.15.4 std.
 *
 * \param dst_addr_mode     destination address mode - can be 16 or 64 bit
 * \param dst_addr          destination address
 * \param src_addr_mode     source address mode - can be 16 or 64 bit
 * \param msdu_handle       msdu handle for the upper layers to track packets
 * \param payload           data payload pointer
 * \param payload_length    data length
 * \param ack_req           specifies ack requested for frame if set to 1
 * \param csma_mode         specifies the csma mode for transmission
 * \param frame_retry       specifies whether frame retry is required or not
 *
 * \return MAC_SUCCESS      if the TAL has accepted the data for frame
 * transmission
 *         TAL_BUSY         if the TAL is busy servicing the previous tx request
 */
retval_t transmit_frame(uint8_t dst_addr_mode,
  uint8_t* dst_addr,
  uint8_t src_addr_mode,
  uint8_t msdu_handle,
  uint8_t *payload,
  uint8_t payload_length,
  bool ack_req,csma_mode_t csma_mode ,bool frame_retry);

/*
 *
 * \brief This function transmits a sample data frame 
 * \param payload address of the payload to be transmitted
 * \payload_length Length of the payload to be trasnmitted *
 */
void transmit_sample_frame(uint8_t* payload,uint8_t payload_length);


/** \brief This method  is called when an error is encountered during Stack Initialization or to alert the user on any undesired activity */
void app_alert(void);

#endif /* WIRELESS_API_H_ */