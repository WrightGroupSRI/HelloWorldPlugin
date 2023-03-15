#ifndef HELLOWORLDRENDEROBJECT_H
#define HELLOWORLDRENDEROBJECT_H

#include "rtRenderObject.h"

class vtkFollower;
class vtkPolyDataMapper;
class vtkVectorText;

class HelloWorldRenderObject : public rtRenderObject
{
public:
    //! Constructor
    HelloWorldRenderObject( int id );

    //! Destructor
    ~HelloWorldRenderObject();

    //! Add this object to the given renderer.
    virtual bool addToRenderWidget(customQVTKWidget *renWid);

    //! Remove this object from the given renderer.
    virtual bool removeFromRenderWidget(customQVTKWidget *renWid);

    //! Get the object's location
    virtual bool getObjectLocation(double loc[6]);

protected:
    void setupDataObject( int id );
    void setupPipeline();
    virtual void update();
private:
    vtkPolyDataMapper *m_msgMapper;
    vtkFollower *m_msgActor;
    vtkVectorText *m_msgText;
};

#endif // HELLOWORLDRENDEROBJECT_H
