/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "buscar.h"

#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qvariant.h>
#include <qstring.h>

buscar::buscar(QWidget* parent, const char* name, bool modal, WFlags fl)
: buscarDlg(parent,name, modal,fl)
{
	QString consulta = "select distinct anyo from paloma where anyo != 0 order by anyo;";
	QSqlQuery queryAnyos (consulta, QSqlDatabase::database("palomar" ));
	if (queryAnyos.isActive()) {
		while( queryAnyos.next() )
		anyo->insertItem (queryAnyos.value(0).toString());
	}	
}

buscar::~buscar()
{
}

void buscar::accept() 
{
	bool previo = false;
	
	if (!this->numAnilla->text().isEmpty())
	{
		consulta += " (anilla = " + this->numAnilla->text () + ")";
		previo = true;
	}
	
	if (!(this->anyo->currentText() == "Cualquiera"))
	{
		if (previo)
			consulta += " AND";
		consulta += " (anyo = " + this->anyo->currentText() + ")";
		previo = true;
	}
	
	if (!(this->sexo->currentText() == "Cualquiera"))
	{
		if (previo)
			consulta += " AND";
		if (this->sexo->currentText() == "Hembra")
			consulta += " (sexo = 1)";
		else
			consulta += " (sexo = 0)";
		previo = true;
	}
	
	if (!(this->estado->currentText() == "Cualquiera"))
	{
		if (previo)
			consulta += " AND";
		consulta += " (estado = \"" + this->estado->currentText()[0] + "\")";
	}
	QDialog::accept();
	
}

/*$SPECIALIZATION$*/


#include "buscar.moc"

