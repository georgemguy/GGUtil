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

#pragma once
#ifndef CCAndToast_CCUtil_h
#define CCAndToast_CCUtil_h
#include "extensions/cocos-ext.h"

#define CC_RANDOM_VARY(mean,variance) ((mean) + ( CCRANDOM_MINUS1_1() * variance ))

inline cocos2d::Node* ScaleNodeToSize( cocos2d::Node* node, cocos2d::Size size )
{
    cocos2d::Size oldSize = node->getContentSize();
    
    node->setScaleX( ( size.width / oldSize.width ) / node->getScaleX() );
    node->setScaleY( ( size.height / oldSize.height ) / node->getScaleY() );
    return node;
}

inline cocos2d::Node* ScaleNodeAndContent( cocos2d::Node* node, cocos2d::Size size )
{
    cocos2d::Size oldSize = node->getContentSize();
    
    node->setContentSize( size );
    node->setScaleX( ( size.width / oldSize.width ) / node->getScaleX() );
    node->setScaleY( ( size.height / oldSize.height ) / node->getScaleY() );
    
    return node;
}

inline cocos2d::Color3B InverseColor3B( cocos2d::Color3B color )
{
    return cocos2d::Color3B( 255 - color.r, 255 - color.g, 255 - color.b );
}

inline cocos2d::Point DiscretePoint( cocos2d::Point point, const cocos2d::Point& min, const cocos2d::Point& max )
{
    point.x = floorf( point.x + 0.5f );
    point.y = floorf( point.y + 0.5f );
    return point.getClampPoint(min, max);
}

inline cocos2d::Point DiscretePoint( cocos2d::Point point )
{
    point.x = floorf( point.x + 0.5f );
    point.y = floorf( point.y + 0.5f );
    return point;
}

inline bool IsTouchInsideNode(cocos2d::Touch* touch, cocos2d::Node* node)
{
    cocos2d::Point touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = node->getParent()->convertToNodeSpace(touchLocation);
    cocos2d::Rect bBox = node->boundingBox();
    return bBox.containsPoint(touchLocation);
}

cocos2d::Color3B Color3BFromString(const char* pszContent);

inline std::string toUppercase(const std::string & Text)
{
    std::string UppperCaseString;
    UppperCaseString.reserve(Text.size());
    for (std::string::const_iterator it=Text.begin(); it<Text.end(); ++it)
    {
        UppperCaseString.push_back(((0x60 < *it) && (*it < 0x7B)) ? (*it - static_cast<char>(0x20)) : *it);
    }
    return UppperCaseString;
}

inline void ccArrayRandomShuffle(cocos2d::__Array* array)
{
    using namespace cocos2d;
    
    for( int i = array->count() - 1; i > 0; i-- )
    {
        array->exchangeObjectAtIndex(i, CCRANDOM_0_1() * i);
    }
}

inline void CCDictToDeck( std::vector<std::string>& deck, cocos2d::__Dictionary* dict )
{
    using namespace cocos2d;
    using namespace std;
    
    if( dict )
    {
        DictElement* e = NULL;
        CCDICT_FOREACH(dict, e)
        {
            const string name = e->getStrKey();
            const int count = atoi(((__String*) e->getObject())->getCString());
            
            for( int d = 0; d < count; d++ )
            {
                if( deck.size() ) deck.insert(deck.begin() + (rand() % deck.size()), name);
                else deck.push_back(name);
            }
        }
    }
}

#endif
