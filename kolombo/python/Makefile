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


UIDIR=./ui
DATADIR=./data
LANGDIR=./resources/lang
UIC=/usr/bin/pyuic4
LUPDATE=/usr/bin/pylupdate4
LRELEASE=/usr/bin/lrelease-qt4
RCC=/usr/bin/pyrcc4


UIS := $(patsubst %.ui,%.py,$(wildcard $(UIDIR)/*.ui))

all: $(UIS) kolombo_rc.py update_translations release_translations

%.py: %.ui
	$(UIC) $< > $@

update_translations: kolombo.pro
	$(LUPDATE) kolombo.pro

release_translations: kolombo.pro
	$(LRELEASE) kolombo.pro

kolombo_rc.py: kolombo.qrc
	$(RCC) kolombo.qrc -o kolombo_rc.py

kolombo.pro kolombo.qrc:
	./genprojectfiles.py

clean:
	rm -f kolombo.pro
	rm -f kolombo.qrc
	rm -f kolombo_rc.py
	rm -f ./*pyc
	rm -f $(UIDIR)/*.py
	rm -f $(UIDIR)/*.pyc
	rm -f $(DATADIR)/*.pyc
	rm -f $(DATADIR)/es/*.pyc
	rm -f $(LANGDIR)/*.qm
	touch $(UIDIR)/__init__.py
