/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#ifndef _KBIRD2VIEW_H_
#define _KBIRD2VIEW_H_

#include <qwidget.h>
#include <kparts/part.h>
#include <kbird2iface.h>
#include <qscrollview.h> 


class QPainter;
class KURL;
class KJanusWidget;
class listados;
class parejas;
class datosPaloma;
class arbolGen;
class concursos;
class QHBoxLayout;


/**
 * This is the main view class for kbird2.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * This kbird2 uses an HTML component as an example.
 *
 * @short Main view
 * @author René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 * @version 0.1
 */
class kbird2View : public QWidget, public kbird2Iface
{
    Q_OBJECT
public:
	int palomaSeleccionada;
	/**
	 * Default constructor
	 */
    kbird2View(QWidget *parent);

	/**
	 * Destructor
	 */
    virtual ~kbird2View();

    /**
     * Random 'get' function
     */
    QString currentURL();

    /**
     * Random 'set' function accessed by DCOP
     */
    virtual void openURL(QString url);

    /**
     * Random 'set' function
     */
    virtual void openURL(const KURL& url);

    /**
     * Print this view to any medium -- paper or not
     */
    void print(QPainter *, int height, int width);

    /**
    * IconList ...
    */
    KJanusWidget* iconList;

	listados* listadosP;
	parejas* parejasP;
	datosPaloma* nuevaPalomaP;
	datosPaloma* editarPalomaP;
	arbolGen* arbolGenP;
	concursos* concursosP;
	QScrollView* arbolGenScrollView;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
	void slotOnURL(const QString& url);
	void slotSetTitle(const QString& title);
	void cambiaPagina(QWidget *page);

	
protected:
	QHBoxLayout* listadosLayout;
	QHBoxLayout* parejasLayout;
	QHBoxLayout* nuevaPalomaLayout;
	QHBoxLayout* editarPalomaLayout;
	QHBoxLayout* arbolGenLayout;
	QHBoxLayout* concursosLayout;
	QHBoxLayout* arbolGenScrollViewLayout;

/*    KParts::ReadOnlyPart *m_html;*/
};

#endif // _KBIRD2VIEW_H_
