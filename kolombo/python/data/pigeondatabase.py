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


from sqlobject import *

class eyeColor(SQLObject):
	name = StringCol ()	

class plumageColor(SQLObject):
	shortName = StringCol ()
	longName = StringCol ()

class nation(SQLObject):
	shortName = StringCol ()
	longName = StringCol ()

class yearColor(SQLObject):
	year = IntCol ()
	r = IntCol ()
	g = IntCol ()
	b = IntCol ()

class pigeon(SQLObject):
	year = IntCol ()
	identification = StringCol ()
	nationality = ForeignKey ("nation")
	sex = BoolCol ()
	mother = IntCol (notNone = False)
	father = IntCol (notNone = False)
	state = StringCol ()
	plumage = ForeignKey ("plumageColor")
	eye = ForeignKey ("eyeColor")
	state = StringCol ()
	name = StringCol ()
	note = StringCol ()
	rfid = StringCol ()

class pair(SQLObject):
	mother = ForeignKey ("pigeon")
	father = ForeignKey ("pigeon")
