/**
 * \file wireless_api.c
 *
 * Created: 18/11/2020 11:57:23
 * Author: kYDke
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

#include "wireless_api.h"
#include "wireless_config.h"

/*Buffer for Transmit Payload*/
static uint8_t storage_buffer[LARGE_BUFFER_SIZE];

/**
 * \brief This Function initializes the Wireless Module and board components i.e clock,irq etc
 */
void wireless_init(void)
{
	/*Initialize IRQ*/
	irq_initialize_vectors();

  sysclk_init();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();


	/*Initialize the Stack SW Timer*/
	sw_timer_init();
	
	/*Initialize the TAL Layer*/
	if(tal_init()!= MAC_SUCCESS)
	{
	//Error in Initializing the TAL Layer
	app_alert();	
	}
	cpu_irq_enable();
	
	uint16_t src_addr = CCPU_ENDIAN_TO_LE16(SRC_ADDR);
	uint16_t pan_id = CCPU_ENDIAN_TO_LE16(SRC_PAN_ID);
	uint8_t channel = CHANNEL_TRANSMIT_RECEIVE;
	uint8_t channel_page = CHANNEL_PAGE_TRANSMIT_RECEIVE;
	
	/* Set Default address. */
	tal_pib_set(macShortAddress, (pib_value_t *)&src_addr);

	/* Set PAN ID. */
	tal_pib_set(macPANId, (pib_value_t *)&pan_id);

	/* Set channel. */ /* Channel 11  is set as default in tal_init() */
	tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel);
	
	/* Set Channel Page */ /* Channel Page 0  is set as default in tal_init() */
	tal_pib_set(phyCurrentPage,(pib_value_t *)&channel_page);
	
	//TODO (Project Wizard) - Change the Transmit Power of the transceiver if required
	/* The Following Lines of Code are used to set the transmit power */	
	/* 	int8_t tx_pwr_dbm = 3;	 // transmit power = 3dBm
	
	uint8_t temp_var;
	
	temp_var = CONV_DBM_TO_phyTransmitPower(tx_pwr_dbm);

	tal_pib_set(phyTransmitPower, (pib_value_t *)&temp_var);	 */

	#if (ANTENNA_DIVERSITY == 1)
    if(ENABLE_ANTENNA_DIVERSITY)
    {
	    tal_ant_div_config(ANT_DIVERSITY_ENABLE,ANTENNA_DEFAULT);
    }
    else
    {
	    tal_ant_div_config(ANT_DIVERSITY_DISABLE,ANT_SELECT); 
    }
    #endif


	init_data_reception();

}

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
		bool ack_req,csma_mode_t csma_mode ,bool frame_retry)
{
	frame_info_t *tx_frame_info;
	uint8_t i;
	uint8_t frame_length;
	uint8_t *frame_ptr;
	uint8_t *temp_frame_ptr;
	uint16_t temp_value,fcf = 0;
	/* Get length of current frame. */

	tx_frame_info = (frame_info_t *)storage_buffer;
	/* Get length of current frame. */
	frame_length = (FRAME_OVERHEAD + payload_length); 

	/* Set payload pointer. */     
	frame_ptr = temp_frame_ptr = (uint8_t *)tx_frame_info +
	LARGE_BUFFER_SIZE -
	payload_length - FCS_LEN;

	/*
	* Payload is stored to the end of the buffer avoiding payload
	* copying by TAL.
	*/
	for (i = 0; i < payload_length; i++) {
	*temp_frame_ptr++ = *(payload + i);
	}


	/* Source address */
	if (FCF_SHORT_ADDR == src_addr_mode) {
	frame_ptr -= SHORT_ADDR_LEN;
	convert_16_bit_to_byte_array(tal_pib.ShortAddress, frame_ptr);

	fcf |= FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR);
	} else {
	frame_ptr -= EXT_ADDR_LEN;
	frame_length += FCF_2_SOURCE_ADDR_OFFSET;

	convert_64_bit_to_byte_array(tal_pib.IeeeAddress, frame_ptr);

	fcf |= FCF_SET_SOURCE_ADDR_MODE(FCF_LONG_ADDR);
	}

		/* Source PAN-Id */
		#if (DST_PAN_ID == SRC_PAN_ID)
		/* No source PAN-Id included, but FCF updated. */
		fcf |= FCF_PAN_ID_COMPRESSION;
		#else
		frame_ptr -= PAN_ID_LEN;
		temp_value = CCPU_ENDIAN_TO_LE16(SRC_PAN_ID);
		convert_16_bit_to_byte_array(temp_value, frame_ptr);
		#endif

	/* Destination address */
	if (FCF_SHORT_ADDR == dst_addr_mode) {
		frame_ptr -= SHORT_ADDR_LEN;
		convert_16_bit_to_byte_array(*((uint16_t *)dst_addr),
				frame_ptr);

		fcf |= FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);
	} else {
		frame_ptr -= EXT_ADDR_LEN;
		frame_length += PL_POS_DST_ADDR_START;

		convert_64_bit_to_byte_array(*((uint64_t *)dst_addr),
				frame_ptr);

		fcf |= FCF_SET_DEST_ADDR_MODE(FCF_LONG_ADDR);
	}


	/* Destination PAN-Id */
	temp_value = CCPU_ENDIAN_TO_LE16(DST_PAN_ID);
	frame_ptr -= PAN_ID_LEN;
	convert_16_bit_to_byte_array(temp_value, frame_ptr);

	/* Set DSN. */
	frame_ptr--;
	*frame_ptr = msdu_handle;

	/* Set the FCF. */
	fcf |= FCF_FRAMETYPE_DATA;
	if (ack_req) {
	fcf |= FCF_ACK_REQUEST;
	}

	frame_ptr -= FCF_LEN;
	convert_16_bit_to_byte_array(CCPU_ENDIAN_TO_LE16(fcf), frame_ptr);

	/* First element shall be length of PHY frame. */
	frame_ptr--;
	*frame_ptr = frame_length;

	/* Finished building of frame. */
	tx_frame_info->mpdu = frame_ptr;

	return(tal_tx_frame(tx_frame_info, csma_mode, frame_retry));
}

/*
 *
 * \brief This function transmits a sample data frame 
 * \param payload address of the payload to be transmitted
 * \payload_length Length of the payload to be trasnmitted *
 */
void transmit_sample_frame(uint8_t* payload,uint8_t payload_length) 
{
	static uint16_t seq_num = 0;   
	bool ack_req = ACK_REQ;
	bool frame_retry = FRAME_RETRY;
	csma_mode_t csma_mode = CSMA_MODE;
	uint16_t dst_addr = CCPU_ENDIAN_TO_LE16((uint16_t)DST_ADDR);

	transmit_frame(
		FCF_SHORT_ADDR,
		(uint8_t*)&dst_addr,
		FCF_SHORT_ADDR,
		seq_num++,
		payload,
		payload_length,
		ack_req,
		csma_mode,
		frame_retry);

}

/** 
 * \brief The Receiver is switched on Using this function,
  * When PROMISCUOUS_MODE is enabled the receiver is put in RX_ON mode ,else it is switched on in RX_AACK_ON Mode
 */
void init_data_reception()
{
       /*Enable Promiscuous Mode pib attribute to put the transceiver in RX_ON mode.*/
       #ifdef PROMISCUOUS_MODE
		   bool mode = true;
		   tal_rxaack_prom_mode_ctrl(true);
		   tal_pib_set(macPromiscuousMode, (pib_value_t *)&mode);
       #endif
       /*RX_AACK_ON Mode is enabled if Promiscuous Mode is not used,else RX is switched on in RX_ON Mode*/
       tal_rx_enable(PHY_RX_ON); 
}



/**
 *
 * \brief This method  is called when an error is encountered during Stack Initialization
 * or to alert the user on any undesired activity
 *
 */
void app_alert(void)
{
	/* Indicate error - flash an LED */
	
	while(1)
	{
		//led_toggle();
		//delay_ms(100);
	}
	
}