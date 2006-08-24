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

from PyQt4 import QtGui, QtCore

import kolombo_rc
import os.path
from ui.ui_firstrundialog import Ui_firstRunDialog

class firstRunDialog (QtGui.QDialog, Ui_firstRunDialog):
	def __init__(self, *args):
		QtGui.QDialog.__init__(self, *args)
		self.setupUi (self)
		self.settiongs = None
	
	def setSettings (self, s):
		self.settings = s
	
	def accept (self):
		if self.settings != None:
			self.settings.setValue ("DataBase/config", QtCore.QVariant(True))
			self.settings.setValue ("DataBase/database", QtCore.QVariant(self.dataBase.text()))
			self.settings.sync()
		QtGui.QDialog.accept (self)

	@QtCore.pyqtSignature("")
	def on_fileDialogButton_clicked (self):
		fileDialog = QtGui.QFileDialog ()
		fileDialog.setDirectory (os.path.abspath(os.path.curdir))
		fileDialog.setFileMode(QtGui.QFileDialog.AnyFile)
		fileDialog.setViewMode(QtGui.QFileDialog.Detail)
		if (fileDialog.exec_()):
			fileNames = fileDialog.selectedFiles()
			self.dataBase.setText (fileNames[0])
