/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "tiposuelta.h"

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

tipoSuelta::tipoSuelta(QWidget *parent)
: tipoSueltaWdg (parent)
{
	modificando = FALSE;
	popup = new KPopupMenu (tablaSueltas, "popup");
	popup->setTitle (i18n("Menu tabla sueltas"));
	KAction *eliminarTipoAction = new KAction(i18n("Eliminar tipo"), "button_cancel", 
					CTRL+Key_D, this, SLOT(eliminarTipo()), this, "eliminarTipoAction");
	eliminarTipoAction->plug (popup);
	KAction *modificarTipoAction = new KAction(i18n("Modificar tipo"), "reload", 
					CTRL+Key_M, this, SLOT(modificarTipo()), this, "modificarTipoAction");
	modificarTipoAction->plug (popup);
}


tipoSuelta::~tipoSuelta()
{
}

void tipoSuelta::contextTablaSueltas (int row, int col, const QPoint & pos) 
{
	popup->exec( QCursor::pos() );
}

void tipoSuelta::insertTipo ()
{
	if (!nombreTipo->text().isEmpty() && !descripcionTipo->text().isEmpty())
	{
		if (modificando) 
		{
			QString consulta = "UPDATE tipoSuelta SET nombre = \"" + nombreTipo->text() + "\", \
			descripcion = \"" + descripcionTipo->text() + "\" \
			WHERE tipoSueltaID = " + tablaSueltas->currentRecord()->value(0).toString();
			qWarning (consulta);
			QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryUpdate.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando el tipo de suelta. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error actualizando tipo de suelta"));
			else
			{
				tablaSueltas->refresh();
				InsertButton->setText(i18n("Insertar"));
				modificando = FALSE;
				descripcionTipo->clear();
				nombreTipo->clear();
			}
		}
		else
		{
			QString consulta = "INSERT INTO tipoSuelta (nombre, descripcion) VALUES ('";
			consulta += nombreTipo->text() + "', '" + descripcionTipo->text() + "');";
			qWarning (consulta);
			QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryInsert.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error insertantdo el tipo de suelta.El error devuelto por la base de datos es:\n") +
													error.databaseText(),
													i18n ("Error añadiendo tipo de suelta"));
			else
			{
				tablaSueltas->refresh();
				descripcionTipo->clear();
				nombreTipo->clear();
			}
		}
	}
	else
		KMessageBox::error (this, i18n("Debe rellenar el nombre y la descripción."),
												i18n ("Error añadiendo tipo de suelta"));
}

void tipoSuelta::eliminarTipo()
{
	QString consulta = "DELETE FROM tipoSuelta WHERE tipoSueltaID = " + tablaSueltas->currentRecord()->value(0).toString();
	QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryDelete.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error eliminando el tipo de suelta. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error eliminando el tipo de suelta"));
	tablaSueltas->refresh();

}

void tipoSuelta::modificarTipo()
{
	nombreTipo->setText(tablaSueltas->currentRecord()->value(1).toString());
	descripcionTipo->setText(tablaSueltas->currentRecord()->value(2).toString());
	InsertButton->setText(i18n("Modificar"));
	modificando = TRUE;
}
