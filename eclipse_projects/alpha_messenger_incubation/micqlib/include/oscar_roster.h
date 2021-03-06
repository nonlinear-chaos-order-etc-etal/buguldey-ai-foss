
#ifndef MICQ_OSCAR_ROSTER_H
#define MICQ_OSCAR_ROSTER_H

#include "oscar_tlv.h"

#define ROSTER_TYPE_NORMAL     0
#define ROSTER_TYPE_GROUP      1
#define ROSTER_TYPE_VISIBLE    2
#define ROSTER_TYPE_INVISIBLE  3
#define ROSTER_TYPE_VISIBILITY 4
#define ROSTER_TYPE_PRESENCE   5
#define ROSTER_TYPE_ICQTIC     9
#define ROSTER_TYPE_IGNORE    14
#define ROSTER_TYPE_LASTUPD   15
#define ROSTER_TYPE_NONCONT   16
#define ROSTER_TYPE_WIERD17   17
#define ROSTER_TYPE_IMPORTT   19
#define ROSTER_TYPE_ICON      20

#define TLV_REQAUTH    102
#define TLV_GROUPITEMS 200
#define TLV_UNKNIDLE   201
#define TLV_PRIVACY    202
#define TLV_VISIBILITY 203
#define TLV_ALLOWIDLE  204
#define TLV_ICQTIC     205
#define TLV_IMPORT     212
#define TLV_ICON       213
#define TLV_NICK       305
#define TLV_LOCALMAIL  311
#define TLV_LOCALSMS   314
#define TLV_LOCALCOMM  316
#define TLV_LOCALACT   317
#define TLV_LOCALSOUND 318
#define TLV_LASTUPD    325

jump_snac_f SnacSrvReplylists, SnacSrvReplyroster, SnacSrvUpdateack,
    SnacSrvRosterok, SnacSrvAuthreq, SnacSrvAuthreply, SnacSrvAddedyou;

typedef struct RosterEntry_s RosterEntry;
typedef struct Roster_s Roster;

struct RosterEntry_s {
  RosterEntry *next;
  char  *name;
  char  *nick;
  TLV   *tlv;
  UWORD  tag;
  UWORD  id;
  UWORD  type;
};

struct Roster_s {
  RosterEntry *generic;
  RosterEntry *groups;
  RosterEntry *normal;
  RosterEntry *visible;
  RosterEntry *invisible;
  RosterEntry *ignore;
  time_t import;
  char *ICQTIC;
  char *delname;
  UDWORD delid;
  UDWORD deltag;
};

Roster *OscarRosterC (void);
void    OscarRosterD (Roster *roster);


UDWORD SnacCliCheckroster (Connection *serv);
void SnacCliRosteradd (Connection *serv, ContactGroup *cg, Contact *cont);
void SnacCliRosterbulkadd (Connection *serv, ContactGroup *cs);
void SnacCliRosterentryadd (Connection *serv, const char *name, UWORD tag, UWORD id, UWORD type, UWORD tlv, void *data, UWORD len);
void SnacCliRosterupdate (Connection *serv, ContactGroup *cg, Contact *cont);
void SnacCliSetvisibility (Connection *serv);
void SnacCliRosterdeletegroup (Connection *serv, ContactGroup *cg);
void SnacCliRosterdeletecontact (Connection *serv, Contact *cont);
void SnacCliRosterentrydelete (Connection *serv, RosterEntry *entry);
void SnacCliAddstart (Connection *serv);
void SnacCliAddend (Connection *serv);
void SnacCliGrantauth (Connection *serv, Contact *cont);
void SnacCliReqauth (Connection *serv, Contact *cont, const char *msg);
void SnacCliAuthorize (Connection *serv, Contact *cont, BOOL accept, const char *msg);

#define SnacCliReqlists(serv)     SnacSend (serv, SnacC (serv, 19, 2, 0, 0))
#define SnacCliReqroster(serv)    SnacSend (serv, SnacC (serv, 19, 4, 0, 0))
#define SnacCliRosterack(serv)    SnacSend (serv, SnacC (serv, 19, 7, 0, 0))

#endif
