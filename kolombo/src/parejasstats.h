/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PAREJASSTATS_H
#define PAREJASSTATS_H

#include "parejasStatsDlg.h"

class baseChart;
class QCanvasView;
class QGridLayout;

class parejasStats : public parejasStatsDlg
{
  Q_OBJECT

public:
  parejasStats(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~parejasStats();
  /*$PUBLIC_FUNCTIONS$*/
  void drawElements();
  void barras(int offset, int vuelo, int desaparecido, int reproduccion);
	void setMadre(QString m);
	void setPadre (QString p);
public slots:
  /*$PUBLIC_SLOTS$*/


protected:
  /*$PROTECTED_FUNCTIONS$*/
  void resizeEvent( QResizeEvent *e );

protected slots:
  /*$PROTECTED_SLOTS$*/

private:
	void getData (int anyo, int *vuelo, int *desaparecido, int *reproduccion);
	QString madre, padre;
	baseChart *m_canvas;
	QCanvasView *m_canvasView;
	QGridLayout *estadisticaLayout;
	bool firstResize;
};

#endif

