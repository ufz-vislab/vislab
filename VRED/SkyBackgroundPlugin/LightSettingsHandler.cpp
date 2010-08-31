/********************************************************************
	created:	8.7.2009   11:21
	filename: 	e:\bilke\apps\vred-2.5\dev\SkyBackgroundPlugin\LightSettingsHandler.cpp
	author:		Lars Bilke
	
	purpose:	Implementation of the light settings xml handler
*********************************************************************/

// ** INCLUDES **
#include "LightSettingsHandler.h"

#include <QMessageBox.h>
#include <QStringList.h>

OSG_USING_NAMESPACE


LightSettingsHandler::LightSettingsHandler()
{
	direction = Vec3f(0.0f, 0.0f, 1.0f);
	ambient = Color4f(0.2f, 0.2f, 0.2f, 1.0f);
	diffuse = Color4f(0.8f, 0.8f, 0.8f, 1.0f);
	specular = Color4f(1.0f, 1.0f, 0.8f, 1.0f);

	inLight = false;
}

bool LightSettingsHandler::startElement( const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes )
{
	if (qName == "Light")
		inLight = true;
	currentText = "";
	return true;
}

bool LightSettingsHandler::endElement( const QString &namespaceURI, const QString &localName, const QString &qName )
{
	if (qName == "Light")
		inLight = false;
	else
	{
		QStringList values = QStringList::split(" ", currentText);
		while (values.size() < 3)
			values.append("1.0");

		if (qName == "Direction")
			direction = Vec3f(values[0].toFloat(), values[1].toFloat(), values[2].toFloat());
		else if (qName == "Ambient")
			ambient = Color4f(values[0].toFloat(), values[1].toFloat(), values[2].toFloat(), 1.0f);
		else if (qName == "Diffuse")
			diffuse = Color4f(values[0].toFloat(), values[1].toFloat(), values[2].toFloat(), 1.0f);
		else if (qName == "Specular")
			specular = Color4f(values[0].toFloat(), values[1].toFloat(), values[2].toFloat(), 1.0f);
	}
	return true;	
}

bool LightSettingsHandler::characters( const QString &str )
{
	currentText += str;
	return true;
}

bool LightSettingsHandler::fatalError( const QXmlParseException &exception )
{
	QMessageBox::information( 0, QObject::tr("Light Settings XML Handler"),
		QObject::tr("Parse error in line %1, column %2:\n %3")
		.arg(exception.lineNumber()).arg(exception.columnNumber())
		.arg(exception.message()));
	return false;
}

QString LightSettingsHandler::errorString() const
{
	return errString;
}