/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "nacion.h"

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

nacion::nacion(QWidget *parent)
: nacionWdg (parent, "nacionWdg", 0)
{
	modificando = FALSE;
	popup = new KPopupMenu (tablaNaciones, "popup");
	popup->setTitle (i18n("Menu tabla naciones"));
	KAction *eliminarNacionAction = new KAction(i18n("Eliminar nacion"), "button_cancel", 
					CTRL+Key_D, this, SLOT(eliminarNacion()), this, "eliminarNacionAction");
	eliminarNacionAction->plug (popup);
	KAction *modificarNacionAction = new KAction(i18n("Modificar nacion"), "reload", 
					CTRL+Key_M, this, SLOT(modificarNacion()), this, "modificarNacionAction");
	modificarNacionAction->plug (popup);
}


nacion::~nacion()
{
}

void nacion::contextNaciones (int row, int col, const QPoint & pos) 
{
	popup->exec( QCursor::pos() );
}

void nacion::insertNacion ()
{
	if (!inicialesNacion->text().isEmpty() && !descripcionNacion->text().isEmpty())
	{
		if (modificando) 
		{
			QString consulta = "UPDATE nacionalidad SET iniciales = \"" + inicialesNacion->text() + "\", \
			descripcion = \"" + descripcionNacion->text() + "\" \
			WHERE nacionalidadID = " + tablaNaciones->currentRecord()->value(0).toString();
			qWarning (consulta);
			QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryUpdate.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando la nacionalidad. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error actualizando nacionalidad"));
			else
			{
				tablaNaciones->refresh();
				InsertButton->setText(i18n("Insertar"));
				modificando = FALSE;
				descripcionNacion->clear();
				inicialesNacion->clear();
			}
		}
		else
		{
			QString consulta = "INSERT INTO nacionalidad (iniciales, descripcion) VALUES ('";
			consulta += inicialesNacion->text() + "', '" + descripcionNacion->text() + "');";
			qWarning (consulta);
			QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryInsert.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error insertantdo la nacionalidad.El error devuelto por la base de datos es:\n") +
													error.databaseText(),
													i18n ("Error añadiendo nacionalidad"));
			else
			{
				tablaNaciones->refresh();
				descripcionNacion->clear();
				inicialesNacion->clear();
			}
		}
	}
	else
		KMessageBox::error (this, i18n("Debe rellenar el nombre y la descripción."),
												i18n ("Error añadiendo tipo de suelta"));
}

void nacion::eliminarNacion()
{
	QString consulta = "DELETE FROM nacionalidad WHERE nacionalidadID = " + tablaNaciones->currentRecord()->value(0).toString();
	QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryDelete.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error eliminando la nacionalidad. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error eliminando la nacionalidad"));
	tablaNaciones->refresh();

}

void nacion::modificarNacion()
{
	inicialesNacion->setText(tablaNaciones->currentRecord()->value(1).toString());
	descripcionNacion->setText(tablaNaciones->currentRecord()->value(2).toString());
	InsertButton->setText(i18n("Modificar"));
	modificando = TRUE;
}
