
#include "HelloWorldRenderObjectMaker.h"
#include "HelloWorldRenderObject.h"

rtRenderObject *HelloWorldRenderObjectMaker::createObject(int id ) const {
  return new HelloWorldRenderObject( id );
}
