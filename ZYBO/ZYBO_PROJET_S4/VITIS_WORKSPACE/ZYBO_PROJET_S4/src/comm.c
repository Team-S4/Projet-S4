#include "comm.h"
//Dependencies
#include "core/net.h"
#include "xil_io.h"
#include "core/socket.h"


#include <stdlib.h>
#include "xparameters.h"
#include <stdio.h>
#include "xil_io.h"
#include "common/os_port.h"
#include "cyclone_tcp/core/net.h"
#include "cyclone_tcp/net_config.h"
#include "cyclone_tcp/drivers/eth/enc624j600_driver.h"
#include "cyclone_tcp/core/udp.h"
#include "common/error.h"
#include "xuartlite.h"
#include "xspi.h"
#include "cyclone_tcp/spi_driver.h"
#include "cyclone_tcp/ext_int_driver.h"
#include <stdbool.h>


error_t error;
   size_t length;
   uint16_t port = 8080;
   IpAddr ipAddr = {4,{0xC0A80165}};

#define APP_USE_DEFAULT_MAC_ADDR ENABLED
#define APP_MAC_ADDR "00-AB-CD-EF-07-95"

#define APP_USE_DHCP DISABLED
#define APP_IPV4_HOST_ADDR "192.168.1.123"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.13.1"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

Socket *socket;
Ipv4Addr ipv4Addr;

void initComm(){


	MacAddr macAddr;

	   //Initialize kernel
	   osInitKernel();

	   //TCP/IP stack initialization
	   netInit();

	   //Configure the first Ethernet interface

	   //Set interface name
	   netSetInterfaceName(&netInterface[0], "eth0");
	   //Set host name
	   netSetHostname(&netInterface[0], "UDPEcho");
	   //Select the relevant network adapter
	   netSetDriver(&netInterface[0], &enc624j600Driver);
	   //netSetPhyDriver(interface, &lan8720PhyDriver);

	   netSetSpiDriver(&netInterface[0], &spiDriver);
	   netSetExtIntDriver(&netInterface[0], &extIntDriver);

	#if (APP_USE_DEFAULT_MAC_ADDR == ENABLED)
	   //Use the factory preprogrammed MAC address
	   macStringToAddr("00-00-00-00-00-00", &macAddr);
	   netSetMacAddr(&netInterface[0], &macAddr);
	#endif

	   //Initialize network interface
	   error = netConfigInterface(&netInterface[0]);
	   //Any error to report?

	#if (IPV4_SUPPORT == ENABLED)

	   //Set IPv4 host address
	   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
	   ipv4SetHostAddr(&netInterface[0], ipv4Addr);

	   //Set subnet mask
	   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
	   ipv4SetSubnetMask(&netInterface[0], ipv4Addr);

	   //Set default gateway
	   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
	   ipv4SetDefaultGateway(&netInterface[0], ipv4Addr);

	   //Set primary and secondary DNS servers
	   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
	   ipv4SetDnsServer(&netInterface[0], 0, ipv4Addr);
	   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
	   ipv4SetDnsServer(&netInterface[0], 1, ipv4Addr);
	#endif
	   netTaskInit();

	  /* if(socket) socketClose(socket);
	   socket = socketOpen(SOCKET_TYPE_DGRAM, SOCKET_IP_PROTO_UDP);

	      //Failed to open socket?
	      if(socket == NULL)
	      {
	         xil_printf("ERROR_OPEN_FAILED \n\r");
	         //Exit immediately
	      }
	      socketBind(socket, &IP_ADDR_ANY, 8080);
	      xil_printf("Comm Ok \n\r");
	      //The server listens for incoming datagrams on port 8080
	      //socketBind(context.socket, &IP_ADDR_ANY, 8080);

*/

}


void transmettreOiseau(char oiseau){
	char buf[12] = {oiseau,oiseau,oiseau,oiseau,oiseau,oiseau,oiseau,oiseau,oiseau,oiseau,oiseau,oiseau};
	ipStringToAddr("192.168.1.122", &ipAddr);
	error_t error = socketSendTo(socket, &ipAddr, 3333, buf, 12, 0, 0);
	xil_printf("%d",error);

}
