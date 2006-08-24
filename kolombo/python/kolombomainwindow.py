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

from PyQt4 import QtCore, QtGui, QtSql
from kolombocentralwidget import kolomboCentralWidget


class kolomboMainWindow (QtGui.QMainWindow):
	def __init__ (self, *args):
		QtGui.QMainWindow.__init__(self, *args)

		centralWidget = kolomboCentralWidget()
		self.setCentralWidget(centralWidget)

		self.settings = QtCore.QSettings ()
		self.resize(
				self.settings.value ("MainWindow/size", 
				QtCore.QVariant(QtCore.QSize (800, 400))).toSize()
			)

		self.move(
				self.settings.value("MainWindow/pos",
				QtCore.QVariant(QtCore.QPoint(200, 200))).toPoint()
			)

	def closeEvent (self, e):
		self.settings.setValue ("MainWindow/size", QtCore.QVariant(self.size()))
		self.settings.setValue ("MainWindow/pos", QtCore.QVariant(self.pos()))
		self.settings.sync()
		QtGui.QMainWindow.closeEvent(self, e)
