/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NACION_H
#define NACION_H

#include <nacionWdg.h>

class KPopupMenu;

/**
@author Ren?Mart? - AgarFu
*/
class nacion : public nacionWdg
{
Q_OBJECT
public:
    nacion(QWidget *parent);

    ~nacion();
	 
private:
	KPopupMenu *popup;
	bool modificando;
	
public slots:
	void virtual contextNaciones (int row, int col, const QPoint & pos);
	void virtual insertNacion ();
	void eliminarNacion ();
	void modificarNacion ();

};

#endif
