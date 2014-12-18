//
//  CCParallaxNodeExtra.h
//  Demo1
//
//  Created by Juan Carlos Araya on 07-09-13.
//
//

#ifndef __Demo1__CCParallaxNodeExtra__
#define __Demo1__CCParallaxNodeExtra__

#include "cocos2d.h"
#endif /* defined(__Demo1__CCParallaxNodeExtra__) */

USING_NS_CC;

class CCParallaxNodeExtras : public CCParallaxNode{
  
public:
    //CCParallaxNodeExtras(); // --> Constructor clase
    static CCParallaxNodeExtras * node(); //
    void incrementOffSet(CCPoint offset,CCNode * nodo);
    
};


