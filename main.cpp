/* 
   KOrganizer (c) 1997-1999 Preston Brown
   All code is covered by the GNU Public License

   $Id$	
*/

#include <stdlib.h>

#include <qdir.h>

#include <kstddirs.h>
#include <kglobal.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#include "koapp.h"
#include "version.h"

KOrganizerApp *app;

static const KCmdLineOptions options[] =
{
  {"l", 0, 0},
  {"list", "List the events for the current day", 0},
  {"s", 0, 0},
  {"show <numdays>", "Show a list of all events for the next <numdays>","1"},
  {"+[calendar]", "A calendar file to load", 0},
  {0,0,0}
};

int main (int argc, char **argv)
{
  KAboutData aboutData("korganizer",I18N_NOOP("KOrganizer"),
      "1.91",I18N_NOOP("A Personal Organizer for KDE"),KAboutData::License_GPL,
      "(c) 1997-1999, Preston Brown, (c) 2000, Cornelius Schumacher",0,
      "http://devel-home.kde.org/~korganiz");
  aboutData.addAuthor("Cornelius Schumacher",I18N_NOOP("Current Maintainer"),
                      "schumacher@kde.org");
  aboutData.addAuthor("Preston Brown",I18N_NOOP("Original Author"),
                      "pbrown@kde.org");
  aboutData.addCredit("Richard Apodaca");              
  aboutData.addCredit("Laszlo Boloni");
  aboutData.addCredit("Barry Benowitz");
  aboutData.addCredit("Christopher Beard");
  aboutData.addCredit("Ian Dawes");
  aboutData.addCredit("Neil Hart");
  aboutData.addCredit("Hans-J�rgen Husel");
  aboutData.addCredit("Christian Kirsch");
  aboutData.addCredit("Uwe Koloska");
  aboutData.addCredit("Glen Parker");
  aboutData.addCredit("Dan Pilone");
  aboutData.addCredit("Roman Rohr");
  aboutData.addCredit("Herwin Jan Steehouwer");
  aboutData.addCredit("Nick Thompson");
  aboutData.addCredit("Fester Zigterman");


  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );
  KUniqueApplication::addCmdLineOptions();
  
  if (!KOrganizerApp::start())
    exit(0);

  KOrganizerApp app;
  qDebug("app.exec");
  return app.exec();
  qDebug("~app.exec");
}
