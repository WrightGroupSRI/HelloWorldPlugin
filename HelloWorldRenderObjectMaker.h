#ifndef HELLOWORLDRENDEROBJECTMAKER_H
#define HELLOWORLDRENDEROBJECTMAKER_H

#include "rtRenderObjectMaker.h"

class HelloWorldRenderObjectMaker : public rtRenderObjectMaker
{
public:
  virtual rtRenderObject *createObject( int id ) const;
};

#endif // HELLOWORLDRENDEROBJECTMAKER_H
