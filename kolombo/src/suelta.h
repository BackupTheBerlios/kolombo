/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SUELTA_H
#define SUELTA_H

#include "sueltasWdg.h"

class KPopupMenu;

class suelta : public sueltasWdg
{
  Q_OBJECT

public:
  suelta(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~suelta();
  /*$PUBLIC_FUNCTIONS$*/
  void showEvent( QShowEvent *e );

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          contextSueltas(int row, int col, const QPoint & pos);
  virtual void          insertSuelta();
	void eliminarSuelta ();
	void modificarSuelta ();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
private:
	KPopupMenu *popup;
	bool modificando;
	void loadCombo();
};

#endif

