/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef BASECHART_H
#define BASECHART_H

#include <qcanvas.h>
#include <qvaluevector.h> 

class QObject;
class QPixmap;
class QString;
class QColor;

/**
Esta clase genera la base para pintar las gráficas dentro

@author Ren?Mart? - AgarFu
*/
class baseChart : public QCanvas
{
Q_OBJECT
public:
	baseChart ( QObject * parent = 0, const char * name = 0 );
	baseChart ( int w, int h );
	baseChart ( QPixmap p, int h, int v, int tilewidth, int tileheight );
	baseChart();

	~baseChart();
	
	void setMaxX(double max);
	void setMaxY(double max);
	double getMaxX();
	double getMaxY();
	
	void setTitle(QString t);
	QString getTitle();
	
	void drawTitle ();
	
	void setRmargin(int m);
	int getRmargin();
	
	void setLmargin(int m);
	int getLmargin();
	
	void setTmargin(int m);
	int getTmargin();
	
	void setBmargin(int m);
	int getBmargin();
	void setMargin (int r, int l, int t, int b);
	
	void drawMargin ();
	void drawYScale ();
	virtual void drawLegend();
	virtual void drawBar (int offset, int width, int height, QColor color);
	virtual void drawXLabel (double offset, QString label);
	int scaleX (double x);
	int scaleY (double y);
	
	void addDataType (QColor c, QString n);
private:
	double maxX, maxY;
	QString title;
	int rmargin, lmargin, tmargin, bmargin;
	QValueVector<QColor> dataColors;
	QValueVector<QString> dataNames;
};

#endif
