/****************************************************************************
 Copyright (c) 2013-2014 George Guy
 Copyright (c) 2013-2014 Casey Loufek
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __RiseOfTheUndeadInfection__ApplyForce__
#define __RiseOfTheUndeadInfection__ApplyForce__
#include "extensions/cocos-ext.h"

/*
 * Useful when one must apply force repeatedly, or after a delay.
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
