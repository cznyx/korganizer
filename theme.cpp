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

#include "theme.h"
#include "themeimporter.h"
#include "koprefs.h"

#include <kdebug.h>
#include <KIO/NetAccess>
#include <KMimeType>
#include <KStandardDirs>
#include <KZip>

#include <QtCore/QFile>

#include "theme.moc"

using namespace KOrg;

void Theme::useThemeFrom( const KUrl &url )
{
  if ( url.isEmpty() )
    return;

  QFile* file = new QFile( url.path() ); // FIXME: does it work with remote URLs?
  kDebug() << file->fileName() << endl;
  if ( ( ! file->open(QFile::ReadOnly | QFile::Text) ) || ( ! url.isLocalFile() ) ) {
    //TODO: KMessageBox "invalid file"
    kDebug() << "Theme: can't import: invalid file: (1) " << url.path() << endl;
    return;
  }

  KMimeType::Ptr mimeType;
  mimeType = KMimeType::findByUrl( url );

  if ( mimeType->name() == "application/zip" ) {
    KZip *zip = new KZip( url.path() );

    if ( ! zip->open(QIODevice::ReadOnly) ) {
      //TODO: KMessageBox "invalid file"
      kDebug() << "Theme: can't import: invalid file: (3) " << url.path() << endl;
      return;
    }

    const KArchiveDirectory *dir = zip->directory();
    if ( dir == 0 ) {
      //TODO: KMessageBox "invalid file"
      kDebug() << "Theme: can't import: invalid file: (4) " << url.path() << endl;
      return;
    }

    if ( ! KIO::NetAccess::del( KUrl::fromPath( storageDir().absolutePath() ),
                                0 ) ) {
      kWarning() << "Theme: could not delete stale theme files" << endl;
    }
    dir->copyTo( storageDir().path() );

    file = new QFile( storageDir().path() + "/theme.xml" );

    if ( ! file->open(QFile::ReadOnly | QFile::Text) ) {
      //TODO: KMessageBox "invalid file"
      kDebug() << "Theme: can't import: invalid file: (5) " << url.path() << endl;
      return;
    }

    KMimeType::Ptr mimeType;
    mimeType = KMimeType::findByUrl( storageDir().path() + "/theme.xml" );
    if ( mimeType->name() != "application/xml" ) {
      //TODO: KMessageBox "invalid file"
      kDebug() << "Theme: can't import: invalid file: (6) " << url.path() << endl;
      return;
    }
  } else if ( mimeType->name() == "application/xml" ) {
    KIO::NetAccess::file_copy( url.path(),
                               storageDir().path() + "/", 0 );
  } else {
    //TODO: KMessageBox "invalid file"
    kDebug() << "Theme: can't import: invalid file: (2) " << url.path() << endl;
    return;
  }


  clearCurrentTheme();
  ThemeImporter reader( file );
}

void Theme::saveThemeTo( const KUrl &url )
{
  KZip *zip = new KZip( url.path() );

  if ( ! zip->open(QIODevice::WriteOnly) ) {
      //TODO: KMessageBox "no write permission"
    kDebug() << "Theme: can't export: no write permission: " << url.path() << endl;
    return;
  }
  if ( ! zip->addLocalDirectory( storageDir().absolutePath(), QString() ) ) {
      //TODO: KMessageBox "could not add theme files"
    kDebug() << "Theme: can't export: could not add theme files to: " << url.path() << endl;
    return;
  }
  if ( ! zip->close() ) {
      //TODO: KMessageBox "could not write theme file"
    kDebug() << "Theme: can't export: could not close theme file: " << url.path() << endl;
    return;
  }
}

void Theme::clearCurrentTheme()
{
  foreach ( QString viewType, Theme::themableViews() ) {
    KConfigGroup( KSharedConfig::openConfig(), "Theme/" + viewType + " view" ).deleteGroup();
  }
}

const QDir Theme::storageDir()
{
  QDir *dir = new QDir( KStandardDirs::locateLocal( "appdata", "theme" ) );
  return *dir;
}

const QStringList Theme::themableViews( const QString &viewType )
{
  QStringList l;
  l.append( "Agenda" );
  l.append( "Month" );
  // TODO:  TodoView?
  if ( l.contains( viewType ) ) {
    return QStringList( viewType );
  }
  else if ( viewType.isEmpty() ) {
    return l;
  }
  else {
    return QStringList();
  }
}
