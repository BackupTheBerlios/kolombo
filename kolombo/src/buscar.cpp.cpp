/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "buscar.cpp.h"

buscar::buscar(QWidget* parent, const char* name, bool modal, WFlags fl)
: buscarDlg(parent,name, modal,fl)
{
}

buscar::~buscar()
{
}

/*$SPECIALIZATION$*/


#include "buscar.cpp.moc"

