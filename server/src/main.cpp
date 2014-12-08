#include <Ecore.h>
#include <dlog.h>
#include <stdio.h>
#include "oduinoSerialReader.h"

#define LOG_TAG		"chameleonD" 
#define DLOG_ERR	DLOG_ERROR
#define PRT_ERR(fmt, arg...) \
	do { SLOG(LOG_ERR, LOG_TAG, fmt, ##arg); } while (0)

Ecore_Timer         *sensor_timer     = NULL;
Ecore_Timer         *redraw_timer     = NULL;
Ecore_Event_Handler *handler   = NULL;
SerialReader serialReader;

int temp=0, hum=0;
Eina_Bool
getSensor(void *data)
{
	serialReader.listen();
	return ECORE_CALLBACK_RENEW;
}
Eina_Bool
redrawScreen(void *data)
{
   FILE *sysfs;
   sysfs = fopen("/sys/class/Vsensor/oduino/oduino_sensor", "w");

   temp = serialReader.get_Temperature();
   hum = serialReader.get_Humidity();
   fprintf(sysfs, "%d %d", temp, hum);
   fclose(sysfs);
   PRT_ERR("Temperature %d, Humidity %d\n",temp, hum);
   return ECORE_CALLBACK_RENEW;
}

Eina_Bool
exit_func(void *data, int ev_type, void *ev)
{
   Ecore_Event_Signal_Exit *e;

   e = (Ecore_Event_Signal_Exit *)ev;
   ecore_main_loop_quit();
   return 1;
}

int
main(int argc, const char **argv)
{
   ecore_init();
   ecore_app_args_set(argc, argv);
   handler = ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, exit_func, NULL);
   sensor_timer = ecore_timer_add(1, getSensor, NULL);
   redraw_timer = ecore_timer_add(1.5, redrawScreen, NULL);
   ecore_main_loop_begin();
   ecore_shutdown();
   return 0;
}

