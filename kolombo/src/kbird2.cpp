/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#include "buscar.h"
#include "kbird2.h"
#include "kbird2view.h"
#include "pref.h"
#include "configuration.h"
#include "gestionparametros.h"
#include "listados.h"
#include "parejas.h"
#include "updatedlg.h"
#include "updateDetailsDlg.h"
#include "palomardatatable.h"


#include <qdragobject.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qstring.h>
#include <qdatatable.h>
#include <qsqlrecord.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qsize.h>
#include <qpoint.h>
#include <qlineedit.h>
#include <qdom.h>

#include <kcmdlineargs.h>
#include <ktextedit.h> 
#include <kprinter.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kdeversion.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kio/netaccess.h>
#include <kio/job.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurldrag.h>
#include <kurlrequesterdlg.h>
#include <kmessagebox.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kjanuswidget.h>

//------------- Xerces --------------
/*#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xercesc;
//-----------------------------------*/

kbird2::kbird2()
    : KMainWindow( 0, "kbird2" ),
      m_view(new kbird2View(this)),
      m_printer(0)
{
	// accept dnd
	setAcceptDrops(true);
	
	// tell the KMainWindow that this is indeed the main widget
	setCentralWidget(m_view);
	
	// then, setup our actions
	setupActions();
	
	// and a status bar
	statusBar()->show();
	
	// apply the saved mainwindow settings, if any, and ask the mainwindow
	// to automatically save settings if changed: window size, toolbar
	// position, icon size, etc.
	setAutoSaveSettings();
	
	// allow the view to change the statusbar and caption

	connect(m_view, SIGNAL(signalChangeStatusbar(const QString&)),
		this,   SLOT(changeStatusbar(const QString&)));
	connect(m_view, SIGNAL(signalChangeCaption(const QString&)),
		this,   SLOT(changeCaption(const QString&)));

	connect ( m_view->iconList, SIGNAL (aboutToShowPage (QWidget *)),
				this, SLOT(cambiaPagina (QWidget *)));
	connect ( m_view->listadosP->tablaListados, 
				SIGNAL (clicked (int, int, int, const QPoint&)),
				this, SLOT(actionesListadosOn (int, int, int, const QPoint&)));
	connect ( m_view->parejasP->tablaHijos, 
				SIGNAL (clicked (int, int, int, const QPoint&)),
				this, SLOT(actionesListadosOn (int, int, int, const QPoint&)));
	connect ( m_view->parejasP->tablaParejas, 
				SIGNAL (clicked (int, int, int, const QPoint&)),
				this, SLOT(actionesParejasOn (int, int, int, const QPoint&)));
	connect ( m_view->parejasP->tablaParejas, 
				SIGNAL (clicked (int, int, int, const QPoint&)),
				this, SLOT(actionesListadosOff (int, int, int, const QPoint&)));

	cambiaPagina(m_view->iconList->pageWidget(0));
	tempFile = "";
}

kbird2::~kbird2()
{
}

void kbird2::cambiaPagina(QWidget *pagina) 
{
	qWarning ("Página cambiada: ");
	//actionCollection()->action ()->setEnabled(false);
	actionCollection()->action ("eliminarPalomaAction")->setEnabled(false);
	actionCollection()->action ("cambiarSexoAction")->setEnabled(false);
	actionCollection()->action ("estadoDesaparecidaAction")->setEnabled(false);
	actionCollection()->action ("estadoVueloAction")->setEnabled(false);
	actionCollection()->action ("estadoReproduccionAction")->setEnabled(false);
	actionCollection()->action ("eliminarParejaAction")->setEnabled(false);
	actionCollection()->action ("editarParejaAction")->setEnabled(false);
	actionCollection()->action ("nuevaParejaAction")->setEnabled(false);
	actionCollection()->action ("nuevoHijoAction")->setEnabled(false);
	actionCollection()->action ("parejasStatsAction")->setEnabled(false);
	switch (m_view->iconList->pageIndex(pagina))
	{
		case 0:
			qWarning("Listados");
/*			actionCollection()->action ("eliminarPalomaAction")->setEnabled(true);
			actionCollection()->action ("cambiarSexoAction")->setEnabled(true);
			actionCollection()->action ("estadoDesaparecidaAction")->setEnabled(true);
			actionCollection()->action ("estadoVueloAction")->setEnabled(true);
			actionCollection()->action ("estadoReproduccionAction")->setEnabled(true);*/
			break;
		
		case 1:
			qWarning ("Parejas");
/*			actionCollection()->action ("eliminarParejaAction")->setEnabled(true);
			actionCollection()->action ("editarParejaAction")->setEnabled(true);*/
			actionCollection()->action ("nuevaParejaAction")->setEnabled(true);
// 			actionCollection()->action ("nuevoHijoAction")->setEnabled(true);
// 			actionCollection()->action ("eliminarPalomaAction")->setEnabled(true);
// 			actionCollection()->action ("cambiarSexoAction")->setEnabled(true);
// 			actionCollection()->action ("estadoDesaparecidaAction")->setEnabled(true);
// 			actionCollection()->action ("estadoVueloAction")->setEnabled(true);
// 			actionCollection()->action ("estadoReproduccionAction")->setEnabled(true);

			break;
		
		case 2:
			qWarning ("Nueva paloma");
			break;
			
		case 3:
			qWarning ("Editar paloma");
			break;
			
		case 4:
			qWarning ("Genealogía");
			break;
			
		default:
			qWarning ("No Esperado");
			break;
	}
}

void kbird2::load(const KURL& url)
{
    QString target;
    // the below code is what you should normally do.  in this
    // example case, we want the url to our own.  you probably
    // want to use this code instead for your app

    #if 0
    // download the contents
    if (KIO::NetAccess::download(url, target))
    {
        // set our caption
        setCaption(url);

        // load in the file (target is always local)
        loadFile(target);

        // and remove the temp file
        KIO::NetAccess::removeTempFile(target);
    }
    #endif

    setCaption(url.prettyURL());
    m_view->openURL(url);
}

void kbird2::setupActions()
{
    KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    KStdAction::save(this, SLOT(fileSave()), actionCollection());
    KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
    KStdAction::print(this, SLOT(filePrint()), actionCollection());
    KStdAction::quit(kapp, SLOT(quit()), actionCollection());

    m_toolbarAction = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
    m_statusbarAction = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
    KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
	KAction *ManageParameters = new KAction (i18n("&Gestionar parÃ¡metros"), "package_utilities",
					CTRL+Key_P, this, SLOT (gestionParametrosSlot()),
					actionCollection(), "ManageParameters");

	(void) new KAction(i18n("Ver reproducciÃ³n"), 0,
					this, SLOT(verReproduccion()),
					actionCollection(), "verReproducionAction");
	 (void) new KAction(i18n("Ver vuelo"), 0,
					this, SLOT(verVuelo()),
					actionCollection(), "verVueloAction");
	(void) new KAction(i18n("Ver desaparecidas"), 0,
					this, SLOT(verDesaparecida()),
					actionCollection(), "verDesaparecidaAction");
	(void) new KAction(i18n("Ver todas"), 0,
					this, SLOT(verTodas()),
					actionCollection(), "verTodasAction");
	(void) new KAction(i18n("Ver Censo"), 0,
					this, SLOT(verCenso()),
					actionCollection(), "verCensoAction");
	(void) new KAction(i18n("Buscar paloma"), "find", CTRL+Key_B,
					this, SLOT(buscarPalomaSlot()),
					actionCollection(), "buscarAction");
	(void) new KAction(i18n("EstadÃsticas generales"), "log",
					CTRL+SHIFT+Key_G,
					m_view->listadosP, SLOT(generalStatsSlot()),
					actionCollection(), "generalStatsAction");

	(void) new KAction(i18n("Actualizar"),
					CTRL+SHIFT+Key_U,
					this, SLOT(actualizarSlot()),
					actionCollection(), "updAction");

	(void) new KAction(i18n("Realizar copia de seguridad"), "backup",
					this, SLOT(backupSlot()),
					actionCollection(), "bkpAction");

    (void) new KAction(i18n("Exportar palomas para TaurisClub"), "log",
                    CTRL+SHIFT+Key_P,
                    m_view->listadosP, SLOT(pigeonsTauris()),
                    actionCollection(), "TaurisPigeonFile");

	(void) new KAction(i18n("Eliminar paloma"), "button_cancel",
					CTRL+Key_D,
					this, SLOT(eliminarPaloma()),
					actionCollection(), "eliminarPalomaAction");
	(void) new KAction(i18n("Cambiar de sexo"), "reload",
					CTRL+Key_S,
					this, SLOT(cambioSexo()),
					actionCollection(), "cambiarSexoAction");
	(void) new KAction(i18n("Estado desaparecida"), "lost",
					CTRL+SHIFT+Key_D,
					this, SLOT(estadoDesaparecida()),
					actionCollection(), "estadoDesaparecidaAction");
	(void) new KAction(i18n("Estado vuelo"), "lost",
					CTRL+SHIFT+Key_V,
					this, SLOT(estadoVuelo()),
					actionCollection(), "estadoVueloAction");

	(void) new KAction(i18n("Estado reproduccion"), "lost",
					CTRL+SHIFT+Key_D,
					this, SLOT(estadoReproduccion()),
					actionCollection(), "estadoReproduccionAction");


	(void) new KAction(i18n("Eliminar pareja"), "button_cancel",
					CTRL+Key_D,
					m_view->parejasP, SLOT(eliminarPareja()),
					actionCollection(), "eliminarParejaAction");
	(void) new KAction(i18n("Editar pareja"), "edit",
					CTRL+Key_S,
					m_view->parejasP, SLOT(editarPareja()),
					actionCollection(), "editarParejaAction");
	(void) new KAction(i18n("Nueva pareja"), "new",
					CTRL+SHIFT+Key_D,
					m_view->parejasP, SLOT(nuevaPareja()),
					actionCollection(), "nuevaParejaAction");
	(void) new KAction(i18n("Nuevo hijo"), "new",
					CTRL+SHIFT+Key_V,
					m_view->parejasP, SLOT(nuevoHijo()),
					actionCollection(), "nuevoHijoAction");
	(void) new KAction(i18n("EstadÃsticas"), "log",
					CTRL+SHIFT+Key_S,
					m_view->parejasP, SLOT(estadisticas()),
					actionCollection(), "parejasStatsAction");
    

	createGUI("/home/heaven/cvs/kbird2/src/kbird2ui.rc");

}

//void kbird2::saveProperties(KConfig *config)
void kbird2::saveProperties()
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
qWarning ("GUARDA las preferencias del objeto de configuracion...");
/*    if (!m_view->currentURL().isEmpty()) {
#if KDE_IS_VERSION(3,1,3)
        config->writePathEntry("lastURL", m_view->currentURL());
#else
        config->writeEntry("lastURL", m_view->currentURL());
#endif
    }
    */
}

//void kbird2::readProperties(KConfig *config)
void kbird2::readProperties()
{
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'
	qWarning ("LEE las preferencias del objeto de configuracion...");
    /*QString url = config->readPathEntry("lastURL");

    if (!url.isEmpty())
        m_view->openURL(KURL(url));
*/
}

void kbird2::dragEnterEvent(QDragEnterEvent *event)
{
    // accept uri drops only
    event->accept(KURLDrag::canDecode(event));
}

void kbird2::dropEvent(QDropEvent *event)
{
    // this is a very simplistic implementation of a drop event.  we
    // will only accept a dropped URL.  the Qt dnd code can do *much*
    // much more, so please read the docs there
    KURL::List urls;

    // see if we can decode a URI.. if not, just ignore it
    if (KURLDrag::decode(event, urls) && !urls.isEmpty())
    {
        // okay, we have a URI.. process it
        const KURL &url = urls.first();

        // load in the file
        load(url);
    }
}

void kbird2::fileNew()
{
    // this slot is called whenever the File->New menu is selected,
    // the New shortcut is pressed (usually CTRL+N) or the New toolbar
    // button is clicked

    // create a new window
    (new kbird2)->show();
}

void kbird2::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
/*
    // this brings up the generic open dialog
    KURL url = KURLRequesterDlg::getURL(QString::null, this, i18n("Open Location") );
*/
    // standard filedialog
    KURL url = KFileDialog::getOpenURL(QString::null, QString::null, this, i18n("Open Location"));
    if (!url.isEmpty())
        m_view->openURL(url);
}

void kbird2::fileSave()
{
    // this slot is called whenever the File->Save menu is selected,
    // the Save shortcut is pressed (usually CTRL+S) or the Save toolbar
    // button is clicked

    // save the current file
}

void kbird2::fileSaveAs()
{
    // this slot is called whenever the File->Save As menu is selected,
    KURL file_url = KFileDialog::getSaveURL();
    if (!file_url.isEmpty() && !file_url.isMalformed())
    {
        // save your info, here
    }
}

void kbird2::filePrint()
{
    // this slot is called whenever the File->Print menu is selected,
    // the Print shortcut is pressed (usually CTRL+P) or the Print toolbar
    // button is clicked
    if (!m_printer) m_printer = new KPrinter;
    if (m_printer->setup(this))
    {
        // setup the printer.  with Qt, you always "print" to a
        // QPainter.. whether the output medium is a pixmap, a screen,
        // or paper
        QPainter p;
        p.begin(m_printer);
		m_view->listadosP->print(&p, m_printer);
        // we let our view do the actual printing
        QPaintDeviceMetrics metrics(m_printer);
        m_view->print(&p, metrics.height(), metrics.width());

        // and send the result to the printer
        p.end();
    }
}

void kbird2::backupSlot()
{
	qWarning("BackupSlot");
}

void kbird2::optionsShowToolbar()
{
    // this is all very cut and paste code for showing/hiding the
    // toolbar
    if (m_toolbarAction->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void kbird2::optionsShowStatusbar()
{
    // this is all very cut and paste code for showing/hiding the
    // statusbar
    if (m_statusbarAction->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void kbird2::optionsConfigureKeys()
{
    KKeyDialog::configureKeys(actionCollection(), "kbird2ui.rc");
}

void kbird2::optionsConfigureToolbars()
{
    // use the standard toolbar editor
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    saveMainWindowSettings(KGlobal::config());
# endif
#else
    saveMainWindowSettings(KGlobal::config());
#endif
}

void kbird2::newToolbarConfig()
{
    // this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
    // recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)
    createGUI();

#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    applyMainWindowSettings(KGlobal::config());
# endif
#else
    applyMainWindowSettings(KGlobal::config());
#endif
}

// Dialogo para buscar una paloma
void kbird2::buscarPalomaSlot() {
	buscar dlg;
	if (dlg.exec() == QDialog::Accepted )
		m_view->listadosP->tablaListados->verBusqueda(dlg.consulta);
}

// Dialogo para gestionar parámetros (ojos, plumaje, sueltas ...)
void kbird2::gestionParametrosSlot() {
	gestionParametros dlg;
	dlg.exec();
}

void kbird2::optionsPreferences()
{
    // popup some sort of preference dialog, here
    kbird2Preferences dlg;

    if (dlg.exec())
    {
        // redo your settings
    }
}

void kbird2::changeStatusbar(const QString& text)
{
    // display the text on the statusbar
    statusBar()->message(text);
}

void kbird2::changeCaption(const QString& text)
{
    // display the text on the caption
    setCaption(text);
}


void kbird2::connectDataBase () {
    // Base de datos
    /*
     * Bueno, esto parece bastante sencillo. Si queremos cambiar de SGBD solo hay que cambiar "QPSQL7" por el nombre del driver
     * así que me parece que esto es portable ande sea, quiero decir la DB, porque hay driver ODBC, con lo que los SQLServer no se nos resistirán y
     * hay driver nativo para ORACLE, por haber hay hasta para SQLite.
     */

	if (config().driverDB != "NONE") {
		defaultDB = QSqlDatabase::addDatabase(config().driverDB, "palomar");
		defaultDB->setDatabaseName(config().nombreDB);
		defaultDB->setUserName(config().userDB);
		defaultDB->setPassword(config().passDB);
		defaultDB->setHostName(config().hostDB);
		if ( ! defaultDB->open() ) {
			KMessageBox::error (this, i18n("Fallo al abrir la base de datos: ")
				+ defaultDB->lastError().driverText(),
				i18n ("ConexiÃ³n a la base de datos"));
				qWarning( defaultDB->lastError().databaseText() );
		}
	} else
		defaultDB = NULL;
}

void kbird2::verReproduccion() {
	m_view->listadosP->tablaListados->verReproduccion();
}

void kbird2::verVuelo() {
	m_view->listadosP->tablaListados->verVuelo();
}

void kbird2::actualizarSlot() {
/*	updateDlg dlg;
	dlg.versionURL->setText(configKCFG.updateURL());
	if (dlg.exec() == QDialog::Accepted) {
		qWarning ("A buscar actualizaciones");*/
		/*
		* Si tenemos un fichero temporal en /tmp lo eliminamos, si es local el
		* fichero espero que esto no lo borre tal y como dice en la documentación.
		*/
		/*if (tempFile.isEmpty())
			KIO::NetAccess::removeTempFile(tempFile);
		*/
		/*
		* Esto es lo que hace que mi piel sea más suave y esas cosas, si
		* KIO::NetAccess::download es true significa que ha descargado correctamente
		* el fichero.
		*/
		//tempFile = "";
		//if (KIO::NetAccess::download(file_name, tempFile, m_widget))
		/*if (KIO::NetAccess::download(dlg.versionURL->text(), tempFile))
		{
*/
			/***** Xerces ******/
		/*	try {
				XMLPlatformUtils::Initialize();
				qWarning ("inicializado");
			}
			catch (const XMLException& toCatch) {
					char* message = XMLString::transcode(toCatch.getMessage());
					qWarning ("Error during initialization! :\n" + QString (message) + "\n");
					XMLString::release(&message);
					return;
			}
			XercesDOMParser* parser = new XercesDOMParser();
			parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
			parser->setDoSchema (true);
			parser->setLoadExternalDTD(true);
			parser->setValidationConstraintFatal(true);
			parser->setExitOnFirstFatalError(true);
			parser->setDoNamespaces(true);
			parser->setValidationSchemaFullChecking(true);
			
			ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
			parser->setErrorHandler(errHandler);
			try {
					parser->parse(tempFile.ascii());
					qWarning ("Parseado y validado");
			}
			catch (const XMLException& toCatch) {
					char* message = XMLString::transcode(toCatch.getMessage());
					qWarning ("Exception message is: \n" + QString (message) + "\n");
					XMLString::release(&message);
					return;
			}
			catch (const DOMException& toCatch) {
					char* message = XMLString::transcode(toCatch.msg);
					qWarning ("Exception message is: \n" + QString (message) + "\n");
					XMLString::release(&message);
					return;
			}
			catch (const SAXException& toCatch) {
					qWarning ("Unexpected Exception");
					char* message = XMLString::transcode(toCatch.getMessage ());
					qWarning ("Exception message is: \n" + QString (message) + "\n");
					XMLString::release(&message);
					return;
			}*/
			/*******************/
		//	QFile file (tempFile);
		// Cacho de código de GOYO para el korei .... gracias Goyito !!!!
		/*	QDomDocument *updateXMLDoc = new QDomDocument();
			if ( !file.open( IO_ReadOnly ) ) {
				qWarning("No se ha podido abrir el fichero " + tempFile);
				return;
			}
			int *errLine = new int;
			int *errColumn = new int;
			QString *errMsg = new QString();
			if ( !updateXMLDoc->setContent( &file, false,  errMsg, errLine, errColumn) ) {
				qWarning("No se ha podido abrir: " + tempFile);
				QString str;
				qWarning("There is an error at line: " + str.number(*errLine) + " Column: " + str.number(*errColumn));
				qWarning("*************Parser error Message*****************");
				qWarning(*errMsg);
				file.close();
				return;
			}
			// Tomamos la raíz del árbol
		
			QDomElement *root = new QDomElement(updateXMLDoc->documentElement());
			if (root->tagName()!="update") {
				qWarning("Esto no es un fichero de definición de actualizaciones válido");
				return;
			}
			updateDetailsDlg detailDlg;
			QString resumen;
			KURL binURL, srcURL, debURL, rpmURL;
			resumen.append("<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:12pt;font-family:helvetica\"><a name=\"top\">");
			QDomNode n = root->firstChild();
			// Cargamos todas las definiciones de programadores
			QDomElement aux;
			while( !n.isNull() ) {
				aux = n.toElement();
				if (aux.attribute("current") == "true") {
					qWarning ("Se ha encontrado la versión más moderna");
					resumen.append("<P align=\"center\"><B>Versión:</B> " + aux.attribute("number") + "</p><br><br>");
					QDomNodeList listaNodos = n.childNodes();
					int i = 0;
					while (!listaNodos.item(i).isNull()) {
						aux = listaNodos.item(i).toElement();
						if (aux.tagName() == "url") {
								if (aux.attribute("type") == "bin") {
									resumen.append("<B>Fichero binario:</B> ");
									binURL = aux.text();
									resumen.append("<A HREF=\"" + aux.text() + "\">" + aux.text() + "</A><BR>");
									qWarning ("La ruta para el binario: " + aux.text());
								}
								if (aux.attribute("type") == "src") {
									resumen.append("<B>Fuentes:</B> ");
									srcURL = aux.text();
									resumen.append("<A HREF=\"" + aux.text() + "\">" + aux.text() + "</A><BR>");
									qWarning ("La ruta para el binario: " + aux.text());
								}
								if (aux.attribute("type") == "deb") {
									resumen.append("<B>DEB:</B> ");
									debURL = aux.text();
									resumen.append("<A HREF=\"" + aux.text() + "\">" + aux.text() + "</A><BR>");
								}
								if (aux.attribute("type") == "rpm") {
									resumen.append("<B>RPM:</B> ");
									rpmURL = aux.text();
									resumen.append("<A HREF=\"" + aux.text() + "\">" + aux.text() + "</A><BR>");
								}
								i++;
								continue;
						}
						if (aux.tagName() == "changelog") {
								resumen.append("<P align=\"center\"><H1>Changelog</H1></p><br>");
								QDomNodeList listaChanges = aux.childNodes();
								int j = 0;
								while (!listaChanges.item(j).isNull()) {
									aux = listaChanges.item(j).toElement();
									if (aux.tagName() == "optimizations") {
										resumen.append("<B>Optimizaciones:</B><ul type=\"disc\">");
										QDomNodeList listaItems = aux.childNodes();
										int k = 0;
										while (!listaItems.item(k).isNull()) {
											aux = listaItems.item(k).toElement();
											resumen.append("<li>" + aux.text() + "</li>");
											k++;
										}
										resumen.append("</ul><br>");
										j++;
										continue;
									}
									if (aux.tagName() == "fixes") {
										resumen.append("<B>Correcciones:</B><ul>");
										QDomNodeList listaItems = aux.childNodes();
										int k = 0;
										while (!listaItems.item(k).isNull()) {
											aux = listaItems.item(k).toElement();
											resumen.append("<li>" + aux.text() + "</li>");
											k++;
										}
										resumen.append("</ul><br>");
										j++;
										continue;
									}
									if (aux.tagName() == "features") {
										resumen.append("<B>Nuevas características:</B><ul>");
										QDomNodeList listaItems = aux.childNodes();
										int k = 0;
										while (!listaItems.item(k).isNull()) {
											aux = listaItems.item(k).toElement();
											resumen.append("<li>" + aux.text() + "</li>");
											k++;
										}
										resumen.append("</ul><br>");
										j++;
										continue;
									}
									j++;
								}
								i++;
								continue;
						}
						if (aux.tagName() == "todo") {
								resumen.append("<br><p align=\"center\"><H1>Cosas por hacer</H1></p><ul>");
								QDomNodeList listaTODO = aux.childNodes();
								int j = 0;
								QDomElement TODO;
								while (!listaTODO.item(j).isNull()) {
									TODO = listaTODO.item(j).toElement();
									resumen.append("<li>" + TODO.text() + "</li>");
									j++;
								}
								resumen.append("</ul>");
								i++;
								continue;
						}
						qWarning ("Esto tiene alguna etiqueta no válida");
						i++;
					}
					break;
				}
				n = n.nextSibling();
			}
			resumen.append("</body></html>");
			detailDlg.textEdit->setText(resumen);
			detailDlg.textEdit->setReadOnly(true);
			detailDlg.textEdit->scrollToAnchor ("top");
			if (detailDlg.exec() == QDialog::Accepted)
			{
				KIO::move("file:" + KCmdLineArgs::cwd () + "/" + KCmdLineArgs::appName(),
					"file:" + KCmdLineArgs::cwd () + "/" + KCmdLineArgs::appName() + "-" + configKCFG.version(), true);
				KIO::copyAs (binURL, "file:" + KCmdLineArgs::cwd () + "/" + KCmdLineArgs::appName(), true);
			}
			file.close();
			configKCFG.writeConfig();
		}
		else {
			KMessageBox::error(this, i18n("No se ha podido abrir el fichero de actualizaciones"));
		}
	}*/
	qWarning ("update");
}

void kbird2::verDesaparecida() {
	m_view->listadosP->tablaListados->verDesaparecida();
}


void kbird2::verTodas() {
	m_view->listadosP->tablaListados->verTodas();
}


void kbird2::verCenso() {
	m_view->listadosP->tablaListados->verCenso();
}

void kbird2::popupRequested()
{
    QWidget *w = factory()->container("contextoLista", this);
    QPopupMenu *popup = static_cast<QPopupMenu *>(w);
    popup->exec(QCursor::pos());
}

void kbird2::contextMenu (int row, int col, const QPoint & pos) {
    QWidget *w = factory()->container("contextoLista", this);
    QPopupMenu *popup = static_cast<QPopupMenu *>(w);
    popup->exec(QCursor::pos());
}



void kbird2::eliminarPaloma()
{
	if (m_view->iconList->activePageIndex())
		m_view->parejasP->eliminarPaloma();
	else
		m_view->listadosP->tablaListados->eliminarPaloma();
}
void kbird2::cambioSexo()
{
	if (m_view->iconList->activePageIndex())
		m_view->parejasP->cambioSexo();
	else
		m_view->listadosP->tablaListados->cambioSexo();
}
void kbird2::estadoDesaparecida()
{
	if (m_view->iconList->activePageIndex())
		m_view->parejasP->estadoDesaparecida();
	else
		m_view->listadosP->tablaListados->estadoDesaparecida();
}
void kbird2::estadoVuelo()
{
	if (m_view->iconList->activePageIndex())
		m_view->parejasP->estadoVuelo();
	else
		m_view->listadosP->tablaListados->estadoVuelo();
}
void kbird2::estadoReproduccion()
{
	if (m_view->iconList->activePageIndex())
		m_view->parejasP->estadoReproduccion();
	else
		m_view->listadosP->tablaListados->estadoReproduccion();
}

void kbird2::actionesListadosOff (int row, int col, int button, const QPoint &p)
{
	actionCollection()->action ("eliminarPalomaAction")->setEnabled(false);
	actionCollection()->action ("cambiarSexoAction")->setEnabled(false);
	actionCollection()->action ("estadoDesaparecidaAction")->setEnabled(false);
	actionCollection()->action ("estadoVueloAction")->setEnabled(false);
	actionCollection()->action ("estadoReproduccionAction")->setEnabled(false);
}


void kbird2::actionesListadosOn (int row, int col, int button, const QPoint &p)
{
	actionCollection()->action ("eliminarPalomaAction")->setEnabled(true);
	actionCollection()->action ("cambiarSexoAction")->setEnabled(true);
	actionCollection()->action ("estadoDesaparecidaAction")->setEnabled(true);
	actionCollection()->action ("estadoVueloAction")->setEnabled(true);
	actionCollection()->action ("estadoReproduccionAction")->setEnabled(true);
}

void kbird2::actionesParejasOn (int row, int col, int button, const QPoint &p)
{

			actionCollection()->action ("eliminarParejaAction")->setEnabled(true);
			actionCollection()->action ("editarParejaAction")->setEnabled(true);
			actionCollection()->action ("nuevoHijoAction")->setEnabled(true);
			actionCollection()->action ("parejasStatsAction")->setEnabled(true);
}

#include "kbird2.moc"
