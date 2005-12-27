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
#include <qlistview.h>
#include <qpixmap.h>
#include <qdatatable.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qrect.h>
#include <qpoint.h>
#include <qsize.h>
#include <qpaintdevicemetrics.h>
#include <qcolor.h>
#include <qpen.h>
#include <qfont.h>
#include <qwmatrix.h>
#include <qdatetime.h>


#include <kprinter.h>

arbolGen::arbolGen(QWidget *parent)
: arbolGenWdg (parent, "arbolGenWdg", 0)
{
	arbolGenlistView->setRootIsDecorated (true);
	arbolGenlistView->setSelectionMode (QListView::Single);
}


arbolGen::~arbolGen()
{
	arbolGenlistView->setRootIsDecorated (true);
}

void arbolGen::addPaloma (QListViewItem *root, QString palomaID) {
	QListViewItem *paloma;

	QString consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
			listaGeneral.sexo, paloma.madreID, paloma.padreID\
			FROM idCompacto, listaGeneral, paloma\
			WHERE idCompacto.palomaID = " + palomaID + " \
			AND listaGeneral.palomaID = " + palomaID + " \
			AND paloma.palomaID = " + palomaID;
	QSqlQuery query (consulta, QSqlDatabase::database("palomar" ));
	if (query.next()) {
		paloma = new QListViewItem( root,
							 query.value(0).toString(),
							 query.value(3).toString(),
							 query.value(1).toString(),
							 query.value(2).toString()
							 );	
		madreID = query.value(4).toString();
		addPaloma (paloma, madreID);
		padreID = query.value(5).toString();
		addPaloma (paloma, padreID);
		if (query.value(3).toString() == "Macho")
			paloma->setPixmap(0, QPixmap("/home/heaven/cvs/kolombo-genealogia/macho-icono.png"));
		else
			paloma->setPixmap(0, QPixmap("/home/heaven/cvs/kolombo-genealogia/hembra-icono.png"));
	}
}

void arbolGen::loadData () {
	QListViewItem *element;

	arbolGenlistView->clear();
	palomaID = palomaID.setNum (config().anillaSeleccion);
	QString consulta = "SELECT idCompacto.idCompacto , listaGeneral.plumaje, listaGeneral.ojo, \
			listaGeneral.sexo, paloma.madreID, paloma.padreID\
			FROM idCompacto, listaGeneral, paloma\
			WHERE idCompacto.palomaID = " + palomaID + " \
			AND listaGeneral.palomaID = " + palomaID + " \
			AND paloma.palomaID = " + palomaID;
	QSqlQuery query (consulta, QSqlDatabase::database("palomar" ));
	if (query.next()) {
		element = new QListViewItem( arbolGenlistView,
							 query.value(0).toString(),
							 query.value(3).toString(),
							 query.value(1).toString(),
							 query.value(2).toString()
							 );	
		madreID = query.value(4).toString();
		addPaloma (element, madreID);
		padreID = query.value(5).toString();
		addPaloma (element, padreID);
		if (query.value(3).toString() == "Macho")
			element->setPixmap(0, QPixmap("/home/heaven/cvs/kolombo-genealogia/macho-icono.png"));
		else
			element->setPixmap(0, QPixmap("/home/heaven/cvs/kolombo-genealogia/hembra-icono.png"));
	}

}

void arbolGen::showEvent( QShowEvent *e )
{
	loadData ();
	palmaresLabel->setText (QString ("Palmares de "));
	tablaPalmares->setFilter (QString ("idCompacto = \"NINGUNO\""));
	tablaPalmares->refresh(QDataTable::RefreshAll);
}


void arbolGen::actualizarPalmares(QListViewItem *item) {
	palmaresLabel->setText (QString ("Palmares de " + item->text(0)));
	tablaPalmares->setFilter (QString ("idCompacto = \"" + item->text(0) + "\""));
	tablaPalmares->refresh(QDataTable::RefreshAll);
}

/*
 * Esta función imprime el pedigree de una paloma.
 ****************************/
void arbolGen::printPedigree (QPainter *p, KPrinter * printer) {
	//QWarning ("Imprimiendo pedigree");
	//printer->setResolution (600);
	QRect rectangulo;
	QPaintDeviceMetrics pdm (printer);
	QSize margenes;
	QPen lapiz;
	QWMatrix rotacion;
	QFont sumaFont( "Sans Serif", 18);
	QFont sansFont( "Sans Serif", 8 );
	int pageNo = 0;

	margenes = printer->margins();
	int comienzoCuadradoX = margenes.width();
	int comienzoCuadradoY = margenes.height();
	int finCuadradoX = pdm.width() - margenes.width();
	int finCuadradoY = pdm.height() / 2;

	// Marco exterior
	p->drawLine (comienzoCuadradoX, comienzoCuadradoY, finCuadradoX, comienzoCuadradoY);
	p->drawLine (comienzoCuadradoX, comienzoCuadradoY, comienzoCuadradoX, finCuadradoY);
	p->drawLine (comienzoCuadradoX, finCuadradoY, finCuadradoX, finCuadradoY);
	p->drawLine (finCuadradoX, finCuadradoY, finCuadradoX, comienzoCuadradoY);

	// Hueco por encima para la paloma en cuestión: 1/3 del ancho total.
	int parteSuperior = ((finCuadradoY - comienzoCuadradoY) / 3) + comienzoCuadradoY;
	p->drawLine (comienzoCuadradoX, parteSuperior, finCuadradoX, parteSuperior);

	// Divisiones verticales para padres, abuelos y tataraabuelos
	int anchoDivision = (finCuadradoX - comienzoCuadradoX) / 3;
	p->drawLine (comienzoCuadradoX + anchoDivision, parteSuperior, comienzoCuadradoX + anchoDivision, finCuadradoY);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior, comienzoCuadradoX + 2 * anchoDivision, finCuadradoY);

	// División horizontal para padres
	int divisionHorizPadres = 2 * ((finCuadradoY - comienzoCuadradoY) / 3) + comienzoCuadradoY;
	p->drawLine (comienzoCuadradoX, divisionHorizPadres, comienzoCuadradoX + anchoDivision, divisionHorizPadres);

	// División horizontal para abuelos
	int divisionHorizAbuelos = ((finCuadradoY - comienzoCuadradoY) / 3) / 2;
	p->drawLine (comienzoCuadradoX + anchoDivision, parteSuperior + divisionHorizAbuelos,
						 comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizAbuelos);
	p->drawLine (comienzoCuadradoX + anchoDivision, parteSuperior + divisionHorizAbuelos * 2,
						 comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizAbuelos * 2);
	p->drawLine (comienzoCuadradoX + anchoDivision, parteSuperior + divisionHorizAbuelos * 3,
						 comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizAbuelos * 3);

	// División horizontal para Tatarabuelos
	int divisionHorizTatarabuelos = ((finCuadradoY - comienzoCuadradoY) / 3) / 4;
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 2,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 2);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 3,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 3);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 4,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 4);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 5,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 5);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 6,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 6);
	p->drawLine (comienzoCuadradoX + 2 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 7,
						 comienzoCuadradoX + 3 * anchoDivision, parteSuperior + divisionHorizTatarabuelos * 7);

	// Texto de la parte superior:

	rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX, comienzoCuadradoY));
	rectangulo.setBottomRight(QPoint::QPoint (2 * anchoDivision, parteSuperior * 0.9));
//	p->setFont(sumaFont);
	if (arbolGenlistView->firstChild ()) {
		QListViewItem *paloma = arbolGenlistView->firstChild ();
		QString texto = QString("Criador: " + config().nombreP + "\n" +
							 config().calleP + " Nº: " + config().numeroP + ", " +
							 config().poblacionP + ", " + config().provinciaP + "\n\nAnilla: " +
							 paloma->text(0) + "\nSexo: " + paloma->text(1) + "\nPlumaje: " +
							 paloma->text(2) + "\nOjo: " + paloma->text(3));
		p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
		rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX, parteSuperior * 0.9));
		rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + anchoDivision, parteSuperior));
		p->fillRect(rectangulo, QBrush::QBrush(Qt::black));
		p->setPen(QPen::QPen(Qt::white));
		p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignBottom, "Padres");
		rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + anchoDivision, parteSuperior * 0.9));
		rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision, parteSuperior));
		p->fillRect(rectangulo, QBrush::QBrush(Qt::black));
		p->setPen(QPen::QPen(Qt::white));
		p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignBottom, "Abuelos");
		rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision, parteSuperior * 0.9));
		rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision, parteSuperior));
		p->fillRect(rectangulo, QBrush::QBrush(Qt::black));
		p->setPen(QPen::QPen(Qt::white));
		p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignBottom, "Bisabuelos");
		p->setPen(QPen::QPen(Qt::black));
		paloma = paloma->firstChild ();
		int offset;
		if (paloma) { // Madre o padre
			do {
				texto = "Anilla: " + paloma->text(0) + "\nSexo: " + paloma->text(1) +
						  "\nPlumaje: " + paloma->text(2) + "\nOjo: " + paloma->text(3);
				if (paloma->text(1) == "Hembra") { //Madre
					rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX, parteSuperior));
					rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + anchoDivision, divisionHorizPadres));
					p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
					QListViewItem *abuelo = paloma->firstChild ();
					if (abuelo) { // Existe alguno de los abuelos maternos
						do {
							texto = abuelo->text(0) + " - " + abuelo->text(1) +
							  "\nPlumaje: " + abuelo->text(2) + "\nOjo: " + abuelo->text(3);
							if (abuelo->text(1) == "Hembra") { //Abuela Materna
								rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + anchoDivision, parteSuperior));
								rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																	parteSuperior + divisionHorizAbuelos));
								p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
								QListViewItem *tatarabuelo = abuelo->firstChild ();
								if (tatarabuelo) {
									do {
										texto = tatarabuelo->text(0);
										if (tatarabuelo->text(1) == "Hembra") { //Tatarabuela Materna
											offset = -1;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										} else {
											offset = 0;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										}
									} while (tatarabuelo = tatarabuelo->nextSibling ());
								}
							} else { //Abuelo Materno
								rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + anchoDivision,
																	parteSuperior + divisionHorizAbuelos));
								rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																	parteSuperior + 2 * divisionHorizAbuelos));
								p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
								QListViewItem *tatarabuelo = abuelo->firstChild ();
								if (tatarabuelo) {
									do {
										texto = tatarabuelo->text(0);
										if (tatarabuelo->text(1) == "Hembra") { //Tatarabuela Materna
											offset = 1;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										} else {
											offset = 3;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										}
									} while (tatarabuelo = tatarabuelo->nextSibling ());
								}

							}
						} while (abuelo = abuelo->nextSibling());
					}
				} else {// Padre
					rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX, divisionHorizPadres));
					rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + anchoDivision, finCuadradoY));
					p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
					QListViewItem *abuelo = paloma->firstChild ();
					if (abuelo) { // Existe alguno de los abuelos paternos
						do {
							texto = abuelo->text(0) + " - " + abuelo->text(1) +
							  "\nPlumaje: " + abuelo->text(2) + "\nOjo: " + abuelo->text(3);
							if (abuelo->text(1) == "Hembra") { //Abuela Paterna
								rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + anchoDivision,
																	parteSuperior + 2 * divisionHorizAbuelos));
								rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																	parteSuperior + 3 * divisionHorizAbuelos));
								p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
								QListViewItem *tatarabuelo = abuelo->firstChild ();
								if (tatarabuelo) { // Tatarabuelos por parte de la abuela materna
									do {
										texto = tatarabuelo->text(0);
										if (tatarabuelo->text(1) == "Hembra") {
											offset = 3;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										} else {
											offset = 4;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										}
									} while (tatarabuelo = tatarabuelo->nextSibling ());
								}

							} else { //Abuelo Paterno
								rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + anchoDivision,
																	parteSuperior + 3 * divisionHorizAbuelos));
								rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																	parteSuperior + 4 * divisionHorizAbuelos));
								p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
								QListViewItem *tatarabuelo = abuelo->firstChild ();
								if (tatarabuelo) {
									do {
										texto = tatarabuelo->text(0);
										if (tatarabuelo->text(1) == "Hembra") { //Tatarabuela Materna
											offset = 5;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										} else {
											offset = 6;
											rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX + 2 * anchoDivision,
																				parteSuperior + (1 + offset) * divisionHorizTatarabuelos));
											rectangulo.setBottomRight(QPoint::QPoint (comienzoCuadradoX + 3 * anchoDivision,
																		parteSuperior + (2 + offset) * divisionHorizTatarabuelos));
											p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, texto);
										}
									} while (tatarabuelo = tatarabuelo->nextSibling ());
								}
							}
						} while (abuelo = abuelo->nextSibling());
					}
				}
			} while (paloma = paloma->nextSibling());
		}
	}
	//p->setFont(sansFont);
	int pieY = pdm.height() - margenes.height() - parteSuperior * 0.1;
	rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX, pieY));
	rectangulo.setBottomRight(QPoint::QPoint (finCuadradoX, pieY + parteSuperior * 0.1));
	p->fillRect(rectangulo, QBrush::QBrush(Qt::gray));
	p->setPen(QPen::QPen(Qt::white));
	QString texto = QDate::currentDate().toString("yyyy-MM-dd");
	p->drawText(rectangulo, Qt::AlignRight | Qt::AlignVCenter, texto);
	p->drawText(rectangulo, Qt::AlignLeft | Qt::AlignVCenter, "http://kolombo.berlios.de/");

	printer->newPage();
	p->fillRect(rectangulo, QBrush::QBrush(Qt::gray));
	p->setPen(QPen::QPen(Qt::white));
	p->drawText(rectangulo, Qt::AlignRight | Qt::AlignVCenter, texto);
	p->drawText(rectangulo, Qt::AlignLeft | Qt::AlignVCenter, "http://kolombo.berlios.de/");

	rectangulo.setTopLeft (QPoint::QPoint (comienzoCuadradoX, comienzoCuadradoY));
	rectangulo.setBottomRight(QPoint::QPoint (finCuadradoX, comienzoCuadradoY + parteSuperior * 0.1));
	p->fillRect(rectangulo, QBrush::QBrush(Qt::black));
	p->setPen(QPen::QPen(Qt::white));
	texto = "Árbol genealógico completo.";
	p->drawText(rectangulo, Qt::AlignRight | Qt::AlignVCenter, texto);
	p->setPen(QPen::QPen(Qt::black));
	printPedigreeRecursivo (p, comienzoCuadradoX + p->fontMetrics().width(' ') * 6,
						 comienzoCuadradoY + p->fontMetrics().height() * 3, arbolGenlistView->firstChild ());
}


int arbolGen::printPedigreeRecursivo (QPainter *p, int x, int y, QListViewItem *paloma) {
	QRect rectangulo;
	QFontMetrics fm = p->fontMetrics();
	int offsetX, offsetY;
	int yInicial = y + (fm.height() / 2);
	int yFinal = y;
	offsetY = fm.height() * 1.5;
	offsetX = fm.width(' ') * 3;

	if (not paloma)
		return y;
	//Horizontal
	QString text = paloma->text(0) + " (" + paloma->text(1) + ")";
	p->drawLine (x - offsetX, yInicial, x, yInicial);
	p->drawEllipse (x - offsetX - fm.height() / 8, yInicial - fm.height() / 8, fm.height() / 4, fm.height() / 4);
	rectangulo = fm.boundingRect (text);
	rectangulo.moveTopLeft (QPoint::QPoint(x + fm.width(' '), y));
	p->drawText(rectangulo, Qt::AlignHCenter | Qt::AlignVCenter, text);
	// Vertical
	y = printPedigreeRecursivo (p, x + offsetX, y + offsetY, paloma->firstChild());
	if (yInicial + offsetY != y + (fm.height() / 2))
		p->drawLine (x, yInicial, x, yInicial + offsetY);
	while (paloma = paloma->nextSibling()) {
		yFinal = y;
		y = printPedigreeRecursivo (p, x, y, paloma);
		p->drawLine (x - offsetX, yInicial, x - offsetX, yFinal +  + (fm.height() / 2));
	}
	return y;
}
