/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#ifndef _KBIRD2_H_
#define _KBIRD2_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
#include <kmainwindow.h>
#include <qsqldatabase.h>

#include "kbird2view.h"
#include "kbird2config.h"

class QPoint;
class KPrinter;
class KToggleAction;
class KURL;

/**
 * This class serves as the main window for kbird2.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author $AUTHOR <$EMAIL>
 * @version $APP_VERSION
 */
class kbird2 : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    kbird2();

    /**
     * Default Destructor
     */
    virtual ~kbird2();

    /**
     * Use this method to load whatever file/URL you have
     */
    void load(const KURL& url);

    /**
     * Conecta con la base de dtos...
     */
    void connectDataBase ();
    QSqlDatabase *defaultDB;

protected:
    /**
     * Overridden virtuals for Qt drag 'n drop (XDND)
     */
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

protected:
    /**
     * This function is called when it is time for the app to save its
     * properties for session management purposes.
     */
    void saveProperties( );

    /**
     * This function is called when this app is restored.  The KConfig
     * object points to the session management config file that was saved
     * with @ref saveProperties
     */
    void readProperties( );

public slots:
	void popupRequested();
	void contextMenu (int row, int col, const QPoint & pos);
	
private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filePrint();
    void optionsShowToolbar();
    void optionsShowStatusbar();
    void optionsConfigureKeys();
    void optionsConfigureToolbars();
    void optionsPreferences();
    void newToolbarConfig();

    void changeStatusbar(const QString& text);
    void changeCaption(const QString& text);
	
	void gestionParametrosSlot();
	void buscarPalomaSlot();    
	void verReproduccion();
	void verVuelo();
	void verDesaparecida();
	void verTodas();
	void verCenso();
	void actualizarSlot();
	void backupSlot();
	void cambiaPagina(QWidget *pagina);
	void eliminarPaloma();
	void cambioSexo();
	void estadoDesaparecida();
	void estadoVuelo();
	void estadoReproduccion();
	void actionesListadosOn (int row, int col, int button, const QPoint &p);
	void actionesParejasOn (int row, int col, int button, const QPoint &p);
	void actionesListadosOff (int row, int col, int button, const QPoint &p);

private:
	kbird2Config configKCFG;
	QString tempFile;
    void setupAccel();
    void setupActions();

private:
    kbird2View *m_view;

    KPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _KBIRD2_H_
