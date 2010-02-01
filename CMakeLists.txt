cmake_minimum_required (VERSION 2.6) 

project(pidgin-awayonlock C)

# TODO: LOCALEDIR should be set for non-default installs
add_definitions(-DPACKAGE="${CMAKE_PROJECT_NAME}" -DLOCALEDIR=NULL)

set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb -Wall" CACHE STRING "" FORCE)

add_library(awayonlock SHARED awayonlock.c callback.c prefs.c)

find_package( PkgConfig REQUIRED )

pkg_check_modules( LIBPURPLE REQUIRED purple>=2.4.0 )
pkg_check_modules( GLIB REQUIRED glib-2.0 )
pkg_check_modules( DBUS_GLIB REQUIRED dbus-glib-1 )
include_directories(${LIBPURPLE_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS} ${DBUS_GLIB_INCLUDE_DIRS})
target_link_libraries(awayonlock ${LIBPURPLE_LIBRARIES} ${GLIB_LIBRARIES} ${DBUS_GLIB_LIBRARIES})

install(TARGETS awayonlock LIBRARY DESTINATION ${LIBPURPLE_LIBDIR}/pidgin NAMELINK_SKIP)

find_package(Gettext)
if(GETTEXT_FOUND)
	add_definitions(-DENABLE_NLS)
	add_subdirectory(i18n)
endif()

include(FeatureSummary)
print_enabled_features()
