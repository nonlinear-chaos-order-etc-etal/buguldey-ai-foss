/* $Id: util_ssl.h,v 1.5 2004/06/23 17:43:36 kuhlmann Exp $ */

#ifdef ENABLE_SSL
#ifndef MICQ_UTIL_SSL_H
#define MICQ_UTIL_SSL_H

/* Number of bits mICQ uses for Diffie-Hellman key exchange of
 * incoming direct connections. You may change this value.
 */
#define DH_OFFER_BITS       768
/* Number of bits mICQ enforces as a minimum for DH of outgoing 
 * direct connections. 
 * This value must be less than or equal to 512 in order to work with licq.
 */
#define DH_EXPECT_BITS      512

int SSLInit ();
int ssl_write (Connection *conn, UBYTE *data, UWORD len);
int ssl_read (Connection *conn, UBYTE *data, UWORD len);
void ssl_close (Connection *conn DEBUGPARAM);
void ssl_disconnect (Connection *conn DEBUGPARAM);
int ssl_supported (Connection *conn DEBUGPARAM);
int ssl_connect (Connection *conn, BOOL is_client DEBUGPARAM);
int ssl_handshake (Connection *conn DEBUGPARAM);
const char *ssl_strerror (int error);
BOOL TCPSendSSLReq (Connection *list, Contact *cont);

#define ssl_close(c)      ssl_close(c DEBUGARGS)
#define ssl_disconnect(c) ssl_disconnect(c DEBUGARGS)
#define ssl_supported(c)  ssl_supported(c DEBUGARGS)
#define ssl_connect(c,i)  ssl_connect(c,i DEBUGARGS)
#define ssl_handshake(c)  ssl_handshake(c DEBUGARGS)

#define LICQ_WITHSSL        0x7D800000  /* taken from licq 1.2.7 */

#define dc_write    ssl_write
#define dc_read     ssl_read
#define dc_close    ssl_close
#if ENABLE_GNUTLS
#define dc_strerror gnutls_strerror
#else
#define dc_strerror(e) "OpenSSL error"
#endif

#endif /* MICQ_UTIL_SSL_H */
#else
#define dc_write(c,d,l) sockwrite((c)->sok,d,l)
#define dc_read(c,d,l)  sockread((c)->sok,d,l)
#define dc_close(c)     sockclose((c)->sok)
#define dc_strerror     strerror
#endif
