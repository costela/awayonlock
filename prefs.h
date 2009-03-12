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

#ifndef _AWAYONLOCK_PREFS_H
#define _AWAYONLOCK_PREFS_H

PurplePluginPrefFrame* get_prefs_frame(PurplePlugin*);

static PurplePluginUiInfo prefs = {
	get_prefs_frame,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};


#endif
