## Makefile.am for kbird2

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kbird2 kbird2_client

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
kbird2_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kbird2_LDADD   = $(LIB_KFILE) $(LIB_KDEPRINT) 

# which sources should be compiled for kbird2
kbird2_SOURCES = main.cpp kbird2.cpp kbird2view.cpp kbird2iface.skel listados.h \
		listados.cpp listadosWdg.ui pageonewdg.ui configuration.h configuration.cpp \
		ojoWdg.ui gestionparametros.h gestionparametros.cpp ojo.h ojo.cpp plumaje.h \
		plumaje.cpp plumajeWdg.ui nacionWdg.ui nacion.h nacion.cpp tiposueltaWdg.ui \
		tiposuelta.h tiposuelta.cpp parejasWdg.ui parejas.h parejas.cpp nuevaparejaWdg.ui \
		nuevoHijoDlg.ui datosPalomaWdg.ui datospaloma.h datospaloma.cpp arbolGenWdg.ui \
		arbolgen.h arbolgen.cpp buscarDlg.ui buscar.cpp parejasStatsDlg.ui \
		parejasstats.cpp basechart.cpp generalstats.cpp sueltasWdg.ui suelta.cpp pagetwowdg.ui \
		kbird2config.kcfgc updatePrefWdg.ui pref.cpp updatedlg.ui updateDetailsDlg.ui \
		concursosWdg.ui concursos.cpp nuevoConcursoDlg.ui nuevoconcurso.cpp comprobarDlg.ui \
		comprobar.cpp palomardatatable.cpp palomardatatable.h exportarTaurisDlg.ui

# Añadido por mi para adaptar la aplicación a KConfigXT
kde_kcfg_DATA = kbird2.kcfg

# these are the headers for your project
noinst_HEADERS = kbird2.h kbird2view.h pref.h buscar.h parejasstats.h basechart.h generalstats.h suelta.h concursos.h nuevoconcurso.h comprobar.h

# client stuff
kbird2_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kbird2_client_LDADD   = $(LIB_KDECORE)
kbird2_client_SOURCES = kbird2_client.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kbird2.pot

KDE_ICON = kbird2

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Utilities
kdelnk_DATA = kbird2.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kbird2
rc_DATA = kbird2ui.rc
