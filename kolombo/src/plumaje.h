/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PLUMAJE_H
#define PLUMAJE_H

#include <plumajeWdg.h>

class KPopupMenu;

/**
@author Ren?Mart? - AgarFu
*/
class plumaje : public plumajeWdg
{
Q_OBJECT
public:
    plumaje(QWidget *parent);

    ~plumaje();

private:
	KPopupMenu *popup;
	bool modificando;
	
public slots:
	void virtual contextPlumaje (int row, int col, const QPoint & pos);
	void virtual insertColor ();
	void eliminarColor ();
	void modificarColor ();
	 
};

#endif
