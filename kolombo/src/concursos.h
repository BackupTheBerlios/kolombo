/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CONCURSOS_H
#define CONCURSOS_H

#include "concursosWdg.h"

class QString;
class QTime;
class QComboBox;

class concursos : public concursosWdg
{
  Q_OBJECT

public:
  concursos(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~concursos();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
	virtual void nuevaSuelta();
	virtual void horaSueltaCambiada (const QTime &hora);
	virtual void sueltaCambiada (int indice);
	virtual void setDesignada ();
	virtual void setSerie ();
	virtual void setConcurso ();
	virtual void setFueraConcurso ();
	virtual void eliminar ();
	virtual void comprobarSlot ();
	virtual void desaparecer ();
	virtual void encestar ();
    virtual void filtroInteligente(const QString&);

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:
	QString concursoID;
	void updateComboConcursos ();
	void setEstado (QString estado, QString palomaID);  
};

#endif

