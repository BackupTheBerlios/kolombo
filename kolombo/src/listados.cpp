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
#include "exportarTaurisWdg.h"

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
#include <qimage.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <qregexp.h>

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
#include <kurlrequester.h>
#include <kdialogbase.h>
#include <kfiledialog.h>

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
    tablaListados->setColumnWidth(10, config().LRFID);
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
    config().LRFID = tablaListados->columnWidth(10);
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
    if (tablaListados->currentRecord())
        config().anillaSeleccion = tablaListados->currentRecord()->value(indicePalomaID).toInt();
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

		// Colocación del icono de la paloma.
		float factorImage = 2.5;
		QPixmap *icono = new QPixmap ("/home/heaven/cvs/kbird2/src/fede.png");
		QImage image = icono->convertToImage();
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width(), margenes.height()));
		rectangulo.setBottomRight(QPoint::QPoint (margenes.width() + factorImage * icono->width(), margenes.height() + factorImage * icono->height()));
		p->drawPixmap(rectangulo, image.smoothScale(factorImage * icono->width(), factorImage * icono->height()));


		rectangulo.setTopLeft (QPoint::QPoint (0, 0));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), margenes.height()));
		p->drawText( rectangulo, Qt::AlignRight | Qt::AlignVCenter | Qt::WordBreak, "HOJA NUM: " + QString::number(ceil (pageNo / 2)));
		sansFont.setUnderline(true);
		p->setFont (sansFont);
		rectangulo.setTopLeft (QPoint::QPoint (margenes.width() + (0.2 + factorImage) * icono->width(), margenes.height()));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->drawText (rectangulo, Qt::WordBreak | Qt::AlignJustify, "REAL FEDERACION COLOMBOFILA ESPAÑOLA");

		rectangulo.setTopLeft (QPoint::QPoint (margenes.width() + 1.2 * icono->width(), margenes.height()));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->drawText (rectangulo, Qt::WordBreak | Qt::AlignRight, "Censo de palomas mensajeras. Año 2003");
		sansFont.setUnderline(false);

		rectangulo.setTopLeft (QPoint::QPoint (margenes.width(), margenes.height() - 10));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->setFont (KGlobalSettings::generalFont());
		p->drawText( rectangulo, Qt::WordBreak | Qt::AlignJustify | Qt::AlignTop,
					"\n\n         Declaración que presenta el socio " + config().nombreP + " con D.N.I. " + config().dniP + " y licencia nº " + config().licenciaP + " perteneciente a " + config().clubP + " de las palomas de su propiedad existentes en su palomar, sito en " + config().poblacionP + " (" + config().provinciaP + "), calle " + config().calleP + " número " + config().numeroP + ".");
		FinDelTexto = pdm.height() * 0.09;
	} else {
		QDateTime reloj;
		QDate date(reloj.currentDateTime().date());
		abajo -= margenes.width() * 0.05;

		rectangulo.setTopLeft (QPoint::QPoint (margenes.width(), abajo));
		rectangulo.setBottomRight(QPoint::QPoint (pdm.width() - margenes.width(), pdm.height()));
		p->setFont (KGlobalSettings::generalFont());
		p->drawText( rectangulo, Qt::AlignRight | Qt::AlignVCenter | Qt::WordBreak,
				"Adeje a " + QString::number(date.day()) + " de " + date.longMonthName(date.month()) + " de " + QString::number(date.year()) + "\nEl propietario.");
		FinDelTexto = margenes.height();

	}
	/** Encabezado de la tabla ... */
	int izquierda = margenes.width(), centro = 0;
	int dos = margenes.width() * 0.05;

	lapiz.setWidth (dos / 2);
	p->setPen (lapiz);
	p->drawLine (izquierda - dos, FinDelTexto - dos, pdm.width() -  margenes.width() + dos / 2, FinDelTexto- dos);
	p->drawLine (izquierda - dos, FinDelTexto - dos, izquierda - dos, abajo);
	p->drawLine (pdm.width() -  margenes.width() + dos / 2, FinDelTexto - dos,
						 pdm.width() - margenes.width() + dos / 2, abajo);

	for (int j = 0; j < 2; j++) {
		if (centro) {
			izquierda = centro + dos / 2;
			centro +=  ((pdm.width() - 2 * margenes.width()) / 2);
		}
		else
			centro = margenes.width() + ((pdm.width() - 2 * margenes.width()) / 2) - dos / 2;

		lapiz.setColor (Qt::black);
		lapiz.setWidth (0);
		lapiz.setStyle (Qt::SolidLine);
		p->setPen (lapiz);
		p->drawLine (izquierda, FinDelTexto, centro, FinDelTexto);
		p->drawLine (izquierda, FinDelTexto, izquierda, abajo);
		p->drawLine (centro, FinDelTexto, centro, abajo);

		/** Texto **/
		int largoDetalleAnilla = pdm.height() * 0.02;
		int anchoDetalleAnilla = pdm.width() * 0.2;
		int largoDetalles = largoDetalleAnilla * 0.8;
		int anchoIniciales = anchoDetalleAnilla * 0.3;
		int anchoNumero = anchoDetalleAnilla * 0.5;
		int anchoAnyo = anchoDetalleAnilla * 0.2;
		p->setFont (sansFont);
		rectangulo.setTopLeft (QPoint::QPoint (izquierda, FinDelTexto));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoDetalleAnilla, FinDelTexto + largoDetalleAnilla));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "DETALLE DE LA ANILLA");

		rectangulo.setTopLeft (QPoint::QPoint (izquierda, FinDelTexto + largoDetalleAnilla));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoIniciales,
																			FinDelTexto + largoDetalleAnilla + largoDetalles));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "Iniciales");

		rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoIniciales, FinDelTexto + largoDetalleAnilla));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoIniciales + anchoNumero,
																			FinDelTexto + largoDetalleAnilla + largoDetalles));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "Número");

		rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoIniciales + anchoNumero, FinDelTexto + largoDetalleAnilla));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoIniciales + anchoNumero + anchoAnyo,
																			FinDelTexto + largoDetalleAnilla + largoDetalles));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "Año");

		QFont sexFont( "Arial", 6 );
		p->setFont (sexFont);
		rotacion.rotate (90);
		p->setWorldMatrix( rotacion );
		rectangulo.setTopLeft (QPoint::QPoint (FinDelTexto, -(izquierda + anchoDetalleAnilla)));
		rectangulo.setBottomRight(QPoint::QPoint (FinDelTexto + largoDetalleAnilla + largoDetalles,
																	-(izquierda + anchoDetalleAnilla + largoDetalles)));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::BreakAnywhere,  "SEXO");
		rotacion.rotate (-90);
		p->setWorldMatrix( rotacion );
		p->setFont (sansFont);

		rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoDetalleAnilla,  FinDelTexto));
		rectangulo.setBottomRight(QPoint::QPoint (centro, FinDelTexto + largoDetalleAnilla + largoDetalles));
		p->drawText( rectangulo, Qt::AlignHCenter | Qt::AlignVCenter | Qt::WordBreak, "C O L O R");
		/** Horizontales **/
		p->drawLine (izquierda, FinDelTexto + largoDetalleAnilla,
							 izquierda + anchoDetalleAnilla, FinDelTexto + largoDetalleAnilla);
		p->drawLine (izquierda, FinDelTexto + largoDetalleAnilla + largoDetalles,
							 izquierda + anchoDetalleAnilla, FinDelTexto + largoDetalleAnilla + largoDetalles);
		p->drawLine (izquierda + dos + anchoDetalleAnilla, FinDelTexto + largoDetalleAnilla + largoDetalles,
							 centro, FinDelTexto + largoDetalleAnilla + largoDetalles);

		/** Verticales **/
		p->drawLine (izquierda + anchoDetalleAnilla, FinDelTexto, izquierda + anchoDetalleAnilla, abajo);
		p->drawLine (izquierda + anchoDetalleAnilla + dos, FinDelTexto, izquierda + anchoDetalleAnilla + dos, abajo);

		p->drawLine (izquierda + anchoDetalleAnilla + largoDetalles, FinDelTexto,
							 izquierda + anchoDetalleAnilla + largoDetalles, abajo);

		p->drawLine (izquierda + anchoIniciales , FinDelTexto + largoDetalleAnilla,
							 izquierda + anchoIniciales , abajo);

		p->drawLine (izquierda + anchoIniciales + anchoNumero, FinDelTexto + largoDetalleAnilla,
							 izquierda + anchoIniciales + anchoNumero, abajo);

		/** Suma anterior ... **/
		if ((pageNo != 1)||(j == 1)) {
			lapiz.setColor(Qt::white);
			lapiz.setWidth (dos * 1.5);
			p->setPen (lapiz);
			p->drawLine (izquierda + anchoIniciales + anchoNumero, FinDelTexto + 1.5 * largoDetalleAnilla + largoDetalles,
								 izquierda + anchoIniciales + anchoNumero, FinDelTexto + 2 * (largoDetalleAnilla + largoDetalles));
			lapiz.setColor(Qt::black);
			lapiz.setStyle (Qt::DotLine);
			lapiz.setWidth (0);
			p->setPen (lapiz);
			p->drawLine (izquierda + anchoDetalleAnilla, FinDelTexto + 2 * largoDetalleAnilla + largoDetalles,
								 centro, FinDelTexto + 2 * largoDetalleAnilla + largoDetalles);
			rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoIniciales,
											FinDelTexto + largoDetalleAnilla + largoDetalles));
			rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoDetalleAnilla,
											FinDelTexto + 2 * largoDetalleAnilla + largoDetalles + dos * 1.5));
			p->drawText( rectangulo,  Qt::AlignRight | Qt::AlignBottom | Qt::BreakAnywhere,  "Suma anterior . . . .");
		}

		/** Horizontales punteadas */
		lapiz.setStyle (Qt::DotLine);
		p->setPen (lapiz);
		int i;
		for (i = FinDelTexto + 2 * (largoDetalles + largoDetalleAnilla); i < abajo - 2 * largoDetalles; i += largoDetalles) {
			p->drawLine (izquierda, i, centro, i);
			divisiones++;
		}

		/** Suma y sigue ... **/
		lapiz.setColor(Qt::white);
		lapiz.setStyle (Qt::SolidLine);
		lapiz.setWidth (dos * 1.5);
		p->setPen (lapiz);
		p->drawLine (izquierda + anchoIniciales + anchoNumero, i,
							 izquierda + anchoIniciales + anchoNumero, abajo);
		lapiz.setColor(Qt::black);
		lapiz.setStyle (Qt::DotLine);
		lapiz.setWidth (0);
		p->setPen (lapiz);
		p->drawLine (izquierda + anchoDetalleAnilla, i + largoDetalleAnilla, centro, i + largoDetalleAnilla);
		rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoIniciales, i));
		rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoDetalleAnilla, i + largoDetalleAnilla + dos * 1.5));
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
	int dos = margenes.width() * 0.05;

	int largoDetalleAnilla = pdm.height() * 0.02;
	int anchoDetalleAnilla = pdm.width() * 0.2;
	int largoDetalles = largoDetalleAnilla * 0.8;
	int anchoIniciales = anchoDetalleAnilla * 0.2;
	int anchoNumero = anchoDetalleAnilla * 0.6;
	int anchoAnyo = anchoDetalleAnilla * 0.2;

	while (true) {
		pageNo++;
		int divisiones = printBase (p, printer, pageNo);
		if (pageNo % 2)
			FinDelTexto = pdm.height() * 0.09;
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
				rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoDetalleAnilla + largoDetalles, y));
				rectangulo.setBottomRight(QPoint::QPoint (centro, y + 2 * largoDetalleAnilla + largoDetalles + 1.5 * dos));
				p->setFont(sumaFont);
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, QString::number(palomasImpresas));
				p->setFont(sansFont);
			}
			y += 2 * largoDetalleAnilla + largoDetalles;
			for (int i = 0; i < divisiones; i++) {
				y += largoDetalles;
				rectangulo.setTopLeft (QPoint::QPoint (izquierda, y - largoDetalles));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoIniciales, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 2));

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoIniciales, y - largoDetalles));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoIniciales + anchoNumero, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 1));

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoIniciales + anchoNumero, y - largoDetalles));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoDetalleAnilla, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 0));

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoDetalleAnilla + dos, y - largoDetalles));
				rectangulo.setBottomRight(QPoint::QPoint (izquierda + anchoDetalleAnilla + largoDetalles, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 3), 1);

				rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoDetalleAnilla + largoDetalles, y - largoDetalles));
				rectangulo.setBottomRight(QPoint::QPoint (centro, y));
				p->drawText( rectangulo, Qt::AlignHCenter |Qt::AlignBottom | Qt::WordBreak, tablaListados->text(palomasImpresas, 6));
				palomasImpresas++;
				if (palomasImpresas >= tablaListados->numRows())
					break;
			}
			rectangulo.setTopLeft (QPoint::QPoint (izquierda + anchoDetalleAnilla + largoDetalles, 0));
			rectangulo.setBottomRight(QPoint::QPoint (centro, FinDelTexto + 3 * largoDetalleAnilla + (divisiones + 2) * largoDetalles + 1.5 * dos));
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

/* Exporta los datos del listado al formato que utiliza la versión 1.41 del TaurisClub, sistema de control de palomas.
El formato es el siguiente:
------------------------------------------------------------------------------------------
 <número de palomas\>
<nacionalidad\>   <año\>    <anilla\><sexo\>1300000000<Color 4 caracteres en mayúsculas\>
------------------------------------------------------------------------------------------

El formato del final de línea tiene que ser el de dos

*/
void listados::pigeonsTauris() 
{
    KDialogBase *k = new KDialogBase ( KDialogBase::Swallow, 0L,this, "exportarTauris", true, "Exportar", KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true );
    
    exportarTaurisWdg *d = new exportarTaurisWdg(this, "exportarTaurisWdg");
    
    k->setMainWidget (d);

    d->path->setURL(config().pathTauris);

    QString consulta (" from listaGeneral");
    if (!tablaListados->filter().isNull() && !tablaListados->filter().isEmpty())
        consulta += " where " + tablaListados->filter();
    d->fichero->append("<a name=\"top\">");
    QSqlQuery queryCount ("select count (*)" + consulta, QSqlDatabase::database("palomar"));
    if (queryCount.isActive()) {
        queryCount.next();
        d->fichero->append(" " + queryCount.value(0).toString());
    }

    QSqlQuery queryLista ("select anyo, anilla, nacionalidad, sexo, plumaje" + consulta, QSqlDatabase::database("palomar"));
    if (queryLista.isActive())
        while (queryLista.next()) {
            d->fichero->append(queryLista.value(2).toString().left(1) + "   " \
                + queryLista.value(0).toString().right(2) + "    " \
                + queryLista.value(1).toString() + queryLista.value(3).toString().left(1) \
                + "1300000000" + queryLista.value(4).toString().left(4).upper());
        }

    d->fichero->scrollToAnchor("top");
    d->fichero->setReadOnly(true);

    if (k->exec() == QDialog::Accepted) {
        QFile file( d->path->url() );
        if ( file.open( IO_WriteOnly ) ) {
            config().pathTauris =  d->path->url();
            config().write();
            QTextStream stream( &file );
            stream << d->fichero->text().replace( QRegExp("\n"), "\r\n" );
            d->fichero->setModified( FALSE );
        }
    }
}

/* Importa los datos al listado desde el formato que utiliza la versión 1.41 del TaurisClub, sistema de control de palomas.
El formato es el siguiente:
------------------------------------------------------------------------------------------
 <número de palomas\>
<nacionalidad\>   <año\>    <anilla\><sexo\>1300000000<Color 4 caracteres en mayúsculas\>
------------------------------------------------------------------------------------------

*/

void listados::importTauris() {
    int numPalomas;
    QString rfid;
    QString nacion;
    QString anilla;
    QString sexo;
    QString consulta;
    QString consultaActualizacion;
    consultaActualizacion = "";
    int anyo;
    QFile fichero(KFileDialog::getOpenFileName ());
    QStringList lines;
    if ( fichero.open( IO_ReadOnly ) ) {
        QTextStream stream( &fichero );
        QString line;
        line = stream.readLine();
        numPalomas = line.toInt();
        printf("Num palomas: %d", numPalomas);
        int i = 0;
        while ( !stream.atEnd() ) {
            anyo = 0;
            line = stream.readLine(); // line of text excluding '\n'
            if (line.mid(4, 2).toInt() > 50)
                anyo = 1900 + line.mid(4, 2).toInt();
            else
                anyo = 2000 + line.mid(4, 2).toInt();
            if (line.mid(17, 10) == "1300000000")
                rfid = "";
            else
                rfid = line.mid(17, 10);
            nacion = line.left(1).latin1();
            anilla = line.mid(10, 6).latin1();
            sexo = line.mid(16, 1).latin1();
            // Ahora hay que buscar la paloma en la tabla y actualizar el valor del rfid y las palomas
            // Que no estén en el listado indicarlas y preguntar si se quiere introducir la paloma en el listado o no.
            QSqlQuery querySelect ("SELECT * \
                                   FROM paloma \
                                   WHERE anyo = " + QString::number(anyo) + " AND anilla = \"" + anilla + "\";\n",
                                   QSqlDatabase::database("palomar"));
            if (querySelect.isActive()) {
                bool almenos1 = false;
                while (querySelect.next())
                    almenos1 = true;
                if (almenos1 && (rfid != "")) {
                    consultaActualizacion =  ("UPDATE paloma SET rfid = \"" + rfid + "\" WHERE anyo = " + QString::number(anyo) + " AND anilla = \"" + anilla + "\";");
                    QSqlQuery queryUpdate (consultaActualizacion,QSqlDatabase::database("palomar"));
                    QSqlError error = queryUpdate.lastError();
                    if (error.type() != QSqlError::None)
                        KMessageBox::error (this, 
                                        i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
                                        error.databaseText(),
                                        i18n ("Error actualizando la paloma"));
                } else if (!almenos1) {
                    
                }
            }
        }
        fichero.close();
    }
}
