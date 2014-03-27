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

#include "CCApplyForce.h"
#include "Box2D/Box2D.h"
#include "ZFDef.h"

USING_NS_CC;
USING_NS_CC_EXT;

ApplyForce::ApplyForce() : CCActionInstant(),
_body(NULL),
_wake(true),
_physicsSpriteTarget(NULL)
{
}

ApplyForce::~ApplyForce()
{
    CC_SAFE_RELEASE_NULL(_physicsSpriteTarget);
}

ApplyForce* ApplyForce::create( b2Body* body, const Point& vector, const Point& offset, bool wake )
{
    ApplyForce* ret = new ApplyForce();
    if( ret && ret->init( body, vector, offset, wake ) )
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE( ret );
        return NULL;
    }
    return ret;
}

ApplyForce* ApplyForce::createWithPhysicsSpriteTarget( cocos2d::extension::PhysicsSprite* physicsSpriteTarget, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake )
{
    ApplyForce* ret = new ApplyForce();
    if( ret && ret->initWithPhysicsSpriteTarget( physicsSpriteTarget, vector, offset, wake ) )
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE( ret );
        return NULL;
    }
    return ret;
}

bool ApplyForce::init( b2Body* body, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake )
{
    CCAssert( body, "ApplyForce::init() -- body undefined" );
    this->setBody( body );
    this->setVector( vector );
    this->setOffset( offset );
    this->setWake(true);
    return true;
}

bool ApplyForce::initWithPhysicsSpriteTarget( PhysicsSprite* physicsSpriteTarget, const cocos2d::Point& vector, const cocos2d::Point& offset, bool wake )
{
    CCAssert( physicsSpriteTarget, "ApplyForce::initWithPhysicsSpriteTarget() -- physicsSpriteTarget undefined" );
    this->setPhysicsSpriteTarget( physicsSpriteTarget );
    this->setVector( vector );
    this->setOffset( offset );
    this->setWake(true);
    return true;
}

void ApplyForce::startWithTarget(cocos2d::Node *target)
{
    super::startWithTarget(target);
    
    if( _physicsSpriteTarget )
    {
        this->setBody(_physicsSpriteTarget->getB2Body());
    }
}

void ApplyForce::update( float delta )
{    
    Point vector = this->getVector();
    Point offset = this->getOffset();
    
    this->getBody()->ApplyForce( ccpToVector(vector), ccpToVector(offset), this->getWake() );
}