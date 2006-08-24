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

import os
from fnmatch import fnmatch
from PyQt4 import QtXml

RESOURCES=os.path.join (".", "resources")

def listdir(root, path=""):
	files = []
	try:
		for file in os.listdir(os.path.join(root, path)):
			pathname = os.path.join(path, file)
			if os.path.isdir(os.path.join(root, pathname)):
				files.extend(listdir(root, pathname))
			else:
				files.append(pathname)
	except OSError:
		pass
	return files

def genProFile(pro):
	"""Search for all .py files in this directory recursively and create a pro
	file"""

	def justPY (filename): return fnmatch(filename, "*.py")
	def justTS (filename): return fnmatch(filename, "*.ts")

	sources = "\\\n\t\t".join (filter (justPY, listdir(".")))
	translations = "\\\n\t\t".join (filter (justTS, listdir(".")))

	proContents = """
TEMPLATE = subdirs

SOURCES = %s

TRANSLATIONS = %s
""" % (sources, translations)

	pro = file (pro, "w")
	pro.write (proContents)
	pro.close ()


def genQrcFile (qrc):
	""" Search for any file in RESOURCES and add an entrance in qrc for it """

	files = listdir (RESOURCES)
	qrcDocument = QtXml.QDomDocument ("RCC")
	rcc = qrcDocument.createElement ("RCC")
	rcc.setAttribute ("version", "1.0")
	qrcDocument.appendChild (rcc)
	qresource = qrcDocument.createElement ("qresource")
	rcc.appendChild (qresource)
	for i in files:
		if not os.path.isdir (i):
			f = qrcDocument.createElement ("file")
			f.appendChild(qrcDocument.createTextNode (os.path.join (RESOURCES, i)))
			qresource.appendChild (f)

	qrc = file (qrc, "w")
	qrc.write (qrcDocument.toString ())
	qrc.close ()

genProFile ("kolombo.pro")
genQrcFile ("kolombo.qrc")
