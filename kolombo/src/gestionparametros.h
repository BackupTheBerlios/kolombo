/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GESTIONPARAMETROS_H
#define GESTIONPARAMETROS_H

#include <kdialogbase.h>

class QHBoxLayout;
/**
@author Ren?Mart? - AgarFu
*/
class gestionParametros : public KDialogBase
{
Q_OBJECT
public:

    gestionParametros();

    ~gestionParametros();
	private:
		QHBoxLayout* gestionParametrosOjoLayout;
		QHBoxLayout* gestionParametrosPlumajeLayout;
		QHBoxLayout* gestionParametrosNacionLayout;
		QHBoxLayout* gestionParametrosTipoSueltaLayout;
		QHBoxLayout* gestionParametrosSueltaLayout;
};

#endif
