/*
  This file is part of KOrganizer.

  Copyright (C) 2011 Sérgio Martins <iamsergio@gmail.com>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "kitemiconcheckcombo.h"
#include "koglobals.h"

#include <KLocalizedString>

KItemIconCheckCombo::KItemIconCheckCombo(ViewType viewType, QWidget *parent)
    : KPIM::KCheckComboBox(parent)
    , mViewType(viewType)
{
    addItem(i18n("Calendar's custom icon"));
    addItem(QIcon::fromTheme(QStringLiteral("view-calendar-tasks")), i18n("To-do"));
    addItem(QIcon::fromTheme(QStringLiteral("view-pim-journal")), i18n("Journal"));
    addItem(QIcon::fromTheme(QStringLiteral("appointment-recurring")), i18n("Recurring"));
    addItem(QIcon::fromTheme(QStringLiteral("appointment-reminder")), i18n("Alarm"));
    addItem(QIcon::fromTheme(QStringLiteral("object-locked")), i18n("Read Only"));
    addItem(QIcon::fromTheme(QStringLiteral("mail-reply-sender")), i18n("Needs Reply"));
    addItem(QIcon::fromTheme(QStringLiteral("meeting-attending")), i18n("Attending"));
    addItem(QIcon::fromTheme(QStringLiteral("meeting-attending-tentative")),
            i18n("Maybe Attending"));
    addItem(QIcon::fromTheme(QStringLiteral("meeting-organizer")), i18n("Organizer"));

    // Agenda view doesn't support journals yet
    setItemEnabled(EventViews::EventView::JournalIcon, viewType != AgendaType);
    setItemEnabled(EventViews::EventView::ReplyIcon, viewType == AgendaType);
    setItemEnabled(EventViews::EventView::AttendingIcon, viewType == AgendaType);
    setItemEnabled(EventViews::EventView::TentativeIcon, viewType == AgendaType);
    setItemEnabled(EventViews::EventView::OrganizerIcon, viewType == AgendaType);

    setDefaultText(i18nc("@item:inlistbox", "Icons to use"));
    setAlwaysShowDefaultText(true);
}

KItemIconCheckCombo::~KItemIconCheckCombo()
{
}

void KItemIconCheckCombo::setCheckedIcons(const QSet<EventViews::EventView::ItemIcon> &icons)
{
    const int itemCount = count();
    for (int i = 0; i < itemCount; ++i) {
        if (itemEnabled(i)) {
            setItemCheckState(
                i,
                icons.contains(static_cast<EventViews::EventView::ItemIcon>(i))
                ? Qt::Checked
                : Qt::Unchecked);
        } else {
            setItemCheckState(i, Qt::Unchecked);
        }
    }
}

QSet<EventViews::EventView::ItemIcon> KItemIconCheckCombo::checkedIcons() const
{
    QSet<EventViews::EventView::ItemIcon> icons;
    const int itemCount = count();
    for (int i = 0; i < itemCount; ++i) {
        const QVariant value = itemCheckState(i);
        if (value.toBool()) {
            icons.insert(static_cast<EventViews::EventView::ItemIcon>(i));
        }
    }
    return icons;
}
