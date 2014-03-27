//
//  CCUtil.h
//
//  Created by George Guy on 8/14/12.
//  Copyright (c) 2012 Perpetual Motion Studios. All rights reserved.
//

#pragma once
#ifndef CCAndToast_CCUtil_h
#define CCAndToast_CCUtil_h
#include "extensions/cocos-ext.h"

const float DOUBLE_TOUCH_TIME = 0.75f;

#define CC_RANDOM_VARY(mean,variance) mean + ( CCRANDOM_MINUS1_1() * variance )

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

inline void ccArrayRandomShuffle(cocos2d::CCArray* array)
{
    using namespace cocos2d;
    
    for( unsigned int i = array->count() - 1; i > 0; i-- )
    {
        unsigned int j = CCRANDOM_0_1() * i;
        array->exchangeObjectAtIndex(i, j);
    }
}

inline void CCDictToDeck( std::vector<std::string>& deck, cocos2d::Dictionary* dict )
{
    using namespace cocos2d;
    using namespace std;
    
    if( dict )
    {
        DictElement* e = NULL;
        CCDICT_FOREACH(dict, e)
        {
            const string name = e->getStrKey();
            const int count = atoi(((String*) e->getObject())->getCString());
            
            for( int d = 0; d < count; d++ )
            {
                if( deck.size() ) deck.insert(deck.begin() + (rand() % deck.size()), name);
                else deck.push_back(name);
            }
        }
    }
}

#endif
