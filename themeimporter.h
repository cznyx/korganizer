/*
  This file is part of KOrganizer.

  Copyright (c) 2007 Loïc Corbasson <loic.corbasson@gmail.com>

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
#ifndef THEMEIMPORTER_H
#define THEMEIMPORTER_H

#include <QtXml/QXmlStreamReader>

#include <KConfig>
#include <KUrl>

namespace KOrg {

  /**
    @class ThemeImporter
    @brief Class to import an XML theme file's settings into KOrganizer's
           KConfig-based configuration file
   */
  class ThemeImporter : public QXmlStreamReader
  {
    public:
      ThemeImporter();
      ThemeImporter( QIODevice* device );
      ~ThemeImporter();

      /**
        Read the XML stream from a device.
       */
      bool read( QIODevice* device );

    private:
      /**
        Read the XML theme from the device.
       */
      void readThemeXml();

      /////////////////////////////////////////////////////////////////////////

      /**
        Read a view tag and act adequately.
       */
      void readView();
      /**
        Read a date tag and act adequately.
       */
      void readDate();

      /**
        Read an element and act adequately.
       */
      void readElement( const QString &viewType = QString(),
                        const int year = 0, const int month = 0,
                        const int day = 0 );

      /**
        Read an unknown element and act adequately.
       */
      void readUnknownElement();

      /////////////////////////////////////////////////////////////////////////

      /**
        Read a grid tag and act adequately.
       */
      void readGrid( const QString &viewType = QString(),
                     const int year = 0, const int month = 0,
                     const int day = 0 );
      /**
        Read a time-labels tag and act adequately.
       */
      void readTimeLabels( const QString &viewType = QString(),
                           const int year = 0, const int month = 0,
                           const int day = 0 );
      /**
        Read a calendar-items tag and act adequately.
       */
      void readCalendarItems( const QString &viewType = QString(),
                              const int year = 0, const int month = 0,
                              const int day = 0 );
          /**
            Calendar items: read an events tag and act adequately.
          */
          void readEvents( const QString &viewType = QString(),
                           const int year = 0, const int month = 0,
                           const int day = 0 );
          /**
            Calendar items: read a to-dos tag and act adequately.
          */
          void readToDos( const QString &viewType = QString(),
                          const int year = 0, const int month = 0,
                          const int day = 0 );
          /**
            Calendar items: read a categories tag and act adequately.
          */
          void readCategories( const QString &viewType = QString(),
                               const int year = 0, const int month = 0,
                               const int day = 0 );
          /**
            Calendar items: read a resources tag and act adequately.
          */
          void readResources( const QString &viewType = QString(),
                              const int year = 0, const int month = 0,
                              const int day = 0 );
      /**
        Read a marcus-bains-line tag and act adequately.
       */
      void readMarcusBainsLine( const QString &viewType = QString(),
                                const int year = 0, const int month = 0,
                                const int day = 0 );
      /**
        Read a holidays tag and act adequately.
       */
      void readHolidays( const QString &viewType = QString(),
                         const int year = 0, const int month = 0,
                         const int day = 0 );

      /////////////////////////////////////////////////////////////////////////

      /**
        Set the color for @param key in view @param viewType for date @p year
        @p month @p day to @param value
       */
      void setColor( const QString &viewType,
                     const int year, const int month,
                     const int day,
                     const QString &key, const QString &value );

      /////////////////////////////////////////////////////////////////////////

      /**
        Return the config group for @param viewType if specified, or all config
        groups corresponding to the various available views if not.
       */
      QList<KConfigGroup*> perViewConfigGroups( const QString &viewType
                                                              = QString() );
      /**
        Register a config group for a given view.
        The config group will be deleted on destruction of this object.
       */
      KConfigGroup* registerPerViewConfigGroup( KConfigGroup* g,
                                                const QString &viewType );
      /**
        Create a config group for a given view.
       */
      KConfigGroup* createPerViewConfigGroup( const QString &viewType ) const;
      QMap<QString, KConfigGroup*> mPerViewConfigGroups;

      /**
        Return all themable views available.
       */
      const QStringList themableViews() const;
  };

}

#endif
