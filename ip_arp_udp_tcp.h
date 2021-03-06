/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/
//@{
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H
#include "ip_config.h"
#include <avr/pgmspace.h>

// -- web server functions --
// you must call this function once before you use any of the other server functions:
extern void init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip,uint16_t port);
// for a UDP server:
extern uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len);
extern void make_udp_reply_from_request(uint8_t *buf,char *data,uint8_t datalen,uint16_t port);
// return 0 to just continue in the packet loop and return the position 
// of the tcp data if there is tcp data part
extern uint16_t packetloop_icmp_tcp(uint8_t *buf,uint16_t plen);
// functions to fill the web pages with data:
extern uint16_t fill_tcp_data_p(uint8_t *buf,uint16_t pos, const const char *progmem_s);
extern uint16_t fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s);


extern uint16_t fill_tcp_data_uint(uint8_t *buf,uint16_t plen,uint8_t i);
extern uint16_t fill_tcp_data_int(uint8_t *buf,uint16_t plen,int8_t i);
// send data from the web server to the client:
extern void www_server_reply(uint8_t *buf,uint16_t dlen);

// -- client functions --
#if defined (WWW_client) || defined (NTP_client)
extern void client_set_gwip(uint8_t *gwipaddr);
extern void client_set_wwwip(uint8_t *wwwipaddr);
#endif

#define HTTP_HEADER_START ((uint16_t)TCP_SRC_PORT_H_P+(buf[TCP_HEADER_LEN_P]>>4)*4)
#ifdef WWW_client
// ----- http get
extern void client_browse_url(const char *urlbuf, char *urlbuf_varpart, const char *hoststr,void (*callback)(uint8_t,uint16_t));
// The callback is a reference to a function which must look like this:
// void browserresult_callback(uint8_t statuscode,uint16_t datapos)
// statuscode=0 means a good webpage was received, with http code 200 OK
// statuscode=1 an http error was received
// statuscode=2 means the other side in not a web server and in this case datapos is also zero
// ----- http post
// client web browser using http POST operation:
// additionalheaderline must be set to NULL if not used.
// postval is a string buffer which can only be de-allocated by the caller 
// when the post operation was really done (e.g when callback was executed).
// postval must be urlencoded.
extern void client_http_post(const char *urlbuf, const char *hoststr, const char *additionalheaderline,char *postval,void (*callback)(uint8_t,uint16_t));
// The callback is a reference to a function which must look like this:
// void browserresult_callback(uint8_t statuscode,uint16_t datapos)
// statuscode=0 means a good webpage was received, with http code 200 OK
// statuscode=1 an http error was received
// statuscode=2 means the other side in not a web server and in this case datapos is also zero
#endif

#ifdef NTP_client
extern void client_ntp_request(uint8_t *buf,uint8_t *ntpip,uint8_t srcport);
extern uint8_t client_ntp_process_answer(uint8_t *buf,uint32_t *time,uint8_t dstport_l);
#endif

// you can find out who ping-ed you if you want:
extern void register_ping_rec_callback(void (*callback)(uint8_t *srcip));

#ifdef PING_client
extern void client_icmp_request(uint8_t *buf,uint8_t *destip);
// you must loop over this function to check if there was a ping reply:
extern uint8_t packetloop_icmp_checkreply(uint8_t *buf,uint8_t *ip_monitoredhost);
#endif // PING_client

#ifdef WOL_client
extern void send_wol(uint8_t *buf,uint8_t *wolmac);
#endif // WOL_client

//
#endif /* IP_ARP_UDP_TCP_H */
//@}

