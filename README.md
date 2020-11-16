# NetSoSe
Repositorio con códigos de ejemplo elaborados para aprender a usar las motas atmega256rfr2
## ptxrx1
Esta carpeta contiene el código básico de transmisión y recepción de tramas, reenviar el payload, no usa ACK.
## tx_rx_ACK
Esta carpeta contiene el código pra activar los leds, reenviar el payload, trabajando con envío de ACK.

## IACK
Carpeta que contiene tres codigos difrentes usando ACK implicito(Sin ACK).
### IACK1
Código que envía una trama desde el coordinador al nodo, el nodo responde enviando una trama hacia el coordinador comprobando la dirección de origen.
Se varía el tamaño del payload cambiando la constante MAX en usr_wireless.c. Para diferenciar al momento de programar entre el coordinador o el nodo, cambiar la variable booleana coordinador en usr_wireless.c y las direcciones en wireless_config.h
### IACK2
Código que envía una trama desde el coordinador al nodo, el nodo responde enviando la misma trama(payload) recibida hacia el coordinador. El coordinador entonces compara el payload y la direccion origen con la trama enviada para proceder a responder con otra trama.
Se varía el tamaño del payload cambiando la constante MAX en usr_wireless.c. Para diferenciar al momento de programar entre el coordinador o el nodo, cambiar la variable booleana coordinador en usr_wireless.c y las direcciones en wireless_config.h
### IACK3
Código que envía tramas desde el coordinador al nodo, el nodo NO responde la primera trama enviada por el coordinador entonces el coordinador hace uso de retransmisión con timers y envia una segunda trama a un T que puede ser cambiado en periodic_timer.h(TIMER_DURATION). 
El nodo  responde a la segunda trama recibida enviando la misma trama(payload) hacia el coordinador. El coordinador entonces compara el payload y la direccion origen con la trama enviada para proceder a responder con otra trama. Se varía el tamaño del payload cambiando la constante MAX en usr_wireless.c. Para diferenciar al momento de programar entre el coordinador o el nodo, cambiar la variable booleana coordinador en usr_wireless.c y las direcciones en wireless_config.h

## ACK
Carpeta que contiene tres codigos difrentes usando ACK explícito(Con ACK).
### ACK1
Código en el que se envía una trama desde el coordinador al nodo, el nodo envía el ACK y transmite una trama hacia el coordinador.
Se varía el tamaño del payload cambiando la constante MAX en usr_wireless.c. Para diferenciar al momento de programar entre el coordinador o el nodo, cambiar la variable booleana coordinador en usr_wireless.c, las direcciones y en valor de la variable ACK Request(1 para el coordinador y 0 para el nodo) en wireless_config.h
### ACK2
Código en el que se envía una trama desde el coordinador al nodo, el nodo envía el ACK y transmite una trama hacia el coordinador, posteriormente el coordinador envía la siguiente trama. En caso de que el coordinador no haya recibido el ACK no puede enviar la siguiente trama.
Se varía el tamaño del payload cambiando la constante MAX en usr_wireless.c. Para diferenciar al momento de programar entre el coordinador o el nodo, cambiar la variable booleana coordinador en usr_wireless.c, las direcciones y en valor de la variable ACK Request(1 para el coordinador y 0 para el nodo) en wireless_config.h


