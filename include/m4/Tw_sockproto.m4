
ifdef(`c_comment', `', `define(`c_comment')')
ifdef(`c_doxygen', `', `define(`c_doxygen')')

c_comment([
/*
 *  m4/sockproto.m4h --  macroized prototypes for libTw functions.
 *                   used as template for a lot of autogenerated files.
 *
 *                   the prototypes are used both on client and server side
 *                   to implement function calls <-> socket data stream
 *                   conversion.
 *                   You can also see this as a custom version of
 *                   `remote procedure calling'.
 *
 *  Copyright (C) 1999-2001 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */
])

define(`rem')

rem([

 format of this file: each non-empty line is of the form
 
 `PROTO'(<rettype>,<retflag>, <action>,<object>,<self>, <arg1type>,<arg1flag>,<arg1name> ...)

 the number of arguments the function wants must be deduced
 from the number of formal args in each `PROTO'(...) definition.

 <flag> : v = void
	  _ = scalar (any integer)
	  x = pointer
	  V(len) = vector of <len> elements (<len> has 'uldat' type)
		<len> can be an expression, and may access the other arguments
		of the function as A(n) where <n> is the progressive number
		of the argument: A(1) is the first arg, A(2) the second, ...
	  W(len) = vector of <len> elements, with double-checking/autodetect on len
		so that you can safely pass NULL instead of the vector.
	  X(len) = same as V(len), but is a vector of pointers (x)
	  Y(len) = same as W(len), but is a vector of pointers (x)
	  O = variable return type. the function has a special (byte [2]) last argument
	      that contains the actual return type.


 Function calls wait until server has processed the command and returned the result
 if their return value is not void.
 
 <action> : the function name (Create, Map, Delete, ...)
 <object> : the object it acts upon (Gadget, Window, ...)
 
 <self> : 0 if the server implementation does not need the Fn##object method pointer
	: 1 if the function in the server needs the Fn##object method pointer
	: 2 if the Fn##object method pointer is extracted from the first argument
	

])

c_doxygen(/** return server-side id of given libTw function name with given prototype */)
PROTOFindFunction(uldat,_, Find,Function,0, byte,_,namelen, char,V(A(1)),name, byte,_,protolen, char,V(A(3)),proto)

c_doxygen(/** sync libTw socket: flush output buffer and wait for server replies */)
PROTOSyncSocket(byte,_,  Sync,Socket,0)

c_doxygen(/** return server idea of sizeof(type) */)
PROTO(byte,_, Server,Sizeof,0, byte,_,type)

c_doxygen(/** return 1 if server supports compression (using zlib) */)
PROTO(byte,_, Can,Compress,0)
c_doxygen(/** used internally by libTw to enable/disable compression only on server side;
 * DO NOT USE THIS`,' use Tw_EnableGzip() and Tw_DisableGzip() instead */)
PROTO(byte,_, Do,Compress,0, byte,_,on_off)

c_doxygen(/** force a server display resize; used by twdisplay */)
PROTO(void,v, NeedResize,Display,0)

c_doxygen(/** tell server to open a new display; used by twattach/twdisplay */)
PROTO(void,v, Attach,HW,0, uldat,_,len, char,V(A(1)),name, byte,_,flags)
c_doxygen(/** tell server to close given display */)
PROTO(byte,_, Detach,HW,0, uldat,_,len, char,V(A(1)),name)

c_doxygen(/** set server global charset translation */)
PROTO(void,v,  Set,  FontTranslation,0,  byte,V(0x80),trans)
c_doxygen(/** set server global charset translation */)
PROTO(void,v,  Set,HWFontTranslation,0,hwfont,V(0x80),trans)

c_doxygen(/** delete given object */)
PROTO(void,v,     Delete,Obj,0, obj,x,O)
c_doxygen(/** change given field of object; new_value = (old_value & ~clearmask) ^ xormask */)
PROTO(void,v,ChangeField,Obj,2, obj,x,O, udat,_,field, uldat,_,clearmask, uldat,_,xormask)


PROTO(widget,x,   Create,Widget,0, dat,_,w, dat,_,h, uldat,_,attrib, uldat,_,flags, dat,_,x, dat,_,y, hwattr,_,fill)
c_doxygen(/** delete given widget and all its children */)
PROTO(void,v,RecursiveDelete,Widget,0, widget,x,W)
PROTO(void,v,        Map,Widget,2, widget,x,W, widget,x,parent)
PROTO(void,v,      UnMap,Widget,2, widget,x,W)
PROTO(void,v,      SetXY,Widget,0, widget,x,W, dat,_,x, dat,_,y)
PROTO(void,v,     Resize,Widget,0, widget,x,W, dat,_,w, dat,_,h)
PROTO(void,v,     Scroll,Widget,0, widget,x,W, ldat,_,dxl, ldat,_,dyl)
PROTO(void,v,       Draw,Widget,0, widget,x,W, dat,_,w, dat,_,h, dat,_,x, dat,_,y, char,W(A(2)*A(3)),text, hwfont,W(A(2)*A(3)),textfont, hwattr,W(A(2)*A(3)),textattr)

PROTO(void,v, SetVisible,Widget,0, widget,x,W, byte,_,on_off)

PROTO(void,v,      FocusSub,Widget,0, widget,x,W)
PROTO(widget,x,FindWidgetAt,Widget,2, widget,x,W, dat,_,x, dat,_,y)

PROTO(void,v,            Raise,Widget,2, widget,x,W)
PROTO(void,v,            Lower,Widget,2, widget,x,W)
PROTO(void,v,  RestackChildren,Widget,0, widget,x,W, uldat,_,n, widget,X(A(2)),children)
PROTO(void,v,CirculateChildren,Widget,0, widget,x,W, byte,_,up_down)


PROTO(gadget,x, Create,Gadget,0,
	widget,x,parent, dat,_,w, dat,_,h, char,W(A(2)*A(3)),text,
	uldat,_,attrib, uldat,_,flags, udat,_,code,
	hwcol,_,coltext, hwcol,_,colselect, hwcol,_,coldisabled, hwcol,_,colselectdisabled,
	dat,_,x, dat,_,y)

PROTO(gadget,x, CreateButton,Gadget,1, widget,x,parent, dat,_,w, dat,_,h, char,V(A(2)*A(3)),text,
	uldat,_,flags, udat,_,code, hwcol,_,colbg, hwcol,_,col, hwcol,_,coldisabled, dat,_,x, dat,_,y)

PROTO(void,v, WriteTexts,Gadget,2, gadget,x,G, byte,_,mask, dat,_,w, dat,_,h, char,W(A(2)*A(3)),text, dat,_,x, dat,_,y)
PROTO(void,v, WriteHWFonts,Gadget,2, gadget,x,G, byte,_,mask, dat,_,w, dat,_,h, hwfont,W(A(2)*A(3)),textfont, dat,_,x, dat,_,y)

PROTO(window,x,     Create,Window,0, dat,_,titlelen, char,V(A(1)),title, hwcol,W(A(1)),coltitle, menu,x,M,
	hwcol,_,coltext, uldat,_,cursortype, uldat,_,attrib, uldat,_,flags, dat,_,w, dat,_,h, dat,_,hscroll)
PROTO(window,x, Create4Menu,Window,1, menu,x,M)

PROTO(void,v,  WriteAscii  ,Window,0, window,x,W, ldat,_,len, char,V(A(2)),ascii)
PROTO(void,v,  WriteString ,Window,0, window,x,W, ldat,_,len, char,V(A(2)),string)
PROTO(void,v,  WriteHWFont, Window,0, window,x,W, ldat,_,len, hwfont,V(A(2)),textfont)
PROTO(void,v,  WriteHWAttr ,Window,0, window,x,W,  dat,_,x, dat,_,y, ldat,_,len, hwattr,V(A(4)),textattr)

PROTO(void,v,        GotoXY,Window,2, window,x,W, ldat,_,x, ldat,_,y)
PROTO(void,v,      SetTitle,Window,0, window,x,W, dat,_,titlelen, char,V(A(2)),title)
PROTO(void,v,    SetColText,Window,2, window,x,W, hwcol,_,coltext)
PROTO(void,v,     SetColors,Window,2, window,x,W, udat,_,mask, hwcol,_,colgadgets, hwcol,_,colarrows,
	hwcol,_,colbars, hwcol,_,coltabs, hwcol,_,colborder, hwcol,_,coltext, hwcol,_,colselect,
	hwcol,_,coldisabled, hwcol,_,colselectdisabled)
PROTO(void,v,     Configure,Window,2, window,x,W, byte,_,mask, dat,_,x, dat,_,y, dat,_,minw, dat,_,minh, dat,_,maxw, dat,_,maxh)
PROTO(row,x,  FindRowByCode,Window,0, window,x,W, dat,_,code)

PROTO(group,x,     Create,Group,0)
PROTO(void,v, InsertGadget,Group,2, group,x,g, gadget,x,G)
PROTO(void,v, RemoveGadget,Group,2, group,x,g, gadget,x,G)

PROTO(gadget,x, GetSelectedGadget,Group,2, group,x,g)
PROTO(void,v,   SetSelectedGadget,Group,2, group,x,g, gadget,x,G)

PROTO(void,v,            Raise,Row,2, row,x,R)
PROTO(void,v,            Lower,Row,2, row,x,R)
PROTO(void,v,  RestackChildren,Row,0, obj,x,O, uldat,_,n, row,X(A(2)),children) 
PROTO(void,v,CirculateChildren,Row,0, obj,x,O, byte,_,up_down)

PROTO(row,x,         Create4Menu,Any,0,       obj,x,parent, window,x,W, udat,_,code, byte,_,flags, ldat,_,len, char,V(A(5)),text)

PROTO(uldat,_, Create4MenuCommon,MenuItem,1, menu,x,M)

PROTO(menu,x, Create,Menu,0, hwcol,_,colitem, hwcol,_,colselect, hwcol,_,coldisabled, hwcol,_,colselectdisabled,
      hwcol,_,colshortcut, hwcol,_,colshortcutselect, byte,_,flags)
PROTO(void,v, SetInfo,Menu,2, menu,x,M, byte,_,flags, ldat,_,len, char,V(A(3)),text, hwcol,W(A(3)),coltext)

PROTO(msgport,x,Create,MsgPort,0, byte,_,len, char,V(A(1)),name)
PROTO(msgport,x,  Find,MsgPort,0, msgport,x,prev, byte,_,len, char,V(A(2)),name)

PROTO(void,v,  BgImage,Screen,2, screen,x,S, dat,_,w, dat,_,h, hwattr,V(A(2)*A(3)),textattr)

PROTO(obj,x,      Prev,Obj,0, obj,x,O)
PROTO(obj,x,      Next,Obj,0, obj,x,O)
PROTO(obj,x,    Parent,Obj,0, obj,x,O)

PROTO(screen  ,x, First,Screen,  0)
PROTO(widget  ,x, First,Widget,  0, widget ,x,W)
PROTO(msgport ,x, First,MsgPort, 0)
PROTO(menu    ,x, First,Menu,    0, msgport,x,P)
PROTO(widget  ,x, First,W,       0, msgport,x,P)
PROTO(group   ,x, First,Group,   0, msgport,x,P)
PROTO(mutex   ,x, First,Mutex,   0, msgport,x,P)
PROTO(menuitem,x, First,MenuItem,0, menu   ,x,M)
PROTO(gadget  ,x, First,Gadget,  0, group  ,x,g)

PROTO(dat,_, GetDisplay,Width,0)
PROTO(dat,_, GetDisplay,Height,0)
PROTO(all,x, Get       ,All,0)

PROTO(byte,_, SendTo,MsgPort,0, msgport,x,P, udat,_,len, byte,V(A(2)),data)
PROTO(void,v, BlindSendTo,MsgPort,0, msgport,x,P, udat,_,len, byte,V(A(2)),data)

PROTO(obj, x, GetOwner,Selection,0)
PROTO(void,v, SetOwner,Selection,0, tany,_,secnow, tany,_,fracnow)
PROTO(void,v,  Request,Selection,0, obj,x,owner, uldat,_,reqprivate)
PROTO(void,v,   Notify,Selection,0, obj,x,requestor, uldat,_,reqprivate, uldat,_,magic,
      char,V(TW_MAX_MIMELEN),mime, uldat,_,len, char,V(A(5)),data)

PROTO(byte,_, SetServer,Uid,0, uldat,_,uid, byte,_,privileges)

PROTO(extension,x, Open,Extension,0, byte,_,namelen, char,V(A(1)),name)
PROTO(tany,O,     CallB,Extension,0, extension,x,id, topaque,_,len, byte,V(A(2)),data, byte,V(2),return_type)
PROTO(void,v,     Close,Extension,0, extension,x,id)

