/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "parejasstats.h"
#include "basechart.h"

#include <qcanvas.h>
#include <qgroupbox.h> 
#include <qlayout.h>
#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qvariant.h>
#include <qdatetime.h>
#include <qdialog.h>

parejasStats::parejasStats(QWidget* parent, const char* name, bool modal, WFlags fl)
: parejasStatsDlg(parent,name, modal,fl)
{

	//http://doc.trolltech.com/3.3/tutorial2-05.html
	estadisticaLayout = new QGridLayout( estadistica );
	estadisticaLayout->setAlignment( Qt::AlignTop );
	m_canvas = new baseChart( estadistica );
	m_canvasView = new QCanvasView( m_canvas, estadistica );
	estadisticaLayout->addWidget(m_canvasView, 0, 0);
	m_canvasView->show();
	m_canvas->setMaxX(32 * 5);
	m_canvas->setMaxY(20);
	m_canvas->setMargin(20, 20, 30, 25);
	m_canvas->setTitle ("Estado de la descendencia");
	m_canvas->addDataType(Qt::blue, "Total");
	m_canvas->addDataType(Qt::green, "Vuelo");
	m_canvas->addDataType(Qt::red, "Desaparecidas");
	m_canvas->addDataType(Qt::yellow, "Reproducion");
	m_canvasView->setVScrollBarMode(QScrollView::AlwaysOff);
	m_canvasView->setHScrollBarMode(QScrollView::AlwaysOff);
	m_canvas->resize(estadistica->width(), estadistica->height());
	drawElements();
	m_canvas->update();
	m_canvasView->repaint ();
	firstResize = true;
}

parejasStats::~parejasStats()
{
}


/*$SPECIALIZATION$*/
void parejasStats::barras(int offset, int vuelo, int desaparecido, int reproduccion)
{
	m_canvas->drawBar (offset * 32 + 0, 6, vuelo + desaparecido + reproduccion, Qt::blue);
	m_canvas->drawBar (offset * 32 + 6, 6, vuelo, Qt::green);
	m_canvas->drawBar (offset * 32 + 12, 6, desaparecido, Qt::red);
	m_canvas->drawBar (offset * 32 + 18, 6, reproduccion, Qt::yellow);
}

void parejasStats::drawElements()
{
	QCanvasItemList list = m_canvas->allItems();
	for ( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
		delete *it;

	m_canvas->drawTitle();
	m_canvas->drawLegend();
	m_canvas->drawMargin();
	m_canvas->drawYScale();

	QDateTime reloj;
	QDate date(reloj.currentDateTime().date());
	int vuelo, desaparecido, reproduccion;
	for (int i = 0; i < 5; i++)
	{
		getData(date.year() - 4 + i, &vuelo, &desaparecido, &reproduccion);
		barras(i, vuelo, desaparecido, reproduccion);
		m_canvas->drawXLabel(12 + i * 32, QString::number(date.year() - 4 + i));
	}
}

void parejasStats::resizeEvent( QResizeEvent *e )
{
	if (firstResize)
	{
		m_canvas->resize(estadistica->width(), estadistica->height());
		firstResize = false;
	}
	else
		m_canvas->resize(m_canvasView->visibleWidth(), m_canvasView->visibleHeight());
	drawElements();
	m_canvas->update();
	m_canvasView->repaint ();
}


void parejasStats::getData (int anyo, int *vuelo, int *desaparecido, int *reproduccion)
{
	QString consulta;
	/* Número de hijos en vuelo... */
	consulta = "select count(anilla) from paloma where anyo = " + QString::number(anyo)
		+ " AND estado = \"V\" AND \
		madreID =(\
			select palomaID from idCompacto where idCompacto =\"" + madre + "\"\
		) AND padreID = (\
			select palomaID from idCompacto where idCompacto = \"" + padre + "\")";
	QSqlQuery query3 (consulta, QSqlDatabase::database("palomar" ));
	if (query3.next())
		*vuelo = query3.value(0).toInt();

	/* Número de hijos desaparecidos... */
	consulta = "select count(anilla) from paloma where anyo = " + QString::number(anyo)
		+ " AND estado = \"D\" AND \
		madreID =(\
			select palomaID from idCompacto where idCompacto =\"" + madre + "\"\
		) AND padreID = (\
			select palomaID from idCompacto where idCompacto = \"" + padre + "\")";
	QSqlQuery query4 (consulta, QSqlDatabase::database("palomar" ));
	if (query4.next())
		*desaparecido = query4.value(0).toInt();

	/* Número de hijos en reproduccion... */
	consulta = "select count(anilla) from paloma where anyo = " + QString::number(anyo)
		+ " AND estado = \"R\" AND \
		madreID =(\
			select palomaID from idCompacto where idCompacto =\"" + madre + "\"\
		) AND padreID = (\
			select palomaID from idCompacto where idCompacto = \"" + padre + "\")";
	QSqlQuery query5 (consulta, QSqlDatabase::database("palomar" ));
	if (query5.next())
		*reproduccion = query5.value(0).toInt();
}


void parejasStats::setMadre(QString m)
{
	madre = m;
}

void parejasStats::setPadre (QString p)
{
	padre = p;
}	
#include "parejasstats.moc"

