//
//  CCActionWait.h
//  ZombiesWillInfectYourFamily
//
//  Created by George Guy on 3/12/14.
//
//

#ifndef __ZombiesWillInfectYourFamily__CCActionWait__
#define __ZombiesWillInfectYourFamily__CCActionWait__

#include "cocos2d.h"

/*
 * I implemented Wait because ActionInterval was made abstract in Cocos2d-x 3,
 * and I liked using ActionIntervals in sequences to delay other actions.
 *
 * For example, this is how you would use a Wait to make the Sprite "sprite"
 * turn invisible three seconds after you run the Action "sequence":
 * 
 * auto wait = Wait::create( 3 );
 * auto invisible = FadeOut::create( 0 );
 * auto sequence = Sequence::createWithTwoActions( wait, invisible );
 * sprite->runAction( sequence );
 */

class Wait : public cocos2d::ActionInterval
{
public:
    
    inline virtual void update( float delta ) override {};
    
    inline virtual Wait* clone() const
    {
        return Wait::create( this->getDuration() );
    };
    
    inline virtual Wait* reverse() const
    {
        return this->clone();
    };
    
    inline static Wait* create( float duration )
    {
        using namespace cocos2d;
        auto ret = new Wait();
        
        if( ret && ret->initWithDuration(duration) )
        {
            ret->autorelease();
            return ret;
        }
        else
        {
            CC_SAFE_DELETE(ret);
            return NULL;
        }
        
        return ret;
    };
};

#endif /* defined(__ZombiesWillInfectYourFamily__CCActionWait__) */
