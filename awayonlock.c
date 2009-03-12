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

#define PURPLE_PLUGINS

#define AWAYONLOCK_VERSION "0.2"
#define AWAYONLOCK_PLUGIN_ID "core-costela-awayonlock"

#include <dbus/dbus-glib.h>

// purple
#include <debug.h>
#include <plugin.h>
#include <version.h>
#include <notify.h>

#include "i18n.h"
#include "callback.h"
#include "prefs.h"

static DBusGConnection *dbus_conn = NULL;

static gboolean plugin_load(PurplePlugin *plugin) {
	DBusGProxy *dbus_proxy = NULL;
	GError *error = NULL;

	bindtextdomain(PACKAGE, LOCALEDIR);

	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("plugin_load called\n"));
	
	dbus_conn = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
	
	if(dbus_conn == NULL) { 
		purple_debug(PURPLE_DEBUG_ERROR, PACKAGE, N_("failed to get DBus connection\n"));
		purple_notify_error(plugin, "Away-on-lock", _("Failed to get a DBus connection."), g_strdup_printf("DBus error: %s\n",error->message));
		return FALSE;
	}
	

	dbus_proxy = dbus_g_proxy_new_for_name( dbus_conn,
			"org.gnome.ScreenSaver",
			"/org/gnome/ScreenSaver",
			"org.gnome.ScreenSaver"
			);

	if(dbus_proxy == NULL) {
		purple_debug(PURPLE_DEBUG_ERROR, PACKAGE, N_("failed to get DBus proxy\n"));
		purple_notify_error(plugin, "Away-on-lock", _("Failed to create a DBus Proxy."), NULL);

		return FALSE;
	}

	dbus_g_proxy_add_signal( dbus_proxy,
			"SessionIdleChanged",
			G_TYPE_BOOLEAN,
			G_TYPE_INVALID
			);
	
	dbus_g_proxy_connect_signal( dbus_proxy,
			"SessionIdleChanged",
			G_CALLBACK(awayonlock_idle_changed_callback),
			NULL,
			NULL
			);

	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("plugin_load finished\n"));
	return TRUE;
}


static gboolean plugin_unload(PurplePlugin *plugin) {
	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("plugin_unload called\n"));
	dbus_g_connection_unref(dbus_conn);
	return TRUE;
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,

	AWAYONLOCK_PLUGIN_ID,
	0,
	AWAYONLOCK_VERSION,

	0,
	0,
	0,
	N_("http://costela.net/projects/away-on-lock"),
	
	plugin_load,
	plugin_unload,
	NULL,

	NULL,
	NULL,
	&prefs,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static void init_plugin(PurplePlugin *plugin)
{
	bindtextdomain(PACKAGE, LOCALEDIR);
	
	info.name = _("Away-on-lock");
	info.summary = _("Sets you as away when your screensaver is activated");
	info.description = _("This plugin sets your status to the default away status whenever your screensaver gets activated.");
	info.author = _("Leo Antunes <leo@costela.net>");

	purple_prefs_add_none("/plugins/core/awayonlock");
	purple_prefs_add_string("/plugins/core/awayonlock/status", NULL);
}

PURPLE_INIT_PLUGIN(awayonlock, init_plugin, info);

