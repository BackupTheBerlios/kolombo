#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

#############################################################################
##
## Copyright (C) 2006 René Martín Rodríguez <agarfu@gmail.com>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
#############################################################################

import sys, os, locale
from PyQt4 import QtCore, QtGui, QtSql

import kolombo_rc
import connection
from kolombomainwindow import kolomboMainWindow
from firstrundialog import firstRunDialog


if __name__ == "__main__":
	app = QtGui.QApplication(sys.argv)

	LANG = locale.getdefaultlocale ()[0][0:2]
	translator = QtCore.QTranslator ()
	translator.load (":%s" % (os.path.join("resources", "lang", "kolombo_%s" % LANG)))
	app.installTranslator (translator)

	app.setOrganizationName ("AgarSoft")
	app.setOrganizationDomain ("kolombo.berlios.de")
	app.setApplicationName ("kolombo")


	settings = QtCore.QSettings ()
	databaseConfigured = settings.value ("DataBase/config", QtCore.QVariant(False)).toBool()
	if not databaseConfigured:
		configDialog = firstRunDialog ()
		configDialog.setSettings (settings)
		if configDialog.exec_() != QtGui.QDialog.Accepted:
			sys.exit(1)

	database = settings.value ("DataBase/database", QtCore.QVariant ("data.db")).toString()
	db_filename = os.path.abspath(str(database))
	if not connection.createConnection(db_filename):
		sys.exit(1)

	mainWindow = kolomboMainWindow ()
	mainWindow.show ()
	sys.exit(app.exec_())
