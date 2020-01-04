/*
 *  twin.h  --  main include for all twin-related data types, functions and macros
 *
 *  Copyright (C) 1993-2001 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#ifndef _TWIN_H
#define _TWIN_H

#include "twautoconf.h" /* for TW_HAVE_* macros */
#include "twconfig.h"   /* for CONF_* macros */

#include "obj/fwd.h"
/* pull in "obj" field in <Tw/stattypes.h> */
#define obj obj

#include "compiler.h"
#include "version.h"
#include "osincludes.h"

#include <Tw/Twtypes.h>
#include <Tw/Tw_defs.h>
#include <Tw/missing.h>
#include <Tw/mouse.h>

/***************/

#include "tty.h" // USER_MAP

#include "obj/magic.h"
#include "obj/obj.h"
#include "obj/widget.h"
#include "obj/gadget.h"
#include "obj/window.h"
#include "obj/screen.h"
#include "obj/group.h"
#include "obj/row.h"
#include "obj/menuitem.h"
#include "obj/menu.h"
#include "obj/event.h"
#include "obj/msg.h"
#include "obj/msgport.h"
#include "obj/mutex.h"
#include "obj/module.h"
#include "obj/extension.h"
#include "obj/display_hw.h"
#include "obj/all.h"
#include "obj/fn.h"

#define NOFD (-1)
/* use for every FD that needs a special RemoteFlush()
 * instead of a plain write() and set PrivateFlush as needed */
#define specFD (-2)

#ifndef NOPID /* some OSes (Solaris for one) already define NOPID */
#define NOPID ((pid_t)0)
#endif
#define NOSLOT TW_MAXULDAT

typedef struct s_font {
  byte AsciiCode;
  char *Bitmap;
} font;

typedef struct s_rgb {
  byte Red, Green, Blue;
} rgb;

/* errors */
#define NOMEMORY ((udat)1)
#define NOTABLES ((udat)2)
#define DLERROR ((udat)3)
#define SYSCALLERROR ((udat)4)
#define USERERROR ((udat)5)

#define IS_OBJ(type, O) (((O)->Id >> magic_shift) == type##_magic_id)
#define IS_WIDGET(O)                                                                               \
  (IS_OBJ(widget, O) || IS_OBJ(gadget, O) || IS_OBJ(window, O) || IS_OBJ(screen, O))
#define IS_GADGET(O) IS_OBJ(gadget, O)
#define IS_WINDOW(O) IS_OBJ(window, O)
#define IS_SCREEN(O) IS_OBJ(screen, O)
#define IS_ROW(O) (IS_OBJ(row, O) || IS_OBJ(menuitem, O))
#define IS_MENUITEM(O) IS_OBJ(menuitem, O)
#define IS_MENU(O) IS_OBJ(menu, O)
#define IS_MSGPORT(O) IS_OBJ(msgport, O)
#define IS_MUTEX(O) IS_OBJ(mutex, O)
#define IS_MSG(O) IS_OBJ(msg, O)
#define IS_MODULE(O) (IS_OBJ(module, O) || IS_OBJ(extension, O))
#define IS_EXTENSION(O) IS_OBJ(extension, O)
#define IS_DISPLAY_HW(O) IS_OBJ(display_hw, O)
#define IS_ALL(O) IS_OBJ(all, O)

/************** Keys **************/

#define KBD_SHIFT_FL (byte)0x1
#define KBD_CTRL_FL (byte)0x2
#define KBD_ALT_FL (byte)0x4
#define KBD_CAPS_LOCK (byte)0x8
#define KBD_NUM_LOCK (byte)0x10

#define ENTER ((udat)'\r')
#define ESCAPE ((udat)'\033')

/**********************************/

/* memory allocation. these function call Error(NOMEMORY) on failure */

void *AllocMem(size_t Size);              /* wrapper for malloc() */
void *ReAllocMem(void *Mem, size_t Size); /* wrapper for realloc() */
#define FreeMem free

void *AllocMem0(size_t ElementSize, size_t Count); /* wrapper for calloc() */
void *ReAllocMem0(void *Mem, size_t ElementSize, size_t OldCount,
                  size_t NewCount); /* wrapper for realloc() + memset() */

/* INLINE/define stuff: */

#define CopyMem(From, To, Size) memcpy(To, From, Size)
#define MoveMem(From, To, Size) memmove(To, From, Size)

typedef enum { none, sgidtty, suidroot } e_privilege;

#define DropPrivileges() (setegid(getgid()), seteuid(getuid()))
#define GainRootPrivileges() seteuid(0)
#define GainGroupPrivileges(g) setegid(g)

char *CloneStr(CONST char *s);

#endif /* _TWIN_H */
