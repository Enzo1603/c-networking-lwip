#include "lwip/src/include/lwip/init.h"     // Initialisierung
#include "lwip/src/include/lwip/netif.h"    // Netzwerk-Interface
#include "lwip/src/include/lwip/opt.h"      // Optionale Einstellungen
#include "lwip/src/include/lwip/sockets.h"  // Socket-API (ähnlich wie BSD-Sockets)
#include "lwip/tcp.h"                       // TCP-Stack
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "lwip/udp.h"        // UDP-Stack
#include "netif/ethernet.h"  // Ethernet-Treiber (falls Ethernet verwendet wird)
#include "netif/ethernet.h"

int main(void) {
    tcpip_init(tcpip_init_done, NULL);  // Initialisiere lwIP in einem separaten Thread
    // RTOS-Scheduler starten, damit das Netzwerk läuft

    // lwIP initialisieren
    lwip_init();

    // Netzwerkinterface hinzufügen und konfigurieren
    struct netif netif;
    ip4_addr_t ipaddr, netmask, gw;

    IP4_ADDR(&ipaddr, 192, 168, 1, 100);   // IP-Adresse des Geräts
    IP4_ADDR(&gw, 192, 168, 1, 1);         // Gateway
    IP4_ADDR(&netmask, 255, 255, 255, 0);  // Subnetzmaske

    netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    while (1) {
        sys_check_timeouts();  // Timer-Verwaltung in NO_SYS-Modus
        // Netzwerk-Pakete abholen und verarbeiten
    }

    return 0;
}

void tcpip_init_done(void *arg) {
    // Hier kannst du das Netzwerkinterface hinzufügen und konfigurieren
}
