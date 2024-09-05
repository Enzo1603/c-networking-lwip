#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char *server_name = "192.168.178.15";
    int server_port = 12002;

    int client_socket;
    struct sockaddr_in server_address;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Fehler beim Erstellen des Sockets");
        exit(1);
    }

    // Setze die IP-Adresse und den Port des Servers
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_name, &server_address.sin_addr) <= 0) {
        perror("Fehler bei der Umwandlung der IP-Adresse");
        exit(1);
    }

    // Verbinde zum Server herstellen
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Fehler beim Herstellen der Verbindung zum Server");
        close(client_socket);
        exit(1);
    }

    // Nachricht einlesen
    char message[1024];
    printf("Input lowercase sentence: ");
    fgets(message, sizeof(message), stdin);

    // Nachrich an den Server senden
    if (send(client_socket, message, strlen(message), 0) < 0) {
        perror("Fehler beim Senden der Nachricht");
        close(client_socket);
        exit(1);
    }

    // Modizifierte Nachricht vom Server empfangen
    char modified_message[1024];
    int message_len = recv(client_socket, modified_message, sizeof(modified_message), 0);
    if (message_len < 0) {
        perror("Fehler beim Empfangen der Nachricht");
        close(client_socket);
        exit(1);
    }

    // Empfangene Nachricht ausgeben
    modified_message[message_len] = '\0';
    printf("From Server: %s\n", modified_message);

    // Socket schliessen
    close(client_socket);

    return 0;
}
