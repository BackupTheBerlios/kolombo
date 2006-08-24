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
from PyQt4 import QtCore, QtGui

from palomarsqlmodel import palomarSqlModel
from palomartableview import palomarTableView


class dovecoteView (QtGui.QWidget):
	PIGEON_ID_COLUMN = 0
	NOTE_COLUMN = 11
	RFID_COLUMN = 12

	def __init__ (self, *args):
		QtGui.QWidget.__init__ (self, *args)

		self.filterLabel = QtGui.QLabel (self)
		self.filterLabel.setText(self.tr ("<b>Filter:</b>"))
		self.filterLineEdit = QtGui.QLineEdit (self)

		self.horizontalLayout = QtGui.QHBoxLayout ()
		self.horizontalLayout.addWidget (self.filterLabel)
		self.horizontalLayout.addWidget (self.filterLineEdit)

		self.model = palomarSqlModel()
		self.listWidget = palomarTableView ()
		self.listWidget.setModel (self.model)

# Hiding some fields
		self.listWidget.setColumnHidden (self.PIGEON_ID_COLUMN, True)
		self.listWidget.setColumnHidden (self.NOTE_COLUMN, True)
		self.listWidget.setColumnHidden (self.RFID_COLUMN, True)
		for i in range(1,10):
			self.listWidget.resizeColumnToContents (i)

		self.verticalLayout = QtGui.QVBoxLayout ()
		self.verticalLayout.addLayout (self.horizontalLayout)
		self.verticalLayout.addWidget (self.listWidget)

		self.setLayout (self.verticalLayout)

		self.filterTimer = QtCore.QTimer ()
		self.filterTimer.setSingleShot (True)
		self.filterText = ""

		self.connect (
				self.filterTimer,
				QtCore.SIGNAL ("timeout ()"),
				self.findAsYouType
			)

		self.connect (
				self.filterLineEdit,
				QtCore.SIGNAL ("textChanged ( QString )"),
				self.startTimer
			)

	def startTimer (self, filter):
		self.filterText = filter.toAscii()
		self.filterTimer.start (280)

	def findAsYouType (self):
		self.filterTimer.stop ()
		self.model.smartFilter (self.filterText)
