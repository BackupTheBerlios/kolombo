/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef OJO_H
#define OJO_H

#include <ojoWdg.h>

class KPopupMenu;

/**
@author Ren?Mart? - AgarFu
*/
class ojo : public ojoWdg
{
Q_OBJECT
public:
    ojo(QWidget *parent);

    ~ojo();
	 
private:
	KPopupMenu *popup;
	bool modificando;
	
public slots:
	void virtual contextOjo (int row, int col, const QPoint & pos);
	void virtual insertarOjo ();
	void eliminarTipo ();
	void modificarTipo ();

};

#endif
