/*
* Javier Abellan, 20 Jun 2000
*
* Programa Cliente de un socket INET, como ejemplo de utilizacion
* de las funciones de sockets
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client_socket.h"

int main ()
{
	ClientSocket client;
	char Cadena[100];
	client.connet("localhost", "eeg");

	strcpy (Cadena, "Hola");
	client.writeSocket(Cadena, 5);

	client.readSocket(Cadena, 5);

	printf ("Soy cliente, He recibido : %s\n", Cadena);

	client.closeSocket();
}
