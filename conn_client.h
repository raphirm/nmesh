#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

int  open_connection_to( in_addr ip, unsigned short portno );
void disconnect_server( int sockfd );

#endif
