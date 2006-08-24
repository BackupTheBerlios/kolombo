#!/bin/env python
# -*- coding: iso-8859-15 -*-

#############################################################################
##
## Copyright (C) 2006 René Martín Rodríguez
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

import sys
import os
import locale
from PyQt4 import QtSql, QtGui, QtCore
from sqlobject import connectionForURI, sqlhub
from pysqlite2 import dbapi2
from data.pigeondatabase import eyeColor, plumageColor, nation, yearColor, pigeon, pair

def createDatabase ():
	""" If the database is empty create new tables and populate them with some
	data """

	def importSet (module, name):
		""" Import a set of data """
		QtCore.qWarning ("Importing %s" % name)
		try:
			__import__ (module)
		except dbapi2.IntegrityError, e:
			QtGui.QMessageBox.critical(None, QtGui.qApp.tr("Error importing data"),
				QtGui.qApp.tr("Message returned by database is:.\n"
								"\"%s\"" % e), QtGui.QMessageBox.Ok)
		else:
			QtCore.qWarning ("%s imported sucessfuly" % name )

	try:
		eyeColor.createTable ()
		plumageColor.createTable()
		nation.createTable ()
		yearColor.createTable()
		pigeon.createTable()
		pair.createTable()
		LANG = locale.getdefaultlocale ()[0][0:2] #Getting the first part of the locale name
		if not os.path.isdir (os.path.join ("data", LANG)):
			QtCore.qWarning ("There is no data for locale %s" % LANG)
			LANG = "es"
		QtCore.qWarning ("Using '%s' data" % LANG)
	
		#TODO: if there is not data set for the locale use default ones (spanish?)
		importSet(os.path.join ("data", LANG, "eyes"), "eye colors")
		importSet (os.path.join ("data", LANG, "nations"), "nations")
		importSet (os.path.join ("data", LANG, "plumagecolors"), "plumage colors")
		importSet (os.path.join ("data", LANG, "yearcolors"), "year colors")
		importSet (os.path.join ("data", LANG, "pigeons"), "pigeons")
	
		QtGui.QMessageBox.information (None, QtGui.qApp.tr ("Successfuly created database"),
			QtGui.qApp.tr ("New database created.\n"
							"The database has been populated with some data"), 
			QtGui.QMessageBox.Ok)
	except:
		QtCore.qWarning ("Ok, Database found")

def createConnection(database):
	""" Create a QtSql connection and a SQLObject connection to a SQLite database """
	db = QtSql.QSqlDatabase.addDatabase("QSQLITE")
	db.setDatabaseName(database)
	if not db.open():
		QtGui.QMessageBox.critical(None, QtGui.qApp.tr("Cannot open database"),
				QtGui.qApp.tr("Unable to establish a database connection.\n"
							  "This application needs SQLite support. Please read "
							  "the Qt SQL driver documentation for information "
							  "how to build it.\n\nClick Cancel to exit."),
				QtGui.QMessageBox.Cancel, QtGui.QMessageBox.NoButton)
		return False

	db_filename = database
	connection_string = 'sqlite:' + db_filename
	connection = connectionForURI(connection_string)
	sqlhub.processConnection = connection
	createDatabase ()

	return True
