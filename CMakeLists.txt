cmake_minimum_required (VERSION 2.6) 

project(pidgin-awayonlock C)

# TODO: LOCALEDIR should be set for non-default installs
add_definitions(-DPACKAGE="${CMAKE_PROJECT_NAME}" -DLOCALEDIR=NULL)

set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb -Wall" CACHE STRING "" FORCE)

add_library(awayonlock SHARED awayonlock.c)
install(TARGETS awayonlock LIBRARY DESTINATION lib/pidgin NAMELINK_SKIP)

find_package( PkgConfig REQUIRED )

pkg_check_modules( LIBPURPLE REQUIRED purple>=2.4.0 )
include_directories(${LIBPURPLE_INCLUDE_DIRS})
pkg_check_modules( GLIB REQUIRED glib-2.0 )
include_directories(${GLIB_INCLUDE_DIRS})
pkg_check_modules( DBUS_GLIB REQUIRED dbus-glib-1 )
include_directories(${DBUS_GLIB_INCLUDE_DIRS})

find_package(Gettext)
add_subdirectory(i18n)

include(FeatureSummary)
print_enabled_features()
