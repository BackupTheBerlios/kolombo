/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <qstring.h>
#include <qvaluelist.h>

class QString;

/**
@author Ren?Mart? - AgarFu
*/
class configuration
{
	public:

		void read ();
		void write();
		
		void updateAnilla(int nuevoValor);
		QString nombreDB, hostDB, passDB, driverDB, userDB;
		QString nombreP, dniP, clubP, poblacionP, licenciaP, provinciaP, calleP, numeroP;
		bool connectOnStart;
        double coordX, coordY;
		int ultimaAnilla;
		int anillaSeleccion;

		// Preferencias de los listados
		int LAnyo, LAnilla, LNacion, LSexo, LMadre, LPadre, LNombre, LOjo, LPlumaje, LEstado;
        QString pathTauris;

		//Preferencias de las tablas de las parejas
		int PNombre, PMadre, PPadre, PNota;
		int HAnyo, HAnilla, HNacion, HSexo, HMadre, HPadre, HNombre, HOjo, HPlumaje, HEstado;
		 QValueList <int> SplitterParejas;

		//Preferencia de las tablas de los concursos
		int LDAnyo, LDAnilla, LDNacion, LDNombre;
		int LCAnyo, LCAnilla, LCNacion, LCModalidad, LCComprobada, LCVelocidad;
		 QValueList <int> SplitterConcursos;

	private:
		configuration();
		configuration(const configuration&);

		friend configuration& config();

};

/// Returns a reference to the application configuration object.
configuration& config();

#endif
