/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define LED1R IOPORT_CREATE_PIN(PORTD, 6)
#define LED2G IOPORT_CREATE_PIN(PORTG, 2)
#define LED3Y IOPORT_CREATE_PIN(PORTE, 2)

#define GPIO_PUSH_BUTTON_0	IOPORT_CREATE_PIN(PORTE, 0)
#define GPIO_PUSH_BUTTON_ON_BOARD GPIO_PUSH_BUTTON_0

#define APAGAR_LED(led_gpio)	ioport_set_pin_level(led_gpio, 1)
#define ENCENDER_LED(led_gpio)	ioport_set_pin_level(led_gpio, 0)
#define COMMUTAR_LED(led_gpio)	ioport_toggle_pin_level(led_gpio)

#endif // CONF_BOARD_H
