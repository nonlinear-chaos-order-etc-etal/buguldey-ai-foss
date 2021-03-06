/* $Id: conv.h,v 1.40 2005/04/21 18:14:31 kuhlmann Exp $ */

#ifndef MICQ_UTIL_CONV
#define MICQ_UTIL_CONV

extern UBYTE conv_error;

void        ConvInit       (void);

UBYTE       ConvEnc        (const char *enc);
const char *ConvEncName    (UBYTE enc);
const char *ConvUTF8       (UDWORD codepoint);
UDWORD      ConvGetUTF8    (strc_t in, int *off);
const char *ConvCrush0xFE  (const char *in);
#define     Conv0xFE       (char)0xfe
BOOL        ConvFits       (const char *in, UBYTE enc);

#define     ConvIsUTF8(t)       ConvFits (t, ENC_UTF8)
#define     c_out_for(t,c,mt)   (CONT_UTF8 (c,mt) ? t : c_out_to_split (t,c))
#define     c_out(t)            c_out_to(t,NULL)
#define     c_out_to(t,c)       (ConvTo        (t, ContactPrefVal ((c), CO_ENCODING))->txt)
#define     c_out_to_split(t,c) (ConvToSplit   (t, ContactPrefVal ((c), CO_ENCODING))->txt)
#define     ConvFromServ(t)     (ConvIsUTF8 ((t)->txt) ? ConvFrom (t, ENC_UTF8)->txt : ConvFromCont (t, NULL))
#define     c_in_to_split(t,c)  (ConvFromSplit (t, ContactPrefVal ((c), CO_ENCODING))->txt)
#define     ConvFromCont(t,c)   (ConvFrom      (t, ContactPrefVal ((c), CO_ENCODING))->txt)
#define     c_strlen(t)         (ENC(enc_loc) == ENC_UTF8 ? s_strlen (t) : strlen (t))
#define     c_offset(t,o)       (ENC(enc_loc) == ENC_UTF8 ? s_offset (t, o) : (o))
#define     c_delta(t)          (int)(ENC(enc_loc) == ENC_UTF8 ? strlen (t) - s_strlen (t) : 0)
#define     s_delta(t)          (int)strlen (t) - (int)s_strlen (t)

const char *ConvTranslit (const char *orig, const char *trans);

strc_t      ConvFrom       (strc_t in, UBYTE enc);
strc_t      ConvFromSplit  (strc_t in, UBYTE enc);
strc_t      ConvTo         (const char *in, UBYTE enc);
strc_t      ConvToSplit    (const char *in, UBYTE enc);
strc_t      ConvToLen      (const char *in, UBYTE enc, size_t len);

#define CHAR_NOT_AVAILABLE '?'
#define CHAR_INCOMPLETE    '_'
#define CHAR_BROKEN        '*'

#define ENC_FLAGS   0xc0
#define ENC_FLC     0x40
#define ENC_FGUESS  0x80
#define ENC_FERR    0x80
#define ENC_AUTO    0xff

#define ENC_ASCII   0x00
#define ENC_UTF8    0x01
#define ENC_LATIN1  0x02
#define ENC_LATIN9  0x03
#define ENC_KOI8    0x04
#define ENC_WIN1251 0x05  /* Windows code page 1251 */
#define ENC_UCS2BE  0x06
#define ENC_WIN1257 0x07
#define ENC_EUC     0x08
#define ENC_SJIS    0x09  /* Windows Shift-JIS */
#define ENC_WCHART  0x0a

#define ENC(enc_x) (prG->enc_x & ~ENC_FLAGS)

#endif /* MICQ_UTIL_CONV */
