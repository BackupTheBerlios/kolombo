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
#include "buscarWdg.h"

#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qvariant.h>
#include <qstring.h>

buscar::buscar(QWidget* parent, const char* name, bool modal, WFlags fl)
: KDialogBase ( KDialogBase::Swallow, 0L,parent, "exportarTauris", true, "Buscar", KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true )
{
    wdg = new buscarWdg (this, "buscarWdg");
    setMainWidget (wdg);
	QString consulta = "select distinct anyo from paloma where anyo != 0 order by anyo desc;";
	QSqlQuery queryAnyos (consulta, QSqlDatabase::database("palomar" ));
	if (queryAnyos.isActive()) {
		while( queryAnyos.next() )
		wdg->anyo->insertItem (queryAnyos.value(0).toString());
	}
}

buscar::~buscar()
{
}

void buscar::accept() 
{
	bool previo = false;
	
	if (!wdg->numAnilla->text().isEmpty())
	{
		consulta += " (anilla = " + wdg->numAnilla->text () + ")";
		previo = true;
	}
	
	if (!(wdg->anyo->currentText() == "Cualquiera"))
	{
		if (previo)
			consulta += " AND";
		consulta += " (anyo = " + wdg->anyo->currentText() + ")";
		previo = true;
	}
	
	if (!(wdg->sexo->currentText() == "Cualquiera"))
	{
		if (previo)
			consulta += " AND";
		if (wdg->sexo->currentText() == "Hembra")
			consulta += " (palomaID in (SELECT palomaID from paloma WHERE sexo = 1))";
		else
			consulta += " (palomaID in (SELECT palomaID from paloma WHERE sexo = 0))";
		previo = true;
	}
	
	if (!(wdg->estado->currentText() == "Cualquiera"))
	{
		if (previo)
			consulta += " AND";
		consulta += " (palomaID in (SELECT palomaID from paloma WHERE  estado = \"" + wdg->estado->currentText()[0] + "\"))";
	}
    qWarning (consulta);
	QDialog::accept();
	
}

/*$SPECIALIZATION$*/


#include "buscar.moc"

