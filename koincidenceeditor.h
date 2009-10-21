/*
  This file is part of KOrganizer.

  Copyright (c) 2001,2003 Cornelius Schumacher <schumacher@kde.org>
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
#ifndef KOINCIDENCEEDITOR_H
#define KOINCIDENCEEDITOR_H

#include <kpagedialog.h>
#include <kurl.h>
#include <QList>

namespace KPIM {
  class DesignerFields;
  class EmbeddedURLPage;
}

namespace KOrg {
  class IncidenceChangerBase;
}

class KOEditorDetails;
class KOAttendeeEditor;

namespace KCal {
  class Calendar;
  class CalendarLocal;
  class Incidence;
}
using namespace KCal;
using namespace KOrg;

/**
  This is the base class for the calendar component editors.
*/
class KOIncidenceEditor : public KPageDialog
{
  Q_OBJECT
  public:
    /**
      Construct new IncidenceEditor.
    */
    KOIncidenceEditor( const QString &caption, Calendar *calendar,
                       QWidget *parent );
    virtual ~KOIncidenceEditor();

    /** This incidence has been modified externally */
    virtual void modified() {}

    virtual void reload() = 0;

    virtual void selectInvitationCounterProposal( bool enable );
    virtual void selectCreateTask( bool enable );

  public slots:
    /** Edit an existing todo. */
    virtual void editIncidence( Incidence *, const QDate &, Calendar * ) = 0;
    virtual void setIncidenceChanger( IncidenceChangerBase *changer )
    { mChanger = changer; }

    /** Initialize editor. This function creates the tab widgets. */
    virtual void init() = 0;

    /**
      Adds attachments to the editor
    */
    void addAttachments( const QStringList &attachments,
                         const QStringList &mimeTypes = QStringList(),
                         bool inlineAttachment = false );

    /**
      Adds attendees to the editor
    */
    void addAttendees( const QStringList &attendees );

  signals:
    void buttonClicked( int );
    void deleteAttendee( Incidence * );

    void editCategories();
    void updateCategoryConfig();
    void dialogClose( Incidence * );
    void editCanceled( Incidence * );

    void deleteIncidenceSignal( Incidence * );
    void signalAddAttachments( const QStringList &attachments,
                               const QStringList &mimeTypes = QStringList(),
                               bool inlineAttachment = false );

  protected slots:
    void reject();
    void accept();

    void openURL( const KUrl &url );

    virtual void slotButtonClicked( int button );
    virtual void slotManageTemplates();

    virtual void slotSaveTemplate( const QString & ) = 0;
    virtual void slotLoadTemplate( const QString & );
    virtual void slotTemplatesChanged( const QStringList & );

  protected:
    virtual void closeEvent( QCloseEvent * );

    virtual QString type() { return QString(); }
    virtual QStringList &templates() const = 0;
    virtual void loadTemplate( CalendarLocal & ) = 0;

    void setupAttendeesTab();
    void setupDesignerTabs( const QString &type );

    void saveAsTemplate( Incidence *, const QString &name );

    void readDesignerFields( Incidence *i );
    void writeDesignerFields( Incidence *i );

    /**
      Returns true if the user made any alteration
    */
    virtual bool incidenceModified();

    // Returns the page widget. To remove the tab, just delete that one.
    QWidget *addDesignerTab( const QString &uifile );

    void setupEmbeddedURLPage( const QString &label, const QString &url,
                               const QString &mimetype );
    void createEmbeddedURLPages( Incidence *i );

    /**
      Process user input and create or update event.
      @return false if input is invalid.
    */
    virtual bool processInput() { return false; }

    virtual void processCancel() {}

    void cancelRemovedAttendees( Incidence *incidence );

    Calendar *mCalendar;

    KOEditorDetails *mDetails;
    KOAttendeeEditor *mAttendeeEditor;
    KOrg::IncidenceChangerBase *mChanger;

    QList<KPIM::DesignerFields*> mDesignerFields;
    QMap<QWidget*, KPIM::DesignerFields*> mDesignerFieldForWidget;
    QList<QWidget*> mEmbeddedURLPages;
    QList<QWidget*> mAttachedDesignerFields;
    bool mIsCounter;
    bool mIsCreateTask;
};

#endif
