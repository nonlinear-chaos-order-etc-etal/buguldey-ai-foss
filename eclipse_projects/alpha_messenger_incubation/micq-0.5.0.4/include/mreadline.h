/* $Id: mreadline.h,v 1.19 2005/04/18 18:16:54 kuhlmann Exp $ */

#ifndef MICQ_MREADLINE_H
#define MICQ_MREADLINE_H

void   rl_print  (const char *str);
void   rl_printf (const char *str, ...) __attribute__ ((format (__printf__, 1, 2)));
int    rl_pos (void);
void   rl_logo (const char *logo);
void   rl_logo_clear (void);

#endif /* MICQ_MREADLINE_H */
