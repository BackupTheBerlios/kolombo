/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "datospaloma.h"
#include "configuration.h"
#include "kbird2view.h"

#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qcombobox.h>
#include <qvariant.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qsqlerror.h>

#include <kpushbutton.h>
#include <kmessagebox.h>
#include <klocale.h>


datosPaloma::datosPaloma(QWidget *parent, bool edit)
: datosPalomaWdg (parent, "datosPalomaWdg", 0)
{
	this->edit = edit;
	this->parent = parent;
	/* Colores pluma... */
	QSqlQuery queryPlumaje ("SELECT  nombre FROM plumaje", QSqlDatabase::database("palomar" ));
	if (queryPlumaje.isActive()) {
		while( queryPlumaje.next() )
		comboPlumaje->insertItem (queryPlumaje.value(0).toString());
	}

	/* Colores ojo... */
	QSqlQuery queryOjo ("SELECT  nombre FROM ojo", QSqlDatabase::database("palomar" ));
	if (queryOjo.isActive()) {
		while( queryOjo.next() )
		comboOjo->insertItem (queryOjo.value(0).toString());
	}

	/* Nacionalidades... */
	QSqlQuery queryNacion ("SELECT iniciales FROM nacionalidad", QSqlDatabase::database("palomar" ));
	if (queryNacion.isActive()) {
		while( queryNacion.next() )
		comboNacion->insertItem (queryNacion.value(0).toString());
	}

	/* Hembras en vuelo o en reproducción ... */
	QSqlQuery queryMadres ("SELECT  idCompacto FROM idCompacto WHERE \
		palomaID in (\
		SELECT palomaID FROM paloma WHERE \
			sexo = 1\
			and (estado = \"V\" or estado = \"R\"));",
		QSqlDatabase::database("palomar" ));
	if (queryMadres.isActive()) {
		comboMadre->insertItem ("0000-0000000000-ESP");
		while( queryMadres.next() )
		comboMadre->insertItem (queryMadres.value(0).toString());
	}

	/* Machos en vuelo o en reproducción ...*/
	QSqlQuery queryPadres ("SELECT  idCompacto FROM idCompacto WHERE \
		palomaID in (\
		SELECT palomaID FROM paloma WHERE \
		sexo = 0\
		and (estado = \"V\" or estado = \"R\"));",
		QSqlDatabase::database("palomar" ));
	if (queryPadres.isActive()) {
		comboPadre->insertItem ("0000-0000000000-ESP");
		while( queryPadres.next() )
		comboPadre->insertItem (queryPadres.value(0).toString());
	}

	if (edit) {
		connect (aceptar, SIGNAL( clicked() ), this, SLOT (updatePaloma()));
		connect (cancelar, SIGNAL (clicked()), this, SLOT (loadData()));
		connect (siguiente, SIGNAL (clicked()), this, SLOT (next()));
		connect (anterior, SIGNAL (clicked()), this, SLOT (previous()));
		aceptar->setText ("Modificar");
	} else {
		connect (aceptar, SIGNAL(clicked()), this, SLOT (insertPaloma()));
		connect (cancelar, SIGNAL (clicked()), this, SLOT (clearData()));
		clearData();
		aceptar->setText ("Insertar");
		anterior->setDisabled(true);
		siguiente->setDisabled(true);
	}
}


datosPaloma::~datosPaloma()
{
}


void datosPaloma::loadData () {

	if (config().anillaSeleccion) {
		QString palomaID;
		palomaID = palomaID.setNum (config().anillaSeleccion);
		QString consulta = "SELECT anyo, anilla, nacionalidad.iniciales, ojo.nombre, plumaje.nombre, madre.idCompacto, \
					padre.idCompacto, paloma.nombre, paloma.nota, paloma.sexo, paloma.estado \
					FROM paloma, nacionalidad, ojo, plumaje, idCompacto madre, idCompacto padre \
					WHERE paloma.palomaID = " + palomaID +" AND \
					paloma.nacionalidadID = nacionalidad.nacionalidadID AND \
					paloma.ojoID = ojo.ojoID AND \
					paloma.plumajeID = plumaje.plumajeID AND madre.palomaID = paloma.madreID AND padre.palomaID = paloma.padreID;";

//		qWarning (consulta);
		QSqlQuery querySelect (consulta, QSqlDatabase::database("palomar" ));
		if (querySelect.next()) {
			anyo->setValue(querySelect.value(0).toInt());
			numeroAnilla->setValue (querySelect.value(1).toInt());
			comboPlumaje->setCurrentText (querySelect.value(4).toString());
			comboOjo->setCurrentText (querySelect.value(3).toString());
			comboMadre->setCurrentText (querySelect.value(5).toString());
			comboPadre->setCurrentText (querySelect.value(6).toString());
			comboNacion->setCurrentText (querySelect.value(2).toString());
			nombre->setText(querySelect.value(7).toString());
			nota->setText(querySelect.value(8).toString());
			if (querySelect.value(9).toInt() == 0)
				macho->setChecked (true);
			else
				hembra->setChecked (true);
			if (querySelect.value(10).toString() == "V")
				vuelo->setChecked (true);
			else {
				if (querySelect.value(10).toString() == "R")
					reproduccion->setChecked (true);
				else
					desaparecida->setChecked (true);
			}
		} else {
			nombre->setText("NO VALIDO");
		}
		/* Comprobamos que se pueda cambiar de sexo ... */
		consulta = "Select count(palomaID) from paloma where madreID =  " + palomaID + " OR \
			padreID = " + palomaID;
		qWarning ("Consulta sexo: " + consulta);
		QSqlQuery querySexo (consulta, QSqlDatabase::database("palomar" ));
		querySexo.next();
		qWarning ("Cuenta: " + querySexo.value(0).toString());
		if ( querySexo.value(0).toInt() != 0)
			grupoSexo->setEnabled(false);
		else
			grupoSexo->setEnabled(true);
	}
}

void datosPaloma::clearData () {
	QDateTime reloj;
	QDate date(reloj.currentDateTime().date());
	anyo->setValue(date.year());
	numeroAnilla->setValue (config().ultimaAnilla);
	comboPlumaje->setCurrentItem (0);
	comboOjo->setCurrentItem (0);
	comboMadre->setCurrentItem (0);
	comboPadre->setCurrentItem (0);
	comboNacion->setCurrentItem (0);
	nombre->clear();
	nota->clear();
	vuelo->setChecked (true);
	macho->setChecked (true);
}

void datosPaloma::insertPaloma () {
	QString sexo;
	QString estado;
	if (macho->isChecked())
		sexo = "0";
	else
		sexo = "1";
	if (reproduccion->isChecked())
		estado = "R";
	else {
		if (vuelo->isChecked())
			estado = "V";
		else
			estado = "D";
	}
	QString consulta = "INSERT INTO paloma (anyo, anilla, nacionalidadID, sexo, madreID, padreID, plumajeID, ojoID, estado, nombre, nota) \
		VALUES (" + anyo->text() +", " + numeroAnilla->text() + ",(\
			SELECT nacionalidadID \
			FROM nacionalidad \
			WHERE iniciales = \"" + comboNacion->currentText() +"\"\
		), " + sexo + ", (\
			SELECT palomaID \
			FROM idCompacto \
			WHERE idCompacto =\"" +  comboMadre->currentText() + "\"\
		), (\
			SELECT palomaID \
			FROM idCompacto \
			WHERE idCompacto =\"" +  comboPadre->currentText() + "\"\
		), (\
			SELECT plumajeID \
			FROM plumaje \
			WHERE nombre = \"" + comboPlumaje->currentText() +"\"\
		), (\
			SELECT ojoID \
			FROM ojo \
			WHERE nombre = \"" + comboOjo->currentText() +"\"\
		), \"" + estado + "\", \"" + nombre->text()  + "\", \"" + nota->text() + "\")";
	QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryInsert.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
					i18n ("Error añadiendo actualizando paloma"));
	else {
		config().updateAnilla(numeroAnilla->value());
		numeroAnilla->stepUp();
		KMessageBox::information (this, 
					i18n("Paloma insertada"), 
					i18n("Paloma insertada con éxito"));
	}
}


void datosPaloma::crearPareja() {

	QString consulta = "INSERT INTO parejas (nombre, madreID, padreID, nota) VALUES (\"\", (\
		SELECT palomaID FROM idCompacto WHERE \
		idCompacto = \"" + comboMadre->currentText() + "\"), (\
		SELECT palomaID FROM idCompacto WHERE \
		idCompacto = \"" + comboPadre->currentText() + "\"), \"\")";
	//qWarning (consulta);
	QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryInsert.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, 
				i18n("Error creando la pareja. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error añadiendo pareja"));
	else
		KMessageBox::information (	this, 
											i18n("Pareja creada"), 
											i18n("Pareja creada con éxito"));
}


void datosPaloma::updatePaloma () {

	QString palomaID;
	palomaID = palomaID.setNum (config().anillaSeleccion);
	QString sexo;
	QString estado;
	if (macho->isChecked())
		sexo = "0";
	else
		sexo = "1";
	if (reproduccion->isChecked())
		estado = "R";
	else {
		if (vuelo->isChecked())
			estado = "V";
		else
			estado = "D";
	}
	QString consulta = "UPDATE paloma SET \
		anyo = " + anyo->text() +", anilla = " + numeroAnilla->text() + ", nacionalidadID = (\
			SELECT nacionalidadID \
			FROM nacionalidad \
			WHERE iniciales = \"" + comboNacion->currentText() +"\"\
		)";
	
	if (grupoSexo->isEnabled())
		consulta += ", sexo = " + sexo + " ";
		
		
	consulta += ", madreID = (\
			SELECT palomaID \
			FROM idCompacto \
			WHERE idCompacto =\"" +  comboMadre->currentText() + "\"\
		), padreID = (\
			SELECT palomaID \
			FROM idCompacto \
			WHERE idCompacto =\"" +  comboPadre->currentText() + "\"\
		), plumajeID = (\
			SELECT plumajeID \
			FROM plumaje \
			WHERE nombre = \"" + comboPlumaje->currentText() +"\"\
		), ojoID = (\
			SELECT ojoID \
			FROM ojo \
			WHERE nombre = \"" + comboOjo->currentText() +"\"\
		), estado = \"" + estado + "\", nombre = \"" + nombre->text()  + "\", nota = \"" + nota->text() + "\"\
		WHERE palomaID = " + palomaID + ";";
	QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryUpdate.lastError();
	if (error.type() != QSqlError::None)
		KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error añadiendo actualizando paloma"));
	else
		KMessageBox::information (this, 
					i18n("Paloma modificada"), 
					i18n("Paloma modificada con éxito"));
}


void datosPaloma::previous () {
	QString consulta = "select palomaID from paloma where anilla < " +  numeroAnilla->text()+ " and anyo = " + anyo->text() + " order by anilla desc limit 1";
	qWarning (consulta);
	QSqlQuery querySelect (consulta, QSqlDatabase::database("palomar" ));
	if (querySelect.next()) {
		config().anillaSeleccion = (querySelect.value(0).toInt());
		loadData ();
	}
}

void datosPaloma::next () {
	QString consulta = "select palomaID from paloma where anilla > " +  numeroAnilla->text()+ " and anyo = " + anyo->text() + " order by anilla limit 1";
	QSqlQuery querySelect (consulta, QSqlDatabase::database("palomar" ));
	if (querySelect.next()) {
		config().anillaSeleccion = (querySelect.value(0).toInt());
		loadData ();
	}
}

void datosPaloma::showEvent( QShowEvent *e )
{
	if (edit) {
		loadData ();
		nota->setFocus();
	 } else
		clearData();
}