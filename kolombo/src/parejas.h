/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PAREJAS_H
#define PAREJAS_H

#include <parejasWdg.h>

#ifndef INDICE_PALOMA
const int indicePalomaID = 0;
#define INDICE_PALOMA
#endif
const int indiceMadre = 2;
const int indicePadre = 3;

class KPopupMenu;

/**
@author Ren?Mart? - AgarFu
*/
class parejas : public parejasWdg
{
Q_OBJECT
public:
    parejas(QWidget *parent);

    QWidget *parent;

    ~parejas();

public slots:
	void parejaChange();
	void nuevaPareja();
	void nuevoHijo();
	void updateSeleccion ();
	void showEvent( QShowEvent *e );
	void cambioSexo();
	void estadoDesaparecida ();
	void estadoVuelo ();
	void estadoReproduccion ();
	void eliminarPaloma ();
	void contentsContextMenuEvent( QContextMenuEvent *e );
	void contexTablaHijos (int row, int col, const QPoint & pos);
	void contexTablaParejas (int row, int col, const QPoint & pos);
	virtual void eliminarPareja ();
	virtual void editarPareja();
	void estadisticas ();

private:
	KPopupMenu *popup;
	KPopupMenu *estadoPopup;
	KPopupMenu *popupParejas;
	void setupActions ();

};



#endif
