// $Id$

#include "config.h"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include "koprefs.h"

#include <kconfig.h>
#include <kstddirs.h>
#include <klocale.h>

KOPrefs *KOPrefs::mInstance = 0;


KOPrefs::KOPrefs()
{
  mCategoryColors.setAutoDelete(true);
  
  mDefaultCategoryColor  = QColor("gray");
  mDefaultHolidayColor   = QColor("red");
  mDefaultHighlightColor = QColor("blue");

  mConfig = new KConfig(locate("config","korganizerrc"));
  
  readConfig();
}


KOPrefs::~KOPrefs()
{
  qDebug ("KOPrefs::~KOPrefs()");
  delete mConfig;
  
  mInstance = 0;
}


KOPrefs *KOPrefs::instance()
{
  if (!mInstance) mInstance = new KOPrefs();
  
  return mInstance;
}

void KOPrefs::setDefaults()
{
  // Default should be set a bit smarter, respecting username and locale
  // settings for example.

  mAutoSave = false;
  mConfirm = true;

  // user information...
  uid_t userId = getuid();
  struct passwd *pwent = getpwuid(userId);
  if (strlen(pwent->pw_gecos) > 0) mName = pwent->pw_gecos;
  else mName = i18n("Anonymous");

  // user email...
  mEmail = pwent->pw_name;
  mEmail += "@";
#ifdef HAVE_GETHOSTNAME
  char cbuf[80];
  if (gethostname(cbuf, 79)) {
    // error getting hostname
    mEmail += "localhost";
  } else {
    hostent he;
    if (gethostbyname(cbuf)) {
      he = *gethostbyname(cbuf);
      mEmail += he.h_name;
    } else {
      // error getting hostname
      mEmail += "localhost";
    }
  }
#else
  mEmail += "localhost";
#endif

  mAdditional = "";
  mHoliday = "(none)";
  
  mTimeZone = "";
  mStartTime = 10;
  mAlarmTime = 0;
  mDaylightSavings = 0;

  mDayBegins = 8;
  mHourSize = 10;
  mDailyRecur = true;
  mWeeklyRecur = true;

  mTimeBarFont = QFont("helvetica",18);

  mHolidayColor = mDefaultHolidayColor;
  mHighlightColor = mDefaultHighlightColor;

  mPrinter = "";
  mPaperSize = 0;
  mPaperOrientation = 0;
  mPrintPreview = "gv";
  
  setCategoryDefaults();
}

void KOPrefs::setCategoryDefaults()
{
  mCustomCategories.clear();

  mCustomCategories << i18n("Appointment") << i18n("Business")
      << i18n("Meeting") << i18n("Phone Call") << i18n("Education")
      << i18n("Holiday") << i18n("Vacation") << i18n("Special Occasion")
      << i18n("Personal") << i18n("Travel") << i18n("Miscellaneous")
      << i18n("Birthday");

  QStringList::Iterator it;
  for (it = mCustomCategories.begin();it != mCustomCategories.end();++it ) {
    setCategoryColor(*it,mDefaultCategoryColor);
  }
}

void KOPrefs::readConfig()
{
  mConfig->setGroup("General");
  mAutoSave = mConfig->readBoolEntry("Auto Save",false);
  mConfirm = mConfig->readBoolEntry("Confirm Deletes",true);
  mCustomCategories = mConfig->readListEntry("Custom Categories");
  if (mCustomCategories.isEmpty()) setCategoryDefaults();

  mConfig->setGroup("Personal Settings");
  mName = mConfig->readEntry("user_name","");
  mEmail = mConfig->readEntry("user_email","");
  mAdditional = mConfig->readEntry("Additional","");

  mHoliday = mConfig->readEntry("Holidays","(none)");
  
  mConfig->setGroup("Time & Date");
  mTimeZone = mConfig->readNumEntry("Time Zone",0);
  mStartTime = mConfig->readNumEntry("Default Start Time",10);
  mAlarmTime = mConfig->readNumEntry("Default Alarm Time",0);
  mDaylightSavings = mConfig->readNumEntry("Daylight Savings", 0);

  mConfig->setGroup("Views");
  mDayBegins = mConfig->readNumEntry("Day Begins",8);
  mHourSize = mConfig->readNumEntry("Hour Size",10);
  mDailyRecur = mConfig->readBoolEntry("Show Daily Recurrences",true);
  mWeeklyRecur = mConfig->readBoolEntry("Show Weekly Recurrences",true);

  mConfig->setGroup("Fonts");
  mTimeBarFont = mConfig->readFontEntry("TimeBar Font");

  mConfig->setGroup("Colors");
  mHolidayColor = mConfig->readColorEntry("Holiday Color",
                                          &mDefaultHolidayColor);
  mHighlightColor = mConfig->readColorEntry("Highlight Color",
                                            &mDefaultHighlightColor);

  mConfig->setGroup("Category Colors");
  QStringList::Iterator it;
  for (it = mCustomCategories.begin();it != mCustomCategories.end();++it ) {
    setCategoryColor(*it,mConfig->readColorEntry(*it,&mDefaultCategoryColor));
  }

  mConfig->setGroup("Printer");
  mPrinter = mConfig->readEntry("Printer Name",0);
  mPaperSize = mConfig->readNumEntry("Paper Size",0);
  mPaperOrientation = mConfig->readNumEntry("Paper Orientation",0);
  mPrintPreview = mConfig->readEntry("Preview","gv");
}


void KOPrefs::writeConfig()
{
//  qDebug("KOPrefs::writeConfig()");

  mConfig->setGroup("General");
  mConfig->writeEntry("Auto Save",mAutoSave);
  mConfig->writeEntry("Confirm Deletes",mConfirm);
  mConfig->writeEntry("Custom Categories",mCustomCategories);

  mConfig->setGroup("Personal Settings");
  mConfig->writeEntry("user_name",mName);
  mConfig->writeEntry("user_email",mEmail);
  mConfig->writeEntry("Additional",mAdditional);
  mConfig->writeEntry("Holidays",mHoliday);

  mConfig->setGroup("Time & Date");
  mConfig->writeEntry("Time Zone",mTimeZone);
  mConfig->writeEntry("Default Start Time",mStartTime);
  mConfig->writeEntry("Default Alarm Time",mAlarmTime);
  mConfig->writeEntry("Daylight Savings",mDaylightSavings);

  mConfig->setGroup("Views");
  mConfig->writeEntry("Day Begins",mDayBegins);
  mConfig->writeEntry("Hour Size",mHourSize);
  mConfig->writeEntry("Show Daily Recurrences",mDailyRecur);
  mConfig->writeEntry("Show Weekly Recurrences",mWeeklyRecur);

  mConfig->setGroup("Fonts");
  mConfig->writeEntry("TimeBar Font",mTimeBarFont);

  mConfig->setGroup("Colors");
  mConfig->writeEntry("Holiday Color",mHolidayColor);
  mConfig->writeEntry("Highlight Color",mHighlightColor);

  mConfig->setGroup("Category Colors");
  QDictIterator<QColor> it(mCategoryColors);
  while (it.current()) {
    mConfig->writeEntry(it.currentKey(),*(it.current()));
    ++it;
  }

  mConfig->setGroup("Printer");
  mConfig->writeEntry("Printer Name",mPrinter);
  mConfig->writeEntry("Paper Size",mPaperSize);
  mConfig->writeEntry("Paper Orientation",mPaperOrientation);
  mConfig->writeEntry("Preview",mPrintPreview);
  
  mConfig->sync();
}

void KOPrefs::setCategoryColor(QString cat,const QColor & color)
{
  mCategoryColors.replace(cat,new QColor(color));
}

QColor *KOPrefs::categoryColor(QString cat)
{
  QColor *color = 0;
  
  if (!cat.isEmpty()) color = mCategoryColors[cat];
  
  if (color) return color;
  else return &mDefaultCategoryColor;
}
