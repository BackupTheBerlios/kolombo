#!/usr/bin/python
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


# Script to import new set of dates to the database
# This script imports eyeColors, plumageColors and nations

import sys
import os
import locale
from sqlobject import connectionForURI, sqlhub
from pysqlite2 import dbapi2

from data.pigeondatabase import eyeColor, plumageColor, nation, yearColor, pigeon, pair


def importSet (module, name):
	""" Import a set of data """
	print "Importing ", name
	try:
		__import__ (module)
	except dbapi2.IntegrityError, e:
		print "Error importing %s: %s" %  (name, e)
	else:
		print name, "imported sucessfuly"


db_filename = os.path.abspath('test.db')
if os.path.exists(db_filename):
	os.unlink(db_filename)
connection_string = 'sqlite:' + db_filename
connection = connectionForURI(connection_string)
sqlhub.processConnection = connection

try:
	eyeColor.createTable ()
	plumageColor.createTable()
	nation.createTable ()
	yearColor.createTable()
	pigeon.createTable()
	pair.createTable()
except:
	print "Database is already done"

LANG = locale.getdefaultlocale ()[0][0:2] #Getting the first part of the locale name
if not os.path.isdir (os.path.join ("data", LANG)):
	print "There is no data for locale ", LANG
	LANG = "es"
print "Using %s data" % LANG

importSet(os.path.join ("data", LANG, "eyes"), "eye colors")
importSet (os.path.join ("data", LANG, "nations"), "nations")
importSet (os.path.join ("data", LANG, "plumagecolors"), "plumage colors")
importSet (os.path.join ("data", LANG, "pigeons"), "pigeons")
importSet (os.path.join ("data", LANG, "yearcolors"), "year colors")
