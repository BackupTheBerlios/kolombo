/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DATOSPALOMA_H
#define DATOSPALOMA_H

#include <datosPalomaWdg.h>

/**
@author Ren?Mart? - AgarFu
*/
class datosPaloma : public datosPalomaWdg
{
Q_OBJECT
public:
    datosPaloma(QWidget *parent, bool edit);
	void showEvent( QShowEvent *e );
	QWidget *parent;

    ~datosPaloma();
public slots:
	void loadData();
	void clearData();
	void insertPaloma();
	virtual void crearPareja();
	void updatePaloma();
	void next();
	void previous ();

private:
	bool edit;
};

#endif
