/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#include "pref.h"
#include "configuration.h"
#include "pageonewdg.h"
#include "pagetwowdg.h"
#include "updatePrefWdg.h"

#include <klocale.h>
#include <kmessagebox.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qsqldatabase.h>
#include <qcombobox.h>
#include <qsqlrecord.h>
#include <qsqlquery.h>
#include <qvaluelist.h>


kbird2Preferences::kbird2Preferences()
    : KDialogBase(TreeList, i18n("kbird2 Preferences"),
                  Ok|Apply|Cancel, Ok)
{
	// this is the base class for your preferences dialog.  it is now
	// a Treelist dialog.. but there are a number of other
	// possibilities (including Tab, Swallow, and just Plain)
	QFrame *frame;
	frame = addPage(i18n("Base de datos"),
					i18n("Configuración de la conexión a la base de datos"));
	pageOneLayout = new QHBoxLayout( frame, 0, 0);
	m_pageOne = new kbird2PrefPageOne(frame);
	m_pageOne->pageWdg->editHost->setText (config().hostDB);
	m_pageOne->pageWdg->editNombre->setText(config().nombreDB);
	m_pageOne->pageWdg->editPassword->setText(config().passDB);
	m_pageOne->pageWdg->editUsuario->setText(config().userDB);
	m_pageOne->pageWdg->autoConnect->setChecked (config().connectOnStart);
	if (config().driverDB != "NONE")
		m_pageOne->pageWdg->comboDriver->setCurrentText (config().driverDB);
	pageOneLayout->addWidget(m_pageOne);
	
	frame = addPage(i18n("Personal"), i18n("Datos personales"));
	pageTwoLayout = new QHBoxLayout( frame, 0, 0);
	m_pageTwo = new kbird2PrefPageTwo(frame);
	pageTwoLayout->addWidget(m_pageTwo);

	frame = addPage(i18n("Mantenimiento"), i18n("Actualizaciones y copias de seguridad"));
	updBkpLayout = new QHBoxLayout( frame, 0, 0);
	m_UpdBkp = new kbird2PrefUpdBkp(frame);
	updBkpLayout->addWidget(m_UpdBkp);
	
	
	connect(this, SIGNAL(okClicked()),  this,   SLOT(savePreferences()));
}

void kbird2Preferences::savePreferences() {
	config().driverDB = m_pageOne->pageWdg->comboDriver->currentText();
	config().connectOnStart = m_pageOne->pageWdg->autoConnect->isChecked();
	config().hostDB = m_pageOne->pageWdg->editHost->text();
	config().nombreDB = m_pageOne->pageWdg->editNombre->text();
	config().passDB = m_pageOne->pageWdg->editPassword->text();
	config().userDB = m_pageOne->pageWdg->editUsuario->text();
	
	config().nombreP = m_pageTwo->pageWdg->nombre->text();
	config().calleP = m_pageTwo->pageWdg->calle->text();
	config().clubP = m_pageTwo->pageWdg->club->text();
	config().dniP = m_pageTwo->pageWdg->dni->text();
	config().licenciaP = m_pageTwo->pageWdg->licencia->text();
	config().poblacionP = m_pageTwo->pageWdg->poblacion->text();
	config().provinciaP = m_pageTwo->pageWdg->provincia->text();
	config().numeroP = m_pageTwo->pageWdg->numero->text();
    if ((config().coordX != m_pageTwo->pageWdg->coordx->text().toInt()) || 
            ((config().coordY != m_pageTwo->pageWdg->coordy->text().toInt()))) {
        QString update;
        QValueList<bloqueSuelta> vectorSueltas;
        bloqueSuelta suelta;
        double a, b, palomarx, palomary;        
        palomarx = m_pageTwo->pageWdg->coordx->text().toDouble();
        palomary = m_pageTwo->pageWdg->coordy->text().toDouble();
        qWarning ("X:" + QString::number(palomarx));
        qWarning ("Y:" + QString::number(palomary));
        config().coordX = palomarx;
        config().coordY = palomary;
        /* Hay que actualizar todos los puntos de suelta */
        /* Update puntosDeSuelta set distancia = sqrt ((puntoSueltaX - palomarX)^2 - (puntoSueltaY - palomarY)^2) */
        QSqlQuery queryUpdate(QSqlDatabase::database("palomar"));
        QSqlQuery selecDistancias ("SELECT sueltaid, coordx, coordy FROM suelta;", QSqlDatabase::database("palomar"));
        if (selecDistancias.isActive()) {
            while( selecDistancias.next() ) {
                suelta.sueltaID = selecDistancias.value(0).toDouble();
                suelta.x = selecDistancias.value(1).toDouble();
                suelta.x = selecDistancias.value(2).toDouble();
                vectorSueltas.append(suelta);
            }
            QValueList<bloqueSuelta>::iterator it;
            for ( it = vectorSueltas.begin(); it != vectorSueltas.end(); ++it ) {
                a = (*it).x - palomarx;
                b = (*it).y - palomary;
                qWarning ("a:" + QString::number(a));
                qWarning ("b:" + QString::number(b));
                queryUpdate.prepare("UPDATE suelta SET distancia = \"" + QString::number(sqrt(((a*a) + (b*b))/1000)) + "\" WHERE sueltaid = " + QString::number((*it).sueltaID) + ";");
                queryUpdate.exec();
                if (queryUpdate.isActive())
                    qWarning (queryUpdate.lastQuery());
                else
                    qWarning ("No va bien");
            }
        }
    }

	config().write();
}

kbird2PrefPageOne::kbird2PrefPageOne(QWidget *parent)
    : QFrame (parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAutoAdd(true);
    pageWdg = new pageOneWdg (this);

/*	QStringList list = QSqlDatabase::database("palomar" )->drivers();
	QStringList::Iterator it = list.begin();
	while( it != list.end() ) {
		this->pageWdg->comboDriver->insertItem( *it );
		++it;
	}*/

	QStringList list = QSqlDatabase::drivers();
	QStringList::Iterator it = list.begin();
	while( it != list.end() ) {
		this->pageWdg->comboDriver->insertItem( *it );
		qWarning (*it);
		++it;
	}

    //new QLabel(i18n("Add something here"), this);
}

kbird2PrefPageTwo::kbird2PrefPageTwo(QWidget *parent)
    : QFrame(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAutoAdd(true);
	pageWdg = new pageTwoWdg(this);
	pageWdg->nombre->setText(config().nombreP);
	pageWdg->calle->setText(config().calleP);
	pageWdg->club->setText(config().clubP);
	pageWdg->dni->setText(config().dniP);
	pageWdg->licencia->setText(config().licenciaP);
	pageWdg->numero->setText(config().numeroP);
	pageWdg->poblacion->setText(config().poblacionP);
	pageWdg->provincia->setText(config().provinciaP);
    pageWdg->coordx->setText(QString::number(config().coordX));
    pageWdg->coordy->setText(QString::number(config().coordY));
}

kbird2PrefUpdBkp::kbird2PrefUpdBkp(QWidget *parent)
    : QFrame(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAutoAdd(true);
	pageWdg = new updatePrefWdg(this);
	qWarning("Generando página de update y backup");
}

#include "pref.moc"
