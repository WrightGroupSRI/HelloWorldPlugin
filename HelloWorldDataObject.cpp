#include "HelloWorldDataObject.h"
#include "rtApplication.h"
#include "rtMessage.h"
#include "rtBaseHandle.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

//! Constructor
HelloWorldDataObject::HelloWorldDataObject( int id ):
    rtDataObject( id )
{
    m_message = "Hello World!";
    setupGUI();
}

//! Destructor
HelloWorldDataObject::~HelloWorldDataObject()
{
    cleanupGUI();
}

void HelloWorldDataObject::update()
{

}

const QString& HelloWorldDataObject::getObjectType() const {
  const static QString type("HelloWorld");
  return type;
}

const QString& HelloWorldDataObject::getFriendlyObjectType() const {
  const static QString type("Hello World <3");
  return type;
}

//! Set the GUI widgets
void HelloWorldDataObject::setupGUI()
{
    m_optionsWidget.setupUi(getBaseWidget());
    connect(m_optionsWidget.messageText, SIGNAL(textChanged(QString)), this, SLOT(messageChanged(QString)));
}

//! Clean the GUI widgets
void HelloWorldDataObject::cleanupGUI()
{

}

//! Save the object to a file
bool HelloWorldDataObject::saveFile(QFile *file)
{
    if (!file->open(QIODevice::WriteOnly)) {
      rtErrorMessage("Failed to open file for writing. Error Code: " << file->error());
      return false;
    }
    QXmlStreamWriter writer(file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("VurtigoFile");
    rtDataObject::saveHeader(&writer, getObjectType(), getObjName());
    rtDataObject::saveMetadata(&writer, m_metadata);

    ////////////////////////////////
    // WRITE OBJECT SPECIFIC DATA HERE
    writer.writeTextElement("message",m_message);
    ////////////////////////////////


    writer.writeEndElement(); // vurtigofile
    writer.writeEndDocument();
    file->close();
    return true;
}

//! Load the object from a file
bool HelloWorldDataObject::loadFile(QFile *file)
{
    if (!file->open(QIODevice::ReadOnly)) {
      rtErrorMessage("Failed to open file for reading. Error Code: " << file->error());
      return false;
    }
    QXmlStreamReader reader(file);
    QString objType;
    QString objName="";
    QString version="";
    while (!reader.atEnd()) {
      if (reader.readNext() == QXmlStreamReader::StartElement )
      {
        if (reader.name() == "FileInfo")
        {
          rtDataObject::loadHeader(&reader, objType, objName,version);
        }
        else if (reader.name() == "Metadata")
        {
          rtDataObject::loadMetadata(&reader, m_metadata);
        }
        else if (reader.name() == "message")
        {
            m_optionsWidget.messageText->setText(reader.readElementText());
        }
      }
    }
    if (reader.hasError())
    {
      return false;
      rtErrorMessage("XML Reader Failed. Error: " << reader.errorString());
    }
    Modified();
    return true;

}

//! The message has changed
void HelloWorldDataObject::messageChanged(const QString &text)
{
    m_message = text;
    Modified();
}
