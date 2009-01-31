#!/usr/bin/env python

"""
incredibly dirty hack to test away-on-lock
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
	def SessionIdleChanged(self, y):
		print "emmitting...", repr(y)
		return True

name = BusName(name="org.gnome.ScreenSaver", bus=bus);

app = gobject.MainLoop()

t = Test('/org/gnome/ScreenSaver')
gobject.timeout_add(   1, t.SessionIdleChanged, dbus.Boolean(1))
gobject.timeout_add(2000, t.SessionIdleChanged, dbus.Boolean(0))
gobject.timeout_add(2001, app.quit)

app.run()
