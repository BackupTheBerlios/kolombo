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

# Description: This is the palomar sql model.
# TODO: Comment a little more

import sys
from PyQt4 import QtCore, QtGui, QtSql, QtXml
from data.pigeondatabase import yearColor, pigeon

YEAR_COLUMN = 1
IDENTIFICATION_COLUMN = 2
NATION_COLUMN = 3
SEX_COLUMN = 4
MOTHER_COLUMN = 5
FATHER_COLUMN = 6
PLUMAGE_COLUMN = 7
EYE_COLUMN = 8
STATE_COLUMN = 9
NAME_COLUMN = 10

class palomarSqlModel (QtSql.QSqlRelationalTableModel):
	MALE = 0
	MALE_COLOR = QtCore.QVariant (QtGui.QColor (102, 166, 255))
	FEMALE = 1
	FEMALE_COLOR = QtCore.QVariant (QtGui.QColor (255, 163, 249))
	FLYER = "F"
	FLYER_COLOR = QtCore.QVariant (QtGui.QColor (94, 255, 99))
	LOST = "L"
	LOST_COLOR = QtCore.QVariant (QtGui.QColor (255, 47, 47))
	REPRODUCTION = "R"
	REPRODUCTION_COLOR = QtCore.QVariant (QtGui.QColor (255, 251, 20))

	def __init__ (self, *args):
		QtSql.QSqlRelationalTableModel.__init__ (self, *args)
		self.setTable("pigeon")
		self.setEditStrategy(QtSql.QSqlTableModel.OnFieldChange)

		self.setHeaderData(YEAR_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Year"))))

		self.setHeaderData(IDENTIFICATION_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Identification"))))
	

		self.setRelation(NATION_COLUMN,
					QtSql.QSqlRelation("nation", "id", "short_name"))
		self.setHeaderData(NATION_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Nacionality"))))

		self.setHeaderData(SEX_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Sex"))))

		self.setHeaderData(STATE_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("State"))))

		self.setHeaderData(MOTHER_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Mother"))))

		self.setHeaderData(FATHER_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Father"))))

		self.setRelation(EYE_COLUMN,
					QtSql.QSqlRelation("eye_color", "id", "name"))
		self.setHeaderData(EYE_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Eye"))))

		self.setRelation(PLUMAGE_COLUMN,
					QtSql.QSqlRelation("plumage_color", "id", "short_name"))
		self.setHeaderData(PLUMAGE_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Plumage"))))

		self.setHeaderData(NAME_COLUMN, QtCore.Qt.Horizontal, 
			QtCore.QVariant(QtCore.QObject.tr(self, QtCore.QT_TR_NOOP("Name"))))

		self.select()


	def data (self, index, role):
# Changing background color
		if (role == QtCore.Qt.BackgroundColorRole):
			column = index.column ()
			if (column == YEAR_COLUMN):
				pigeonYear, ok = (QtSql.QSqlRelationalTableModel.data (self,
									index,
									QtCore.Qt.DisplayRole).toInt ()
									)
				if (ok):
# whe shoud add a table in the database containing year and RGB value, then
# extract from that table the color to paint the cell.
					color = list(yearColor.selectBy(year = pigeonYear))
					if len(color) == 1:
						color = QtGui.QColor (color[0].r, color[0].g, color[0].b)
					else:
						color = QtGui.QColor (255, 255, 255)
					return QtCore.QVariant (color)
			elif (column == SEX_COLUMN):
				sex, ok = QtSql.QSqlRelationalTableModel.data (self,
									index,
									QtCore.Qt.DisplayRole).toInt ()
				if (sex == self.MALE):
					return self.MALE_COLOR
				else:
					return self.FEMALE_COLOR
			elif (column == STATE_COLUMN):
				state = QtSql.QSqlRelationalTableModel.data (self,
									index,
									QtCore.Qt.DisplayRole).toString()
				if (state == self.FLYER):
					return self.FLYER_COLOR
				elif (state == self.LOST):
					return self.LOST_COLOR
				else:
					return self.REPRODUCTION_COLOR

# Changing some values, this coud be done with a sql view, but we can lost the
# database independence, and the ability to transale the states
		if (role == QtCore.Qt.DisplayRole):
			column = index.column ()
			if (column == SEX_COLUMN):
				sex, ok = QtSql.QSqlRelationalTableModel.data (self,
									index,
									QtCore.Qt.DisplayRole).toInt ()
				if (sex == self.MALE):
					return QtCore.QVariant(self.tr("M"))
				else:
					return QtCore.QVariant(self.tr("F"))
			elif (column == STATE_COLUMN):
				state = QtSql.QSqlRelationalTableModel.data (self,
									index,
									QtCore.Qt.DisplayRole).toString()
				if (state == self.FLYER):
					return QtCore.QVariant(self.tr("F"))
				elif (state == self.LOST):
					return QtCore.QVariant(self.tr("L"))
				else:
					return QtCore.QVariant(self.tr("R"))
			elif ((column == MOTHER_COLUMN) or (column == FATHER_COLUMN)):
			# Get the ID, if it is not Null get the compact ID of the pigeon
			# using SQLObject. Whe must to implement getCompactID method on
			# pigeon SQLObject model
			# return QtCore.QVariant (pigeon.get(progenitor).getCompactID())
				progenitor, ok = QtSql.QSqlRelationalTableModel.data (self,
									index,
									QtCore.Qt.DisplayRole).toInt ()
				if ok:
					return QtCore.QVariant ("00-identification-ESP")
				else:
					return QtCore.QVariant (self.tr ("No information"))
			else:
				return QtSql.QSqlRelationalTableModel.data (self, index, role)

		return QtSql.QSqlRelationalTableModel.data (self, index, role)


	def changeSex (self, index):
		if not index.isValid():
			print (self.tr("Error: ivalid index"))
			return 

		row = index.row()
		sex, ok = QtSql.QSqlRelationalTableModel.data (self,
							self.index(row, SEX_COLUMN),
							QtCore.Qt.DisplayRole).toInt ()
		if (sex == self.MALE):
			self.setData (self.index(row, SEX_COLUMN), QtCore.QVariant (self.FEMALE))
		else:
			self.setData (self.index(row, SEX_COLUMN), QtCore.QVariant(self.MALE))

	def setState (self, index, newState):
		if not index.isValid():
			print (self.tr("Error: ivalid index"))
			return 
		self.setData (self.index(index.row(), STATE_COLUMN), QtCore.QVariant (newState))

	def deletePiggeon (self, index):
		if not index.isValid():
			print (self.tr("Error: ivalid index"))
			return 

		self.removeRows (index.row (), 1)

	def viewReproduction (self):
		self.setFilter ("state = 'R'")
		self.select ()

	def viewFlyers (self):
		self.setFilter ("state = 'F'")
		self.select ()

	def viewLost (self):
		self.setFilter ("state = 'L'")
		self.select ()

	def resetFilter (self):
		self.setFilter ("")
		self.select ()

	def smartFilter (self, filterText):
# We must adapt this to the new database schema, but essentially this is working
		tokens = str (filterText).split ()
		filter = ""
		if ( len(tokens) ):
			filter += " ( 1 ";
			for i in tokens:
				filter += " AND ( 0 ";
				filter += "or year like '%" + i + "%' ";
				filter += "or identification like '%" + i + "%' ";
				filter += "or nationality_id like '%" + i + "%' ";
				filter += "or sex like '%" + i + "%' ";
				filter += "or plumage_id like '%" + i + "%' ";
				filter += "or eye_id like '%" + i + "%' ";
				filter += "or state like '%" + i + "%' ";
				filter += "or name like '%" + i + "%' ";
				filter += "or mother like '%" + i + "%' ";
				filter += "or father like '%" + i + "%'";
				filter += ")";
			filter += " ) ";
		#print "select * from pigeon where ", filter
		# I don't know why, but this won't work ;-(
		self.setFilter (filter)
		self.select ()

	def getHtml (self):
		""" HTML output, used to print the list """
		while (self.canFetchMore()):
			self.fetchMore()
		htmlDocument = QtXml.QDomDocument ("list")
		html = htmlDocument.createElement ("html")
		htmlDocument.appendChild (html)
		body = htmlDocument.createElement ("body")
		html.appendChild (body)
		table = htmlDocument.createElement ("table")
		table.setAttribute ("style", "border: 1px solid black")
		table.setAttribute ("border", "none")
		body.appendChild (table)
		for i in range (0, self.rowCount()):
			row = htmlDocument.createElement ("tr")
			data = htmlDocument.createElement ("td")
			piggeonId = self.index(i,1).data().toString()
			piggeonId += "-"
			piggeonId += self.index(i,2).data().toString()
			piggeonId += "-"
			piggeonId += self.index(i, 3).data().toString()
			text = htmlDocument.createTextNode (piggeonId)
			data.appendChild (text)
			row.appendChild (data)
			for j in range (4, 10):
				data = htmlDocument.createElement ("td")
				text = htmlDocument.createTextNode (self.index(i, j).data().toString())
				data.appendChild (text)
				row.appendChild (data)
			table.appendChild (row)
#		fichero = file ("/tmp/fichero.html", "w")
#		fichero.write (htmlDocument.toString ())
#		fichero.close ()
		return htmlDocument.toString ()
