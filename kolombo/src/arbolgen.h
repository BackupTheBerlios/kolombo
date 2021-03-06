/***************************************************************************
 *   Copyright (C) 2003 by Ren� Mart�n - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ARBOLGEN_H
#define ARBOLGEN_H

#include <arbolGenWdg.h>

class QPainter;
class KPrinter;
class QListViewItem;

/**
@author Ren?Mart? - AgarFu
*/
class arbolGen : public arbolGenWdg
{
Q_OBJECT
public:
    arbolGen(QWidget *parent);

    ~arbolGen();

public:
	void addPaloma (QListViewItem *root, QString palomaID);
	void printPedigree (QPainter *p, KPrinter * printer);
	int printPedigreeRecursivo (QPainter *p, int x, int y, QListViewItem *paloma);
	 
public slots:
	void loadData ();
	void showEvent( QShowEvent *e );
	void actualizarPalmares(QListViewItem *item);

private:
	QString palomaID, madreID, padreID, abueloMID, abuelaMID, abueloPID, abuelaPID;
};

#endif
