/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TIPOSUELTA_H
#define TIPOSUELTA_H

#include <tiposueltaWdg.h>

class KPopupMenu;

/**
@author Ren?Mart? - AgarFu
*/
class tipoSuelta : public tipoSueltaWdg
{
Q_OBJECT
public:
    tipoSuelta(QWidget *parent);

    ~tipoSuelta();

private:
	KPopupMenu *popup;
	bool modificando;
	
public slots:
	void virtual contextTablaSueltas (int row, int col, const QPoint & pos);
	void virtual insertTipo ();
	void eliminarTipo ();
	void modificarTipo ();

};

#endif
