/*
 * Copyright (C) 2003 René Martín - AgarFu <heaven@miniyo.croasanaso.org>
 */

#include "kbird2.h"
#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A KDE Application");

static const char version[] = "0.1";

static KCmdLineOptions options[] =
{
    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kbird2", I18N_NOOP("kbird2"), version, description,
                     KAboutData::License_GPL, "(C) 2003 René Martín - AgarFu", 0, 0, "heaven@miniyo.croasanaso.org");
    about.addAuthor( "René Martín - AgarFu", 0, "heaven@miniyo.croasanaso.org" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // register ourselves as a dcop client
    app.dcopClient()->registerAs(app.name(), false);

    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(kbird2);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            kbird2 *widget = new kbird2;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                kbird2 *widget = new kbird2;
                widget->show();
                widget->load(args->url(i));
            }
        }
        args->clear();
    }

    return app.exec();
}
