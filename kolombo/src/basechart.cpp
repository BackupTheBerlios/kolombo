/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "basechart.h"

#include <qobject.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qvaluevector.h> 

baseChart::baseChart ( QObject * parent, const char * name)
	:QCanvas (parent, name)
{
	maxX = maxY = 100;
	rmargin = lmargin = tmargin = bmargin = 10;
}

baseChart::baseChart ( int w, int h )
	: QCanvas (w, h)
{
	maxX = maxY = 100;
	rmargin = lmargin = tmargin = bmargin = 10;
}

baseChart::baseChart ( QPixmap p, int h, int v, int tilewidth, int tileheight )
	:QCanvas (p, h, v, tilewidth, tileheight)
{
	maxX = maxY = 100;
	rmargin = lmargin = tmargin = bmargin = 10;
}

baseChart::baseChart()
{
	maxX = maxY = 100;
	rmargin = lmargin = tmargin = bmargin = 10;
}


baseChart::~baseChart()
{
}

void baseChart::setMaxX(double max)
{
	maxX = max;
}

void baseChart::setMaxY(double max)
{
	maxY = max;
}

double baseChart::getMaxX()
{
	return(maxX);
}

double baseChart::getMaxY()
{
	return(maxY);
}


void baseChart::setTitle(QString t)
{
	title = t;
	drawTitle();
}

QString baseChart::getTitle()
{
	return(title);
}

void baseChart::setRmargin(int m)
{
	rmargin = m;
}

int baseChart::getRmargin()
{
	return(rmargin);
}

void baseChart::setLmargin(int m)
{
	lmargin = m;
}

int baseChart::getLmargin()
{
	return(lmargin);
}
	
void baseChart::setTmargin(int m)
{
	tmargin = m;
}

int baseChart::getTmargin()
{
	return(tmargin);
}	
void baseChart::setBmargin(int m)
{
	bmargin = m;
}

int baseChart::getBmargin()
{
	return(bmargin);
}

void baseChart::setMargin (int r, int l, int t, int b)
{
	lmargin = l;
	rmargin = r;
	tmargin = t;
	bmargin = b;
}


void baseChart::drawTitle ()
{
	QCanvasText *text = new QCanvasText(title, this);
	text->setColor( Qt::black );
	text->setTextFlags(Qt::AlignHCenter);
	text->setX( width()/2 );
	text->setY(5);
	text->setZ(0);
	text->show();
}

void baseChart::addDataType (QColor c, QString n)
{
	dataColors.append(c);
	dataNames.append(n);
}

void baseChart::drawMargin ()
{
	QPen pen(Qt::black, 1);
	QCanvasRectangle *margin = new QCanvasRectangle(
										lmargin, tmargin,
										width() - lmargin - rmargin,
										height() - bmargin - tmargin, this);
	margin->setPen(pen);
	margin->setZ( 0 );
	margin->show();
}

/*
	Esto pinta las líneas horizontales, lo que voy a hacer es pintar en las unidades
	una línea punteada y en los múltiplos de 5 una línea rayada.
	
	Esto lo voy a intentar ajustar automáticamente, es decir, no quiero pintar más de una
	línea cada 5 pixels, así que lo que voy a intentar hacer es ajustarlo automáticamente.
*/
void baseChart::drawYScale()
{
	int paso = 20;
	int baseY = height() - bmargin;
	int utilPixels = baseY - tmargin;
	int segments = utilPixels / paso;
	int pasoReal = maxY / segments;
	
	QPen pen(Qt::gray, 1);
	for (int i = 1; i < segments; i ++) 
	{
		QCanvasLine *linea = new QCanvasLine(this);
		linea->setPoints(lmargin, baseY - paso * i, width() - rmargin, baseY - paso * i);
		if (!(i % 5))
		{
			pen.setWidth(0);
			pen.setStyle(Qt::DashLine);
			QCanvasText *text = new QCanvasText(QString::number(i * pasoReal), this);
			text->setTextFlags(Qt::AlignRight || Qt::AlignVCenter);
			QFontMetrics fm (text->font());
			text->setColor( Qt::black );
			text->setX( lmargin - fm.width(QString::number(i * pasoReal)) - 3);
			text->setY(baseY - paso * i - (fm.height()/2));
			text->setZ( -1 );
			text->show();
			QCanvasLine *marca = new QCanvasLine(this);
			pen.setColor (Qt::black);
			marca->setPoints(lmargin - 3, baseY - paso * i, lmargin, baseY - paso * i);
			pen.setStyle(Qt::SolidLine);
			marca->setPen(pen);
			marca->setZ(-1);
			marca->show();
			pen.setColor (Qt::gray);
		}
		else
		{
			pen.setWidth(0);
			pen.setStyle(Qt::DotLine);
		}
		linea->setPen(pen);
		linea->setZ(-1);
		linea->show();
	}
}

void baseChart::drawLegend()
{
	int legendWidth = 100;
	int legendHeight = 100;
	QPen pen;
	
	QCanvasText *text = new QCanvasText("Leyenda:", this);
	QFontMetrics fm (text->font());
	legendWidth = fm.width("Leyenda:");
	
	for (QValueVector<QString>::iterator n = dataNames.begin(); n != dataNames.end(); ++n )
	{
		if (legendWidth < fm.width(*n))
			legendWidth = fm.width(*n);
	}
	
	legendWidth += 25;	
	text->setColor( Qt::black );
	text->setTextFlags(Qt::AlignLeft || Qt::AlignBottom);
	text->setX( width() - rmargin - legendWidth );
	text->setY(tmargin + 14);
	text->setZ( 3 );
	text->show();

	int textY = tmargin + 17 + fm.height();
	QValueVector<QColor>::iterator colors = dataColors.begin();
	for (QValueVector<QString>::iterator n = dataNames.begin(); n != dataNames.end(); ++n )
	{
		QCanvasText *name = new QCanvasText(*n, this);
		name->setColor( Qt::black );
		name->setTextFlags(Qt::AlignLeft || Qt::AlignBottom);
		name->setX( width() - rmargin - legendWidth + 7);
		name->setY(textY);
		name->setZ( 3 );
		name->show();
		QCanvasRectangle *color = new QCanvasRectangle(
											width() - rmargin - legendWidth - 5, textY,
											10, 10, this);
		color->setBrush(QBrush(*colors, BrushStyle(Qt::SolidLine)));
		color->setPen(pen);
		color->setZ(3);
		color->show();
		textY += fm.height();
		colors++;
	}
	legendHeight = textY - fm.height() - 10;
	QCanvasRectangle *legendRect = new QCanvasRectangle(
											width() - rmargin - legendWidth - 10, tmargin + 10,
											legendWidth, legendHeight, this);
	legendRect->setBrush(QBrush(Qt::white, BrushStyle(Qt::SolidLine)));
	legendRect->setPen(pen);
	legendRect->setZ(2);
	legendRect->show();

}

void baseChart::drawBar (int offset, int width, int height, QColor color)
{
	if (height > maxY)
		return;
	if (offset + width > maxX)
		return;
		
	int xutil = this->width() - rmargin - lmargin - 5;
	int yutil = this->height() - bmargin - tmargin;

	height = (height * yutil) / maxY;
	width = (width * xutil) / maxX;
	offset = (offset * xutil) / maxX;
		

	QPen pen;
	QCanvasRectangle *bar = new QCanvasRectangle(
											lmargin + 5 + offset, this->height() - bmargin - height,
											width, height, this);
	bar->setBrush(QBrush(color, BrushStyle(Qt::SolidLine)));
	bar->setPen(pen);
	bar->setZ(0);
	bar->show();
}

int baseChart::scaleY(double y)
{
	return (y * (height() - bmargin - tmargin) / maxY);
}

int baseChart::scaleX(double x)
{
	return (x * (width() - rmargin - lmargin) / maxX);
}

void baseChart::drawXLabel (double offset, QString label)
{
	QCanvasText *text = new QCanvasText(label, this);
	text->setColor( Qt::black );
	text->setTextFlags(Qt::AlignHCenter);
	text->setX( lmargin + scaleX(offset));
	text->setY(height() - bmargin);
	text->setZ(0);
	text->show();
}

#include "basechart.moc"
