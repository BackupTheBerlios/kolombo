/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "suelta.h"
#include "configuration.h"

#include <math.h>

#include <qdatatable.h>
#include <qcursor.h>
#include <qsqlquery.h>
#include <qsqldatabase.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qvalidator.h>

#include <kpushbutton.h>
#include <kpopupmenu.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kaction.h>

suelta::suelta(QWidget* parent, const char* name, WFlags fl)
: sueltasWdg(parent,name,fl)
{
	loadCombo();
	QValidator* validatorCoord = new QIntValidator( 0, 999999999, this );
        coordx->setValidator(validatorCoord);
        coordy->setValidator(validatorCoord);
	modificando = FALSE;
	popup = new KPopupMenu (tablaSuelta, "popup");
	popup->setTitle (i18n("Menu tabla sueltas"));
	KAction *eliminarSueltaAction = new KAction(i18n("Eliminar suelta"), "button_cancel", 
					CTRL+Key_D, this, SLOT(eliminarSuelta()), this, "eliminarSueltaAction");
	eliminarSueltaAction->plug (popup);
	KAction *modificarSueltaAction = new KAction(i18n("Modificar suelta"), "reload", 
					CTRL+Key_M, this, SLOT(modificarSuelta()), this, "modificarSueltaAction");
	modificarSueltaAction->plug (popup);

}

suelta::~suelta()
{
}

/*$SPECIALIZATION$*/
void suelta::contextSueltas(int row, int col, const QPoint & pos)
{
	popup->exec( QCursor::pos() );
}

void suelta::insertSuelta()
{
	if (!nombre->text().isEmpty()
			&& !coordx->text().isEmpty()
            && !coordy->text().isEmpty()
			&& !descripcionSuelta->text().isEmpty())
	{
        double a, b;
        a = coordx->text().toDouble() - config().coordX;
        b = coordy->text().toDouble() - config().coordY;
		if (modificando) 
		{
			QString consulta = "UPDATE suelta SET nombre = \"" + nombre->text() + "\", \
			descripcion = \"" + descripcionSuelta->text() + "\", \
            distancia = \"" + QString::number(sqrt(((a*a) + (b*b))/1000)) + "\", \
            coordx = " + coordx->text() + ", \
            coordy = " + coordy->text() + ", \
			tipoSueltaID = (SELECT tipoSueltaID FROM tipoSuelta \
			WHERE nombre = \"" + tipoSueltaCombo->currentText() + "\" )\
			WHERE sueltaID = " + tablaSuelta->currentRecord()->value(0).toString();
			qWarning (consulta);
			QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryUpdate.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando la suelta. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error actualizando la suelta"));
			else
			{
				tablaSuelta->refresh();
				InsertButton->setText(i18n("Insertar"));
				modificando = FALSE;
				descripcionSuelta->clear();
				nombre->clear();
				distancia->setText(QString::number(0));
                coordx->setText("0");
                coordy->setText("0");
			}
		}
		else
		{
			QString consulta = "INSERT INTO suelta (nombre,descripcion,distancia,tipoSueltaID,coordx,coordy)";
			consulta += " VALUES ('";
			consulta += nombre->text() + "', '" + descripcionSuelta->text() + "', '";
			consulta += QString::number(sqrt(((a*a) + (b*b))/1000)) + "',";
			consulta += "(SELECT tipoSueltaID from tipoSuelta WHERE ";
			consulta += "nombre = '"+tipoSueltaCombo->currentText()+"'), " + coordx->text() + ", " + coordy->text() + " );";
			qWarning (consulta);
			QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryInsert.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error insertantdo la suelta.El error devuelto por la base de datos es:\n") +
													error.databaseText(),
													i18n ("Error añadiendo la suelta"));
			else
			{
				tablaSuelta->refresh();
				descripcionSuelta->clear();
				nombre->clear();
				distancia->setText(QString::number(0));
                coordx->setText("0");
                coordy->setText("0");
			}
		}
	}
	else
		KMessageBox::error (this, i18n("Debe rellenar el nombre, la descripción, la distancia y las coordenadas del punto de suelta."),
												i18n ("Error añadiendo suelta"));
}



void suelta::eliminarSuelta()
{
	QString consulta = "DELETE FROM suelta WHERE sueltaID = " + tablaSuelta->currentRecord()->value(0).toString();
	QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryDelete.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error eliminando la suelta de la base de datos. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error eliminando la suelta"));
	tablaSuelta->refresh();
}

void suelta::modificarSuelta()
{
	nombre->setText(tablaSuelta->currentRecord()->value(1).toString());
	descripcionSuelta->setText(tablaSuelta->currentRecord()->value(4).toString());
	InsertButton->setText(i18n("Modificar"));
	distancia->setText(tablaSuelta->currentRecord()->value(3).toString());
	tipoSueltaCombo->setCurrentText (tablaSuelta->currentRecord()->value(2).toString());
    coordx->setText(tablaSuelta->currentRecord()->value(5).toString());
    coordy->setText(tablaSuelta->currentRecord()->value(6).toString());
	modificando = TRUE;
}

void suelta::loadCombo()
{
	QSqlQuery tipoSuelta ("SELECT nombre FROM tipoSuelta", QSqlDatabase::database("palomar" ));
	tipoSueltaCombo->clear();
	if (tipoSuelta.isActive()) {
		while( tipoSuelta.next() )
		tipoSueltaCombo->insertItem (tipoSuelta.value(0).toString());
	}
}

void suelta::showEvent( QShowEvent *e )
{
	loadCombo();
}

void suelta::actualizaDistancia()
{
    double a, b;
    a = coordx->text().toDouble() - config().coordX;
    b = coordy->text().toDouble() - config().coordY;
    distancia->setText(QString::number(sqrt(((a*a) + (b*b))/1000)));
}

#include "suelta.moc"
