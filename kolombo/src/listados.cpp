/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "listados.h"
#include "kbird2.h"
#include "kbird2view.h"
#include "configuration.h"
#include "generalstats.h"
#include "palomardatatable.h"

#include <qwidget.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qdatatable.h>
#include <qsqldatabase.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qvariant.h>
#include <qstring.h>
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
#include <qpixmap.h>

#include <kxmlguifactory.h>
#include <klocale.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kapplication.h>
#include <kmainwindow.h>
#include <kpopupmenu.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kglobalsettings.h>
#include <kprinter.h>

#include <math.h>

//class QTextParag;


listados::listados(QWidget *parent, KMainWindow *mainWindow)
: listadosWdg (parent)
{
	this->mainWindow = mainWindow;
	this->parent = parent;

	tablaListados->setReadOnly(true);

	popup = new KPopupMenu (tablaListados, "popup");
	verPopup = new KPopupMenu (tablaListados, "verPopup");
	estadoPopup = new KPopupMenu (tablaListados, "estadoPopup");
	setupActions ();
	tablaListados->setColumnWidth(0, config().LAnyo);
	tablaListados->setColumnWidth(1, config().LAnilla);
	tablaListados->setColumnWidth(2, config().LNacion);
	tablaListados->setColumnWidth(3, config().LSexo);
	tablaListados->setColumnWidth(4, config().LMadre);
	tablaListados->setColumnWidth(5, config().LPadre);
	tablaListados->setColumnWidth(6, config().LPlumaje);
	tablaListados->setColumnWidth(7, config().LOjo);
	tablaListados->setColumnWidth(8, config().LEstado);
	tablaListados->setColumnWidth(9, config().LNombre);
	tablaListados->refresh(QDataTable::RefreshColumns);
	tablaListados->setNullText(QString::null);
    // signals and slots connections
    connect( tablaListados, SIGNAL( selectionChanged() ), this, SLOT( updateSeleccion() ) );
    connect( tablaListados, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contextMenu(int,int,const QPoint&)));
}



listados::~listados()
{
	config().LAnyo = tablaListados->columnWidth(0);
	config().LAnilla = tablaListados->columnWidth(1);
	config().LNacion = tablaListados->columnWidth(2);
	config().LSexo = tablaListados->columnWidth(3);
	config().LPlumaje = tablaListados->columnWidth(6);
	config().LOjo = tablaListados->columnWidth(7);
	config().LEstado = tablaListados->columnWidth(8);
	config().LNombre = tablaListados->columnWidth(9);
	config().LMadre = tablaListados->columnWidth(4);
	config().LPadre = tablaListados->columnWidth(5);
	config().write();
}


void listados::setupActions () {

	KAction *verReproduccionAction = new KAction(i18n("Ver reproduccion"), 0,
					tablaListados, SLOT(verReproduccion()),
					this, "verReproducionAction");
	verReproduccionAction->plug(verPopup);

	KAction *verVueloAction = new KAction(i18n("Ver vuelo"), 0,
					tablaListados, SLOT(verVuelo()),
					this,  "verVueloAction");
	verVueloAction->plug(verPopup);

	KAction *verDesaparecidaAction = new KAction(i18n("Ver desaparecidas"), 0,
					tablaListados, SLOT(verDesaparecida()),
					this, "verDesaparecidaAction");
	verDesaparecidaAction->plug(verPopup);

	KAction *verTodasAction = new KAction(i18n("Ver todas"), 0,
					tablaListados, SLOT(verTodas()),
					this, "verTodasAction");
	verTodasAction->plug(verPopup);

	KAction *verCensoAction = new KAction(i18n("Ver censo"), 0,
					tablaListados, SLOT(verCenso()),
					this, "verCensoAction");
	verCensoAction->plug(verPopup);
    
    KAction *verHermanosMadreAction = new KAction(i18n("Ver hermanos de madre"), 0,
                    tablaListados, SLOT(verHermanosMadre()),
                    this, "verHermanosMadreAction");
    verHermanosMadreAction->plug(verPopup);
    
    KAction *verHermanosPadreAction = new KAction(i18n("Ver hermanos de padre"), 0,
                    tablaListados, SLOT(verHermanosPadre()),
                    this, "verHermanosPadreAction");
    verHermanosPadreAction->plug(verPopup);
    
    KAction *verHermanosMadreYPadreAction = new KAction(i18n("Ver hermanos de madre y padre"), 0,
                    tablaListados, SLOT(verHermanosMadreYPadre()),
                    this, "verHermanosMadreYPadreAction");
    verHermanosMadreYPadreAction->plug(verPopup);
    
	KAction *cambioSexoAction = new KAction(i18n("Cambiar de sexo"), 
					"reload",
					CTRL+Key_S,
					tablaListados, SLOT(cambioSexo()),
					this, "cambioSexoAction");

	KAction *estadoDesaparecidaAction = new KAction(i18n("Desaparecida"), 0,
					tablaListados, SLOT(estadoDesaparecida()),
					this, "estadoDesaparecidaAction");
	estadoDesaparecidaAction->plug(estadoPopup);

	KAction *estadoVueloAction = new KAction(i18n("Vuelo"), 0,
					tablaListados, SLOT(estadoVuelo()),
					this, "estadoVueloAction");
	estadoVueloAction->plug(estadoPopup);

	KAction *estadoReproduccionAction = new KAction(i18n("Reproduccion"), 0,
					tablaListados, SLOT(estadoReproduccion()),
					this, "estadoReproduccionAction");
	estadoReproduccionAction->plug(estadoPopup);


	KAction *crearParejaAction = new KAction(i18n("Crear pareja con padres"), 
					"kdmconfig",
					CTRL+Key_P,
					tablaListados, SLOT(crearPareja()),
					this, "crearParejaAction");


	KAction *eliminarPalomaAction = new KAction(i18n("Eliminar paloma"),
					"button_cancel",
					CTRL+Key_D,
					tablaListados, SLOT(eliminarPaloma()),
					this, "eliminarPalomaAction");

	popup->setTitle (i18n("Menu tabla"));
	popup->insertItem (KGlobal::iconLoader()->loadIcon("xeyes",KIcon::Panel,0,false), i18n("Ver"), verPopup);
	popup->setTitle (i18n("Menu paloma"));
	crearParejaAction->plug (popup);
	cambioSexoAction->plug(popup);
	popup->insertItem (KGlobal::iconLoader()->loadIcon("zoom",KIcon::Panel,0,false), i18n("Estado"), estadoPopup);
	eliminarPalomaAction->plug (popup);

}

void listados::contentsContextMenuEvent( QContextMenuEvent * )
    {
        popup->exec( QCursor::pos() );
    }

// PopUp para la tabla de lista general...
void listados::contextMenu (int row, int col, const QPoint & pos) {
	//QWidget *w = ((kbird2*) kapp->mainWidget())->factory()->container ("context_popup", (kbird2*) kapp->mainWidget(),true);
	/*if (mainWindow)
		qWarning ("Por lo menos no es null....");
	QPopupMenu *popup = static_cast<QPopupMenu*>(mainWindow->factory()->container("contextoLista", mainWindow));
	qWarning ("A esto si que llega ...");*/
	popup->exec (QCursor::pos());
}

void listados::showEvent( QShowEvent *e )
{
	tablaListados->refresh();
}

void listados::updateSeleccion () {
	if ((tablaListados->numRows() > 0) and (tablaListados->currentSelection () != -1)) {
		config().anillaSeleccion = tablaListados->currentRecord()->value(indicePalomaID).toInt();
//		qWarning ("Seleccionado: " + config().anillaSeleccion);
	}
	
}

int listados::printBase (QPainter *p, KPrinter *printer, int pageNo) {
	QRect rectangulo;
	QPaintDeviceMetrics pdm (printer);
	QSize margenes;
	QPen lapiz;
	QWMatrix rotacion;
	int divisiones = 0;
	int FinDelTexto;
	QFont sansFont( "Arial", 8 );

	margenes = printer->margins();

	// margenes de la página...
	/*p->drawLine (margenes.width(), pdm.height() - margenes.height(), pdm.width() - margenes.width(), pdm.height() - margenes.height());
	p->drawLine (margenes.width(), margenes.height(), pdm.width() - margenes.width(), margenes.height());
	p->drawLine (margenes.width(), margenes.height(), margenes.width(), pdm.height() - margenes.height());
	p->drawLine (pdm.width() - margenes.width(), margenes.height(), pdm.width() - margenes.width(), pdm.height() - margenes.height());

	/** Trama */
	/*lapiz.setColor(Qt::blue);
	lapiz.setStyle (Qt::DotLine);
	p->setPen(lapiz);
	for (int i = margenes.width(); i < pdm.width() - margenes.width(); i += 10) {
		if (!(i % 100)) {
			lapiz.setStyle (Qt::SolidLine);;
			p->setPen(lapiz);
		}
		p->drawLine (i, 140, i, pdm.height() - margenes.height());
		if (!(i % 100)) {
			lapiz.setStyle (Qt::DotLine);
			p->setPen(lapiz);
		}

	}

	for (int i = margenes.height(); i < pdm.height() - margenes.height(); i += 10) {
		if (!(i % 100)) {
			lapiz.setStyle (Qt::SolidLine);
			p->setPen(lapiz);
		}
		p->drawLine (margenes.width(), i,pdm.width() - margenes.width(), i);
		if (!(i % 100)) {
			lapiz.setStyle (Qt::DotLine);
			p->setPen(lapiz);
		}
	}
	*/


	/** Encabezado de la tabla ... */
	int abajo = pdm.height() - margenes.height();
	if (pageNo % 2) {
		/***** ENCABEZADO ***/
		p->setFont (KGlobalSettings::generalFont());
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width(), margenes.height() - 10));
		rectangulo.setBottomRight(QPoint::QPoint (margenes.width() + (62 / 4) * 3, margenes.height() + (78 / 4) * 3 - 10));
		p->drawPixmap(rectangulo, QPixmap::QPixmap ("/home/heaven/cvs/kbird2/src/fede.png"));
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width() + 10, 0));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), margenes.height()));
		p->drawText( rectangulo, Qt::AlignRight | Qt::AlignVCenter | Qt::WordBreak, "HOJA NUM: " + QString::number(ceil (pageNo / 2)));
		sansFont.setUnderline(true);
		p->setFont (sansFont);
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width() + (62 / 4) * 3 + 10, margenes.height()));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->drawText (rectangulo, Qt::WordBreak | Qt::AlignJustify, "REAL FEDERACION COLOMBOFILA ESPAÑOLA");
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width() + (62 / 4) * 3, margenes.height()));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->drawText (rectangulo, Qt::WordBreak | Qt::AlignRight, "Censo de palomas mensajeras. Año 2003");
		sansFont.setUnderline(false);
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width(), margenes.height() - 10));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->setFont (KGlobalSettings::generalFont());
		p->drawText( rectangulo, Qt::WordBreak | Qt::AlignJustify | Qt::AlignTop,
					"\n\n         Declaración que presenta el socio " + config().nombreP + " con D.N.I. " + config().dniP + " y licencia nº " + config().licenciaP + " perteneciente a " + config().clubP + " de las palomas de su propiedad existentes en su palomar, sito en " + config().poblacionP + " (" + config().provinciaP + "), calle " + config().calleP + " número " + config().numeroP + ".");
		FinDelTexto = 140;
	} else {
		QDateTime reloj;
		QDate date(reloj.currentDateTime().date());
		abajo -= 100;
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width(), abajo));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), abajo + 100));
		p->setFont (KGlobalSettings::generalFont());
		p->drawText( rectangulo, Qt::AlignRight | Qt::AlignVCenter | Qt::WordBreak,
				"Adeje a " + QString::number(date.day()) + " de " + date.longMonthName(date.month()) + " de " + QString::number(date.year()) + "\nEl propietario.");
		FinDelTexto = margenes.height();

	}
	/** Encabezado de la tabla ... */
	int izquierda = margenes.width(), centro = 0;

	lapiz.setWidth (1);
	p->setPen (lapiz);
	p->drawLine (izquierda - 2, FinDelTexto - 2, pdm.width() -  margenes.width() + 1, FinDelTexto- 2);
	p->drawLine (izquierda - 2, FinDelTexto - 2, izquierda - 2, abajo);
	p->drawLine (pdm.width() -  margenes.width() + 1, FinDelTexto - 2, pdm.width() -  margenes.width() + 1, abajo);

	for (int j = 0; j < 2; j++) {
		if (centro) {
			izquierda = centro + 1;
			centro +=  ((pdm.width() - 2 * margenes.width()) / 2);
		}
		else
			centro = margenes.width() + ((pdm.width() - 2 * margenes.width()) / 2) - 1;

		lapiz.setColor (Qt::black);
		lapiz.setWidth (0);
		lapiz.setStyle (Qt::SolidLine);
		p->setPen (lapiz);
		p->drawLine (izquierda, FinDelTexto, centro, FinDelTexto);
		p->drawLine (izquierda, FinDelTexto, izquierda, abajo);
		p->drawLine (centro, FinDelTexto, centro, abajo);

		/** Texto **/
		p->setFont (sansFont);
		rectangulo.setTopLeft (QPoint::QPoint (izquierda, FinDelTexto));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda +170, FinDelTexto + 30));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "DETALLE DE LA ANILLA");

		rectangulo.setTopLeft (QPoint::QPoint (izquierda, FinDelTexto + 30));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + 50, FinDelTexto + 50));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "Iniciales");

		rectangulo.setTopLeft (QPoint::QPoint (izquierda + 50, FinDelTexto + 30));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + 140, FinDelTexto + 50));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "Número");

		rectangulo.setTopLeft (QPoint::QPoint (izquierda + 140, FinDelTexto + 30));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + 170, FinDelTexto + 50));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "Año");

		QFont sexFont( "Arial", 6 );
		p->setFont (sexFont);
		rotacion.rotate (90);
		p->setWorldMatrix( rotacion );
		rectangulo.setTopLeft (QPoint::QPoint (FinDelTexto, -(izquierda + 185)));
		rectangulo.setBottomRight(QPoint::QPoint (FinDelTexto + 50, -(izquierda + 172)));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::BreakAnywhere,  "SEXO");
		rotacion.rotate (-90);
		p->setWorldMatrix( rotacion );
		p->setFont (sansFont);

		rectangulo.setTopLeft (QPoint::QPoint (izquierda + 185,  FinDelTexto));
		rectangulo.setBottomRight(QPoint::QPoint (centro, FinDelTexto + 50));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "C O L O R");
		/** Horizontales **/
		p->drawLine (izquierda, FinDelTexto + 30, izquierda + 170, FinDelTexto + 30);
		p->drawLine (izquierda, FinDelTexto + 50, izquierda + 170, FinDelTexto + 50);
		p->drawLine (izquierda + 172, FinDelTexto + 50, centro, FinDelTexto + 50);

		/** Verticales **/
		p->drawLine (izquierda + 170, FinDelTexto, izquierda + 170, abajo);
		p->drawLine (izquierda + 172, FinDelTexto, izquierda + 172, abajo);
		p->drawLine (izquierda + 185, FinDelTexto, izquierda + 185, abajo);
		p->drawLine (izquierda + 50, FinDelTexto + 30, izquierda + 50, abajo);
		p->drawLine (izquierda + 140, FinDelTexto + 30, izquierda + 140, abajo);

		/** Suma anterior ... **/
		if ((pageNo != 1)||(j == 1)) {
			lapiz.setColor(Qt::white);
			lapiz.setWidth (3);
			p->setPen (lapiz);
			p->drawLine (izquierda + 140, FinDelTexto + 70, izquierda + 140, FinDelTexto + 100);
			lapiz.setColor(Qt::black);
			lapiz.setStyle (Qt::DotLine);
			lapiz.setWidth (0);
			p->setPen (lapiz);
			p->drawLine (izquierda + 172, FinDelTexto + 90, centro, FinDelTexto + 90);
			rectangulo.setTopLeft (QPoint::QPoint (izquierda + 70, FinDelTexto + 80));
			rectangulo.setBottomRight(QPoint::QPoint (izquierda + 168, FinDelTexto + 93));
			p->drawText( rectangulo,  Qt::AlignRight | Qt::AlignBottom | Qt::BreakAnywhere,  "Suma anterior . . . .");
		}

		/** Horizontales punteadas */
		lapiz.setStyle (Qt::DotLine);
		p->setPen (lapiz);
		int i;
		for (i = FinDelTexto + 110; i < abajo - 40; i += 20) {
			p->drawLine (izquierda, i, centro, i);
			divisiones++;
		}

		/** Suma y sigue ... **/
		lapiz.setColor(Qt::white);
		lapiz.setStyle (Qt::SolidLine);
		lapiz.setWidth (3);
		p->setPen (lapiz);
		p->drawLine (izquierda + 140, i, izquierda + 140, i + 30);
		lapiz.setColor(Qt::black);
		lapiz.setStyle (Qt::DotLine);
		lapiz.setWidth (0);
		p->setPen (lapiz);
		p->drawLine (izquierda + 172, i + 20, centro, i + 20);
		rectangulo.setTopLeft (QPoint::QPoint (izquierda + 70, i));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + 168, i + 23));
		p->drawText( rectangulo,  Qt::AlignRight | Qt::AlignBottom | Qt::BreakAnywhere,  "Suma y sigue . . . .");
	}
	return (divisiones / 2);
}

/**
* Esta función imprime el listado
****************************/
void listados::print (QPainter *p, KPrinter * printer) {
	QRect rectangulo;
	QPaintDeviceMetrics pdm (printer);
	QSize margenes;
	QPen lapiz;
	QWMatrix rotacion;
	QFont sumaFont( "Arial", 18);
	QFont sansFont( "Arial", 8 );
	int pageNo = 0;

	margenes = printer->margins();

	int FinDelTexto;
	int izquierda;// = margenes.width(), centro = 0;
//	int abajo;// = pdm.height() - margenes.height();
	int y;
	int palomasImpresas = 0;
	int centro = 0;

	while (true) {
		pageNo++;
		int divisiones = printBase (p, printer, pageNo);
		if (pageNo % 2)
			FinDelTexto = 140;
		else
			FinDelTexto = margenes.height();
		izquierda = margenes.width();
		centro = margenes.width() + ((pdm.width() - 2 * margenes.width()) / 2) - 1;
		for (int j = 0; j < 2; j++) {
			if (j) {
				izquierda = centro + 1;
				centro +=  ((pdm.width() - 2 * margenes.width()) / 2);
			}
			y = FinDelTexto;
			if ((pageNo != 1)||(j == 1)) {
				rectangulo.setTopLeft (QPoint::QPoint (izquierda + 185, y + 50));
				rectangulo.setBottomRight(QPoint::QPoint (centro, y + 90));
				p->setFont(sumaFont);
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, QString::number(palomasImpresas));
				p->setFont(sansFont);
			}
			y += 90;
			for (int i = 0; i < divisiones; i++) {
				y += 20;
				rectangulo.setTopLeft (QPoint::QPoint (izquierda, y - 20));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + 50, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 2));

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + 50, y - 20));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + 140, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 1));

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + 140, y - 20));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + 170, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 0));

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + 172, y - 20));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + 185, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 3), 1);

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + 185, y - 20));
				rectangulo.setBottomRight(QPoint::QPoint (centro, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 6));
				palomasImpresas++;
				if (palomasImpresas >= tablaListados->numRows())
					break;
			}
			rectangulo.setTopLeft (QPoint::QPoint (izquierda + 185, FinDelTexto + 90 + (divisiones - 1) * 20));
			rectangulo.setBottomRight(QPoint::QPoint (centro, FinDelTexto + 90 + (divisiones + 2) * 20));
			p->setFont(sumaFont);
			p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, QString::number(palomasImpresas));
			p->setFont(sansFont);
			if (palomasImpresas >= tablaListados->numRows())
				break;
		}
		if (palomasImpresas >= tablaListados->numRows())
			break;
		printer->newPage();
	}
}

void listados::generalStatsSlot()
{
	generalStats dlg;
	dlg.exec();
}
