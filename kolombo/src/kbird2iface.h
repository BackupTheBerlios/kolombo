/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#ifndef _KBIRD2IFACE_H_
#define _KBIRD2IFACE_H_

#include <dcopobject.h>

class kbird2Iface : virtual public DCOPObject
{
  K_DCOP
public:

k_dcop:
  virtual void openURL(QString url) = 0;
};

#endif // _KBIRD2IFACE_H_
