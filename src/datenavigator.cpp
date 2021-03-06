/*
  This file is part of KOrganizer.
  Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>
  Copyright (C) 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>

  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  Author: Sergio Martins <sergio@kdab.com>

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

#include "datenavigator.h"
#include "koglobals.h"

#include "korganizer_debug.h"

#include <QDate>
#include <QLocale>

DateNavigator::DateNavigator(QObject *parent) : QObject(parent)
{
    mSelectedDates.append(QDate::currentDate());
}

DateNavigator::~DateNavigator()
{
}

KCalCore::DateList DateNavigator::selectedDates()
{
    return mSelectedDates;
}

int DateNavigator::datesCount() const
{
    return mSelectedDates.count();
}

void DateNavigator::selectDates(const KCalCore::DateList &dateList, const QDate &preferredMonth)
{
    if (!dateList.isEmpty()) {
        mSelectedDates = dateList;
        emitSelected(preferredMonth);
    }
}

void DateNavigator::selectDate(const QDate &date)
{
    QDate d = date;

    if (!d.isValid()) {
        qCDebug(KORGANIZER_LOG) << "an invalid date was passed as a parameter!";
        d = QDate::currentDate();
    }
    mSelectedDates.clear();
    mSelectedDates.append(d);

    emitSelected();
}

void DateNavigator::selectDates(int count)
{
    count = qMin(count, static_cast<int>(MAX_SELECTABLE_DAYS));
    selectDates(mSelectedDates.first(), count);
}

void DateNavigator::selectDates(const QDate &d, int count, const QDate &preferredMonth)
{
    KCalCore::DateList dates;
    dates.reserve(count);

    for (int i = 0; i < count; ++i) {
        dates.append(d.addDays(i));
    }

    mSelectedDates = dates;
    emitSelected(preferredMonth);
}

void DateNavigator::selectWeekByDay(int weekDay, const QDate &d, const QDate &preferredMonth)
{
    int dateCount = mSelectedDates.count();
    bool weekStart = (weekDay == QLocale().firstDayOfWeek());
    if (weekStart && dateCount == 7) {
        selectWeek(d, preferredMonth);
    } else {
        selectDates(d, dateCount, preferredMonth);
    }
}

void DateNavigator::selectWeek()
{
    selectWeek(mSelectedDates.first());
}

void DateNavigator::selectWeek(const QDate &d, const QDate &preferredMonth)
{
    const int dayOfWeek = d.dayOfWeek();
    const int weekStart = QLocale().firstDayOfWeek();

    QDate firstDate = d.addDays(weekStart - dayOfWeek);

    if (weekStart != 1 && dayOfWeek < weekStart) {
        firstDate = firstDate.addDays(-7);
    }

    selectDates(firstDate, 7, preferredMonth);
}

void DateNavigator::selectWorkWeek()
{
    selectWorkWeek(mSelectedDates.first());
}

void DateNavigator::selectWorkWeek(const QDate &d)
{
    const int weekStart = QLocale().firstDayOfWeek();
    const int dayOfWeek = d.dayOfWeek();
    QDate currentDate = d.addDays(weekStart - dayOfWeek);

    if (weekStart != 1 && dayOfWeek < weekStart) {
        currentDate = currentDate.addDays(-7);
    }

    mSelectedDates.clear();
    const int mask = KOGlobals::self()->getWorkWeekMask();

    for (int i = 0; i < 7; ++i) {
        if ((1 << ((i + weekStart + 6) % 7)) & (mask)) {
            mSelectedDates.append(currentDate.addDays(i));
        }
    }

    emitSelected();
}

void DateNavigator::selectToday()
{
    QDate d = QDate::currentDate();

    int dateCount = mSelectedDates.count();

    if (dateCount == 7) {
        selectWeek(d);
    } else if (dateCount == 5) {
        selectWorkWeek(d);
    } else {
        selectDates(d, dateCount);
    }
}

void DateNavigator::selectPreviousYear()
{
    QDate firstSelected = mSelectedDates.first();
    int weekDay = firstSelected.dayOfWeek();
    firstSelected = firstSelected.addYears(-1);

    selectWeekByDay(weekDay, firstSelected);
}

void DateNavigator::selectPreviousMonth(const QDate &currentMonth, const QDate &selectionLowerLimit,
                                        const QDate &selectionUpperLimit)
{
    shiftMonth(currentMonth,
               selectionLowerLimit,
               selectionUpperLimit,
               -1);
}

void DateNavigator::selectPreviousWeek()
{
    QDate firstSelected = mSelectedDates.first();
    const int weekDay = firstSelected.dayOfWeek();
    firstSelected = firstSelected.addDays(-7);

    selectWeekByDay(weekDay, firstSelected);
}

void DateNavigator::selectNextWeek()
{
    QDate firstSelected = mSelectedDates.first();
    const int weekDay = firstSelected.dayOfWeek();

    firstSelected = firstSelected.addDays(7);

    selectWeekByDay(weekDay, firstSelected);
}

void DateNavigator::shiftMonth(const QDate &currentMonth, const QDate &selectionLowerLimit,
                               const QDate &selectionUpperLimit, int offset)
{
    QDate firstSelected = mSelectedDates.first();
    const int weekDay = firstSelected.dayOfWeek();
    firstSelected = firstSelected.addMonths(offset);

    /* Don't trust firstSelected to calculate the nextMonth. firstSelected
       can belong to a month other than currentMonth because KDateNavigator
       displays 7*6 days. firstSelected should only be used for selection
       purposes */
    const QDate nextMonth = currentMonth.isValid() ? currentMonth.addMonths(offset) : firstSelected;

    /* When firstSelected doesn't belong to currentMonth it can happen
       that the new selection won't be visible on our KDateNavigators
       so we must adjust it */
    if (selectionLowerLimit.isValid()
        && firstSelected < selectionLowerLimit) {
        firstSelected = selectionLowerLimit;
    } else if (selectionUpperLimit.isValid()
               && firstSelected > selectionUpperLimit) {
        firstSelected = selectionUpperLimit.addDays(-6);
    }

    selectWeekByDay(weekDay, firstSelected, nextMonth);
}

void DateNavigator::selectNextMonth(const QDate &currentMonth, const QDate &selectionLowerLimit,
                                    const QDate &selectionUpperLimit)
{
    shiftMonth(currentMonth,
               selectionLowerLimit,
               selectionUpperLimit,
               1);
}

void DateNavigator::selectNextYear()
{
    QDate firstSelected = mSelectedDates.first();
    int weekDay = firstSelected.dayOfWeek();
    firstSelected = firstSelected.addYears(1);

    selectWeekByDay(weekDay, firstSelected);
}

void DateNavigator::selectPrevious()
{
    int offset = -7;
    if (datesCount() == 1) {
        offset = -1;
    }

    selectDates(mSelectedDates.first().addDays(offset), datesCount());
}

void DateNavigator::selectNext()
{
    int offset = 7;
    if (datesCount() == 1) {
        offset = 1;
    }

    selectDates(mSelectedDates.first().addDays(offset), datesCount());
}

void DateNavigator::selectMonth(int month)
{
    // always display starting at the first week of the specified month
    QDate firstSelected = QDate(mSelectedDates.first().year(), month, 1);

    int day = firstSelected.day();
    firstSelected.setDate(firstSelected.year(), month, 1);
    int days = firstSelected.daysInMonth();
    // As day we use either the selected date, or if the month has less days
    // than that, we use the max day of that month
    if (day > days) {
        day = days;
    }
    QDate requestedMonth;
    firstSelected.setDate(firstSelected.year(), month, day);
    requestedMonth.setDate(firstSelected.year(), month, 1);

    selectWeekByDay(1, firstSelected, requestedMonth);
}

void DateNavigator::selectYear(int year)
{
    QDate firstSelected = mSelectedDates.first();
    const int deltaYear = year - firstSelected.year();
    firstSelected = firstSelected.addYears(deltaYear);

    const int weekDay = firstSelected.dayOfWeek();
    selectWeekByDay(weekDay, firstSelected);
}

void DateNavigator::emitSelected(const QDate &preferredMonth)
{
    Q_EMIT datesSelected(mSelectedDates, preferredMonth);
}
