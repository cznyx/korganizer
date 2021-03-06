########### next target ###############
set(libcommon_SRCS korg_uniqueapp.cpp)
ecm_qt_declare_logging_category(libcommon_SRCS HEADER korganizerplugin_debug.h IDENTIFIER KORGANIZERPLUGIN_LOG CATEGORY_NAME org.kde.pim.korganizer_plugin)

set(kontact_korganizerplugin_PART_SRCS korganizerplugin.cpp apptsummarywidget.cpp summaryeventinfo.cpp ${libcommon_SRCS})

qt5_add_dbus_interfaces(kontact_korganizerplugin_PART_SRCS ${korganizer_SOURCE_DIR}/src/data/org.kde.Korganizer.Calendar.xml  ${korganizer_SOURCE_DIR}/src/data/org.kde.korganizer.Korganizer.xml)

add_library(kontact_korganizerplugin MODULE ${kontact_korganizerplugin_PART_SRCS})

target_link_libraries(kontact_korganizerplugin KF5::AkonadiCalendar KF5::CalendarUtils KF5::Contacts KF5::CalendarCore KF5::Libkdepim KF5::KontactInterface korganizerprivate KF5::CalendarSupport KF5::AkonadiCalendar KF5::WindowSystem KF5::I18n KF5::IconThemes)

########### next target ###############

set(kontact_todoplugin_PART_SRCS todoplugin.cpp todosummarywidget.cpp ${libcommon_SRCS})

qt5_add_dbus_interfaces(kontact_todoplugin_PART_SRCS ${korganizer_SOURCE_DIR}/src/data/org.kde.Korganizer.Calendar.xml  ${korganizer_SOURCE_DIR}/src/data/org.kde.korganizer.Korganizer.xml)

add_library(kontact_todoplugin MODULE ${kontact_todoplugin_PART_SRCS})

target_link_libraries(kontact_todoplugin KF5::AkonadiCalendar  KF5::Contacts KF5::Libkdepim KF5::KontactInterface KF5::CalendarCore KF5::CalendarUtils KF5::CalendarSupport KF5::AkonadiCalendar KF5::IconThemes KF5::Notifications KF5::WindowSystem)

########### next target ###############

set(kontact_journalplugin_PART_SRCS journalplugin.cpp ${libcommon_SRCS})

qt5_add_dbus_interfaces(kontact_journalplugin_PART_SRCS ${korganizer_SOURCE_DIR}/src/data/org.kde.Korganizer.Calendar.xml)

add_library(kontact_journalplugin MODULE ${kontact_journalplugin_PART_SRCS})

target_link_libraries(kontact_journalplugin  KF5::KontactInterface KF5::IconThemes KF5::WindowSystem)

########### next target ###############

set(kcm_apptsummary_PART_SRCS kcmapptsummary.cpp)

ki18n_wrap_ui(kcm_apptsummary_PART_SRCS apptsummaryconfig_base.ui)

add_library(kcm_apptsummary MODULE ${kcm_apptsummary_PART_SRCS})

target_link_libraries(kcm_apptsummary  KF5::KCMUtils KF5::I18n)

########### next target ###############

set(kcm_todosummary_PART_SRCS kcmtodosummary.cpp)

ki18n_wrap_ui(kcm_todosummary_PART_SRCS todosummaryconfig_base.ui)

add_library(kcm_todosummary MODULE ${kcm_todosummary_PART_SRCS})

target_link_libraries(kcm_todosummary Qt5::Widgets KF5::KCMUtils KF5::I18n)

########## Unit Test ###########
if (BUILD_TESTING)
   add_subdirectory(autotests)
endif()
########### install files ###############

install(TARGETS kcm_apptsummary DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(TARGETS kcm_todosummary DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(TARGETS kontact_korganizerplugin DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(TARGETS kontact_todoplugin DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(TARGETS kontact_journalplugin DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES korganizerplugin.desktop todoplugin.desktop journalplugin.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR}/kontact)
install(FILES kcmapptsummary.desktop kcmtodosummary.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

install(FILES korganizer.setdlg DESTINATION ${KDE_INSTALL_DATADIR}/kontact/ksettingsdialog)


