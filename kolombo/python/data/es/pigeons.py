#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

#############################################################################
##
## Copyright (C) 2006 Ren� Mart�n Rodr�guez <agarfu@gmail.com>
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

from data.pigeondatabase import pigeon

p = pigeon (
			year = 0,
			identification = "000000",
			nationality = 1,
			sex = 0,
			state = "L",
			plumage = 1,
			eye = 1,
			mother = None,
			father = None,
			name = None,
			note = None,
			rfid = None
		)

p = pigeon (
			year = 0,
			identification = "000000",
			nationality = 1,
			sex = 1,
			plumage = 1,
			state = "L",
			eye = 1,
			mother = None,
			father = None,
			name = None,
			note = None,
			rfid = None
		)
