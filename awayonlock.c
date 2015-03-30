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

#define AWAYONLOCK_VERSION "0.5.1"
#define AWAYONLOCK_PLUGIN_ID "core-costela-awayonlock"

#include <dbus/dbus-glib.h>

// purple
#include <debug.h>
#include <plugin.h>
#include <version.h>
#include <notify.h>
#include <savedstatuses.h>

#include "i18n.h"
#include "callback.h"
#include "prefs.h"

static DBusGConnection *dbus_conn = NULL;

void *
awayonlock_get_handle(void)
{
	static char* handle = "awayonlock";
	return handle;
}


static gboolean plugin_load(PurplePlugin *plugin) {
	DBusGProxy *dbus_proxy_gnome = NULL;
        DBusGProxy *dbus_proxy_mate = NULL;
	DBusGProxy *dbus_proxy = NULL;
	GError *error = NULL;

	bindtextdomain(PACKAGE, LOCALEDIR);

	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("plugin_load called\n"));

	if(purple_prefs_get_int(AWAYONLOCK_PREF_OLD_STATUS) != 0) {
		PurpleSavedStatus *status_saved = purple_savedstatus_find_by_creation_time((time_t)purple_prefs_get_int(AWAYONLOCK_PREF_OLD_STATUS));
		purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("stale status '%s' located, restoring and clearing\n"), purple_savedstatus_get_title(status_saved));
		purple_savedstatus_activate(status_saved);
		purple_prefs_set_int(AWAYONLOCK_PREF_OLD_STATUS, 0);
	}

	dbus_conn = dbus_g_bus_get(DBUS_BUS_SESSION, &error);

	if(dbus_conn == NULL) {
		purple_debug(PURPLE_DEBUG_ERROR, PACKAGE, N_("failed to get DBus connection\n"));
		purple_notify_error(plugin, "Away-on-lock", _("Failed to get a DBus connection."), g_strdup_printf("DBus error: %s\n",error->message));
		return FALSE;
	}


	/*
	 * Gnome-screensaver 2.26 specific stuff
	 */
	dbus_proxy_gnome = dbus_g_proxy_new_for_name( dbus_conn,
			"org.gnome.ScreenSaver",
			"/org/gnome/ScreenSaver",
			"org.gnome.ScreenSaver"
			);

	if(dbus_proxy_gnome == NULL) {
		purple_debug(PURPLE_DEBUG_ERROR, PACKAGE, N_("failed to get DBus proxy\n"));
		purple_notify_error(plugin, "Away-on-lock", _("Failed to create a DBus Proxy."), NULL);
		dbus_g_connection_unref(dbus_conn);
		return FALSE;
	}

	dbus_g_proxy_add_signal( dbus_proxy_gnome,
			"ActiveChanged",
			G_TYPE_BOOLEAN,
			G_TYPE_INVALID
			);

	dbus_g_proxy_connect_signal( dbus_proxy_gnome,
			"ActiveChanged",
			G_CALLBACK(awayonlock_idle_changed_callback),
			NULL,
			NULL
			);
	/*
	 * END Gnome-screensaver specific stuff
	 */

	/*
	 * Mate-screensaver specific stuff
	 */
	dbus_proxy_mate = dbus_g_proxy_new_for_name( dbus_conn,
			"org.mate.ScreenSaver",
			"/org/mate/ScreenSaver",
			"org.mate.ScreenSaver"
			);

	if(dbus_proxy_mate == NULL) {
		purple_debug(PURPLE_DEBUG_ERROR, PACKAGE, N_("failed to get DBus proxy\n"));
		purple_notify_error(plugin, "Away-on-lock", _("Failed to create a DBus Proxy."), NULL);
		dbus_g_connection_unref(dbus_conn);
		return FALSE;
	}

	dbus_g_proxy_add_signal( dbus_proxy_mate,
			"ActiveChanged",
			G_TYPE_BOOLEAN,
			G_TYPE_INVALID
			);

	dbus_g_proxy_connect_signal( dbus_proxy_mate,
			"ActiveChanged",
			G_CALLBACK(awayonlock_idle_changed_callback),
			NULL,
			NULL
			);
	/*
	 * END Mate-screensaver specific stuff
	 */
	/*
	 * freedesktop stuff
	 */
	dbus_proxy = dbus_g_proxy_new_for_name( dbus_conn,
			"org.freedesktop.ScreenSaver",
			"/ScreenSaver",
			"org.freedesktop.ScreenSaver"
			);

	if(dbus_proxy == NULL) {
		purple_debug(PURPLE_DEBUG_ERROR, PACKAGE, N_("failed to get DBus proxy\n"));
		purple_notify_error(plugin, "Away-on-lock", _("Failed to create a DBus Proxy."), NULL);
		dbus_g_connection_unref(dbus_conn);
		return FALSE;
	}

	dbus_g_proxy_add_signal( dbus_proxy,
			"ActiveChanged",
			G_TYPE_BOOLEAN,
			G_TYPE_INVALID
			);

	dbus_g_proxy_connect_signal( dbus_proxy,
			"ActiveChanged",
			G_CALLBACK(awayonlock_idle_changed_callback),
			NULL,
			NULL
			);
	/*
	 * END freedesktop stuff
	 */


	/*
	 * avoid dangling references to savedstatuses in the config
	 */
	purple_signal_connect(purple_savedstatuses_get_handle(), "savedstatus-deleted",
						awayonlock_get_handle(),
						PURPLE_CALLBACK(prefs_status_deleted_cb),
						NULL
						);

	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("plugin_load finished\n"));
	return TRUE;
}


static gboolean plugin_unload(PurplePlugin *plugin) {
	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("plugin_unload called\n"));
	dbus_g_connection_unref(dbus_conn);
	purple_signals_disconnect_by_handle(awayonlock_get_handle());
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
	N_("http://costela.net/projects/awayonlock"),

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
	info.summary = _("Changes your status when your screensaver gets activated");
	info.description = _("This plugin changes your status to a preselected saved status or the default away status whenever your screensaver gets activated. It doesn't interfere if you're already marked as auto-away and can also avoid changing your status if you've manually marked yourself as any non-available status.");
	info.author = _("Leo Antunes <leo@costela.net>");

	purple_prefs_add_none(AWAYONLOCK_PREF_ROOT);
	purple_prefs_add_string(AWAYONLOCK_PREF_STATUS, NULL);
	purple_prefs_add_int(AWAYONLOCK_PREF_OLD_STATUS, 0);
	purple_prefs_add_bool(AWAYONLOCK_PREF_AVAILABLE_ONLY, 0);
}

PURPLE_INIT_PLUGIN(awayonlock, init_plugin, info);

