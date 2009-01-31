/*
 awayonlock - plugin to set away status on screensaver activation
 Copyright (C) 2009  Leo Antunes <leo@costela.net>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc.

*/

#ifndef _AWAYONLOCK_I18N
#define _AWAYONLOCK_I18N

#ifdef ENABLE_NLS
#	include <libintl.h>
#	include <locale.h>
#	define _(String) ((const char *)dgettext(PACKAGE, String))
#	define gettext_noop(String) String
#	define N_(String) gettext_noop (String)
#else
#	define _(String) (String)
#	define N_(String) String
#	define textdomain(Domain)
#	define bindtextdomain(Package, Directory)
#endif

#endif
