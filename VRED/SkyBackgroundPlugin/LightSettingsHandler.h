/********************************************************************
	created:	8.7.2009   10:59
	filename: 	e:\bilke\apps\vred-2.5\dev\SkyBackgroundPlugin\LightSettingsHandler.h
	author:		Lars Bilke
	
	purpose:	Definition of the light settings xml handler
*********************************************************************/
#ifndef LIGHTSETTINGSHANDLER_HEADER
#define LIGHTSETTINGSHANDLER_HEADER

// ** INCLUDES **
#include <QXml.h>
#include <QString.h>
#include <OpenSG/OSGVector.h>
#include <OpenSG/OSGColor.h>

OSG_USING_NAMESPACE

class QXmlParseException;

/**
 * LightSettingsHandler
 */
class LightSettingsHandler : public QXmlDefaultHandler
{
public:
	LightSettingsHandler();
	
	bool startElement(const QString &namespaceURI, const QString &localName,
		const QString &qName, const QXmlAttributes &attributes);
	bool endElement(const QString &namespaceURI, const QString &localName,
		const QString &qName);
	bool characters(const QString &str);
	bool fatalError(const QXmlParseException &exception);
	QString errorString() const;
	
	Vec3f direction;
	Color4f ambient;
	Color4f diffuse;
	Color4f specular;

private:
	bool inLight;
	QString errString;
	QString currentText;

};

#endif // LIGHTSETTINGSHANDLER_HEADER