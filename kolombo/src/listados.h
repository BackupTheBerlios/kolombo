/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef LISTADOS_H
#define LISTADOS_H

#include <qevent.h>

#include <listadosWdg.h>
#include <kxmlguiclient.h>
#include <kmainwindow.h>

#ifndef INDICE_PALOMA
const int indicePalomaID = 0;
#define INDICE_PALOMA
#endif

class KPopupMenu;
class QPainter;
class KPrinter;

/**
@author Ren?Mart? - AgarFu
*/
class listados : public listadosWdg
{
Q_OBJECT
public:
    listados(QWidget *parent,  KMainWindow *mainWindow);
	void showEvent( QShowEvent *e );
    ~listados();

    void print (QPainter *p, KPrinter *printer);
    int printBase (QPainter *p, KPrinter *printer, int pageNo);

private:
	KMainWindow* mainWindow;
	QWidget *parent;
	KPopupMenu *popup;
	KPopupMenu *verPopup;
	KPopupMenu *estadoPopup;
	void setupActions ();

public slots:
	void contextMenu (int row, int col, const QPoint & pos);
	void updateSeleccion ();
	void contentsContextMenuEvent( QContextMenuEvent * );
	void generalStatsSlot();
};

#endif
