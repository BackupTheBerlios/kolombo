/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ojo.h"

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

ojo::ojo(QWidget *parent)
: ojoWdg (parent, "ojoWdg", 0)
{
	modificando = FALSE;
	popup = new KPopupMenu (tablaOjos, "popup");
	popup->setTitle (i18n("Menu tabla ojos"));
	KAction *eliminarTipoAction = new KAction(i18n("Eliminar tipo"), "button_cancel", 
					CTRL+Key_D, this, SLOT(eliminarTipo()), this, "eliminarTipoAction");
	eliminarTipoAction->plug (popup);
	KAction *modificarTipoAction = new KAction(i18n("Modificar tipo"), "reload", 
					CTRL+Key_M, this, SLOT(modificarTipo()), this, "modificarTipoAction");
	modificarTipoAction->plug (popup);
}


ojo::~ojo()
{
}

void ojo::contextOjo (int row, int col, const QPoint & pos) 
{
	popup->exec( QCursor::pos() );
}

void ojo::insertarOjo ()
{
	if (!nombreOjo->text().isEmpty() && !descripcionOjo->text().isEmpty())
	{
		if (modificando) 
		{
			QString consulta = "UPDATE ojo SET nombre = \"" + nombreOjo->text() + "\", \
			descripcion = \"" + descripcionOjo->text() + "\" \
			WHERE ojoID = " + tablaOjos->currentRecord()->value(0).toString();
			qWarning (consulta);
			QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryUpdate.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando el tipo de ojo. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error actualizando tipo de ojo"));
			else
			{
				tablaOjos->refresh();
				InsertButton->setText(i18n("Insertar"));
				modificando = FALSE;
				descripcionOjo->clear();
				nombreOjo->clear();
			}
		}
		else
		{
			QString consulta = "INSERT INTO ojo (nombre, descripcion) VALUES ('";
			consulta += nombreOjo->text() + "', '" + descripcionOjo->text() + "');";
			qWarning (consulta);
			QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryInsert.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error insertantdo el tipo de ojo.El error devuelto por la base de datos es:\n") +
													error.databaseText(),
													i18n ("Error añadiendo tipo de ojo"));
			else
			{
				tablaOjos->refresh();
				descripcionOjo->clear();
				nombreOjo->clear();
			}
		}
	}
	else
		KMessageBox::error (this, i18n("Debe rellenar el nombre y la descripción."),
												i18n ("Error añadiendo tipo de ojo"));
}

void ojo::eliminarTipo()
{
	QString consulta = "DELETE FROM ojo WHERE ojoID = " + tablaOjos->currentRecord()->value(0).toString();
	QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryDelete.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error eliminando el tipo de ojo. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error eliminando el tipo de ojo"));
	tablaOjos->refresh();

}

void ojo::modificarTipo()
{
	nombreOjo->setText(tablaOjos->currentRecord()->value(1).toString());
	descripcionOjo->setText(tablaOjos->currentRecord()->value(2).toString());
	InsertButton->setText(i18n("Modificar"));
	modificando = TRUE;
}

