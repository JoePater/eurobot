#include "scheduledevent.h"

ScheduledEvent::ScheduledEvent(ConditionalFunc cond,ActionFunc act):
    condition(cond),action(act){}

ScheduledEvent::operator bool(){
    return condition();
}
void ScheduledEvent::operator()(){
    action();
}
