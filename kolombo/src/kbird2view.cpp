/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#include "kbird2view.h"
#include "kbird2.h"
#include "configuration.h"
#include "listados.h"
#include "parejas.h"
#include "datospaloma.h"
#include "arbolgen.h"
#include "concursos.h"

#include <qpainter.h>
#include <qlayout.h>
#include <qframe.h>
#include <qwidget.h>

#include <kurl.h>

#include <ktrader.h>
#include <klibloader.h>
#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <kjanuswidget.h>
#include <kiconloader.h>

kbird2View::kbird2View(QWidget *parent)
    : QWidget(parent),
      DCOPObject("kbird2Iface")
{
	palomaSeleccionada = -1;
	if (config().connectOnStart)
		((kbird2*) parent)->connectDataBase ();
	QFrame* frame;
	// setup our layout manager to automatically add our widgets
	QHBoxLayout *top_layout = new QHBoxLayout(this);
	top_layout->setAutoAdd(true);
	iconList = new KJanusWidget (this, "iconList", KJanusWidget::IconList);

	/* Generación de listados...*/
	frame  = iconList->addPage (i18n("Listados"),
				i18n("GeneraciÃ³n de listados"),
				KGlobal::iconLoader()->loadIcon("revert",KIcon::Panel,0,false));
	listadosLayout = new QHBoxLayout( frame, 0, 0, "listados");
	listadosP = new listados (frame, (kbird2*) parent);
	listadosLayout->addWidget(listadosP);


	// Parejas...
	frame  = iconList->addPage (i18n("Parejas"),
				i18n("Conrol de parejas"),
				KGlobal::iconLoader()->loadIcon("kdmconfig",KIcon::Panel,0,false));
	parejasLayout = new QHBoxLayout( frame, 0, 0, "parejas");
	parejasP = new parejas (frame);
	parejasLayout->addWidget(parejasP);

	// Nueva Paloma...
	frame  = iconList->addPage (i18n("Nueva paloma"),
				i18n("Insertar nueva paloma"),
				KGlobal::iconLoader()->loadIcon("wordprocessing",KIcon::Panel,0,false));
	nuevaPalomaLayout = new QHBoxLayout( frame, 0, 0, "nuevaPaloma");
	nuevaPalomaP = new datosPaloma (frame, false);
	nuevaPalomaLayout->addWidget(nuevaPalomaP);

	// Editar Paloma...
	frame  = iconList->addPage (i18n("Editar paloma"),
				i18n("Editar paloma"),
				KGlobal::iconLoader()->loadIcon("edit",KIcon::Panel,0,false));
	editarPalomaLayout = new QHBoxLayout( frame, 0, 0, "editarPaloma");
	editarPalomaP = new datosPaloma (frame, true);
	editarPalomaLayout->addWidget(editarPalomaP);

	// Arbol Genealógico...
	frame = iconList->addPage (i18n("GenealogÃ­a"),
				i18n("Arbol GenealÃ³gico"),
				KGlobal::iconLoader()->loadIcon("view_tree",KIcon::Panel,0,false));
	//qWarning (QString ("Genealógico   Generación").utf8());
	arbolGenLayout = new QHBoxLayout( frame, 0, 0, "gen");
	arbolGenScrollView = new QScrollView (frame);
	arbolGenLayout->addWidget(arbolGenScrollView);

	arbolGenScrollViewLayout = new QHBoxLayout(arbolGenScrollView, 0, 0);
	arbolGenP = new arbolGen (arbolGenScrollView);
	arbolGenScrollView->addChild (arbolGenP, 0, 0);
	arbolGenScrollViewLayout->addWidget(arbolGenP);


	// Concursos...
	frame = iconList->addPage (i18n("Concursos"),
				i18n("Concursos"),
				KGlobal::iconLoader()->loadIcon("view_tree",KIcon::Panel,0,false));
	//qWarning (QString ("Genealógico   Generación").utf8());
	concursosLayout = new QHBoxLayout( frame, 0, 0, "conc");
	concursosP = new concursos (frame);
	concursosLayout->addWidget(concursosP);


	connect ( iconList, SIGNAL (aboutToShowPage (QWidget *)), this, SLOT(cambiaPagina (QWidget *)));
/*
    // we want to look for all components that satisfy our needs.  the
    // trader will actually search through *all* registered KDE
    // applications and components -- not just KParts.  So we have to
    // specify two things: a service type and a constraint
    //
    // the service type is like a mime type.  we say that we want all
    // applications and components that can handle HTML -- 'text/html'
    //
    // however, by itself, this will return such things as Netscape..
    // not what we wanted.  so we constrain it by saying that the
    // string 'KParts/ReadOnlyPart' must be found in the ServiceTypes
    // field.  with this, only components of the type we want will be
    // returned.
    KTrader::OfferList offers = KTrader::self()->query("text/html", "'KParts/ReadOnlyPart' in ServiceTypes");

    KLibFactory *factory = 0;
    // in theory, we only care about the first one.. but let's try all
    // offers just in case the first can't be loaded for some reason
    KTrader::OfferList::Iterator it(offers.begin());
    for( ; it != offers.end(); ++it)
    {
        KService::Ptr ptr = (*it);

        // we now know that our offer can handle HTML and is a part.
        // since it is a part, it must also have a library... let's try to
        // load that now
        factory = KLibLoader::self()->factory( ptr->library() );
        if (factory)
        {
            m_html = static_cast<KParts::ReadOnlyPart *>(factory->create(this, ptr->name(), "KParts::ReadOnlyPart"));
            break;
        }

    }

    // if our factory is invalid, then we never found our component
    // and we might as well just exit now
    if (!factory)
    {
        KMessageBox::error(this, i18n("Could not find a suitable HTML component"));
        return;
    }

    connect(m_html, SIGNAL(setWindowCaption(const QString&)),
            this,   SLOT(slotSetTitle(const QString&)));
    connect(m_html, SIGNAL(setStatusBarText(const QString&)),
            this,   SLOT(slotOnURL(const QString&)));
*/
}

kbird2View::~kbird2View()
{
}

void kbird2View::print(QPainter *p, int height, int width)
{
    // do the actual printing, here
    // p->drawText(etc..)
}

QString kbird2View::currentURL()
{
//    return m_html->url().url();
	return ("No está implementada...");
}

void kbird2View::openURL(QString url)
{
    openURL(KURL(url));
}

void kbird2View::cambiaPagina(QWidget *page)
{
	QString palomaID;
	palomaID = palomaID.setNum (palomaSeleccionada);
}


void kbird2View::openURL(const KURL& url)
{
//    m_html->openURL(url);
	qWarning ("No está implementado ...");
}

void kbird2View::slotOnURL(const QString& url)
{
    emit signalChangeStatusbar(url);
}

void kbird2View::slotSetTitle(const QString& title)
{
    emit signalChangeCaption(title);
}
#include "kbird2view.moc"
