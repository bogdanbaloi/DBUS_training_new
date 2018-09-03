CC	=	gcc
CFLAGS = `pkg-config --cflags --libs glib-2.0 gtk+-2.0` 

dbus_server_app:
	${CC}	dbus_server_app.c	alarmClockStubs.c	${CFLAGS}	-o	dbus_server_app
clean:
	rm	-f	alarmClockStubs	dbus_server_app
