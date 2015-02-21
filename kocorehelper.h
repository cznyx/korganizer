/*
  This file is part of KOrganizer.

  Copyright (c) 1999 Preston Brown <pbrown@kde.org>
  Copyright (c) 2000,2001 Cornelius Schumacher <schumacher@kde.org>
  Copyright (C) 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

  As a special exception, permission is given to link this program
  with any edition of Qt, and distribute the resulting executable,
  without including the source code for Qt in the source distribution.
*/

#ifndef KORG_KOCOREHELPER_H
#define KORG_KOCOREHELPER_H

#include "kocore.h"
#include "koglobals.h"
#include "koprefs.h"
#include "korganizer/corehelper.h"

class KOCoreHelper : public KOrg::CoreHelper
{
public:
    KOCoreHelper() {}
    virtual ~KOCoreHelper() {}

    QColor categoryColor(const QStringList &cats) Q_DECL_OVERRIDE;

    QString holidayString(const QDate &dt) Q_DECL_OVERRIDE;

    QTime dayStart() Q_DECL_OVERRIDE {
        return KOPrefs::instance()->mDayBegins.time();
    }

    const KCalendarSystem *calendarSystem() Q_DECL_OVERRIDE {
        return KOGlobals::self()->calendarSystem();
    }
};

#endif
