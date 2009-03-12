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

#include <glib.h>

// purple
#include <debug.h>
#include <plugin.h>
#include <version.h>
#include <savedstatuses.h>
#include <pluginpref.h>

#include "i18n.h"

PurplePluginPrefFrame *get_prefs_frame(PurplePlugin *plugin) {
	PurplePluginPrefFrame *frame;
	PurplePluginPref *ppref;
	GList *statuses;

	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("creating preferences frame\n"));

	frame = purple_plugin_pref_frame_new();

	ppref = purple_plugin_pref_new_with_name_and_label("/plugins/core/awayonlock/status",_("Status to set on screensaver activation"));
	purple_plugin_pref_set_type(ppref, PURPLE_PLUGIN_PREF_CHOICE);
	purple_plugin_pref_add_choice(ppref, _("Default away status"), "");

	statuses = g_list_copy(purple_savedstatuses_get_all());

	for(statuses = g_list_first(statuses);
	    statuses;
	    statuses = g_list_next(statuses)) {
		if(! purple_savedstatus_is_transient(statuses->data)) {
			purple_plugin_pref_add_choice(ppref, purple_savedstatus_get_title(statuses->data), (char *)purple_savedstatus_get_title(statuses->data));
		}
	}
	purple_plugin_pref_frame_add(frame, ppref);
	g_list_free(statuses);

	return frame;
}

