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

# Description: This is the palomar model view.

from PyQt4 import QtCore, QtGui, QtSql


class palomarTableView (QtGui.QTableView):
	def __init__(self, *args):
		QtGui.QTableView.__init__(self, *args)

		self.index = QtCore.QModelIndex ()

		self.changeSexAction = QtGui.QAction(self.tr("Change sex"), self)
		self.connect (self.changeSexAction, QtCore.SIGNAL("triggered()"), self.changeSex)

		self.setFlyerAction = QtGui.QAction(self.tr("Flyer"), self)
		self.connect (self.setFlyerAction, QtCore.SIGNAL("triggered()"), self.setFlyer)

		self.setReproducerAction = QtGui.QAction(self.tr("Reproducer"), self)
		self.connect (self.setReproducerAction, QtCore.SIGNAL("triggered()"), self.setReproducer)

		self.setLostAction = QtGui.QAction(self.tr("Lost"), self)
		self.connect (self.setLostAction, QtCore.SIGNAL("triggered()"), self.setLost)

		self.deletePiggeonAction = QtGui.QAction(self.tr("Remove"), self)
		self.connect (self.deletePiggeonAction, QtCore.SIGNAL("triggered()"), self.deletePiggeon)

		self.viewReproductionAction = QtGui.QAction(self.tr("View Reproduction"), self)
		self.connect (self.viewReproductionAction, QtCore.SIGNAL("triggered()"), self.viewReproduction)

		self.viewFlyersAction = QtGui.QAction(self.tr("View Flyers"), self)
		self.connect (self.viewFlyersAction, QtCore.SIGNAL("triggered()"), self.viewFlyers)

		self.viewLostAction = QtGui.QAction(self.tr("View Lost"), self)
		self.connect (self.viewLostAction, QtCore.SIGNAL("triggered()"), self.viewLost)

		self.viewAllAction = QtGui.QAction(self.tr("View All"), self)
		self.connect (self.viewAllAction, QtCore.SIGNAL("triggered()"), self.viewAll)

		self.printAction= QtGui.QAction(self.tr("Print"), self)
		self.connect (self.printAction, QtCore.SIGNAL("triggered()"), self.printList)


	def contextMenuEvent(self, event):
		self.index = self.indexAt (event.pos())

		menu = QtGui.QMenu(self.tr ("Piggeon menu"), self)
		menu.addAction(self.viewReproductionAction)
		menu.addAction(self.viewFlyersAction)
		menu.addAction(self.viewLostAction)
		menu.addAction(self.viewAllAction)
		menu.addAction(self.printAction)
		if self.index.isValid():
			menu.addAction(self.changeSexAction)
			menu.addAction(self.deletePiggeonAction)
			menu.addSeparator()
			stateMenu = QtGui.QMenu (self.tr ("Change state"), menu)
			stateMenu.addAction (self.setFlyerAction)
			stateMenu.addAction (self.setReproducerAction)
			stateMenu.addAction (self.setLostAction)
			menu.addMenu (stateMenu)
		menu.exec_(event.globalPos())

	def changeSex (self):
		self.model().changeSex (self.index)

	def setFlyer (self):
		self.model().setState (self.index, self.model().FLYER)

	def setReproducer (self):
		self.model().setState (self.index, self.model().REPRODUCTION)

	def setLost (self):
		self.model().setState (self.index, self.model().LOST)

	def deletePiggeon (self):
		self.model().deletePiggeon (self.index)

	def viewReproduction (self):
		self.model().viewReproduction ()

	def viewFlyers (self):
		self.model().viewFlyers ()

	def viewLost (self):
		self.model().viewLost ()

	def viewAll (self):
		self.model().resetFilter ()

	def printList (self):
		printer = QtGui.QPrinter()

		dialog = QtGui.QPrintDialog(printer, self)
		dialog.setWindowTitle(self.tr("Print Document"))
		if dialog.exec_() != QtGui.QDialog.Accepted:
			return

		document = QtGui.QTextDocument (self)
		document.setHtml (self.model().getHtml ())
		document.print_(printer)


