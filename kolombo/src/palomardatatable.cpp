/***************************************************************************
 *   Copyright (C) 2005 by AgarFu                                          *
 *   heaven@croasanaso.sytes.net                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "palomardatatable.h"

#include <klocale.h>
#include <kmessagebox.h>

#include <qstringlist.h>
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qpainter.h>
#include <qrect.h>
#include <qcolor.h>
#include <qstring.h>
#include <qtimer.h>

#include <qdrawutil.h>

palomarDataTable::palomarDataTable( QWidget * parent, const char * name)
 : QDataTable(parent, name)
 , timerFiltro(new QTimer(this))
{
    // Cacho de puthon para generar los colorcitos ...
    //for i in range (0, 40):
    //    print "coloresAnyos.append(QColor (" + str(int(random.random()*255)) + ", " + str(int(random.random()*255)) + ", "+ str(int(random.random()*255)) + "));"
    coloresAnyos.append(QColor (33, 49, 197));
    coloresAnyos.append(QColor (170, 202, 175));
    coloresAnyos.append(QColor (6, 94, 21));
    coloresAnyos.append(QColor (85, 74, 106));
    coloresAnyos.append(QColor (234, 123, 48));
    coloresAnyos.append(QColor (219, 91, 58));
    coloresAnyos.append(QColor (117, 140, 246));
    coloresAnyos.append(QColor (44, 184, 208));
    coloresAnyos.append(QColor (154, 211, 188));
    coloresAnyos.append(QColor (214, 236, 234));
    coloresAnyos.append(QColor (148, 60, 226));
    coloresAnyos.append(QColor (147, 181, 162));
    coloresAnyos.append(QColor (11, 187, 235));
    coloresAnyos.append(QColor (184, 215, 214));
    coloresAnyos.append(QColor (35, 89, 141));
    coloresAnyos.append(QColor (215, 142, 156));
    coloresAnyos.append(QColor (118, 130, 196));
    coloresAnyos.append(QColor (166, 158, 237));
    coloresAnyos.append(QColor (145, 184, 25));
    coloresAnyos.append(QColor (81, 185, 184));
    coloresAnyos.append(QColor (148, 221, 199));
    coloresAnyos.append(QColor (139, 90, 102));
    coloresAnyos.append(QColor (41, 63, 206));
    coloresAnyos.append(QColor (44, 30, 84));
    coloresAnyos.append(QColor (141, 86, 40));
    coloresAnyos.append(QColor (1, 234, 67));
    coloresAnyos.append(QColor (237, 78, 128));
    coloresAnyos.append(QColor (228, 87, 113));
    coloresAnyos.append(QColor (184, 68, 198));
    coloresAnyos.append(QColor (67, 162, 34));
    coloresAnyos.append(QColor (154, 83, 61));
    coloresAnyos.append(QColor (225, 200, 243));
    coloresAnyos.append(QColor (87, 18, 211));
    coloresAnyos.append(QColor (66, 18, 205));
    coloresAnyos.append(QColor (140, 120, 92));
    coloresAnyos.append(QColor (71, 19, 159));
    coloresAnyos.append(QColor (151, 36, 202));
    coloresAnyos.append(QColor (113, 237, 24));
    coloresAnyos.append(QColor (218, 239, 49));
    coloresAnyos.append(QColor (207, 249, 81));

    this->addColumn( "anyo", tr2i18n( "AÃ±o" ) );
    this->addColumn( "anilla", tr2i18n( "Anilla" ) );
    this->addColumn( "nacionalidad", tr2i18n( "Nacionalidad" ) );
    this->addColumn( "sexo", tr2i18n( "Sexo" ) );
    this->addColumn( "madre", tr2i18n( "Madre" ) );
    this->addColumn( "padre", tr2i18n( "Padre" ) );
    this->addColumn( "plumaje", tr2i18n( "Plumaje" ) );
    this->addColumn( "ojo", tr2i18n( "Ojo" ) );
    this->addColumn( "estado", tr2i18n( "Estado" ) );
    this->addColumn( "nombre", tr2i18n( "Nombre" ) );
    this->setReadOnly( TRUE );
    this->setSorting( TRUE );
    QStringList dataTable1_stringlist;
    dataTable1_stringlist << "anyo DESC";
    dataTable1_stringlist << "anilla ASC";
    dataTable1_stringlist << "nacionalidad DESC";
    this->setSort( dataTable1_stringlist );
    
    palomarConnection = QSqlDatabase::database( "palomar" );
    connect( timerFiltro, SIGNAL( timeout() ), SLOT( filtroInteligenteTimeout () ) );
}


palomarDataTable::~palomarDataTable()
{
}


/*
 *  Widget polish.  Reimplemented to handle
 *  default data table initialization
 */
void palomarDataTable::polish()
{
    QSqlCursor* cursor = this->sqlCursor();
    if ( !cursor ) {
        cursor = new QSqlCursor( "listaGeneral", TRUE, palomarConnection );
        if ( this->isReadOnly() )
            cursor->setMode( QSqlCursor::ReadOnly );
        this->setSqlCursor( cursor, FALSE, TRUE );
    }
    if ( !cursor->isActive() )
        this->refresh( QDataTable::RefreshAll );
    QWidget::polish();
}

void palomarDataTable::paintField( QPainter * p, const QSqlField* field, const QRect & cr, bool b)
{
    if ( !field )
        return;
    if ( field->name() == "sexo" ) {
        if (field->value().toString() == "Macho") {
            p->setBrush(QColor(212, 231, 255));
            p->setPen(QColor(212, 231, 255));
        } else {
            p->setBrush(QColor(255, 217, 217));
            p->setPen(QColor(255, 217, 217));
        }
        p->drawRect( 0, 0, cr.width(), cr.height() );
        p->setPen(QColor(0, 0, 0));
        if (cr.width() > 50)
            p->drawText( 2,2, cr.width() - 4, cr.height() - 4, fieldAlignment( field ), field->value().toString());
    } else if ( field->name() == "estado" ) {
        if (field->value().toString() == "Desaparecida") {
            p->setBrush(QColor(255, 172, 172));
            p->setPen(QColor(255, 172, 172));
        } else if (field->value().toString() == "Reproduccion") {
            p->setBrush(QColor(255, 242, 146));
            p->setPen(QColor(255, 242, 146));
        } else {
            p->setBrush(QColor(168, 226, 127));
            p->setPen(QColor(168, 226, 127));
        }
        p->drawRect(0, 0, cr.width(), cr.height());
        p->setPen(QColor(0, 0, 0));
        if (cr.width() > 50)
            p->drawText(2,2, cr.width() - 4, cr.height() - 4, fieldAlignment( field ), field->value().toString());
    } else if ( field->name() == "anyo" ) {
        int index = field->value().toInt() % 40;
        p->setBrush(coloresAnyos[index]);
        p->setPen(coloresAnyos[index]);
        p->drawRect(0, 0, cr.width(), cr.height());
        p->setPen(QColor(0, 0, 0));
        p->drawText(2,2, cr.width() - 4, cr.height() - 4, fieldAlignment( field ), field->value().toString());
    }
    else {
        QDataTable::paintField( p, field, cr, b) ;
    }
}

// Slots ...

void palomarDataTable::verReproduccion() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo DESC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from paloma WHERE estado = \"R\")");
   refresh();
}

void palomarDataTable::verVuelo() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo DESC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from paloma WHERE estado = \"V\")");
   refresh();
}

void palomarDataTable::verDesaparecida() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo DESC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from paloma WHERE estado = \"D\")");
   refresh();
}

void palomarDataTable::verTodas() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo DESC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "");
   refresh();
}

void palomarDataTable::verCenso() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo ASC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from paloma WHERE estado != \"D\")");
   refresh();
}

void palomarDataTable::verHermanosMadre() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo ASC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from listaGeneral WHERE madre = \"" + currentRecord()->value("madre").toString() + "\")");
   refresh();
}


void palomarDataTable::verHermanosPadre() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo ASC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from listaGeneral WHERE padre = \"" + currentRecord()->value("padre").toString() + "\")");
   refresh();
}


void palomarDataTable::verHermanosMadreYPadre() {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo ASC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter ( "palomaID in (SELECT palomaID from listaGeneral WHERE padre = \"" + currentRecord()->value("padre").toString() + "\" and madre = \"" + currentRecord()->value("madre").toString() + "\")");
   refresh();
}

void palomarDataTable::verBusqueda(const QString condicion) {
   QStringList dataTable1_stringlist;
   dataTable1_stringlist << "anyo DESC";
   dataTable1_stringlist << "anilla ASC";
   this->setSort( dataTable1_stringlist );
   setFilter (condicion);
   refresh();
}

void palomarDataTable::filtroInteligente (const QString &filtro) {
    this->filtro = filtro;
    timerFiltro->start( 280, true );
}

void palomarDataTable::filtroInteligenteTimeout () {
   timerFiltro->stop();
   QString condicion;
   condicion = "anyo like '%%s%' or anilla like '%%s%' or nacionalidad like '%%s%' or sexo like '%%s%' ";
   condicion += "or plumaje like '%%s%' or ojo like '%%s%' or estado like '%%s%' or nombre like '%%s%' ";
   condicion += "or padre like '%%s%' or madre like '%%s%'or anyo || \"-\" || anilla || \"-\" || nacionalidad like '%%s%'";
   condicion.replace ("%s", filtro);
   setFilter (condicion);
   refresh();
}

void palomarDataTable::crearPareja() {

    QString consulta = "INSERT INTO parejas (nombre, madreID, padreID, nota) VALUES (\"\", (\
        SELECT palomaID FROM idCompacto WHERE \
        idCompacto = \"" + currentRecord()->value("madre").toString() + "\"), (\
        SELECT palomaID FROM idCompacto WHERE \
        idCompacto = \"" + currentRecord()->value("padre").toString() + "\"), \"\")";
    qWarning (consulta);
    QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
    QSqlError error = queryInsert.lastError();
    if (error.type() != QSqlError::None)
            KMessageBox::error (this, i18n("Error creando la pareja. El error devuelto por la base de datos es:\n") +
                error.databaseText(),
                i18n ("Error añadiendo pareja"));
    else
        KMessageBox::information (  this, 
                                            i18n("Pareja creada"), 
                                            i18n("Pareja creada con éxito"));
}


void palomarDataTable::crearPareja(const QString madre, const QString padre) {

    QString consulta = "INSERT INTO parejas (nombre, madreID, padreID, nota) VALUES (\"\", (\
        SELECT palomaID FROM idCompacto WHERE \
        idCompacto = \"" + madre + "\"), (\
        SELECT palomaID FROM idCompacto WHERE \
        idCompacto = \"" + padre + "\"), \"\")";
    qWarning (consulta);
    QSqlQuery queryInsert (consulta, QSqlDatabase::database("palomar" ));
    QSqlError error = queryInsert.lastError();
    if (error.type() != QSqlError::None)
            KMessageBox::error (this, i18n("Error creando la pareja. El error devuelto por la base de datos es:\n") +
                error.databaseText(),
                i18n ("Error añadiendo pareja"));
    else
        KMessageBox::information (  this, 
                                            i18n("Pareja creada"), 
                                            i18n("Pareja creada con éxito"));
}

void palomarDataTable::cambioSexo () {
    QString sexo;
    QString consulta;

    /* Comprobamos que se pueda cambiar de sexo ... */
    consulta = "Select count(palomaID) from paloma where madreID =  " + currentRecord()->value(0).toString() + " OR \
        padreID = " + currentRecord()->value(0).toString();
    QSqlQuery querySexo (consulta, QSqlDatabase::database("palomar" ));
    querySexo.next();
    if ( querySexo.value(0).toInt() != 0) {
        KMessageBox::error (this, i18n("No se puede cambiar el sexo\nEsta paloma es padre o madre de otra existente en la base de datos. "),
            i18n ("Cambio de sexo."));
        return;
    }

    if (currentRecord()->value(4).toString() == "Macho")
        sexo = "1";
    else
        sexo = "0";

    consulta = "UPDATE paloma SET sexo = " + sexo + " WHERE palomaID = " + currentRecord()->value(0).toString();
    //qWarning (consulta);
    QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
    QSqlError error = queryUpdate.lastError();
    if (error.type() != QSqlError::None)
            KMessageBox::error (this, i18n("Error actualizando la paloma. El error devuelto por la base de datos es:\n") +
                error.databaseText(),
                i18n ("Error añadiendo actualizando paloma"));


    refresh();
}


void palomarDataTable::estadoReproduccion () {

    QString consulta = "UPDATE paloma SET estado = \"R\" WHERE palomaID = " + currentRecord()->value(0).toString();
    //qWarning (consulta);
    QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
    refresh();
}

void palomarDataTable::estadoVuelo () {

    QString consulta = "UPDATE paloma SET estado = \"V\" WHERE palomaID = " + currentRecord()->value(0).toString();
    //qWarning (consulta);
    QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
    refresh();
}

void palomarDataTable::estadoDesaparecida () {

    QString consulta = "UPDATE paloma SET estado = \"D\" WHERE palomaID = " + currentRecord()->value(0).toString();
    //qWarning (consulta);
    QSqlQuery queryUpdate (consulta, QSqlDatabase::database("palomar" ));
    refresh();
}

void palomarDataTable::eliminarPaloma () {
    QString consulta = "DELETE FROM paloma WHERE palomaID = " + currentRecord()->value(0).toString();
    QSqlQuery queryDelete (consulta, QSqlDatabase::database("palomar" ));
    QSqlError error = queryDelete.lastError();
    if (error.type() != QSqlError::None)
            KMessageBox::error (this, i18n("Error eliminando la paloma. El error devuelto por la base de datos es:\n") +
                error.databaseText(),
                i18n ("Error eliminando la paloma"));
    refresh();

}


#include "palomardatatable.moc"
