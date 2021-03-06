/*
  This file is part of the KOrganizer interfaces.

  Copyright (c) 2001,2003 Cornelius Schumacher <schumacher@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KORG_INTERFACES_PART_H
#define KORG_INTERFACES_PART_H

#include "mainwindow.h"

#include <KPluginFactory>
#include <KParts/Part>

#include <QWidget>

namespace KOrg {
class Part : public KParts::Part
{
    Q_OBJECT
public:
    static int interfaceVersion()
    {
        return 2;
    }

    static QString serviceType()
    {
        return QStringLiteral("KOrganizer/Part");
    }

    typedef QList<Part *> List;

    explicit Part(MainWindow *parent)
        : KParts::Part(parent ? (parent->topLevelWidget()) : nullptr)
        , mMainWindow(parent)
    {
    }

    virtual ~Part()
    {
    }

    virtual QString info() = 0;

    /** short name of the part, used as category in the keybindings dialog */
    virtual QString shortInfo() = 0;

    MainWindow *mainWindow()
    {
        return mMainWindow;
    }

private:
    MainWindow *mMainWindow = nullptr;
};

class PartFactory : public KPluginFactory
{
public:
    virtual Part *createPluginFactory(MainWindow *parent) = 0;

protected:
    QObject *createObject(QObject *, const char *, const QStringList &) override
    {
        return nullptr;
    }
};
}

#endif
