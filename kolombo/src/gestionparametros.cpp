/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ojo.h"
#include "plumaje.h"
#include "nacion.h"
#include "tiposuelta.h"
#include "gestionparametros.h"
#include "suelta.h"

#include <klocale.h>

#include <qframe.h>
#include <qlayout.h>
#include <kiconloader.h>    // for KIconLoader



gestionParametros::gestionParametros()
: KDialogBase(IconList, i18n("Gestión de parámetros"),Ok, Ok)
{

	QFrame *frame;
	frame = addPage(i18n("Ojos"), i18n("Colores de ojo disponibles"), KGlobal::iconLoader()->loadIcon("xeyes",KIcon::Panel,0,false));
	gestionParametrosOjoLayout = new QHBoxLayout( frame, 0, 0);
	ojo *ojoP = new ojo (frame);
	gestionParametrosOjoLayout->addWidget (ojoP);

	frame = addPage(i18n("Plumaje"), i18n("Colores de pluma disponibles"), KGlobal::iconLoader()->loadIcon("colors",KIcon::Panel,0,false));
	gestionParametrosPlumajeLayout = new QHBoxLayout( frame, 0, 0);
	plumaje *plumajeP = new plumaje (frame);
	gestionParametrosPlumajeLayout->addWidget (plumajeP);

	frame = addPage(i18n("Nacionalidad"), i18n("Nacionalidades disponibles"), KGlobal::iconLoader()->loadIcon("locale",KIcon::Panel,0,false));
	gestionParametrosNacionLayout = new QHBoxLayout( frame, 0, 0);
	nacion *nacionP = new nacion (frame);
	gestionParametrosNacionLayout->addWidget (nacionP);

	frame = addPage(i18n("Tipo Suelta"), i18n("Tipos de suelta disponibles"), KGlobal::iconLoader()->loadIcon("redo",KIcon::Panel,0,false));
	gestionParametrosTipoSueltaLayout = new QHBoxLayout( frame, 0, 0);
	tipoSuelta *tipoSueltaP = new tipoSuelta (frame);
	gestionParametrosTipoSueltaLayout->addWidget (tipoSueltaP);


	frame = addPage(i18n("Sueltas"), i18n("Suelta disponibles"), KGlobal::iconLoader()->loadIcon("launch",KIcon::Panel,0,false));
	gestionParametrosSueltaLayout = new QHBoxLayout( frame, 0, 0);
	suelta *sueltaP = new suelta (frame);
	gestionParametrosSueltaLayout->addWidget (sueltaP);
}


gestionParametros::~gestionParametros()
{
}


