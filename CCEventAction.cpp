//
//  EventAction.cpp
//  ZombiesWillInfectYourFamily
//
//  Created by George Guy on 2/13/14.
//
//

#include "CCEventAction.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

void EventAction::update( float delta )
{    
    CCNotificationCenter* publisher = CCNotificationCenter::sharedNotificationCenter();
    publisher->postNotification(_event.c_str(), _argument);
}

bool EventAction::init(const std::string& event, CCObject* argument)
{
    if( event.length() )
    {
        this->_event = event;
        this->_argument = argument;
        CC_SAFE_RETAIN(_argument);
        return true;
    }
    
    return false;
}

EventAction* EventAction::create(const std::string& event, CCObject* argument)
{
    EventAction* ret = new EventAction();
    
    if(ret && ret->init(event, argument))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return NULL;
    }
}