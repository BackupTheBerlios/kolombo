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
#ifndef PALOMARDATATABLE_H
#define PALOMARDATATABLE_H

#include <qdatatable.h>
#include <qcombobox.h>


class QSqlDatabase;
class QSqlCursor;
class QDataTable;
class QSqlRecord;

class QPatinter;
class QSqlField;
class QRect;
class QColor;
class QTimer;

/**
Esta clase extiende QDataTable para hacer que las celdas de puedan colorear en función de determinados parámetros y para añadir widgets personalizados para la edición.

@author AgarFu
*/
class palomarDataTable : public QDataTable
{

    Q_OBJECT
    
public:
    palomarDataTable ( QWidget * parent = 0, const char * name = 0 );

    ~palomarDataTable();
 
    QSqlDatabase* palomarConnection;

private:
    typedef QValueList<QColor> ColorList;
    ColorList coloresAnyos;
    QTimer *timerFiltro;
    QString *filtro;
    
public slots:
    virtual void polish();
    void verReproduccion ();
    void verVuelo ();
    void verDesaparecida ();
    void verTodas ();
    void verCenso ();
    void verBusqueda(const QString condicion);
    void verHermanosMadre();
    void verHermanosPadre();
    void verHermanosMadreYPadre();
    void crearPareja();
    void crearPareja(const QString madre, const QString padre);
    void cambioSexo();
    void estadoDesaparecida ();
    void estadoVuelo ();
    void estadoReproduccion ();
    void eliminarPaloma ();
    void filtroInteligenteTimeout ();
    void filtroInteligente (const QString &filtro);



protected:
    virtual void paintField( QPainter * p, const QSqlField* field, const QRect & cr, bool b);
};

#endif
