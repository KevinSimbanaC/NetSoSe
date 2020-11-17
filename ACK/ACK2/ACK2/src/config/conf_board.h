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

//Declaro las variables para el uso de los tres leds: amarillo(LDEA), rojo (LEDR) y verde(LEDV)

#define LEDV IOPORT_CREATE_PIN(PORTG, 2)
#define LEDR IOPORT_CREATE_PIN(PORTD, 6)
#define LEDA IOPORT_CREATE_PIN(PORTE, 2)

//Funciones para manejar el comportamiendo de los leds
#define apagar(led_gpio) ioport_set_pin_level(led_gpio, 1)
#define prender(led_gpio) ioport_set_pin_level(led_gpio, 0)
#define cambiar(led_gpio) ioport_toggle_pin_level(led_gpio)

#endif // CONF_BOARD_H
