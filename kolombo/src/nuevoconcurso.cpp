/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "nuevoconcurso.h"

#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <kdatepicker.h> 
#include <kdatetbl.h> 
#include <kmessagebox.h>
#include <klocale.h>

nuevoConcurso::nuevoConcurso(QWidget* parent, const char* name, bool modal, WFlags fl)
: nuevoConcursoDlg(parent,name, modal,fl)
{
	KDateTable::BackgroundMode bgMode = KDateTable::RectangleMode;
	QColor fgColor = QColor::QColor (1, 1, 1);
	QColor bgColor = QColor::QColor (42, 255, 91);
	/* Concursos definidos ...*/
	QSqlQuery queryConcursos ("SELECT diaSuelta FROM concursos;",
		QSqlDatabase::database("palomar"));
	if (queryConcursos.isActive()) {
		while( queryConcursos.next() )
			fechaConcurso->dateTable()->setCustomDatePainting(
				QDate::fromString ((queryConcursos.value(0).toString()), Qt::ISODate), 
				fgColor, bgMode, bgColor);
	}

	/* Puntos de suelta definidos ...*/
	QSqlQuery queryPuntosDeSuelta ("SELECT  nombre FROM suelta;",
		QSqlDatabase::database("palomar" ));
	if (queryPuntosDeSuelta.isActive()) {
		while( queryPuntosDeSuelta.next() )
			comboPuntosSuelta->insertItem (queryPuntosDeSuelta.value(0).toString());
	}

}

nuevoConcurso::~nuevoConcurso()
{
}

/*$SPECIALIZATION$*/
void nuevoConcurso::reject()
{
	QDialog::reject();
}

void nuevoConcurso::accept()
{
	QString consulta = "INSERT INTO concursos (sueltaID, diaSuelta, horaSuelta) values ";
	consulta += "((select sueltaID from suelta where nombre = \"" + comboPuntosSuelta->currentText() + "\"), ";
	consulta += "\"" + fechaConcurso->date().toString("yyyy:MM:dd") + "\",\"10:00:00\")";

	QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryInsert.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error insertando el nuevo concurso. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
					i18n ("Error añadiendo nuevo concurso"));
	QDialog::accept();
}



#include "nuevoconcurso.moc"

