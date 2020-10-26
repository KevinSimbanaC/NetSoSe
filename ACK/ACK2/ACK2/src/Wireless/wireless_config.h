/**
 * \file wireless_config.h
 *
 * Created: 26/10/2020 16:54:05
 * Author: cisco2-pc
 *
 * \brief  Configuration file with transceiver details generated by Project Wizard
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


#ifndef WL_WIZARD_CONFIG_H_
#define WL_WIZARD_CONFIG_H_


//Transeiver Configuration
#define TRANSMITTER_ENABLED

#define DEFAULT_PAN_ID          0xCAFE

#define SRC_ADDR				0x0002
#define SRC_PAN_ID              0xCAFE

#define CHANNEL_TRANSMIT_RECEIVE		18
#define CHANNEL_PAGE_TRANSMIT_RECEIVE	0

#define DST_ADDR		0x0001
#define DST_PAN_ID      0xCAFE
#define ACK_REQ			0
#define FRAME_RETRY		0
#define CSMA_MODE		CSMA_UNSLOTTED

#define ENABLE_ANTENNA_1                          (1)
#define ENABLE_ANTENNA_2                          (2)

#define ANT_SELECT					ENABLE_ANTENNA_2
#define ENABLE_ANTENNA_DIVERSITY	false

#define FRAME_OVERHEAD          (11)


#endif /* WL_WIZARD_CONFIG_H_ */