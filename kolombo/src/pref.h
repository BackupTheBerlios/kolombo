/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#ifndef _KBIRD2PREF_H_
#define _KBIRD2PREF_H_

#include <kdialogbase.h>
#include <qframe.h>

class kbird2PrefPageOne;
class kbird2PrefPageTwo;
class kbird2PrefUpdBkp;
class pageOneWdg;
class pageTwoWdg;
class updatePrefWdg;
class QHBoxLayout;

typedef struct {
    int sueltaID;
    double x;
    double y; 
} bloqueSuelta;

class kbird2Preferences : public KDialogBase
{
    Q_OBJECT
public:
    kbird2Preferences();

private:
    kbird2PrefPageOne *m_pageOne;
    kbird2PrefPageTwo *m_pageTwo;
	 kbird2PrefUpdBkp *m_UpdBkp;
    QHBoxLayout* pageOneLayout;
	 QHBoxLayout* pageTwoLayout;
	 QHBoxLayout* updBkpLayout;

	private slots:
		void savePreferences();
};

class kbird2PrefPageOne : public QFrame
{
    Q_OBJECT
public:
    kbird2PrefPageOne(QWidget *parent = 0);
	pageOneWdg* pageWdg;
};

class kbird2PrefPageTwo : public QFrame
{
    Q_OBJECT
public:
    kbird2PrefPageTwo(QWidget *parent = 0);
	pageTwoWdg* pageWdg;
};


class kbird2PrefUpdBkp : public QFrame
{
	Q_OBJECT
public:
	kbird2PrefUpdBkp(QWidget *parent = 0);
	updatePrefWdg* pageWdg;
};

#endif // _KBIRD2PREF_H_
