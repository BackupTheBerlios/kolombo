/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#include <kapplication.h>
#include <kcmdlineargs.h> 

#include <dcopclient.h>
#include <qdatastream.h>
#include <qstring.h>

int main(int argc, char **argv)
{
	// Initialize command line args
	KCmdLineArgs::init(argc, argv, "kbird2", "kbird2", "Control de palomar", "2.7.0");
	KApplication app(argc, argv, "kbird2_client", false);

	// get our DCOP client and attach so that we may use it
	DCOPClient *client = app.dcopClient();
	client->attach();

	// do a 'send' for now
	QByteArray data;
	QDataStream ds(data, IO_WriteOnly);
	if (argc > 1)
		ds << QString(argv[1]);
	else
		ds << QString("http://www.kde.org");
	client->send("kbird2", "kbird2Iface", "openURL(QString)", data);

	return app.exec();
}
