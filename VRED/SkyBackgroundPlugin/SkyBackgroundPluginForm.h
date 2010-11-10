/********************************************************************
	created:	1.7.2009   13:50
	filename: 	e:\bilke\apps\vred-2.5\dev\SkyBackgroundPlugin\SkyBackgroundPluginForm.h
	author:		Lars Bilke
	
	purpose:	Definition of the sky background plugin form
*********************************************************************/
#ifndef SKYBACKGROUNDPLUGINFORM_HEADER
#define SKYBACKGROUNDPLUGINFORM_HEADER

// ** INCLUDES **
#include "SkyBackgroundPluginFormBase.h"
#include "LightSettingsHandler.h"

#include "OpenSG/OSGImage.h"
#include "OpenSG/OSGTextureChunk.h"
#include "OpenSG/OSGSkyBackground.h"
#include "OpenSG/OSGLight.h"

#include "qdir.h"
#include "qsettings.h"

#include <string>

OSG_USING_NAMESPACE
using namespace std;


/**
 * SkyBackgroundPluginForm
 */
class SkyBackgroundPluginForm : public SkyBackgroundPluginFormBase
{
	Q_OBJECT

public:
	
	SkyBackgroundPluginForm(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
	~SkyBackgroundPluginForm();

private:

	SkyBackgroundPtr skyBackground;
	BackgroundPtr oldBackground;

	ImagePtr images[6];
	TextureChunkPtr textures[6];
	QDir skyDir;
	QDir presetDirs[3];

	QSettings settings;

	void setLightSettings( LightPtr light, LightSettingsHandler &handler );

public slots:
	void setupSkyBackground( const QDir dir, const bool zUp );

	void setupSkyBackgroundStr( const string dir, const bool zUp );
	void on_enableCheckBox_toggled(bool checked);
	void on_zUpCheckBox_toggled(bool checked);

private slots:
	void on_loadPushButton_clicked();
	void on_presetDirPushButton_clicked();

	void on_Preset1_1PushButton_clicked();
	void on_Preset1_2PushButton_clicked();
	void on_Preset1_3PushButton_clicked();
	void on_Preset1_4PushButton_clicked();

	void on_Preset2_1PushButton_clicked();
	void on_Preset2_2PushButton_clicked();
	void on_Preset2_3PushButton_clicked();
	void on_Preset2_4PushButton_clicked();


	void on_Preset3_1PushButton_clicked();
	void on_Preset3_2PushButton_clicked();
	void on_Preset3_3PushButton_clicked();
	void on_Preset3_4PushButton_clicked();

};

#endif // SKYBACKGROUNDPLUGINFORM_HEADER
