#ifndef SCHEDULEDEVENT_H
#define SCHEDULEDEVENT_H

#include <functional>

typedef std::function<bool(void)> ConditionalFunc;
typedef std::function<void(void)> ActionFunc;

class ScheduledEvent{
    ConditionalFunc condition;
    ActionFunc action;
public:
    //ScheduledEvent(System*); //
    ScheduledEvent(ConditionalFunc cond,ActionFunc act);
    operator bool();
    void operator()();
};







#endif
