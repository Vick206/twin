/*
 *  libTw++.h  --  C++ wrapper for all libTw data types, functions and macros
 *
 *  Copyright (C) 2001 by Massimiliano Ghilardi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 */


/*
 * This include is a C++ front-end to raw libTw functionalities.
 * It does not add any new feature to libTw (with an exception: non-static
 * method listeners) but just wraps raw libTw types and function calls inside
 * C++ objects and methods.
 * 
 * libTw++.h currently has a limitation: libTw can handle multiple simultaneous
 * connections to multiple twin servers (it's multi-headed), while the objects
 * and methods declared in this file allow only a single connection to a single
 * twin server (single-headed). This should not be a problem for most
 * applications, but you'd better know it.
 * Even though techincally possible, extending libTw++.h to be multi-headed
 * would noticeably complicate it, as EVERY object would need to remember the
 * connection that was used to create it.
 */

#ifndef _LIB_TW_PLUS_PLUS_H
#define _LIB_TW_PLUS_PLUS_H

#include <stdio.h>
#include <string.h>

#ifndef _LIB_TW_H
# include <libTw.h>
#endif
#ifndef _LIB_TW_ERRNO_H
# include <libTwerrno.h>
#endif

class TEmpty;
class TObj;
class TWidget;
class TGadget;
class TWindow;
class TGroup;
class TEvent;
class TMsg;
class TMenu;
class TMsgPort;
class TListener;
class TListenerNonStatic;
class TDefaultListener;
class TDefaultListenerNonStatic;
class TW;

class TEmpty {
};

class TObj : public TEmpty {
  protected:
    inline TObj( )				{ }
    inline ~TObj( )				{ if (Id != NOID) TwDeleteObj(Id); }
  public:
    tobj Id;
    inline tobj id( )	const			{ return Id; }
    inline tobj Prev( ) const			{ return TwPrevObj(Id); }
    inline tobj Next( )	const			{ return TwNextObj(Id); }
    inline tobj Parent( ) const			{ return TwParentObj(Id); }
    static inline tobj Prev(tobj myId)		{ return TwPrevObj(myId); }
    static inline tobj Next(tobj myId)		{ return TwNextObj(myId); }
    static inline tobj Parent(tobj myId)	{ return TwParentObj(myId); }
};

class TMenu : public TObj {
  protected:
    inline TMenu( )				{ }
  public:
    inline ~TMenu( )				{ }
    
    inline TMenu(const TW *dpy, hwcol, hwcol, hwcol, hwcol, hwcol, hwcol, byte)
	; /* forward */
    
    inline tmenuitem commonItem( ) const
    {
	return TwItem4MenuCommon(Id);
    }
    inline void	setInfo(byte Flags = TW_ROW_ACTIVE, ldat Len = 0, const char *Text = 0, const hwcol *ColText = 0)  const
    {
	TwSetInfoMenu(Id, Flags, Len, (const byte *)Text, ColText);
    }
    inline void	setInfo(const char *Text, const hwcol *ColText = 0) const
    {
	setInfo(TW_ROW_ACTIVE, Text ? strlen(Text) : 0, Text, ColText);
    }
    static inline tmenuitem firstMenuItem(tmenu myId)
    {
	return TwFirstMenuItem(myId);
    }
    inline tmenuitem firstMenuItem( ) const
    {
	return TwFirstMenuItem(Id);
    }
};

class TWidget : public TObj {
  protected:
    inline TWidget( )				{ }
  public:
    inline ~TWidget( )				{ }
    
    inline TWidget(const TW *dpy, dat, dat, hwattr, dat, dat)
	; /* forward */
    
    static inline void map(twidget myId, twidget parentId )
    {
	TwMapWidget(myId, parentId);
    }
    inline void map( twidget parentId ) const
    {
	TwMapWidget(Id, parentId);
    }
    inline void map( TWidget *parent ) const
    {
	TwMapWidget(Id, parent->Id);
    }
    static inline void unMap( twidget myId )
    {
	TwUnMapWidget(myId);
    }
    inline void unMap( ) const
    {
	TwUnMapWidget(Id);
    }
    inline void setXY(dat X, dat Y) const
    {
	TwSetXYWidget(Id, X, Y);
    }
    static inline tmsgport getOwner( twidget myId )
    {
	return TwGetOwnerWidget(myId);
    }
    inline tmsgport getOwner( ) const
    {
	return TwGetOwnerWidget(Id);
    }
    inline twidget findWidgetAt(dat x, dat y) const
    {
	return TwFindWidgetAtWidget(Id, x, y);
    }
    static inline twidget firstWidget(twidget parentId)
    {
	return TwFirstWidget(parentId);
    }
    inline twidget firstWidget( ) const
    {
	return TwFirstWidget(Id);
    }
    inline void recursiveDelete( )
    {
	TwRecursiveDeleteWidget(Id);
	Id = NOID;
    }
};

class TGadget : public TWidget {
  protected:
    inline TGadget( )				{ }
  public:
    inline ~TGadget( )				{ }
    
    inline TGadget( TWidget *parent, dat XWidth, dat YWidth, const char *TextNormal,
		   udat Code = 0, udat Flags = TW_GADGET_USE_DEFCOL,
		   hwcol ColText = COL(BLACK,GREEN), hwcol ColTextSelect = COL(HIGH|WHITE,GREEN),
		   hwcol ColTextDisabled = COL(HIGH|BLACK,GREEN), hwcol ColTextSelectDisabled = COL(HIGH|BLACK,GREEN),
		   dat Left = 0, dat Up = 0, 
		   TW_CONST char *TextSelect = 0, TW_CONST char *TextDisabled = 0, TW_CONST char *TextSelectDisabled = 0,
		   TW_CONST hwcol *ColNormal = 0,
		   TW_CONST hwcol *ColSelect = 0, TW_CONST hwcol *ColDisabled = 0, TW_CONST hwcol *ColSelectDisabled = 0)
    {
	Id = TwCreateGadget( /* TwCreateGadget(...) is a macro... */ \
		parent->Id, XWidth, YWidth, (const byte *)TextNormal, Code, Flags,
		ColText, ColTextSelect, ColTextDisabled, ColTextSelectDisabled,
		Left, Up,
		(const byte *)TextSelect, (const byte *)TextDisabled, (const byte *)TextSelectDisabled,
		ColNormal, ColSelect, ColDisabled, ColSelectDisabled);
    }
    static inline TGadget *CreateButton( TWidget *parent, dat XWidth, dat YWidth, const char *name,
		   udat Code = 0, udat Flags = 0, hwcol BgCol = COL(BLACK,WHITE),
		   hwcol Col = COL(BLACK,GREEN), hwcol ColDisabled = COL(HIGH|BLACK,GREEN),
		   dat Left = 0, dat Up = 0)
    {
	TGadget *G = new TGadget( );
	G->Id = TwCreateButtonGadget(parent->Id, XWidth, YWidth, (const byte *)name,
				     Code, Flags, BgCol, Col, ColDisabled,
				     Left, Up);
	return G;
    }
    inline bool isPressed( )		const	{ return TwIsPressedGadget(Id); }
    inline bool isChecked( )		const	{ return TwIsPressedGadget(Id); }
    inline bool isOn( )			const	{ return TwIsPressedGadget(Id); }
    inline void setPressed(bool on)	const	{ TwSetPressedGadget(Id, (byte)on); }
    inline void setChecked(bool on)	const	{ TwSetPressedGadget(Id, (byte)on); }
    inline void setOn(bool on)		const	{ TwSetPressedGadget(Id, (byte)on); }
    inline bool isToggle( )		const	{ return TwIsToggleGadget(Id); }
    inline bool isToggleButton( )	const	{ return TwIsToggleGadget(Id); }
    inline void setToggle(bool on)	const	{ TwSetToggleGadget(Id, (byte)on); }
    inline void setToggleButton(bool on)const	{ TwSetToggleGadget(Id, (byte)on); }
    
    inline void setText(const char *text = 0, dat Left = 0, dat Up = 0) const
    {
	if (text && text[0])
	    TwSetTextGadget(Id, strlen(text), 1, (const byte *)text, Left, Up);
	else
	    TwSetTextGadget(Id, MAXDAT, MAXDAT, NULL, 0, 0);
    }
    inline void setTextLines(dat XWidth, dat YWidth, const char *text = 0, dat Left = 0, dat Up = 0) const
    {
	TwSetTextGadget(Id, XWidth, YWidth, (const byte *)text, Left, Up);
    }
    inline void setTextsLines(byte bitmap, dat XWidth, dat YWidth, const char *text = 0, dat Left = 0, dat Up = 0) const
    {
	TwSetTextsGadget(Id, bitmap, XWidth, YWidth, (const byte *)text, Left, Up);
    }
    inline void writeText(const char *text = 0, dat Left = 0, dat Up = 0) const
    {
	if (text && text[0])
	    TwWriteTextGadget(Id, strlen(text), 1, (const byte *)text, Left, Up);
    }
    inline void writeTextLines(dat XWidth, dat YWidth, const char *text = 0, dat Left = 0, dat Up = 0) const
    {
	TwWriteTextGadget(Id, XWidth, YWidth, (const byte *)text, Left, Up);
    }
    inline void writeTextsLines(byte bitmap, dat XWidth, dat YWidth, const char *text = 0, dat Left = 0, dat Up = 0) const
    {
	TwWriteTextsGadget(Id, bitmap, XWidth, YWidth, (const byte *)text, Left, Up);
    }
};

/* a handy alias for TGadget::CreateButton(...) */
class TButton : public TGadget {
  protected:
    inline TButton( )			{ }
  public:
    inline ~TButton( )			{ }
    
    inline TButton( TWidget *parent, dat XWidth, dat YWidth, const char *name,
		   udat Code = 0, udat Flags = 0, hwcol BgCol = COL(BLACK,WHITE),
		   hwcol Col = COL(BLACK,GREEN), hwcol ColDisabled = COL(HIGH|BLACK,GREEN),
		   dat Left = 0, dat Up = 0)
    {
	Id = TwCreateButtonGadget(parent->Id, XWidth, YWidth, (const byte *)name,
				  Code, Flags, BgCol, Col, ColDisabled,
				  Left, Up);
    }
};

class TWindow : public TWidget {
  protected:
    inline TWindow( )			{ } 
  public:
    inline ~TWindow( )			{ } 
    
    inline TWindow( TMenu *Menu, const char *name ="",
		   hwcol ColText = COL(BLACK,WHITE), uldat CursorType = TW_NOCURSOR,
		   uldat Attrib = TW_WINDOW_WANT_KEYS|TW_WINDOW_CLOSE|TW_WINDOW_DRAG|TW_WINDOW_RESIZE,
		   udat Flags = 0, dat XWidth = 0, dat YWidth = 0, dat ScrollBackLines = 0)
    {
	Id = TwCreateWindow(strlen(name), (const byte *)name, NULL, Menu->Id, ColText, CursorType,
			    Attrib, Flags, XWidth, YWidth, ScrollBackLines);
    }
    static inline TWindow *create4Menu(TMenu *Menu)
    {
	TWindow *W = new TWindow( );
	W->Id = TwCreate4MenuWindow(Menu->Id);
	return W;
    }
    inline void writeAscii(const char *text) const
    {
	TwWriteAsciiWindow(Id, strlen(text), (const byte *)text);
    }
    inline void writeAscii(ldat len, const char *text) const
    {
	TwWriteAsciiWindow(Id, len, (const byte *)text);
    }
    inline void writeHWAttr(ldat len, dat x, dat y, ldat Len, const hwattr *Attr) const
    {
	TwWriteHWAttrWindow(Id, x, y, Len, Attr);
    }
    inline void writeRow(const char *text) const
    {
	TwWriteRowWindow(Id, strlen(text), (const byte *)text);
    }
    inline void writeRow(ldat len, const char *text) const
    {
	TwWriteRowWindow(Id, len, (const byte *)text);
    }
    inline void gotoXY(ldat x, ldat y) const
    {
	TwGotoXYWindow(Id, x, y);
    }
    inline void setColText(hwcol C) const
    {
	TwSetColTextWindow(Id, C);
    }
    inline void setColors(udat Bitmap,
			  hwcol ColGadgets = COL(HIGH|YELLOW,CYAN), hwcol ColArrows = COL(HIGH|GREEN,HIGH|BLUE),
			  hwcol ColBars = COL(WHITE,HIGH|BLUE), hwcol ColTabs = COL(HIGH|WHITE,HIGH|BLUE),
			  hwcol ColBorder = COL(HIGH|WHITE,HIGH|BLUE), hwcol ColText = COL(BLACK,WHITE),
			  hwcol ColSelect = COL(WHITE,BLACK), hwcol ColDisabled = COL(HIGH|BLACK,BLACK),
			  hwcol ColSelectDisabled = COL(BLACK,HIGH|BLACK)) const
    {
	TwSetColorsWindow(Id, Bitmap, ColGadgets, ColArrows, ColBars, ColTabs,
			  ColBorder, ColText, ColSelect, ColDisabled, ColSelectDisabled);
    }
    inline void	configure(byte Bitmap, dat Left = 0, dat Up = 0, dat MinXWidth = 0,
			  dat MinYWidth = 0, dat MaxXWidth = MAXDAT, dat MaxYWidth = MAXDAT) const
    {
	TwConfigureWindow(Id, Bitmap, Left, Up, MinXWidth, MinYWidth, MaxXWidth, MaxYWidth);
    }
    inline void setSize(dat x, dat y) const
    {
	TwResizeWindow(Id, x, y);
    }
    inline void	create4MenuRow(udat Code = 0, byte FlagActive = TW_ROW_ACTIVE, const char *Text = "") const
    {
	TwRow4Menu(Id, Code, FlagActive, strlen(Text), (const byte *)Text);
    }
    inline void	row4Menu(udat Code = 0, byte FlagActive = TW_ROW_ACTIVE, const char *Text = "") const
    {
	create4MenuRow(Code, FlagActive, Text);
    }
    inline void	create4MenuRow(udat Code, byte FlagActive, ldat TextLen, const char *Text) const
    {
	TwRow4Menu(Id, Code, FlagActive, TextLen, (const byte *)Text);
    }
    inline void	row4Menu(udat Code, byte FlagActive, ldat TextLen, const char *Text) const
    {
	create4MenuRow(Code, FlagActive, TextLen, Text);
    }
};

class TGroup : public TObj {
  protected:
    inline TGroup( )			{ }
  public:
    inline ~TGroup( )			{ }
    
    inline TGroup( const TW *dpy )
	; /* forward */
    inline void insertGadget( TGadget *G ) const
    {
	TwInsertGadgetGroup(Id, G->Id);
    }
    inline void removeGadget( TGadget *G ) const
    {
	TwRemoveGadgetGroup(Id, G->Id);
    }
    inline tgadget getSelectedGadget( ) const
    {
	return TwGetSelectedGadgetGroup(Id);
    }
    inline void setSelectedGadget( TGadget *G ) const
    {
	TwSetSelectedGadgetGroup(Id, G->Id);
    }
};

class TColor : public TEmpty {
  public:
    hwcol C;
    inline TColor(hwcol c = 0)		{ C = c; }
    inline ~TColor( )			{ }
    inline hwcol col( ) const		{ return C; }
};

class TKeyEvent : public s_tevent_keyboard {
  protected:
    inline TKeyEvent( )			{ }
  public:
    inline ~TKeyEvent( )		{ }
    
    inline TKeyEvent(TWindow *W, udat code, udat shiftflags )
    {
	Window = W->Id;
	Code = code;
	ShiftFlags = shiftflags;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat key( )		const	{ return Code; }
    inline udat shiftflags( )	const	{ return ShiftFlags; }
    inline udat flags( )	const	{ return ShiftFlags; }
    inline udat seqlen( )	const	{ return SeqLen; }
    inline udat len( )		const	{ return SeqLen; }
    inline const char *asciiseq( ) const{ return (const char *)AsciiSeq; }
    inline const char *seq( )	const	{ return (const char *)AsciiSeq; }
};

class TMouseEvent : public s_tevent_mouse {
  protected:
    inline TMouseEvent( )		{ }
  public:
    inline ~TMouseEvent( )		{ }
    
    inline TMouseEvent(TWindow *W, udat code, udat shiftflags )
    {
	Window = W->Id;
	Code = code;
	ShiftFlags = shiftflags;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat mouse( )	const	{ return Code; }
    inline udat shiftflags( )	const	{ return ShiftFlags; }
    inline udat flags( )	const	{ return ShiftFlags; }
    inline  dat getX( )		const	{ return X; }
    inline  dat getY( )		const	{ return Y; }
};

class TControlEvent : public s_tevent_control {
  protected:
    inline TControlEvent( )		{ }
  public:
    inline ~TControlEvent( )		{ }
    
    inline TControlEvent(TWindow *W, udat code )
    {
	Window = W->Id;
	Code = code;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat control( )	const	{ return Code; }
    inline udat len( )		const	{ return Len; }
    inline  dat getX( )		const	{ return X; }
    inline  dat getY( )		const	{ return Y; }
    inline const char *data( )	const	{ return (const char *)Data; }
};

class TClientMsgEvent : public s_tevent_clientmsg {
  protected:
    inline TClientMsgEvent( )		{ }
  public:
    inline ~TClientMsgEvent( )		{ }
    
    inline TClientMsgEvent(TWindow *W, udat code )
    {
	Window = W->Id;
	Code = code;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat clientmsg( )	const	{ return Code; }
    inline udat len( )		const	{ return Len; }
    inline const char *data( )	const	{ return (const char *)Data; }
};

class TDisplayEvent : public s_tevent_display {
  protected:
    inline TDisplayEvent( )		{ }
  public:
    inline ~TDisplayEvent( )		{ }
    
    inline TDisplayEvent( udat code )
    {
	Window = NOID;
	Code = code;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat display( )	const	{ return Code; }
    inline udat len( )		const	{ return Len; }
    inline  dat getX( )		const	{ return X; }
    inline  dat getY( )		const	{ return Y; }
    inline const char *data( )	const	{ return (const char *)Data; }
};

class TWindowEvent : public s_tevent_window {
  protected:
    inline TWindowEvent( )		{ }
  public:
    inline ~TWindowEvent( )		{ }
    
    inline TWindowEvent(TWindow *W, udat code )
    {
	Window = W->Id;
	Code = code;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline  dat getXWidth( )	const	{ return XWidth; }
    inline  dat getYWidth( )	const	{ return YWidth; }
    inline  dat getX( )		const	{ return X; }
    inline  dat getY( )		const	{ return Y; }
};

class TGadgetEvent : public s_tevent_gadget {
  protected:
    inline TGadgetEvent( )		{ }
  public:
    inline ~TGadgetEvent( )		{ }
    
    inline TGadgetEvent(TWindow *W, udat code )
    {
	Window = W->Id;
	Code = code;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat gadget( )	const	{ return Code; }
    inline udat flags( )	const	{ return Flags; }
};
typedef TGadgetEvent TCloseEvent;

class TMenuEvent : public s_tevent_menu {
  protected:
    inline TMenuEvent( )		{ }
  public:
    inline ~TMenuEvent( )		{ }
    
    inline TMenuEvent(TWindow *W, TMenu *M, udat code )
    {
	Window = W->Id;
	Menu = M->Id;
	Code = code;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline tmenu menu( )	const	{ return Menu; }
};

class TSelectionEvent : public s_tevent_selection {
  protected:
    inline TSelectionEvent( )		{ }
  public:
    inline ~TSelectionEvent( )		{ }
    
    inline TSelectionEvent( TWindow * W)
    {
	Window = W->Id;
	Code = 0;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline udat selection( )	const	{ return Code; }
    inline  dat getX( )		const	{ return X; }
    inline  dat getY( )		const	{ return Y; }
};

class TSelectionNotifyEvent : public s_tevent_selectionnotify {
  public:
    inline ~TSelectionNotifyEvent( )	{ }
    
    inline TSelectionNotifyEvent( )
    {
	Window = NOID;
	Code = 0;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline uldat reqprivate( )	const	{ return ReqPrivate; }
    inline uldat magic( )	const	{ return Magic; }
    inline const char *mime( )	const	{ return (const char *)MIME; }
    inline uldat datalen( )	const	{ return Len; }
    inline uldat len( )		const	{ return Len; }
    inline const char *data( )	const	{ return (const char *)Data; }
};
typedef TSelectionNotifyEvent TClipboard;

class TSelectionRequestEvent : public s_tevent_selectionrequest {
  public:
    inline ~TSelectionRequestEvent( )	{ }
    
    inline TSelectionRequestEvent( )
    {
	Window = NOID;
	Code = 0;
    }
    inline twindow window( )	const	{ return Window; }
    inline udat code( )		const	{ return Code; }
    inline tmsgport requestor( )const	{ return Requestor; }
    inline uldat reqprivate( )	const	{ return ReqPrivate; }
};

class TEvent : public s_tevent_common {
    /* C++ cannot use a union (s_tevent_any) as base for a class :( */
  protected:
    inline TEvent( )				{ }
  public:
    inline ~TEvent( )				{ }
    
    inline TKeyEvent       *keyEvent       ( )	const	{ return (TKeyEvent *)      &((tevent_any)this)->EventKeyboard;  }
    inline TMouseEvent     *mouseEvent     ( )	const	{ return (TMouseEvent *)    &((tevent_any)this)->EventMouse;     }
    inline TControlEvent   *controlEvent   ( )	const	{ return (TControlEvent *)  &((tevent_any)this)->EventControl;   }
    inline TClientMsgEvent *clientMsgEvent ( )	const	{ return (TClientMsgEvent *)&((tevent_any)this)->EventClientMsg; }
    inline TDisplayEvent   *displayEvent   ( )	const	{ return (TDisplayEvent *)  &((tevent_any)this)->EventDisplay;   }
    inline TWindowEvent    *windowEvent    ( )	const	{ return (TWindowEvent *)   &((tevent_any)this)->EventWindow;    }
    inline TGadgetEvent    *gadgetEvent    ( )	const	{ return (TGadgetEvent *)   &((tevent_any)this)->EventGadget;    }
    inline TMenuEvent      *menuEvent      ( )	const	{ return (TMenuEvent *)     &((tevent_any)this)->EventMenu;      }
    inline TSelectionEvent *selectionEvent ( )	const	{ return (TSelectionEvent *)&((tevent_any)this)->EventSelection; }
    inline TSelectionNotifyEvent *selectionNotifyEvent ( ) const
    {
	return (TSelectionNotifyEvent *)&((tevent_any)this)->EventSelectionNotify;
    }
    inline TSelectionRequestEvent *selectionRequestEvent ( ) const
    {
	return (TSelectionRequestEvent *)&((tevent_any)this)->EventSelectionRequest;
    }
    inline TClipboard *clipboard ( ) const
    {
	return (TClipboard *)&((tevent_any)this)->EventSelectionNotify;
    }
};

class TMsg : public s_tmsg {
  protected:
    inline TMsg( )			{ }
    inline ~TMsg( )			{ }
  public:
    static TMsg *create(uldat Type, uldat Len, TEvent *E = 0)
    {
	TMsg *M = (TMsg *)TwCreateMsg(Type, Len);
	if (M && E)
	    TwCopyMem((void *)E, (void *)M->event(), Len);
	return M;
    }
    /*
     * you should call TMsg::free( ) ONLY on messages created
     * with `TMsg::create(...)' or with `TW::cloneReadMsg(bool)' !!!
     */
    inline void free( )
    {
	TwDeleteMsg((tmsg)this);
    }
    inline uldat len    ( )	const	{ return Len; }
    inline uldat magic  ( )	const	{ return Magic; }
    inline uldat type   ( )	const	{ return Type; }
    inline TEvent *event( )	const	{ return (TEvent *)&Event; }
};

class TMsgPort : public TObj {
  protected:
    inline TMsgPort ( )		{ }
  public:
    inline ~TMsgPort ( )	{ }
    
    inline TMsgPort( const char *name )
    {
	Id = TwCreateMsgPort(strlen(name), (const byte *)name, 0, 0, 0);
    }
    /*
     * TODO:
     * byte TwSendMsg(tmsgport MsgPort, tmsg Msg);
     * void TwBlindSendMsg(tmsgport MsgPort, tmsg Msg);
     */
    static inline tmsgport findMsgPort(const char *Name, tmsgport Prev, byte NameLen)
    {
	return TwFindMsgPort(Prev, NameLen, (const byte *)Name);
    }
    static inline tmsgport findMsgPort(const char *Name, tmsgport Prev = NOID)
    {
	return TwFindMsgPort(Prev, strlen(Name), (const byte *)Name);
    }
    static inline tmenu firstMenu(tmsgport MsgPortId)
    {
	return TwFirstMenu(MsgPortId);
    }
    inline tmenu firstMenu( ) const
    {
	return TwFirstMenu(Id);
    }
};

class TDialog : public TWindow {
  protected:
    inline TDialog( )			{ }
  public:
    inline ~TDialog( )			{ }
    
    static inline uldat message(const char *title, const char *text, const char *button1,
				const char *button2 = 0, const char *button3 = 0, const char *button4 = 0)
    { return 0; }
};

typedef void (*TFnListener)(TEvent *, void *);
typedef void (TEmpty::*TMtdListener)(TEvent *, void *);

typedef void (*TFnDefaultListener)(TMsg *, void *);
typedef void (TEmpty::*TMtdDefaultListener)(TMsg *, void *);


class TListener : public s_tlistener {
  friend TW;
  protected:
    inline TListener( )			{ }
  public:
    inline ~TListener( )
	; /* forward */
    
    inline TListener(TFnListener listener, void *arg = 0)
    {
	Listener = (tfn_listener)listener;
	Arg = arg;
	TwD = NULL;
    }
};

class TListenerNonStatic : public TListener {
  friend TW;
  protected:
    TEmpty *MtdThis;
    TMtdListener MtdListener;
    void *MtdArg;

    inline TListenerNonStatic( )	{ }
    
    /*
     * the only way I know to call a non-static C++ method from C
     * (in this case from TwMainLoop()) is to wrap it in a static method.
     * and the only way to let the static method know what it must call is
     * to pass it as argument (in this case `arg').
     */
    static void wrapper(TEvent *E, void *arg)
    {
	TListenerNonStatic *This = (TListenerNonStatic *)arg;
	TEmpty *MtdThis = This->MtdThis;
	TMtdListener MtdListener = This->MtdListener;
	void *MtdArg = This->MtdArg;
	
	(MtdThis->*MtdListener)(E, MtdArg);
    }
  public:
    inline ~TListenerNonStatic( )	{ }
    
    /*
     * here we must "cheat" :
     * as said above, we cannot call a non-static C++ method
     * (TMtdListener listener) from C, so we set `wrapper' above
     * as the listener to be called from C and let it call the real listener.
     * Of course we must set `this->Arg' to point to `this' so that `wrapper'
     * can extract the fields it needs.
     */
    inline TListenerNonStatic(TEmpty *This, TMtdListener listener, void *arg = 0)
	 : TListener ( wrapper, (void *)this )
    {
	MtdThis = This;
	MtdListener = listener;
	MtdArg = arg;
    }
};

/*
 * in order to allow adding listeners with slightly different prototypes,
 * we force a cast to the exact needed types. The macro below will fail
 * if you exagerate.
 * In particular, `This' should be a pointer to a class derived from `TEmpty'
 * (all libTw++ classes, except for T*Msg, T*Event and T*Listener* are
 * derived from `TEmpty')
 * and `L' must be a method pointer with prototype compatible to
 * `void (TEmpty::*)(void *, TEvent *)' -- possibly with `TEmpty::*'
 * replaced by a method pointer from the same class as `This'.
 */
#define TListenerSTATIC(L)		  TListener((TFnListener)L)
#define TListenerSTATIC2(L, arg)	  TListener((TFnListener)L, (void *)arg)
#define TListenerNONSTATIC(This, L)	  TListenerNonStatic((TEmpty *)This, (TMtdListener)L)
#define TListenerNONSTATIC2(This, L, arg) TListenerNonStatic((TEmpty *)This, (TMtdListener)L, (void *)arg)

/* shortcuts */
#define TLS(L)			new TListenerSTATIC(L)
#define TLS2(This, L, arg)	new TListenerSTATIC2(This, L, arg)
#define TL(This, L)		new TListenerNONSTATIC(This, L)
#define TL2(This, L, arg)	new TListenerNONSTATIC2(This, L, arg)


/*
 * same trick for default listeners:
 */
class TDefaultListener {
  protected:
    inline TDefaultListener( )		{ }
  public:
    TFnDefaultListener Listener;
    void *Arg;

    inline ~TDefaultListener( )		{ }
    
    inline TDefaultListener(TFnDefaultListener listener, void *arg = 0)
    {
	Listener = listener;
	Arg = arg;
    }
};

class TDefaultListenerNonStatic : public TDefaultListener {
  friend TW;
  protected:
    TMtdDefaultListener MtdListener;
    TEmpty *MtdThis;
    void *MtdArg;
    
    inline TDefaultListenerNonStatic( )	{ }
    
    static void wrapper(TMsg *M, void *arg)
    {
	TDefaultListenerNonStatic *This = (TDefaultListenerNonStatic *)arg;
	TEmpty *MtdThis = This->MtdThis;
	TMtdDefaultListener MtdListener = This->MtdListener;
	void *MtdArg = This->MtdArg;
	
	(MtdThis->*MtdListener)(M, MtdArg);
    }
  public:
    inline ~TDefaultListenerNonStatic( ){ }
    
    inline TDefaultListenerNonStatic(TEmpty *This, TMtdDefaultListener listener, void *arg = 0)
	: TDefaultListener( wrapper, (void *)this )
    {
	MtdThis = This;
	MtdListener = listener;
	MtdArg = arg;
    }
};

#define TDefaultListenerSTATIC(L) \
    TDefaultListener((TFnDefaultListener)L)
#define TDefaultListenerSTATIC2(L, arg) \
    TDefaultListener((TFnDefaultListener)L, (void *)arg)
#define TDefaultListenerNONSTATIC(This, L) \
    TDefaultListenerNonStatic((TEmpty *)This, (TMtdDefaultListener)L)
#define TDefaultListenerNONSTATIC2(This, L, arg) \
    TDefaultListenerNonStatic((TEmpty *)This, (TMtdDefaultListener)L, (void *)arg)

/* shortcuts */
#define TDLS(L)			new TDefaultListenerSTATIC(L)
#define TDLS2(L, arg)		new TDefaultListenerSTATIC2(L, arg)
#define TDL(This, L)		new TDefaultListenerNONSTATIC(This, L)
#define TDL2(This, L, arg)	new TDefaultListenerNONSTATIC2(This, L, arg)


class TW : public TEmpty {
  public:
    TMsgPort *MsgPort;
    bool Open;

    inline TW( )			{ }
    inline ~TW( )			{ close(); }
    
    static inline void configMalloc(void *(*my_malloc) (size_t),
				    void *(*my_realloc)(void *, size_t),
				    void  (*my_free)   (void *))
    {
	TwConfigMalloc(my_malloc, my_realloc, my_free);
    }
    static inline uldat FindFunction(const char *name) {
	return TwFindFunction(strlen(name), (const byte *)name);
    }
    static inline bool enableGzip( ) {
	return TwEnableGzip();
    }
    static inline bool disableGzip( ) {
	return TwDisableGzip();
    }
    /* the following methods are used by clients that want
     * to register themselves as displays of the server */
    static inline void needResizeDisplay( )
    {
	TwNeedResizeDisplay();
    }
    static inline void attachHW(const char *name, byte flags)
    {
	TwAttachHW(strlen(name), (const byte *)name, flags);
    }
    static inline const char *attachGetReply(uldat *len)
    {
	return (const char *)TwAttachGetReply(len);
    }
    static inline void attachConfirm( )
    {
	 TwAttachConfirm();
    }
    static inline bool detachHW(const char *name)
    {
	return TwDetachHW(strlen(name), (const byte *)name);
    }
        /* ok, back to normal methods */
    static inline void setFontTranslation(const char trans[0x80])
    {
	TwSetFontTranslation((const byte *)trans);
    }
    static inline bool open(const char *dpy = 0)
    {
	return TwOpen((const byte *)dpy);
    }
    static inline void close( )
    {
	TwClose();
    }
    inline TMsgPort *newMsgPort(const char *argv0)
    {
	return MsgPort = new TMsgPort(argv0);
    }
    inline TMsgPort *getMsgPort( ) const
    {
	return MsgPort;
    }
    static inline void beep( )
    {
	//TwBeep();
    }
    static inline tobj getOwnerSelection( )
    {
	return TwGetOwnerSelection();
    }
    static inline void exportSelection( time_t Time = TW_SEL_CURRENTTIME, frac_t Frac = TW_SEL_CURRENTTIME)
    {
	TwSetOwnerSelection(Time, Frac);
    }
    static inline void setOwnerSelection( time_t Time = TW_SEL_CURRENTTIME, frac_t Frac = TW_SEL_CURRENTTIME)
    {
	TwSetOwnerSelection(Time, Frac);
    }
    static inline void requestSelection(tobj Owner, uldat ReqPrivate = NOID)
    {
	TwRequestSelection(Owner, ReqPrivate);
    }
    static inline void requestSelection( )
    {
	TwRequestSelection(TwGetOwnerSelection(), NOID);
    }
    static inline void notifySelection(tobj Requestor, uldat ReqPrivate,
			uldat Magic, const char MIME[TW_MAX_MIMELEN],
			uldat Len, const char *Data)
    {
	TwNotifySelection(Requestor, ReqPrivate, Magic, (const byte *)MIME,
			  Len, (const byte *)Data);
    }
    static inline tscreen firstScreen()
    {
	return TwFirstScreen();
    }
    static inline dat getDisplayWidth( )
    {
	return TwGetDisplayWidth();
    }
    static inline dat getDisplayHeight( )
    {
	return TwGetDisplayHeight();
    }
    static inline int connectionFd( )
    {
	return TwConnectionFd();
    }
    static inline uldat libraryVersion( )
    {
	return TwLibraryVersion();
    }
    static inline uldat serverVersion( )
    {
	return TwServerVersion();
    }
    static inline bool inPanic( )
    {
	return TwInPanic();
    }
    static inline bool flush()
    {
	return TwFlush();
    }
    static inline byte timidFlush()
    {
	return TwTimidFlush();
    }
    static inline bool sync()
    {
	return TwSync();
    }
    static inline TMsg *pendingMsg()
    {
	return (TMsg *)TwPendingMsg();
    }
    static inline TMsg *peekMsg()
    {
	return (TMsg *)TwPeekMsg();
    }
    static inline TMsg *readMsg(bool Wait)
    {
	return (TMsg *)TwReadMsg(Wait);
    }
    static inline TMsg *cloneReadMsg(bool Wait)
    {
	return (TMsg *)TwCloneReadMsg(Wait);
    }
    static inline TListener *addListener(TKeyEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_WINDOW_KEY, L);
    }
    static inline TListener *addListener(TMouseEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_WINDOW_MOUSE, L);
    }
    static inline TListener *addListener(TControlEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_USER_CONTROL, L);
    }
    static inline TListener *addListener(TClientMsgEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_USER_CLIENTMSG, L);
    }
    static inline TListener *addListener(TDisplayEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_DISPLAY, L);
    }
    static inline TListener *addListener(TWindowEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_WINDOW_CHANGE, L);
    }
    static inline TListener *addListener(TGadgetEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_WINDOW_GADGET, L);
    }
    static inline TListener *addListener(TMenuEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_MENU_ROW, L);
    }
    static inline TListener *addListener(TSelectionEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_SELECTION, L);
    }
    static inline TListener *addListener(TSelectionNotifyEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_SELECTIONNOTIFY, L);
    }
    static inline TListener *addListener(TSelectionRequestEvent *E, TListener *L)
    {
	return addListener((TEvent *)E, TW_MSG_SELECTIONREQUEST, L);
    }
    /*
     * these are aliases to allow writing
     * TW::addListener(new T*Event(...), listener [,arg]);
     * instead of
     * TW::addListener(new T*Event(...), new TListener(listener [,arg]));
     * 
     * if you want to use a non-static method as listener, you must use:
     * TW::addListener(new T*Event(...), new TListenerNONSTATIC(listener, arg));
     */
    static inline TListener *addListener(TKeyEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TMouseEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TControlEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TClientMsgEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TDisplayEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TWindowEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TGadgetEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TMenuEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TSelectionEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TSelectionNotifyEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline TListener *addListener(TSelectionRequestEvent *E, TFnListener listener, void *arg = 0)
    {
	return addListener(E, new TListener(listener, arg));
    }
    static inline void removeListener(TListener *L)
    {
	TwRemoveListener((tlistener)L);
    }
    static inline void deleteListener(TListener *L)
    {
	L->~TListener();
    }
  private:
    static inline TListener *addListener(TEvent *E, uldat type, TListener *L)
    {
	L->Type = type;
	L->Event = (tevent_any)E;
	TwInsertListener(L);
	return L;
    }
  public:
    static inline TDefaultListener *setDefaultListener(TDefaultListener *L)
    {
	TwSetDefaultListener((tfn_default_listener)L->Listener, L->Arg);
	return L;
    }
    /*
     * this is an alias to allow writing
     * TW::setDefaultListener(listener [,arg]);
     * instead of
     * TW::setDefaultListener(new TDefaultListener(listener [,arg]));
     * 
     * if you want to use a non-static method as default listener, you must use:
     * TW::setDefaultListener(new TDefaultListenerNONSTATIC(listener, arg));
     */
    static inline void setDefaultListener(TFnDefaultListener listener, void *arg = 0)
    {
	TwSetDefaultListener((tfn_default_listener)listener, arg);
    }
    static inline uldat mainLoop()
    {
	return TwMainLoop();
    }
    static inline uldat exec()
    {
	return TwMainLoop();
    }
    static inline uldat error()
    {
	return TwErrno;
    }
    static inline uldat errorDetail()
    {
	return TwErrnoDetail;
    }
    static inline const char *strError(uldat e)
    {
	return (const char *)TwStrError(e);
    }
    static inline const char *strErrorDetail(uldat e, uldat d)
    {
	return (const char *)TwStrErrorDetail(e, d);
    }
    static inline void dumpError(const char *argv0)
    {
	uldat e = error();
	fprintf(stderr, "%s: libTw error: %s%s\n", argv0, strError(e), strErrorDetail(e, errorDetail()));
    }
};


inline TGroup::TGroup( const TW *dpy )
{
    Id = TwCreateGroup(dpy->getMsgPort()->Id);
}


inline TMenu::TMenu( const TW *dpy, hwcol ColItem = COL(BLACK,WHITE), hwcol ColSelect = COL(BLACK,GREEN),
		    hwcol ColDisabled = COL(HIGH|BLACK,WHITE), hwcol ColSelectDisabled = COL(HIGH|BLACK,BLACK),
		    hwcol ColShtCut = COL(RED,WHITE), hwcol ColSelShtCut = COL(RED,GREEN),
		    byte FlagDefColInfo = 0)
{
    Id = TwCreateMenu(dpy->getMsgPort()->Id, ColItem, ColSelect, ColDisabled, 
		      ColSelectDisabled, ColShtCut, ColSelShtCut, FlagDefColInfo);
}

inline TWidget::TWidget( const TW *dpy, dat XWidth = 0, dat YWidth = 0,
			hwattr Bg = HWATTR(COL(BLACK,WHITE),' '), dat Left = 0, dat Up = 0)
{
    Id = TwCreateWidget(XWidth, YWidth, Bg, Left, Up);
}

inline TListener::~TListener( )
{
    TW::removeListener(this);
    if (Event)
	((TEvent *)Event)->~TEvent();
}

#endif /* _LIB_TW_PLUS_PLUS_H */

