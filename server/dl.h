#ifndef _TW_DL_H
#define _TW_DL_H

byte DlOpen(module Module);
void DlClose(module Module);

#define MainSo		((uldat)0)
#define WMSo		((uldat)1)
#define TermSo		((uldat)2)
#define SocketSo	((uldat)3)
#define SocketAlienSo	((uldat)4)
#define RCParseSo	((uldat)5)
#define MAX_So		((uldat)6)

module DlLoad(uldat code);
void   DlUnLoad(uldat code);
module DlIsLoaded(uldat code);
udat   DlName2Code(byte *name);

module DlLoadAny(uldat len, byte *name);

#endif /* _TW_DL_H */
