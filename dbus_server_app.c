#include <glib.h>
#include <stdio.h>
#include "alarmClockStubs.h"

alarmClockAlarmClock * skeletonNew;

/* ---------------------------------------------------------------------------------------------------- */

gboolean on_set_time(alarmClockAlarmClock *object, GDBusMethodInvocation *invocation, const gchar *arg_hour, const gchar *arg_minutes,const gchar *arg_seconds)
{
    printf("The method setTime was called!\n");
    alarm_clock_alarm_clock_complete_set_time(skeletonNew,invocation);
    return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

gboolean on_set_alarm_time(alarmClockAlarmClock *object, GDBusMethodInvocation *invocation, const gchar *arg_hour, const gchar *arg_minutes)
{
    printf("The method setAlarmTime was called!\n");
    alarm_clock_alarm_clock_complete_set_alarm_time (skeletonNew, invocation);
    return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

gboolean on_set_alarm_status(alarmClockAlarmClock *object, GDBusMethodInvocation *invocation, const gchar *arg_hour)
{
    printf("The method setAlarmStatus was called!\n");
    alarm_clock_alarm_clock_complete_set_alarm_status (skeletonNew, invocation);
    return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

gboolean on_get_alarm_status(alarmClockAlarmClock *object,GDBusMethodInvocation *invocation, const gchar *result)
{
    printf("The method getAlarmStatus was called!\n");
    alarm_clock_alarm_clock_complete_get_alarm_status (skeletonNew,invocation,"OK!!!");
    return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

static void
on_method_to_interface()
{
    g_signal_connect(skeletonNew, "handle-set-time", G_CALLBACK(on_set_time), NULL);
    g_signal_connect(skeletonNew, "handle-set-alarm-time", G_CALLBACK(on_set_alarm_time), NULL);
    g_signal_connect(skeletonNew, "handle-set-alarm-status", G_CALLBACK(on_set_alarm_status), NULL);
    g_signal_connect(skeletonNew, "handle-get-alarm-status", G_CALLBACK(on_get_alarm_status), NULL);

    printf("On_method_to_interface with g_signal_connect called and finished\n");
}

/* ---------------------------------------------------------------------------------------------------- */

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
    printf("Se apeleaza on_bus_acquired!!!\n");
    GDBusObjectManagerServer * registration_id;
    alarmClockObjectSkeleton * objectSkeleton;

    registration_id = g_dbus_object_manager_server_new("/com/time/service/manager");
    objectSkeleton = alarm_clock_object_skeleton_new("/com/time/service/manager/object");
    skeletonNew = alarm_clock_alarm_clock_skeleton_new();

    printf("Skeleton interface created!!!\n");

    alarm_clock_object_skeleton_set_alarm_clock(objectSkeleton, skeletonNew);

    g_dbus_object_manager_server_export(registration_id, G_DBUS_OBJECT_SKELETON(objectSkeleton));

    g_dbus_object_manager_server_set_connection(registration_id, connection);

    on_method_to_interface();

    g_object_unref (objectSkeleton);
    g_object_unref (skeletonNew);

    printf("Bus Acquired!!\n");

}

/* ---------------------------------------------------------------------------------------------------- */

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    printf("Acquiring started!!");
    alarm_clock_alarm_clock_emit_ring_alarm(skeletonNew,"Ringing!!\n");
    printf("Name Acquired!!\n");
}

/* ---------------------------------------------------------------------------------------------------- */

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    printf("Name Lost!!\n");
}

/* ---------------------------------------------------------------------------------------------------- */

int
main (int argc, char ** argv)
{
    guint owner_id;
    GMainLoop *loop;

    owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                             "com.time.service.AlarmClock",
                             G_BUS_NAME_OWNER_FLAGS_NONE,
                             on_bus_acquired,
                             on_name_acquired,
                             on_name_lost,
                             NULL,
                             NULL);

    loop = g_main_loop_new (NULL, FALSE);

    printf("The Main Loop Started!!\n");

    g_main_loop_run (loop);

    g_bus_unown_name (owner_id);

    return 0;
}
