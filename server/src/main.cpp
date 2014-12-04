#include <Ecore.h>
#include "oduinoSerialReader.h"

Ecore_Timer         *timer     = NULL;
Ecore_Event_Handler *handler1   = NULL;
SerialReader serialReader;
int temp=0, hum=0;


int
getSensor_redrawScreen(void *data)
{
   temp = serialReader.get_Temperature();
   hum = serialReader.get_Humidity();
printf("Temperature %d, Humidity %d\n",temp, hum);
   return ECORE_CALLBACK_RENEW;
}

int
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

