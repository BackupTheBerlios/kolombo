/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "comprobar.h"

#include <time.h>

#include <qdatetime.h>
#include <kdatetimewidget.h>
#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>

comprobar::comprobar(QString &concursoID, QString &palomaID, QWidget* parent, const char* name, bool modal, WFlags fl)
: comprobarDlg(parent,name, modal,fl)
{
	qWarning ("Constructor");
	this->palomaID = palomaID;
	this->concursoID = concursoID;
	QString consulta = "select diaSuelta, horaSuelta from concursos where concursoID = " + concursoID;
	QSqlQuery queryTiempo (consulta, QSqlDatabase::database("palomar"));
	if (queryTiempo.isActive()) {
		queryTiempo.next();
		Tinicio = QDateTime::fromString (queryTiempo.value(0).toString() + ":" + queryTiempo.value(1).toString(),  Qt::ISODate);
		tiempoComprobacion->setDateTime (Tinicio);
	}
}

comprobar::~comprobar()
{
}

/*$SPECIALIZATION$*/
void comprobar::accept()
{
	QString consulta = "select distancia from concursos c, suelta s where c.sueltaID = s.sueltaID and concursoID = " + concursoID;
	QSqlQuery queryDistancia (consulta, QSqlDatabase::database("palomar"));
	if (queryDistancia.isActive()) {
		queryDistancia.next();
		double distancia = queryDistancia.value(0).toInt();
		qWarning ("Distancia en kilómetros: " + QString::number (distancia));
		double diferencia = difftime (tiempoComprobacion->dateTime().toTime_t(), Tinicio.toTime_t());
		qWarning ("Diferencia en segundos: " + QString::number (diferencia));
		double velocidad = distancia / (diferencia / 3600);
		QString ds = QString::number( velocidad, 'f', 3 );
		qWarning ("Velocidad: " + ds);
		consulta = "UPDATE concursantes SET comprobada = \"" + tiempoComprobacion->dateTime().toString(Qt::ISODate) + "\", velocidad = " + ds;
		consulta += " where palomaID = " + palomaID + " and concursoID = " + concursoID;
		qWarning (consulta);
		QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar"));
	}
  QDialog::accept();
}



#include "comprobar.moc"

