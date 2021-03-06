/*
  This file is part of KOrganizer.

  Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef KORG_VIEWS_KOWHATSNEXTVIEW_H
#define KORG_VIEWS_KOWHATSNEXTVIEW_H

#include "baseview.h"
#include <EventViews/WhatsNextView>

/**
  This class provides a view of the next events and todos
*/
class KOWhatsNextView : public KOrg::BaseView
{
    Q_OBJECT
public:
    explicit KOWhatsNextView(QWidget *parent = nullptr);
    ~KOWhatsNextView();

    int currentDateCount() const override;
    Akonadi::Item::List selectedIncidences() override
    {
        return Akonadi::Item::List();
    }

    KCalCore::DateList selectedIncidenceDates() override
    {
        return KCalCore::DateList();
    }

    bool supportsDateNavigation() const override
    {
        return true;
    }

    CalendarSupport::CalPrinterBase::PrintType printType() const override;
    void setCalendar(const Akonadi::ETMCalendar::Ptr &) override;

public Q_SLOTS:
    void updateView() override;
    void showDates(const QDate &start, const QDate &end, const QDate &preferredMonth) override;
    void showIncidences(const Akonadi::Item::List &incidenceList, const QDate &date) override;

    void changeIncidenceDisplay(const Akonadi::Item &,
                                Akonadi::IncidenceChanger::ChangeType) override;

private:
    EventViews::WhatsNextView *mView = nullptr;
};

#endif
