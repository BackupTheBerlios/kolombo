/***************************************************************************
 *   Copyright (C) 2003 by René Martín - AgarFu                            *
 *   heaven@miniyo.croasanaso.org                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "configuration.h"

#include <kapplication.h>       // for 'kapp'
#include <kconfig.h>            // for KConfig



configuration::configuration()
{
	read();
}


void configuration::read () {
	anillaSeleccion = 0;
	KConfig* conf = kapp->config();

	conf->setGroup ("General");
	connectOnStart = conf->readEntry ("connectOnStart", false);

	conf->setGroup ("DataBase");
	nombreDB = conf->readEntry("nombreDB", "NONE");
	hostDB = conf->readEntry("hostDB", "NONE");
	passDB = conf->readEntry("passDB", "NONE");
	driverDB = conf->readEntry("driverDB", "NONE");
	userDB = conf->readEntry("userDB", "NONE");

	conf->setGroup ("Palomar");
	ultimaAnilla = conf->readNumEntry ("ultimaAnilla", 0);
	
	conf->setGroup("Personal");
	nombreP = conf->readEntry("nombreP", "");
	dniP = conf->readEntry("dniP", "");
	clubP = conf->readEntry("clubP", "");
	poblacionP = conf->readEntry("poblacionP", "");
	licenciaP = conf->readEntry("licenciaP", "");
	provinciaP = conf->readEntry("provinciaP", "");
	calleP = conf->readEntry("calleP", "");
	numeroP = conf->readEntry("numeroP", "");
    coordX = conf->readNumEntry("coordX", 0);
    coordY = conf->readNumEntry("coordY", 0);

    	
	// Preferencias de las tablas de los concursos
	conf->setGroup("Concursos");
	LDAnyo = conf->readNumEntry ("LDAnyo", 43);
	LDAnilla = conf->readNumEntry ("LDAnilla", 64);
	LDNacion = conf->readNumEntry ("LDNacion", 51);
	LDNombre = conf->readNumEntry ("LDNombre", 114);

	LCAnyo = conf->readNumEntry ("LCAnyo", 43);
	LCAnilla = conf->readNumEntry ("LCAnilla", 73);
	LCNacion = conf->readNumEntry ("LCNacion", 53);
	LCModalidad = conf->readNumEntry ("LCModalidad", 140);
	LCComprobada = conf->readNumEntry ("LCComprobada", 149);
	LCVelocidad = conf->readNumEntry ("LCVelocidad", 75);
	SplitterConcursos = conf->readIntListEntry ("SplitterConcursos");

	// Preferencias de parejas
	conf->setGroup ("Parejas");
	PNombre = conf->readNumEntry ("PNombre", 230);
	PMadre = conf->readNumEntry ("PMadre", 124);
	PPadre = conf->readNumEntry ("PPadre", 124);
	PNota = conf->readNumEntry ("PNota", 220);

	HAnyo = conf->readNumEntry ("HAnyo", 43);
	HAnilla = conf->readNumEntry ("HAnilla", 64);
	HNacion = conf->readNumEntry ("HNacion", 51);
	HNombre = conf->readNumEntry ("HNombre", 114);
	HSexo = conf->readNumEntry ("HSexo", 16);
	HMadre = conf->readNumEntry ("HMadre", 124);
	HPadre = conf->readNumEntry ("HPadre", 124);
	HEstado = conf->readNumEntry ("HEstado", 16);
	HOjo = conf->readNumEntry ("HOjo", 114);
	HPlumaje = conf->readNumEntry ("HPlumaje", 114);
	SplitterParejas = conf->readIntListEntry ("SplitterParejas");

	// Preferencias de listados
	conf->setGroup ("Listados");
	LAnyo = conf->readNumEntry ("LAnyo", 43);
	LAnilla = conf->readNumEntry ("LAnilla", 64);
	LNacion = conf->readNumEntry ("LNacion", 51);
	LNombre = conf->readNumEntry ("LNombre", 114);
	LSexo = conf->readNumEntry ("LSexo", 16);
	LMadre = conf->readNumEntry ("LMadre", 124);
	LPadre = conf->readNumEntry ("LPadre", 124);
	LEstado = conf->readNumEntry ("LEstado", 16);
	LOjo = conf->readNumEntry ("LOjo", 114);
	LPlumaje = conf->readNumEntry ("LPlumaje", 114);
}

void configuration::write () {
	KConfig* conf = kapp->config();
	conf->setGroup ("DataBase");
	conf->writeEntry ("nombreDB", nombreDB);
	conf->writeEntry ("hostDB", hostDB);
	conf->writeEntry ("passDB", passDB);
	conf->writeEntry ("driverDB", driverDB);
	conf->writeEntry ("userDB", userDB);
	conf->setGroup ("General");
	conf->writeEntry ("connectOnStart", connectOnStart);
	conf->setGroup ("Palomar");
	conf->writeEntry("ultimaAnilla", ultimaAnilla);
	
	conf->setGroup ("Personal");
	conf->writeEntry ("nombreP", nombreP);
	conf->writeEntry ("dniP", dniP);
	conf->writeEntry ("clubP", clubP);
	conf->writeEntry ("poblacionP", poblacionP);
	conf->writeEntry ("licenciaP", licenciaP);
	conf->writeEntry ("provinciaP", provinciaP);
	conf->writeEntry ("calleP", calleP);
	conf->writeEntry ("numeroP", numeroP);
    conf->writeEntry ("coordX", coordX);
    conf->writeEntry ("coordY", coordY);
	
	// Preferencias de las tablas de los concursos
	conf->setGroup ("Concursos");
	conf->writeEntry ("LDAnyo", LDAnyo);
	conf->writeEntry ("LDAnilla", LDAnilla);
	conf->writeEntry ("LDNacion", LDNacion);
	conf->writeEntry ("LDNombre", LDNombre);
	
	conf->writeEntry ("LCAnyo", LCAnyo);
	conf->writeEntry ("LCAnilla", LCAnilla);
	conf->writeEntry ("LCNacion", LCNacion);
	conf->writeEntry ("LCModalidad", LCModalidad);
	conf->writeEntry ("LCComprobada", LCComprobada);
	conf->writeEntry ("LCVelocidad", LCVelocidad);
	conf->writeEntry ("SplitterConcursos", SplitterConcursos);

	// Preferencias de parejas
	conf->setGroup ("Parejas");
	conf->writeEntry ("HAnyo", HAnyo);
	conf->writeEntry ("HAnilla", HAnilla);
	conf->writeEntry ("HNacion", HNacion);
	conf->writeEntry ("HSexo", HSexo);
	conf->writeEntry ("HMadre", HMadre);
	conf->writeEntry ("HPadre", HPadre);
	conf->writeEntry ("HEstado", HEstado);
	conf->writeEntry ("HOjo", HOjo);
	conf->writeEntry ("HPlumaje", HPlumaje);
	conf->writeEntry ("HNombre", HNombre);
	
	conf->writeEntry ("PNombre", PNombre);
	conf->writeEntry ("PMadre", PMadre);
	conf->writeEntry ("PPadre", PPadre);
	conf->writeEntry ("PNota", PNota);
	conf->writeEntry ("SplitterParejas", SplitterParejas);

	// Preferencias de listados
	conf->setGroup ("Listados");
	conf->writeEntry ("LAnyo", LAnyo);
	conf->writeEntry ("LAnilla", LAnilla);
	conf->writeEntry ("LNacion", LNacion);
	conf->writeEntry ("LSexo", LSexo);
	conf->writeEntry ("lMadre", LMadre);
	conf->writeEntry ("LPadre", LPadre);
	conf->writeEntry ("LEstado", LEstado);
	conf->writeEntry ("LOjo", LOjo);
	conf->writeEntry ("LPlumaje", LPlumaje);
	conf->writeEntry ("LNombre", LNombre);
}

void configuration::updateAnilla (int nuevoValor) {
	KConfig* conf = kapp->config();
	ultimaAnilla = ++nuevoValor;
	conf->writeEntry ("ultimaAnilla", ultimaAnilla);
}

configuration& config() {
    static configuration conf;
    return conf;
};

