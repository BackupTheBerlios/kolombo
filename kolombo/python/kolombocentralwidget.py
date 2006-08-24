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

import kolombo_rc
import os
from palomarsqlmodel import palomarSqlModel
from palomartableview import palomarTableView
from dovecoteview import dovecoteView

class kolomboCentralWidget (QtGui.QWidget):
	def __init__ (self, *args):
		QtGui.QWidget.__init__(self, *args)

		self.contentsWidget = QtGui.QListWidget()
		self.contentsWidget.setViewMode (QtGui.QListView.IconMode)
		self.contentsWidget.setIconSize (QtCore.QSize (96,84))
		self.contentsWidget.setMovement (QtGui.QListView.Static)
		self.contentsWidget.setMaximumWidth (128)
		self.contentsWidget.setSpacing (12)
	
		self.pagesWidget = QtGui.QStackedWidget()
		self.horizontalLayout = QtGui.QHBoxLayout ()
		self.horizontalLayout.addWidget(self.contentsWidget)
		self.horizontalLayout.addWidget(self.pagesWidget, 1)
	
# Lists page
		self.listWidget = dovecoteView()
		self.pagesWidget.addWidget (self.listWidget)
		self.listsButton = QtGui.QListWidgetItem(self.contentsWidget);
		self.listsButton.setIcon(
				QtGui.QIcon(":" + os.path.join (
						"resources",
						"icons", 
						"big", 
						"clock.png")
					)
				)
		self.listsButton.setText(self.tr("Lists"));
		self.listsButton.setTextAlignment(QtCore.Qt.AlignHCenter);
		self.listsButton.setFlags(QtCore.Qt.ItemIsSelectable | QtCore.Qt.ItemIsEnabled);
	
# New Piggeon page
		self.pagesWidget.addWidget (QtGui.QPushButton("New Piggeon!!"))
		self.newPiggeonButton = QtGui.QListWidgetItem(self.contentsWidget);
		self.newPiggeonButton.setIcon(
				QtGui.QIcon(":" + os.path.join (
						"resources",
						"icons", 
						"big", 
						"tux.png")
					)
				)
		self.newPiggeonButton.setText(self.tr("New Piggeon"));
		self.newPiggeonButton.setTextAlignment(QtCore.Qt.AlignHCenter);
		self.newPiggeonButton.setFlags(QtCore.Qt.ItemIsSelectable | QtCore.Qt.ItemIsEnabled);
	
# Pairs page
		self.pagesWidget.addWidget (QtGui.QPushButton("Pairs!!"))
	  	self.pairsButton = QtGui.QListWidgetItem(self.contentsWidget);
		self.pairsButton.setIcon(
				QtGui.QIcon(":" + os.path.join (
						"resources",
						"icons", 
						"big", 
						"multimedia.png")
					)
				)
		self.pairsButton.setText(self.tr("Pairs"));
		self.pairsButton.setTextAlignment(QtCore.Qt.AlignHCenter);
		self.pairsButton.setFlags(QtCore.Qt.ItemIsSelectable | QtCore.Qt.ItemIsEnabled);
	
		self.setLayout (self.horizontalLayout)

		self.connect (
				self.contentsWidget,
				QtCore.SIGNAL ("currentItemChanged(QListWidgetItem *, QListWidgetItem *)"),
				self.changePage
			)

		self.contentsWidget.setCurrentRow (0)


	def changePage (self, current, previous):
		if not current:
			current = previous
		self.pagesWidget.setCurrentIndex (self.contentsWidget.row(current))
