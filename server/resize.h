#ifndef _TW_RESIZE_H
#define _TW_RESIZE_H

byte EnsureLenRow(row Row, ldat Len, byte DefaultCol);
byte WriteRow(window Window, ldat Len, byte CONST * Text);

void ExposeAscii(window Window, dat XWidth, dat YWidth, CONST byte *Text, dat Left, dat Up);
void ExposeHWAttr(window Window, dat XWidth, dat YWidth, CONST hwattr *Attr, dat Left, dat Up);

void UpdateCursor(void);

byte CheckResizeWindowContents(window Window);
byte ResizeWindowContents(window Window);

/*
void SetNewFont(void);
void GetPalette(void);
void SetPalette(void);
 */

void ResizeFirstScreen(dat DeltaY);
void DragFirstScreen(ldat DeltaX, ldat DeltaY);
void HideMenu(byte on_off);
void ChangeMenuFirstScreen(menuitem NewMenuItem, byte ByMouse, byte Flag);
#define ACTIVATE_MENU_FLAG	(byte)1
#define DISABLE_MENU_FLAG	(byte)2
#define KEEP_ACTIVE_MENU_FLAG	(byte)3


void DragFirstWindow(dat i, dat j);
void DragWindow(window Window, dat i, dat j);
void ResizeRelFirstWindow(dat i, dat j);
void ResizeRelWindow(window Window, dat i, dat j);
void CenterWindow(window Window);
byte ExecScrollFocusWindow(void);
void ScrollFirstWindow(dat DeltaX, dat DeltaY, byte byXYLogic);
void ScrollWindow(window Window, dat DeltaX, dat DeltaY);
void ScrollFirstWindowArea(dat X1, dat Y1, dat X2, dat Y2, dat DeltaX, dat DeltaY);

void UnFocusWidget(widget W);
void RollUpWindow(window W, byte on_off);

void MakeFirstWindow(window Window, byte alsoFocus);
void MakeLastWindow(window Window, byte alsoDeFocus);

void PressGadget(gadget G);
void UnPressGadget(gadget G, byte maySendMsgIfNotToggle);
void SendMsgGadget(gadget G);
void WriteTextsGadget(gadget G, byte bitmap, dat XWidth, dat YWidth, CONST byte *Text, dat Left, dat Up);

#endif /* _TW_RESIZE_H */

