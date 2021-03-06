#include "stdafx.h"
/* $Id: server.c,v 1.81 2005/03/31 17:28:15 kuhlmann Exp $ */
/* Copyright: This file may be distributed under version 2 of the GPL licence. */

#include "micq.h"
#include <fcntl.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include "util_ui.h"
#include "datatype.h"
#include "mreadline.h"
#include "server.h"
#include "util.h"
#include "icq_response.h"
#include "oldicq_compat.h"
#include "oldicq_client.h"
#include "oscar_snac.h"
#include "oscar_icbm.h"
#include "oscar_oldicq.h"
#include "oscar_bos.h"
#include "cmd_user.h"
#include "contact.h"
#include "connection.h"
#include "preferences.h"
#include "tcp.h"
#include "packet.h"
#include "conv.h"

static void CallbackMeta (Event *event);
static void CallbackTogvis (Event *event);

static void CallbackTogvis (Event *event)
{
    if (!event)
        return;
    if (!event->cont || !event->conn || ContactPrefVal (event->cont, CO_INTIMATE) || !(event->conn->status & STATUSF_INV))
    {
        EventD (event);
        return;
    }
    SnacCliRemvisible (event->conn, event->cont);
    EventD (event);
}

UBYTE IMCliMsg (Connection *conn, Contact *cont, Opt *opt)
{
    const char *opt_text;
    UDWORD opt_type, opt_trans;
    
    if (!cont || !conn || !OptGetStr (opt, CO_MSGTEXT, &opt_text))
    {
        OptD (opt);
        return RET_FAIL;
    }
    if (!OptGetVal (opt, CO_MSGTYPE, &opt_type))
        OptSetVal (opt, CO_MSGTYPE, opt_type = MSG_NORM);
    if (!OptGetVal (opt, CO_MSGTRANS, &opt_trans))
        OptSetVal (opt, CO_MSGTRANS, opt_trans = CV_MSGTRANS_ANY);
    
    putlog (conn, NOW, cont, STATUS_ONLINE, 
            opt_type == MSG_AUTO ? LOG_AUTO : LOG_SENT, opt_type, opt_text);

    return IMCliReMsg (conn, cont, opt);
}

UBYTE IMCliReMsg (Connection *conn, Contact *cont, Opt *opt)
{
    const char *opt_text;
    Event *event;
    UDWORD opt_type, opt_trans, reveal_time;
    UBYTE ret;
    
    if (!cont || !conn || !OptGetStr (opt, CO_MSGTEXT, &opt_text))
    {
        OptD (opt);
        return RET_FAIL;
    }
    if (!OptGetVal (opt, CO_MSGTYPE, &opt_type))
        OptSetVal (opt, CO_MSGTYPE, opt_type = MSG_NORM);
    if (!OptGetVal (opt, CO_MSGTRANS, &opt_trans))
        OptSetVal (opt, CO_MSGTRANS, opt_trans = CV_MSGTRANS_ANY);
    
    if ((conn->status & STATUSF_INV)  && !ContactPrefVal (cont, CO_INTIMATE)
        && !(opt_type & MSGF_GETAUTO) && !ContactPrefVal (cont, CO_HIDEFROM)
        && (reveal_time = ContactPrefVal (cont, CO_REVEALTIME)))
    {
        event = QueueDequeue2 (conn, QUEUE_TOGVIS, 0, cont);
        if (event)
            EventD (event);
        else
            SnacCliAddvisible (conn, cont);
        QueueEnqueueData (conn, QUEUE_TOGVIS, 0, time (NULL) + reveal_time, NULL, cont, NULL, CallbackTogvis);
    }

#ifdef ENABLE_PEER2PEER
    if (opt_trans & CV_MSGTRANS_DC)
        if (conn->assoc)
            if (RET_IS_OK (ret = PeerSendMsg (conn->assoc, cont, opt)))
                return ret;
    OptSetVal (opt, CO_MSGTRANS, opt_trans &= ~CV_MSGTRANS_DC);
#endif
    if (opt_trans & CV_MSGTRANS_TYPE2)
        if (conn->connect & CONNECT_OK && conn->type == TYPE_SERVER)
            if (RET_IS_OK (ret = SnacCliSendmsg2 (conn, cont, opt)))
                return ret;
    OptSetVal (opt, CO_MSGTRANS, opt_trans &= ~CV_MSGTRANS_TYPE2);
    if (opt_trans & CV_MSGTRANS_ICQv8)
        if (conn->connect & CONNECT_OK && conn->type == TYPE_SERVER)
            if (RET_IS_OK (ret = SnacCliSendmsg (conn, cont, opt_text, opt_type, 0)))
                return ret;
    OptSetVal (opt, CO_MSGTRANS, opt_trans &= ~CV_MSGTRANS_ICQv8);
    if (opt_trans & CV_MSGTRANS_ICQv5)
        if (conn->connect & CONNECT_OK && conn->type == TYPE_SERVER_OLD)
        {
            CmdPktCmdSendMessage (conn, cont, opt_text, opt_type);
            OptD (opt);
            return RET_OK;
        }
    OptD (opt);
    return RET_FAIL;
}

void IMCliInfo (Connection *conn, Contact *cont, int group)
{
    UDWORD ref;
    if (cont)
    {
        cont->updated = 0;
        if (conn->type == TYPE_SERVER)
            ref = SnacCliMetareqinfo (conn, cont);
        else
            ref = CmdPktCmdMetaReqInfo (conn, cont);
    }
    else
    {
        if (conn->type == TYPE_SERVER)
            ref = SnacCliSearchrandom (conn, group);
        else
            ref = CmdPktCmdRandSearch (conn, group);
    }
    QueueEnqueueData (conn, QUEUE_REQUEST_META, ref, time (NULL) + 60, NULL,
                      cont, NULL, &CallbackMeta);
}

static void CallbackMeta (Event *event)
{
    Contact *cont;
    
    assert (event && event->conn && event->conn->type & TYPEF_ANY_SERVER);

    cont = event->cont;
    if ((cont->updated != UP_INFO) && !(event->flags & QUEUE_FLAG_CONSIDERED))
        QueueEnqueue (event);
    else
    {
        UtilUIDisplayMeta (cont);
        if (~cont->oldflags & CONT_ISEDITED)
            ContactMetaSave (cont);
        else
            cont->updated &= ~UPF_DISC;
        EventD (event);
    }
}
