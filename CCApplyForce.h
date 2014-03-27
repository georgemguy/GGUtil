//
//  CCApplyForce.h
//  RiseOfTheUndeadInfection
//
//  Created by George Guy on 2/22/13.
//
//

#ifndef __RiseOfTheUndeadInfection__ApplyForce__
#define __RiseOfTheUndeadInfection__ApplyForce__
#include "extensions/cocos-ext.h"

/*
 * Useful when one must apply force repeatedly, or after a delay
 */

class b2Body;
class ApplyForce : public cocos2d::ActionInstant
{
public:
    CC_SYNTHESIZE_PASS_BY_REF( cocos2d::Point, _vector, Vector );
    CC_SYNTHESIZE_PASS_BY_REF( cocos2d::Point, _offset, Offset );
    CC_SYNTHESIZE( b2Body*, _body, Body );
    CC_SYNTHESIZE(bool, _wake, Wake);
    CC_SYNTHESIZE_RETAIN(cocos2d::extension::PhysicsSprite*, _physicsSpriteTarget, PhysicsSpriteTarget);
    
    typedef cocos2d::ActionInstant super;
    ApplyForce();
    virtual ~ApplyForce();
    static ApplyForce* createWithPhysicsSpriteTarget( cocos2d::extension::PhysicsSprite* physicsSpriteTarget, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake = true );
    static ApplyForce* create( b2Body* body, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake );
    virtual bool init( b2Body* body, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake );
    virtual bool initWithPhysicsSpriteTarget( cocos2d::extension::PhysicsSprite* physicsSpriteTarget, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake );
    
    virtual void update( float delta );
    virtual void startWithTarget(cocos2d::Node* target);
    
    inline virtual ApplyForce* clone() const
    {
        auto ret = ApplyForce::create( _body, _vector, _offset, _wake );
        ret->setPhysicsSpriteTarget( ret->getPhysicsSpriteTarget() );
        return ret;
    }
    
    inline virtual ApplyForce* reverse() const
    {
        return this->clone();
    }    
};


#endif /* defined(__RiseOfTheUndeadInfection__ApplyForce__) */
