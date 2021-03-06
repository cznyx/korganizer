/*
  This file is part of KOrganizer.

  Copyright (c) 2002 Mike Pilone <mpilone@slac.com>
  Copyright (c) 2002 Don Sanders <sanders@kde.org>
  Copyright (c) 2003,2004 Cornelius Schumacher <schumacher@kde.org>
  Copyright (C) 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>
  Copyright (c) 2005 Rafal Rzepecki <divide@users.sourceforge.net>

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

#ifndef KORG_ACTIONMANAGER_H
#define KORG_ACTIONMANAGER_H

#include "korganizerprivate_export.h"
#include "part.h"

#include <AkonadiCore/Item>
#include <Akonadi/Calendar/ETMCalendar>

#include <QUrl>
#include <KViewStateMaintainer>

#include <QObject>

class AkonadiCollectionView;
class CalendarView;
class KOWindowList;

namespace Akonadi {
class ETMViewStateSaver;
}

class QAction;
class QMenuBar;
class KSelectAction;
class QTemporaryFile;
class KToggleAction;

/**
  The ActionManager creates all the actions in KOrganizer. This class
  is shared between the main application and the part so all common
  actions are in one location.
  It also provides D-Bus interfaces.
*/
class KORGANIZERPRIVATE_EXPORT ActionManager : public QObject
{
    Q_OBJECT
public:
    ActionManager(KXMLGUIClient *client, CalendarView *widget, QObject *parent,
                  KOrg::MainWindow *mainWindow, bool isPart, QMenuBar *menuBar = nullptr);
    ~ActionManager() override;

    /** Peform initialization that requires this* to be full constructed */
    void init();

    CalendarView *view() const
    {
        return mCalendarView;
    }

    /**
      Create Calendar object based on the akonadi framework and set it on the view.
    */
    void createCalendarAkonadi();

public Q_SLOTS:
    bool importURL(const QUrl &url, bool merge);

    /** Save calendar file to URL of current calendar */
    bool saveURL();

    /** Save calendar file to URL */
    bool saveAsURL(const QUrl &QUrl);

    void toggleMenubar(bool dontShowWarning = false);

public:
    /** Get current URL */
    QUrl url() const
    {
        return mURL;
    }

    /** Is there a instance with this URL? */
    static KOrg::MainWindow *findInstance(const QUrl &url);

    /** Open calendar file from URL */
    bool openURL(const QString &url);

    /** Open calendar file from URL */
    bool mergeURL(const QString &url);

    /** Save calendar file to URL */
    bool saveAsURL(const QString &url);

    /** Get current URL as QString */
    QString getCurrentURLasString() const;

    /**
      Delete the incidence with the given unique id from current calendar.
      @param uid UID of the incidence to delete.
      @param force If true, all recurrences and sub-todos (if applicable) will
      be deleted without prompting for confirmation.
    */
    virtual bool deleteIncidence(Akonadi::Item::Id id, bool force = false);

    bool editIncidence(Akonadi::Item::Id id);

    /**
      Add an incidence to the active calendar.
      @param ical A calendar in iCalendar format containing the incidence.
    */
    bool addIncidence(const QString &ical);
    //bool addIncidence( const Akonadi::Item::Id &ical );

    bool showIncidence(Akonadi::Item::Id id);

    /**
      Show an incidence in context. This means showing the todo, agenda or
      journal view (as appropriate) and scrolling it to show the incidence.
      @param uid Unique ID of the incidence to show.
    */
    bool showIncidenceContext(Akonadi::Item::Id id);

    /**
     * Called by KOrganizerUniqueAppHandler in the kontact plugin
     * Returns true if the command line was successfully handled
     * false otherwise.
     */
    bool handleCommandLine(const QStringList &args);

public Q_SLOTS:
    void openEventEditor(const QString &);
    void openEventEditor(const QString &summary, const QString &description,
                         const QStringList &attachments);
    void openEventEditor(const QString &summary, const QString &description,
                         const QStringList &attachments, const QStringList &attendees);
    void openEventEditor(const QString &summary, const QString &description, const QString &uri,
                         const QString &file, const QStringList &attendees,
                         const QString &attachmentMimetype);
    void openEventEditor(const QString &summary, const QString &description,
                         const QStringList &attachmentUris, const QStringList &attendees,
                         const QStringList &attachmentMimetypes, bool attachmentIsInline);
    void openTodoEditor(const QString &);
    void openTodoEditor(const QString &summary, const QString &description,
                        const QStringList &attachments);
    void openTodoEditor(const QString &summary, const QString &description,
                        const QStringList &attachments, const QStringList &attendees);
    void openTodoEditor(const QString &summary, const QString &description, const QString &uri,
                        const QString &file, const QStringList &attendees,
                        const QString &attachmentMimetype);
    void openTodoEditor(const QString &summary, const QString &description,
                        const QStringList &attachmentUris, const QStringList &attendees,
                        const QStringList &attachmentMimetypes, bool attachmentIsInline);

    void openJournalEditor(const QDate &date);
    void openJournalEditor(const QString &text, const QDate &date);
    void openJournalEditor(const QString &text);

    void showJournalView();
    void showTodoView();
    void showEventView();

    void goDate(const QDate &);
    void goDate(const QString &);
    void showDate(const QDate &date);

public:
    QString localFileName();

    bool queryClose();

Q_SIGNALS:
    /**
      Emitted when the "New" action is activated.
    */
    //void actionNewMainWindow( const QUrl &url = QUrl() );
    void toggleMenuBar();
    /**
      When change is made to options dialog, the topwidget will catch this
      and Q_EMIT this signal which notifies all widgets which have registered
      for notification to update their settings.
    */
    void configChanged();

public Q_SLOTS:
    /**
      Options dialog made a changed to the configuration. we catch this
      and notify all widgets which need to update their configuration.
    */
    void updateConfig();

    void processIncidenceSelection(const Akonadi::Item &item, const QDate &date);
    void keyBindings();

    /**
      Using the KConfig associated with the kapp variable, read in the
      settings from the config file.
    */
    void readSettings();

    /**
      Write current state to config file.
    */
    void writeSettings();

    /* Session management */
    void saveProperties(KConfigGroup &);
    void readProperties(const KConfigGroup &);

    void loadParts();

    void importCalendar(const QUrl &url);

protected Q_SLOTS:
    void setItems(const QStringList &, int);

    /** open a file, load it into the calendar. */
    void file_open();

    /** open a file from the list of recent files. Also called from file_open()
        after the URL is obtained from the user.
        @param url the URL to open
    */
    void file_open(const QUrl &url);

    /** import a generic ics file */
    void file_import();

    /** delete or archive old entries in your calendar for speed/space. */
    void file_archive();

    /** Open kcontrol module for configuring date and time formats */
    void configureDateTime();

    void downloadNewStuff();

    void toggleDateNavigator();
    void toggleTodoView();
    void toggleEventViewer();
    void toggleResourceView();

    /** connected to CalendarView's signal which comes from the ArchiveDialog */
    void slotAutoArchivingSettingsModified();

    /** called by the auto archive timer to automatically delete/archive events */
    void slotAutoArchive();

    void setTitle();

    void updateUndoRedoActions();

protected:

    /**
      Return widget used as parent for dialogs and message boxes.
    */
    QWidget *dialogParent();

private Q_SLOTS:
    void dumpText(const QString &);    // only for debugging purposes

    void slotDefaultResourceChanged(const Akonadi::Collection &);
    void slotResourcesAddedRemoved();

    void slotNewEvent();
    void slotNewTodo();
    void slotNewSubTodo();
    void slotNewJournal();

    void slotMergeFinished(bool success, int total);
    void slotNewResourceFinished(bool);

private:
    class ActionStringsVisitor;
    void setHelpText(QAction *act, const QString &text);
    void restoreCollectionViewSetting();
    /** Create all the actions. */
    void initActions();
    void enableIncidenceActions(bool enable);
    Akonadi::ETMCalendar::Ptr calendar() const;

    Akonadi::Collection selectedCollection() const;

    KOrg::Part::List mParts; // List of parts loaded
    QUrl mURL;               // URL of calendar file
    QString mFile;           // Local name of calendar file
    QString mLastUrl;        // URL of last loaded calendar.

    QTemporaryFile *mTempFile = nullptr;
    QTimer *mAutoExportTimer = nullptr;    // used if calendar is to be autoexported
    QTimer *mAutoArchiveTimer = nullptr; // used for the auto-archiving feature

    // list of all existing KOrganizer instances
    static KOWindowList *mWindowList;

    KToggleAction *mDateNavigatorShowAction = nullptr;
    KToggleAction *mTodoViewShowAction = nullptr;
    KToggleAction *mCollectionViewShowAction = nullptr;
    KToggleAction *mEventViewerShowAction = nullptr;

    KToggleAction *mHideMenuBarAction = nullptr;

    QAction *mImportAction = nullptr;

    QAction *mNewEventAction = nullptr;
    QAction *mNewTodoAction = nullptr;
    QAction *mNewSubtodoAction = nullptr;
    QAction *mNewJournalAction = nullptr;
    QAction *mConfigureViewAction = nullptr;

    QAction *mShowIncidenceAction = nullptr;
    QAction *mEditIncidenceAction = nullptr;
    QAction *mDeleteIncidenceAction = nullptr;

    QAction *mCutAction = nullptr;
    QAction *mCopyAction = nullptr;
    QAction *mDeleteAction = nullptr;
    QAction *mNextXDays = nullptr;
    QAction *mPublishEvent = nullptr;
    QAction *mForwardEvent = nullptr;

    QAction *mSendInvitation = nullptr;
    QAction *mSendCancel = nullptr;
    QAction *mSendStatusUpdate = nullptr;

    QAction *mRequestChange = nullptr;
    QAction *mRequestUpdate = nullptr;

    QAction *mUndoAction = nullptr;
    QAction *mRedoAction = nullptr;
    QMenuBar *mMenuBar = nullptr;

    KSelectAction *mFilterAction = nullptr;

    KXMLGUIClient *mGUIClient = nullptr;
    KActionCollection *mACollection = nullptr;
    CalendarView *mCalendarView = nullptr;
    KOrg::MainWindow *mMainWindow = nullptr;
    bool mIsPart;

    AkonadiCollectionView *mCollectionView = nullptr;
    KViewStateMaintainer<Akonadi::ETMViewStateSaver> *mCollectionViewStateSaver = nullptr;
    KViewStateMaintainer<Akonadi::ETMViewStateSaver> *mCollectionSelectionModelStateSaver = nullptr;
};

#endif
