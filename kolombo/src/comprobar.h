/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef COMPROBAR_H
#define COMPROBAR_H

#include "comprobarDlg.h"

#include <qstring.h>
#include <qdatetime.h>

class comprobar : public comprobarDlg
{
  Q_OBJECT

public:
  comprobar(QString &concursoID, QString &palomaID, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
  ~comprobar();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          accept();

private:
	QDateTime Tinicio;
	QString palomaID;
	QString concursoID;
};

#endif

