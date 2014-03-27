//
//  CCEventAction.h
//  ZombiesWillInfectYourFamily
//
//  Created by George Guy on 2/13/14.
//
//

#ifndef __ZombiesWillInfectYourFamily__EventAction__
#define __ZombiesWillInfectYourFamily__EventAction__

#include "cocos2d.h"
#include "GGUtil.h"

class EventAction : public cocos2d::CCActionInstant
{
public:
    typedef cocos2d::CCActionInstant super;
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, _event, Event);
    CC_SYNTHESIZE_READONLY(cocos2d::CCObject*, _argument, Argument);
    
    EventAction() : super(), _argument(NULL) {};
    virtual ~EventAction() { CC_SAFE_RELEASE_NULL(_argument); };
    virtual bool init(const std::string& event, cocos2d::CCObject* argument = NULL);
    static EventAction* create(const std::string& event, cocos2d::CCObject* argument = NULL);
    virtual void update( float delta );
    
    inline virtual EventAction* clone() const
    {
        return EventAction::create( this->getEvent(), this->getArgument() );
    };
    
    inline virtual EventAction* reverse() const
    {
        return EventAction::create( this->getEvent(), this->getArgument() );
    };
};

#endif /* defined(__ZombiesWillInfectYourFamily__EventAction__) */
