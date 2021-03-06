/* $Id: util_io.h,v 1.20 2003/10/11 23:07:44 kuhlmann Exp $ */

#ifndef MICQ_UTIL_IO_H
#define MICQ_UTIL_IO_H

void    UtilIOConnectUDP (Connection *conn);
void    UtilIOConnectTCP (Connection *conn DEBUGPARAM);
void    UtilIOConnectF   (Connection *conn);
int     UtilIOError      (Connection *conn);
void    UtilIOSocksAccept(Connection *conn);
Packet *UtilIOReceiveUDP (Connection *conn);
Packet *UtilIOReceiveTCP (Connection *conn);
Packet *UtilIOReceiveF   (Connection *conn);
BOOL    UtilIOSendTCP    (Connection *conn, Packet *pak);
void    UtilIOSendUDP    (Connection *conn, Packet *pak);
strc_t  UtilIOReadline   (FILE *fd);

void    UtilIOSelectInit (int sec, int usec);
void    UtilIOSelectAdd  (FD_T sok, int nr);
BOOL    UtilIOSelectIs   (FD_T sok, int nr);
void    UtilIOSelect     (void);

#define READFDS   1
#define WRITEFDS  2
#define EXCEPTFDS 4

#define UtilIOConnectTCP(c) UtilIOConnectTCP(c DEBUGARGS)

#endif /* MICQ_UTIL_IO_H */
