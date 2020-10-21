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

#define LED0 IOPORT_CREATE_PIN(PORTG, 2)
#define apagar(led_gpio) ioport_set_pin_level(led_gpio, 1)
#define prender(led_gpio) ioport_set_pin_level(led_gpio, 0)
#define cambiar(led_gpio) ioport_toggle_pin_level(led_gpio)

#endif // CONF_BOARD_H
