/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "plumaje.h"

#include <qdatatable.h>
#include <qcursor.h>
#include <qsqlquery.h>
#include <qsqldatabase.h>
#include <qlineedit.h>
#include <qtextedit.h>

#include <kpushbutton.h>
#include <kpopupmenu.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kaction.h>

plumaje::plumaje(QWidget *parent)
: plumajeWdg (parent, "plumaWdg", 0)
{
	modificando = FALSE;
	popup = new KPopupMenu (tablaPlumaje, "popup");
	popup->setTitle (i18n("Menu tabla plumaje"));
	KAction *eliminarColorAction = new KAction(i18n("Eliminar color"), "button_cancel", 
					CTRL+Key_D, this, SLOT(eliminarColor()), this, "eliminarColorAction");
	eliminarColorAction->plug (popup);
	KAction *modificarColorAction = new KAction(i18n("Modificar color"), "reload", 
					CTRL+Key_M, this, SLOT(modificarColor()), this, "modificarColorAction");
	modificarColorAction->plug (popup);
}


plumaje::~plumaje()
{
}


void plumaje::contextPlumaje (int row, int col, const QPoint & pos) 
{
	popup->exec( QCursor::pos() );
}

void plumaje::insertColor ()
{
	if (!nombrePluma->text().isEmpty() && !descripcionPluma->text().isEmpty())
	{
		if (modificando) 
		{
			QString consulta = "UPDATE plumaje SET nombre = \"" + nombrePluma->text() + "\", \
			descripcion = \"" + descripcionPluma->text() + "\" \
			WHERE plumajeID = " + tablaPlumaje->currentRecord()->value(0).toString();
			qWarning (consulta);
			QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryUpdate.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando el color de pluma. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error actualizando color de pluma"));
			else
			{
				tablaPlumaje->refresh();
				InsertButton->setText(i18n("Insertar"));
				modificando = FALSE;
				descripcionPluma->clear();
				nombrePluma->clear();
			}
		}
		else
		{
			QString consulta = "INSERT INTO plumaje (nombre, descripcion) VALUES ('";
			consulta += nombrePluma->text() + "', '" + descripcionPluma->text() + "');";
			qWarning (consulta);
			QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryInsert.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error insertantdo el color de pluma.El error devuelto por la base de datos es:\n") +
													error.databaseText(),
													i18n ("Error añadiendo el color de pluma"));
			else
			{
				tablaPlumaje->refresh();
				descripcionPluma->clear();
				nombrePluma->clear();
			}
		}
	}
	else
		KMessageBox::error (this, i18n("Debe rellenar el nombre y la descripción."),
												i18n ("Error añadiendo tipo de suelta"));
}

void plumaje::eliminarColor()
{
	QString consulta = "DELETE FROM plumaje WHERE plumajeID = " + tablaPlumaje->currentRecord()->value(0).toString();
	QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryDelete.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error eliminando el color de pluma. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error eliminando el color de pluma"));
	tablaPlumaje->refresh();
}

void plumaje::modificarColor()
{
	nombrePluma->setText(tablaPlumaje->currentRecord()->value(1).toString());
	descripcionPluma->setText(tablaPlumaje->currentRecord()->value(2).toString());
	InsertButton->setText(i18n("Modificar"));
	modificando = TRUE;
}

