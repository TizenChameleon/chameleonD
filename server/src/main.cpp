#include <Ecore.h>
#include <dlog.h>
#include "oduinoSerialReader.h"

#define LOG_TAG		"chameleonD" 
#define DLOG_ERR	DLOG_ERROR
#define PRT_ERR(fmt, arg...) \
	do { SLOG(LOG_ERR, LOG_TAG, fmt, ##arg); } while (0)

Ecore_Timer         *timer     = NULL;
Ecore_Event_Handler *handler   = NULL;
SerialReader serialReader;
int temp=0, hum=0;


Eina_Bool
getSensor_redrawScreen(void *data)
{
   temp = serialReader.get_Temperature();
   hum = serialReader.get_Humidity();
PRT_ERR("Temperature %d, Humidity %d\n",templ, hum);
   return ECORE_CALLBACK_RENEW;
}

Eina_Bool
exit_func(void *data, int ev_type, void *ev)
{
   Ecore_Event_Signal_Exit *e;

   e = (Ecore_Event_Signal_Exit *)ev;
   if (e->interrupt)      printf("Exit: interrupt\n");
   else if (e->quit)      printf("Exit: quit\n");
   else if (e->terminate) printf("Exit: terminate\n");
   ecore_main_loop_quit();
   return 1;
}

int
main(int argc, const char **argv)
{
   ecore_init();
   ecore_app_args_set(argc, argv);


   handler = ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, exit_func, NULL);
   timer = ecore_timer_add(0.5, getSensor_redrawScreen, NULL);
   ecore_main_loop_begin();
   ecore_shutdown();
   return 0;
}

