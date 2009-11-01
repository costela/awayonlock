#!/usr/bin/env python

"""
incredibly dirty hack to test away-on-lock
if gnome-screensaver is already running this wont work (it wont be able
to pretend to be gnome-screensaver)
"""

import dbus, dbus.service, gobject
from dbus.mainloop.glib import DBusGMainLoop
from dbus.service import BusName

bus = dbus.SessionBus(mainloop=DBusGMainLoop())

class Test(dbus.service.Object):
	def __init__(self, path):
		dbus.service.Object.__init__(self, bus, path)

	x = dbus.Boolean(1);

	@dbus.service.signal(dbus_interface='org.gnome.ScreenSaver', signature='b')
	def ActiveChanged(self, y):
		print "emmitting...", repr(y)
		return True

name = BusName(name="org.gnome.ScreenSaver", bus=bus);

app = gobject.MainLoop()

t = Test('/org/gnome/ScreenSaver')
gobject.timeout_add(   1, t.ActiveChanged, dbus.Boolean(1))
gobject.timeout_add(5000, t.ActiveChanged, dbus.Boolean(0))
gobject.timeout_add(5001, app.quit)

app.run()
