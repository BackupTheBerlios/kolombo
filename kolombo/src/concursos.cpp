/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "concursos.h"
#include "nuevoconcurso.h"
#include "comprobar.h"
#include "configuration.h"


#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qdatetimeedit.h> 
#include <qcursor.h>
#include <qdatatable.h>
#include <qvariant.h>
#include <qsplitter.h>

#include <kdatepicker.h> 
#include <kdatetbl.h> 
#include <kmessagebox.h>
#include <klocale.h>

concursos::concursos(QWidget* parent, const char* name, WFlags fl)
: concursosWdg(parent,name,fl)
{
	updateComboConcursos();
	sueltaCambiada(0);
	listaGeneral->setColumnWidth(0, config().LDAnyo);
	listaGeneral->setColumnWidth(1, config().LDAnilla);
	listaGeneral->setColumnWidth(2, config().LDNacion);
	listaGeneral->setColumnWidth(3, config().LDNombre);
	listaGeneral->refresh(QDataTable::RefreshColumns);

	listaConcursantes->setColumnWidth(0, config().LCAnyo);
	listaConcursantes->setColumnWidth(1, config().LCAnilla);
	listaConcursantes->setColumnWidth(2, config().LCNacion);
	listaConcursantes->setColumnWidth(3, config().LCModalidad);
	listaConcursantes->setColumnWidth(4, config().LCComprobada);
	listaConcursantes->setColumnWidth(5, config().LCVelocidad);
	listaConcursantes->refresh(QDataTable::RefreshColumns);

	splitterConcursos->setSizes (config().SplitterConcursos);
}

concursos::~concursos()
{
	config().LDAnyo = listaGeneral->columnWidth(0);
	config().LDAnilla = listaGeneral->columnWidth(1);
	config().LDNacion = listaGeneral->columnWidth(2);
	config().LDNombre = listaGeneral->columnWidth(3);
	config().LCAnyo = listaConcursantes->columnWidth(0);
	config().LCAnilla = listaConcursantes->columnWidth(1);
	config().LCNacion = listaConcursantes->columnWidth(2);
	config().LCModalidad = listaConcursantes->columnWidth(3);
	config().LCComprobada = listaConcursantes->columnWidth(4);
	config().LCVelocidad = listaConcursantes->columnWidth(5);
	config().SplitterConcursos = splitterConcursos->sizes ();
	config().write();
}

void concursos::updateComboConcursos() {
	/* Concursos definidos ...*/
	comboSueltas->clear();
	QSqlQuery queryConcursos ("SELECT descripcion FROM vistaConcursos;", QSqlDatabase::database("palomar"));
	if (queryConcursos.isActive()) {
		while( queryConcursos.next() )
			comboSueltas->insertItem (queryConcursos.value(0).toString());
	}
}

/*$SPECIALIZATION$*/
void concursos::nuevaSuelta()
{
	nuevoConcurso concurso;
	if (concurso.exec() == QDialog::Accepted)
		updateComboConcursos();
}

void concursos::horaSueltaCambiada(const QTime& hora) {
	QString consulta = "UPDATE concursos SET horaSuelta = \"" + horaSuelta->time().toString(Qt::ISODate) + "\" where ";
	consulta += "concursoID = " + concursoID;
	qWarning (consulta);
	QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryInsert.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error actualizando la hora de suelta. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
					i18n ("Error actualizando el concurso"));
}

void concursos::sueltaCambiada(int indice)
{
	QString concursoActual = comboSueltas->currentText();
	int inicio = concursoActual.find ( '[');
	int fin = concursoActual.find ( ']');
	concursoID = concursoActual.mid (inicio + 1, fin - inicio - 1);
	qWarning ("concursoID: " + concursoID);
	QSqlQuery queryHora ("SELECT horaSuelta FROM vistaConcursos Where concursoID = \"" + concursoID + "\";",
			QSqlDatabase::database("palomar"));
	disconnect( horaSuelta, SIGNAL( valueChanged(const QTime&) ), this, SLOT( horaSueltaCambiada(const QTime&) ) );
	if (queryHora.isActive()) {
		queryHora.next();
		horaSuelta->setTime (QTime::fromString (queryHora.value(0).toString(), Qt::ISODate));
	}
	connect( horaSuelta, SIGNAL( valueChanged(const QTime&) ), this, SLOT( horaSueltaCambiada(const QTime&) ) );
	listaConcursantes->setFilter ("concursoID = " + concursoID);
	listaConcursantes->refresh();
}

void concursos::setEstado (QString estado, QString palomaID) {
	QString consulta;
	consulta = "SELECT modalidad FROM concursantes WHERE palomaID = " + palomaID + " and concursoID = " + concursoID + ";";
	QSqlQuery querySelect (consulta, QSqlDatabase::database("palomar" ));
	if (querySelect.next()) { /* Si está la paloma, cambiamos el estado */
		if (estado == "D") {/* Cambiar el estado a designada o designada + serie */
			if (querySelect.value(0).toString() == "S")
				estado = "X";
		}
		if (estado == "S") {
			if (querySelect.value(0).toString() == "D")
				estado = "X";	
		}
		consulta = "UPDATE concursantes set modalidad = \"" + estado + "\" WHERE concursoID = " + concursoID + " and ";
		consulta += " palomaID = " + palomaID;
		while (querySelect.next())
			qWarning("Hola..");
	} else {
		consulta = "INSERT INTO concursantes (palomaID, modalidad, concursoID) VALUES ";
		consulta += "(" + palomaID + ", \"" + estado + "\"," + concursoID + ")";
	}
	

	qWarning (consulta);
	QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryInsert.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error añadiendo la paloma al concurso. El error devuelto por la base de datos es:\n") +
			error.databaseText(),
			i18n ("Error añadiendo la paloma al concurso"));
	listaConcursantes->refresh();
}

void concursos::setDesignada() {
	int fila = listaConcursantes->currentRow();
	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		setEstado ("D", listaConcursantes->currentRecord()->value(8).toString());
		listaConcursantes->selectRow (fila);
	}
}

void concursos::setSerie() {
	int fila = listaConcursantes->currentRow();

	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		setEstado ("S", listaConcursantes->currentRecord()->value(8).toString());
		listaConcursantes->selectRow (fila);
	}
}

void concursos::setConcurso() {
	int fila = listaConcursantes->currentRow();
	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		setEstado ("C", listaConcursantes->currentRecord()->value(8).toString());
		listaConcursantes->selectRow (fila);
	}
}

void concursos::encestar() {
	if (listaGeneral->currentRecord() && !listaGeneral->currentRecord()->isNull (0))
		setEstado ("F", listaGeneral->currentRecord()->value(0).toString());
}


void concursos::setFueraConcurso() {
	int fila = listaConcursantes->currentRow();
	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		setEstado ("F", listaConcursantes->currentRecord()->value(8).toString());
		listaConcursantes->selectRow (fila);
	}
}

void concursos::eliminar() {
	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		QString palomaID = listaConcursantes->currentRecord()->value(8).toString();
		QString consulta;
		qWarning ("Eliminar");
		consulta = "DELETE FROM concursantes WHERE concursoID = " + concursoID + " and palomaID = " + palomaID;
		qWarning (consulta);
		QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
		QSqlError error = queryDelete.lastError();
		if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error añadiendo la paloma al concurso. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error añadiendo la paloma al concurso"));
		listaConcursantes->refresh();
	}
}

void concursos::comprobarSlot() {
	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		QString palomaID = listaConcursantes->currentRecord()->value(8).toString();
		comprobar dlg(concursoID, palomaID);
		if (dlg.exec() == QDialog::Accepted)
			listaConcursantes->refresh();
	}
}

/*
	Me tengo que acostumbrar a usar doxygen .... pero mientras ....
	Esto establece el estado a desaparecida y pone "Desaparecida" en la base de datos, menos mal que es mala
	y no tiene soporte para el tipo de dato TIME. Desde que se use otra base de datos esto no vale.
*/
void concursos::desaparecer() {
	if (listaConcursantes->currentRecord() && !listaConcursantes->currentRecord()->isNull (8)) {
		QString consulta;
		consulta = "UPDATE paloma SET estado = \"D\" WHERE palomaID = " + listaConcursantes->currentRecord()->value(8).toString();
		QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
		/* ATENCION COCHINADA!!!!!!!!!!!!!!!!!*/
		consulta = "UPDATE concursantes SET comprobada = \"Desaparecida\" WHERE palomaID = " + listaConcursantes->currentRecord()->value(8).toString() + "and concursoID = " + concursoID;
		QSqlQuery queryUpdateConcursos (consulta, QSqlDatabase::database("palomar" ));
		listaConcursantes->refresh();
	}
}

#include "concursos.moc"
