# NetSoSe
Repositorio con códigos de ejemplo elaborados para aprender a usar las motas atmega256rfr2
## ptxrx1
Esta carpeta contiene el codigo basico de transmision y recepcion de tramas, reenviar el payload, no usa ACK.
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


