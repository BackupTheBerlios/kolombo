/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "arbolgen.h"
#include "configuration.h"

#include <qstring.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qvariant.h>
#include <qsqlquery.h>
#include <qsqldatabase.h>

arbolGen::arbolGen(QWidget *parent)
: arbolGenWdg (parent, "arbolGenWdg", 0)
{
}


arbolGen::~arbolGen()
{
}

void arbolGen::loadData () {

	palomaID = palomaID.setNum (config().anillaSeleccion);

	QString consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
			listaGeneral.sexo, paloma.madreID, paloma.padreID\
			FROM idCompacto, listaGeneral, paloma\
			WHERE idCompacto.palomaID = " + palomaID + " AND listaGeneral.palomaID = " + palomaID + " AND paloma.palomaID = " + palomaID;
	QSqlQuery query1 (consulta, QSqlDatabase::database("palomar" ));
	if (query1.next()) {
		individuo->setTitle(query1.value(0).toString());
		indSexo->setText(query1.value(3).toString());
		indOjo->setText(query1.value(2).toString());
		indPlumaje->setText(query1.value(1).toString());
		madreID = query1.value(4).toString();
		padreID = query1.value(5).toString();
	}

	consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
					listaGeneral.sexo, paloma.madreID, paloma.padreID\
					FROM idCompacto, listaGeneral, paloma\
					WHERE idCompacto.palomaID = " + madreID + " AND listaGeneral.palomaID = " + madreID + " AND paloma.palomaID = " + madreID;
	QSqlQuery query2 (consulta, QSqlDatabase::database("palomar" ));
	if (query2.next()) {
		madre->setTitle(query2.value(0).toString());
		madreSexo->setText(query2.value(3).toString());
		madreOjo->setText(query2.value(2).toString());
		madrePlumaje->setText(query2.value(1).toString());
		abuelaMID = query2.value(4).toString();
		abueloMID = query2.value(5).toString();
	} else {
		abuelaMID = "1";
		abueloMID = "2";
	}

	consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
					listaGeneral.sexo, paloma.madreID, paloma.padreID\
					FROM idCompacto, listaGeneral, paloma\
					WHERE idCompacto.palomaID = " + padreID + " AND listaGeneral.palomaID = " + padreID + " AND paloma.palomaID = " + padreID;
	QSqlQuery query3 (consulta, QSqlDatabase::database("palomar"));
	if (query3.next()) {
		padre->setTitle(query3.value(0).toString());
		padreSexo->setText(query3.value(3).toString());
		padreOjo->setText(query3.value(2).toString());
		padrePlumaje->setText(query3.value(1).toString());
		abuelaPID = query3.value(4).toString();
		abueloPID = query3.value(5).toString();
	} else {
		abuelaPID = "1";
		abueloPID = "2";
	}

	consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
					listaGeneral.sexo \
					FROM idCompacto, listaGeneral, paloma\
					WHERE idCompacto.palomaID = " + abuelaMID + " AND listaGeneral.palomaID = " + abuelaMID + " AND \
					paloma.palomaID = " + abuelaMID;
	QSqlQuery query4 (consulta, QSqlDatabase::database("palomar"));
	if (query4.next()) {
		abuelaM->setTitle(query4.value(0).toString());
		abuelaMSexo->setText(query4.value(3).toString());
		abuelaMOjo->setText(query4.value(2).toString());
		abuelaMPlumaje->setText(query4.value(1).toString());
	}

	consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
					listaGeneral.sexo \
					FROM idCompacto, listaGeneral, paloma\
					WHERE idCompacto.palomaID = " + abueloMID + " AND listaGeneral.palomaID = " + abueloMID + " AND \
					paloma.palomaID = " + abueloMID;
	QSqlQuery query5 (consulta, QSqlDatabase::database("palomar"));
	if (query5.next()) {
		abueloM->setTitle(query5.value(0).toString());
		abueloMSexo->setText(query5.value(3).toString());
		abueloMOjo->setText(query5.value(2).toString());
		abueloMPlumaje->setText(query5.value(1).toString());
	}

	consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
					listaGeneral.sexo\
					FROM idCompacto, listaGeneral, paloma\
					WHERE idCompacto.palomaID = " + abuelaPID + " AND listaGeneral.palomaID = " + abuelaPID + " AND \
					paloma.palomaID = " + abuelaPID;
	QSqlQuery query6 (consulta, QSqlDatabase::database("palomar"));
	if (query6.next()) {
		abuelaP->setTitle(query6.value(0).toString());
		abuelaPSexo->setText(query6.value(3).toString());
		abuelaPOjo->setText(query6.value(2).toString());
		abuelaPPlumaje->setText(query6.value(1).toString());
	}

	consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
					listaGeneral.sexo\
					FROM idCompacto, listaGeneral, paloma\
					WHERE idCompacto.palomaID = " + abueloPID + " AND listaGeneral.palomaID = " + abueloPID + " AND \
					paloma.palomaID = " + abueloPID;
	QSqlQuery query7 (consulta, QSqlDatabase::database("palomar"));
	if (query7.next()) {
		abueloP->setTitle(query7.value(0).toString());
		abueloPSexo->setText(query7.value(3).toString());
		abueloPOjo->setText(query7.value(2).toString());
		abueloPPlumaje->setText(query7.value(1).toString());
	}
}

void arbolGen::showEvent( QShowEvent *e )
{
	loadData ();
}


void arbolGen::subirMadre() {
	config().anillaSeleccion = madreID.toInt();
	loadData ();
}

void arbolGen::subirPadre() {
	config().anillaSeleccion = padreID.toInt();
	loadData ();
}

void arbolGen::subirAbuelaM() {
	config().anillaSeleccion = abuelaMID.toInt();
	loadData ();
}

void arbolGen::subirAbueloM() {
	config().anillaSeleccion = abueloMID.toInt();
	loadData ();
}

void arbolGen::subirAbuelaP() {
	config().anillaSeleccion = abuelaPID.toInt();
	loadData ();
}

void arbolGen::subirAbueloP() {
	config().anillaSeleccion = abueloPID.toInt();
	loadData ();
}
