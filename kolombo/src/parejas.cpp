/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "parejas.h"
#include "nuevaparejaWdg.h"
#include "nuevoHijoDlg.h"
#include "configuration.h"
#include "kbird2view.h"
#include "parejasstats.h"
#include "palomardatatable.h"

#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qdatatable.h>
#include <qvariant.h>
#include <qstring.h>
#include <qdialog.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qdatetime.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qcursor.h>
#include <qsplitter.h>

#include <kmessagebox.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <kaction.h>
#include <kpushbutton.h>

parejas::parejas(QWidget *parent)
: parejasWdg (parent, "parejasWdg", 0)
{
	this->parent = parent;
	tablaHijos->setFilter ("palomaID = -1");
	tablaHijos->refresh();
	tablaHijos->setReadOnly (true);
	tablaParejas->setReadOnly (true);
	popup = new KPopupMenu (tablaHijos, "popup");
	popupParejas = new KPopupMenu (tablaParejas, "popupParejas");
	estadoPopup = new KPopupMenu (tablaHijos, "estadoPopup");
	setupActions ();
	botonNuevoHijo->setEnabled(false);
	botonBorrarPareja->setEnabled(false);

	tablaHijos->setColumnWidth(0, config().HAnyo);
	tablaHijos->setColumnWidth(1, config().HAnilla);
	tablaHijos->setColumnWidth(2, config().HNacion);
	tablaHijos->setColumnWidth(3, config().HSexo);
	tablaHijos->setColumnWidth(4, config().HPlumaje);
	tablaHijos->setColumnWidth(5, config().HOjo);
	tablaHijos->setColumnWidth(6, config().HEstado);
	tablaHijos->setColumnWidth(7, config().HNombre);
	tablaHijos->setColumnWidth(8, config().HMadre);
	tablaHijos->setColumnWidth(9, config().HPadre);
	tablaHijos->refresh(QDataTable::RefreshColumns);
	tablaHijos->setEnabled(false);
	
	tablaParejas->setColumnWidth(0, config().PNombre);
	tablaParejas->setColumnWidth(1, config().PMadre);
	tablaParejas->setColumnWidth(2, config().PPadre);
	tablaParejas->setColumnWidth(3, config().PNota);
	tablaParejas->refresh(QDataTable::RefreshColumns);
	tablaHijos->setNullText(QString::null);
	tablaParejas->setNullText(QString::null);
	
	splitterParejas->setSizes (config().SplitterParejas);
    
    connect( tablaHijos, SIGNAL( selectionChanged() ), this, SLOT( updateSeleccion() ) );
    connect( tablaHijos, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contexTablaHijos(int,int,const QPoint&) ) );
}


parejas::~parejas()
{
	config().HAnyo = tablaHijos->columnWidth(0);
	config().HAnilla = tablaHijos->columnWidth(1);
	config().HNacion = tablaHijos->columnWidth(2);
	config().HSexo = tablaHijos->columnWidth(3);
	config().HPlumaje = tablaHijos->columnWidth(4);
	config().HOjo = tablaHijos->columnWidth(5);
	config().HEstado = tablaHijos->columnWidth(6);
	config().HNombre = tablaHijos->columnWidth(7);
	config().HMadre = tablaHijos->columnWidth(8);
	config().HPadre = tablaHijos->columnWidth(9);
	
	config().PNombre = tablaParejas->columnWidth(0);
	config().PMadre = tablaParejas->columnWidth(1);
	config().PPadre = tablaParejas->columnWidth(2);
	config().PNota = tablaParejas->columnWidth(3);
	config().SplitterParejas = splitterParejas->sizes ();
	
	config().write();
}

/**
* Este slot se llama cuando seleccionamos una pareja nueva en la tabla de las parejas
* Básicamente cambia el filtro de la tabla hijos para que muestre los hijos de la pareja seleccionada. Y habilita los botones.
*/
void parejas::parejaChange() {
	int numHembras = 0, numMachos = 0;

	if (tablaParejas->currentRecord())
	{
		botonNuevoHijo->setEnabled(true);
		botonBorrarPareja->setEnabled(true);
		tablaHijos->setEnabled(true);
	
		QString consulta = " palomaID in (select palomaID from paloma where \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\"))";
		tablaHijos->setFilter(consulta);

		/* Número de hijos machos en vuelo... */
		consulta = "select count(anilla) from paloma where sexo = 0 AND estado = \"V\" AND \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\")";
		QSqlQuery query3 (consulta, QSqlDatabase::database("palomar" ));
		if (query3.next()) {
			MVue->setText("    En vuelo: " + query3.value(0).toString());
			numMachos = query3.value(0).toInt();
		}
	
		/* Número de hijos machos en desaparecidos... */
		consulta = "select count(anilla) from paloma where sexo = 0 AND estado = \"D\" AND \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\")";
		QSqlQuery query4 (consulta, QSqlDatabase::database("palomar" ));
		if (query4.next()) {
			MDes->setText("    Desaparecidos: " + query4.value(0).toString());
			numMachos += query4.value(0).toInt();
		}
	
		/* Número de hijos machos en reproduccion... */
		consulta = "select count(anilla) from paloma where sexo = 0 AND estado = \"R\" AND \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\")";
		QSqlQuery query5 (consulta, QSqlDatabase::database("palomar" ));
		if (query5.next()) {
			MRep->setText("    Reproduccion: " + query5.value(0).toString());
			numMachos += query5.value(0).toInt();
		}

		/* Número de hijos hembra en vuelo... */
		consulta = "select count(anilla) from paloma where sexo = 1 AND estado = \"V\" AND \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\")";
		QSqlQuery query7 (consulta, QSqlDatabase::database("palomar" ));
		if (query7.next()) {
			HVue->setText("    En vuelo: " + query7.value(0).toString());
			numHembras = query7.value(0).toInt();
		}
	
		/* Número de hijos hembra en reproduccion... */
		consulta = "select count(anilla) from paloma where sexo = 1 AND estado = \"R\" AND \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\")";
		QSqlQuery query8 (consulta, QSqlDatabase::database("palomar" ));
		if (query8.next()) {
			HRep->setText("    Reproduccion: " + query8.value(0).toString());
			numHembras += query8.value(0).toInt();
		}
	
		/* Número de hijos hembra en desaparecidas... */
		consulta = "select count(anilla) from paloma where sexo = 1 AND estado = \"D\" AND \
			madreID =(\
				select palomaID from idCompacto where \
					idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
			) AND padreID = (\
				select palomaID from idCompacto where \
					idCompacto = \"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\")";
		QSqlQuery query9 (consulta, QSqlDatabase::database("palomar" ));
		if (query9.next()) {
			HDes->setText("    Desaparecidas: " + query9.value(0).toString());
			numHembras += query9.value(0).toInt();
		}
		hijos->setText(" <b>Número de hijos de la pareja: " + QString::number(numHembras + numMachos) + "</b>");
		Hembras->setText("Hembras:" + QString::number(numHembras));
		Machos->setText("Machos:" + QString::number(numMachos));
		tablaHijos->refresh();
	}
	else
	{
		botonNuevoHijo->setEnabled(false);
		botonBorrarPareja->setEnabled(false);
		tablaHijos->setEnabled(false);
		tablaHijos->setFilter ("palomaID = -1");
		tablaHijos->refresh();
		HDes->setText("    Desaparecidas: ");
		HRep->setText("    Reproduccion: ");
		HVue->setText("    En vuelo: ");
		Hembras->setText("Hembras:");
		MRep->setText("    Reproduccion: ");
		MDes->setText("    Desaparecidos: ");
		MVue->setText("    En vuelo: ");
		Machos->setText("Machos:");
		hijos->setText(" <b>Número de hijos de la pareja: </b>");
	}
}

void parejas::nuevaPareja()
{
	nuevaParejaDlg dlg;

	/* Hembras en vuelo o en reproducción ... */
	QSqlQuery queryMadres ("SELECT  idCompacto FROM idCompacto WHERE \
		palomaID in (\
		SELECT palomaID FROM paloma WHERE \
			sexo = 1 and palomaID != 1 and palomaID != 2\
			and (estado = \"v\" or estado = \"R\") \
			ORDER BY paloma.anyo DESC, paloma.anilla);", QSqlDatabase::database("palomar" ));
	if (queryMadres.isActive()) {
		while( queryMadres.next() )
		dlg.comboMadre->insertItem (queryMadres.value(0).toString());
	}

	/* Machos en vuelo o en reproducción ...*/
	QSqlQuery queryPadres ("SELECT  idCompacto FROM idCompacto WHERE \
		palomaID in (\
		SELECT palomaID FROM paloma WHERE \
		sexo = 0 and palomaID != 1 and palomaID != 2\
		and (estado = \"v\" or estado = \"R\") \
		ORDER BY paloma.anyo DESC, paloma.anilla);", QSqlDatabase::database("palomar" ));
	if (queryPadres.isActive()) {
		while( queryPadres.next() )
		dlg.comboPadre->insertItem (queryPadres.value(0).toString());
	}


	if (dlg.exec() == QDialog::Accepted) {
		QString consulta = "INSERT INTO parejas (nombre, madreID, padreID, nota) VALUES (\"" + dlg.nombre->text()  + "\", (\
			SELECT palomaID FROM idCompacto WHERE \
			idCompacto = \"" + dlg.comboMadre->currentText() + "\"), (\
			SELECT palomaID FROM idCompacto WHERE \
			idCompacto = \"" + dlg.comboPadre->currentText() + "\"), \"" + dlg.nota->text() + "\")";
		QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
		QSqlError error = queryInsert.lastError();
		if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error añadiendo actualizando paloma"));
		else
			tablaParejas->refresh();
	}
}

void parejas::nuevoHijo () {
	if (tablaParejas->currentRecord()) {
		nuevoHijoDlg dlg;

		QDateTime reloj;
		QDate date(reloj.currentDateTime().date());
		dlg.anyo->setValue(date.year());
		dlg.numeroAnilla->setValue (config().ultimaAnilla);
		/* Colores pluma... */
		QSqlQuery queryPlumaje ("SELECT  nombre FROM plumaje", QSqlDatabase::database("palomar" ));
		if (queryPlumaje.isActive()) {
			while( queryPlumaje.next() )
			dlg.comboPlumaje->insertItem (queryPlumaje.value(0).toString());
		}

		/* Colores ojo... */
		QSqlQuery queryOjo ("SELECT  nombre FROM ojo", QSqlDatabase::database("palomar" ));
		if (queryOjo.isActive()) {
			while( queryOjo.next() )
			dlg.comboOjo->insertItem (queryOjo.value(0).toString());
		}

		/* Nacionalidades... */
		QSqlQuery queryNacion ("SELECT iniciales FROM nacionalidad", QSqlDatabase::database("palomar" ));
		if (queryNacion.isActive()) {
			while( queryNacion.next() )
			dlg.comboNacion->insertItem (queryNacion.value(0).toString());
		}

		if (dlg.exec() == QDialog::Accepted) {
			QString sexo;
			QString estado;
			if (dlg.macho->isChecked())
				sexo = "0";
			else
				sexo = "1";
			if (dlg.reproduccion->isChecked())
				estado = "R";
			else {
				if (dlg.vuelo->isChecked())
					estado = "V";
				else
					estado = "D";
			}
			QString consulta = "INSERT INTO paloma (anyo, anilla, nacionalidadID, sexo, madreID, padreID, plumajeID, ojoID, estado, nombre, nota) \
				VALUES (" + dlg.anyo->text() +", " + dlg.numeroAnilla->text() + ",(\
					SELECT nacionalidadID \
					FROM nacionalidad \
					WHERE iniciales = \"" + dlg.comboNacion->currentText() +"\"\
				), " + sexo + ", (\
					SELECT palomaID \
					FROM idCompacto \
					WHERE idCompacto =\"" +  tablaParejas->currentRecord()->value(indiceMadre).toString() + "\"\
				), (\
					SELECT palomaID \
					FROM idCompacto \
					WHERE idCompacto =\"" +  tablaParejas->currentRecord()->value(indicePadre).toString() + "\"\
				), (\
					SELECT plumajeID \
					FROM plumaje \
					WHERE nombre = \"" + dlg.comboPlumaje->currentText() +"\"\
				), (\
					SELECT ojoID \
					FROM ojo \
					WHERE nombre = \"" + dlg.comboOjo->currentText() +"\"\
				), \"" + estado + "\", \"" + dlg.nombre->text()  + "\", \"" + dlg.nota->text() + "\")";
			qWarning (consulta);
			QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
			QSqlError error = queryInsert.lastError();
			if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
					i18n ("Error añadiendo actualizando paloma"));
			else {
				config().updateAnilla(dlg.numeroAnilla->value());
				tablaHijos->refresh();
			}

		}
	}
	else {
			KMessageBox::error (this, i18n("Previamente debe seleccionar una pareja."),
					i18n ("Error añadiendo nuevo hijo"));
	}
}


void parejas::updateSeleccion () {
	if (tablaHijos->currentRecord())
		config().anillaSeleccion = tablaHijos->currentRecord()->value(indicePalomaID).toInt();
}


void parejas::showEvent( QShowEvent *e )
{
	tablaHijos->refresh();
	tablaParejas->refresh();
}

void parejas::setupActions () {

// 	KAction *verReproduccionAction = new KAction(i18n("Ver reproduccion"), 0,
// 					this, SLOT(verReproduccion()),
// 					this, "verReproducionAction");
// 	verReproduccionAction->plug(verPopup);
//
// 	KAction *verVueloAction = new KAction(i18n("Ver vuelo"), 0,
// 					this, SLOT(verVuelo()),
// 					this,  "verVueloAction");
// 	verVueloAction->plug(verPopup);
//
// 	KAction *verDesaparecidaAction = new KAction(i18n("Ver desaparecidas"), 0,
// 					this, SLOT(verDesaparecida()),
// 					this, "verDesaparecidaAction");
// 	verDesaparecidaAction->plug(verPopup);
//
// 	KAction *verTodasAction = new KAction(i18n("Ver todas"), 0,
// 					this, SLOT(verTodas()),
// 					this, "verTodasAction");
// 	verTodasAction->plug(verPopup);

	KAction *cambioSexoAction = new KAction(i18n("Cambiar de sexo"), 0,
					tablaHijos, SLOT(cambioSexo()),
					this, "cambioSexoAction");

	KAction *estadoDesaparecidaAction = new KAction(i18n("Desaparecida"), 0,
					tablaHijos, SLOT(estadoDesaparecida()),
					this, "estadoDesaparecidaAction");
	estadoDesaparecidaAction->plug(estadoPopup);

	KAction *estadoVueloAction = new KAction(i18n("Vuelo"), 0,
					tablaHijos, SLOT(estadoVuelo()),
					this, "estadoVueloAction");
	estadoVueloAction->plug(estadoPopup);

	KAction *estadoReproduccionAction = new KAction(i18n("Reproduccion"), 0,
					tablaHijos, SLOT(estadoReproduccion()),
					this, "estadoReproduccionAction");
	estadoReproduccionAction->plug(estadoPopup);


	KAction *eliminarPalomaAction = new KAction(i18n("Eliminar paloma"), "button_cancel", CTRL+Key_D,
					tablaHijos, SLOT(eliminarPaloma()),
					this, "eliminarPalomaAction");

//	popup->setTitle (i18n("Menu tabla"));
//	popup->insertItem (KGlobal::iconLoader()->loadIcon("xeyes",KIcon::Panel,0,false), i18n("Ver"), verPopup);
	popup->setTitle (i18n("Menu paloma"));
	cambioSexoAction->plug(popup);
	popup->insertItem (KGlobal::iconLoader()->loadIcon("zoom",KIcon::Panel,0,false), i18n("Estado"), estadoPopup);
	eliminarPalomaAction->plug (popup);

//---------------------------------------- Popup de la tabla de las parejas ------------------------------------------
	KAction *eliminarParejaAction = new KAction(i18n("Eliminar pareja"), "button_cancel", CTRL+Key_D,
					this, SLOT(eliminarPareja()),
					this, "eliminarParejaAction");
	KAction *editarParejaAction = new KAction(i18n("Editar pareja"), "edit", CTRL+Key_E,
					this, SLOT(editarPareja()),
					this, "editarParejaAction");
	KAction *nuevaParejaAction = new KAction(i18n("Nueva pareja"), "kdmconfig", CTRL+Key_N,
					this, SLOT(nuevaPareja()),
					this, "nuevaParejaAction");
	KAction *nuevoHijoAction = new KAction(i18n("Nuevo hijo"), "wordprocessing", CTRL+Key_N,
					this, SLOT(nuevoHijo()),
					this, "nuevoHijoAction");

	popupParejas->setTitle (i18n("Menu parejas"));
	nuevaParejaAction->plug (popupParejas);
	editarParejaAction->plug (popupParejas);
	eliminarParejaAction->plug(popupParejas);
	popupParejas->insertSeparator();
	nuevoHijoAction->plug (popupParejas);

}


void parejas::eliminarPareja () {
	if (KMessageBox::questionYesNo( 
			this,
			i18n("¿EstÃ¡ usted seguro que desea eliminar esta pareja?")) == KMessageBox::No)
		return;
	QString consulta = "DELETE FROM parejas WHERE parejaID = " + tablaParejas->currentRecord()->value(0).toString();
	QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
	QSqlError error = queryDelete.lastError();
	if (error.type() != QSqlError::None)
			KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
				error.databaseText(),
				i18n ("Error añadiendo actualizando paloma"));
	tablaParejas->refresh();
}

void parejas::editarPareja() {
	nuevaParejaDlg dlg;

	dlg.comboMadre->insertItem (tablaParejas->currentRecord()->value(2).toString());
	dlg.comboMadre->setEnabled (false);
	dlg.comboPadre->insertItem (tablaParejas->currentRecord()->value(3).toString());
	dlg.comboPadre->setEnabled (false);
	if (!tablaParejas->currentRecord()->value(1).isNull())
		dlg.nombre->setText(tablaParejas->currentRecord()->value(1).toString());
	if (!tablaParejas->currentRecord()->value(4).isNull())
		dlg.nota->setText(tablaParejas->currentRecord()->value(4).toString());


	if (dlg.exec() == QDialog::Accepted) {
		QString consulta = "UPDATE parejas  SET nombre = \"" + dlg.nombre->text()  + "\", \
			nota = \"" + dlg.nota->text() + "\" \
			WHERE parejaID = " + tablaParejas->currentRecord()->value(0).toString();
		qWarning (consulta);
		QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
		QSqlError error = queryUpdate.lastError();
		if (error.type() != QSqlError::None)
				KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
					error.databaseText(),
						i18n ("Error añadiendo actualizando paloma"));
		else
			tablaParejas->refresh();
	}

}


void parejas::contentsContextMenuEvent( QContextMenuEvent *e )
{
        popup->exec( QCursor::pos() );
}

void parejas::contexTablaHijos (int row, int col, const QPoint & pos) {
	popup->exec( QCursor::pos() );
}

void parejas::contexTablaParejas (int row, int col, const QPoint & pos) {
	popupParejas->exec( QCursor::pos() );
}


void parejas::estadisticas ()
{
	parejasStats dlg;
	dlg.setMadre(tablaParejas->currentRecord()->value(indiceMadre).toString());
	dlg.setPadre(tablaParejas->currentRecord()->value(indicePadre).toString());
	dlg.exec();
}
