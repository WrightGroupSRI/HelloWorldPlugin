#include "HelloWorldRenderObject.h"
#include "HelloWorldDataObject.h"

#include "rtBaseHandle.h"
#include "rtApplication.h"
#include "rtMessage.h"
#include "CompatibilityWithVTK.h"

#include <vtkPolyDataMapper.h>
#include <vtkFollower.h>
#include <vtkVectorText.h>

HelloWorldRenderObject::HelloWorldRenderObject( int id )
{
    setObjectType("HelloWorld");
    setName("HelloWorld Renderer");
    m_msgMapper = vtkPolyDataMapper::New();
    m_msgActor = vtkFollower::New();
    m_msgText = vtkVectorText::New();

    m_msgText->SetText("Hello World!");
    vtkSetInputConnection(m_msgMapper, m_msgText);
    m_msgActor->SetMapper(m_msgMapper);

    m_msgActor->SetPosition(0,0,0);
    m_msgActor->SetScale(10);

    // This object can be rendered in 3D
    m_canRender3D = true;

    setupDataObject(id);
}

HelloWorldRenderObject::~HelloWorldRenderObject()
{
    if (m_msgMapper) m_msgMapper->Delete();
    if (m_msgActor) m_msgActor->Delete();
    if (m_msgText) m_msgText->Delete();
}

//! Take info from the data object
void HelloWorldRenderObject::update()
{
    HelloWorldDataObject *dObj = static_cast<HelloWorldDataObject*>(m_dataObj);
    if (!dObj) {
      rtWarningMessage("Update failed! Data object is NULL.");
      return;
    }

    m_msgText->SetText(dObj->getMessage().toStdString().c_str());

}

//! Add this object to the given renderer.
bool HelloWorldRenderObject::addToRenderWidget(customQVTKWidget *renWid)
{
    if (!renWid)
    {
      rtWarningMessage("addToRenderer failed! Renderer object is NULL.");
      return false;
    }

    renWid->addPropToRenderer(m_msgActor,this);
    m_msgActor->SetCamera(renWid->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera());

    renWid->connectEvents(this);

    return true;
}

//! Remove this object from the given renderer.
bool HelloWorldRenderObject::removeFromRenderWidget(customQVTKWidget *renWid)
{
    if (!renWid) return false;
    renWid->removePropFromRenderer(m_msgActor);
    renWid->disconnectEvents(this);
    return true;
}

//! Create the correct data object.
void HelloWorldRenderObject::setupDataObject( int id )
{
  m_dataObj = new HelloWorldDataObject(id);
}

bool HelloWorldRenderObject::getObjectLocation(double loc[6])
{
    if (!m_msgActor)
    {
        rtErrorMessage("Could not get object location. Object actor is NULL.");
        return false;
    }

    m_msgActor->GetBounds(loc);
    return true;
}
