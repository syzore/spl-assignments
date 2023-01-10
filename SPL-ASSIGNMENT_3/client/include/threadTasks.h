#ifndef THREAD_TASKS_
#define THREAD_TASKS_

#include "../include/StompClient.h"

void socket_listener_task(StompClient &client);

void keyboard_handler_task(StompClient &client);

#endif