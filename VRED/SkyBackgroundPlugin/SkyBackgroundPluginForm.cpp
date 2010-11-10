/********************************************************************
	created:	1.7.2009   13:51
	filename: 	e:\bilke\apps\vred-2.5\dev\SkyBackgroundPlugin\SkyBackgroundPluginForm.cpp
	author:		Lars Bilke
	
	purpose:	Implementation of sky background plugin form
*********************************************************************/

// ** INCLUDES **
#include "SkyBackgroundPluginForm.h"

#include "qfiledialog.h"
#include "vrOSGWidget.h"
#include "vrLog.h"
#include "vrLights.h"
#include "vrImage.h"
#include "vrScript.h"
#include "vrScenegraph.h"

#include "exSlider.h"

#include "qcheckbox.h"
#include "qlineedit.h"
#include "QFile.h"
#include <QXml.h>

#include "OpenSG/OSGSimpleAttachments.h"

OSG_USING_NAMESPACE
using namespace std;

SkyBackgroundPluginForm::SkyBackgroundPluginForm( QWidget* parent /*= 0*/, const char* name /*= 0*/, WFlags fl /*= 0*/ )
	: SkyBackgroundPluginFormBase(parent, name, fl)
{

	settings.setPath("UFZ", "VRED-SkyBackgroundPlugin");

	skyDir = QDir(settings.readEntry("presetDir", "D:/Data/SkyBackgrounds"));
	presetDirLineEdit->setText(skyDir.absPath());

	presetDirs[0] = QDir("NiceDay");
	//presetDirs[1] = QDir("NiceDay/1024");
	//presetDirs[2] = QDir("NiceDay/512");

	skyBackground = NullFC;
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	ViewportPtr vredViewport = gl->getViewport();
	oldBackground = vredViewport->getBackground();

	for (int i = 0; i < 6; i++)
	{
		images[i] = Image::create();

		textures[i] = TextureChunk::create();
		beginEditCP(textures[i]);
		textures[i]->setMinFilter( GL_LINEAR );
		textures[i]->setMagFilter( GL_LINEAR );
		textures[i]->setWrapS    ( GL_CLAMP_TO_EDGE );	// other clamping causes black seams on the edges
		textures[i]->setWrapT    ( GL_CLAMP_TO_EDGE );	// of the sky cube because of gl_linear filtering
		textures[i]->setWrapR    ( GL_CLAMP_TO_EDGE );
		textures[i]->setEnvMode  ( GL_REPLACE );
		endEditCP(textures[i]);
	}

	LightDistanceExSlider->setRange(1, 10000);
	LightDistanceExSlider->setValue(1000);
}

SkyBackgroundPluginForm::~SkyBackgroundPluginForm()
{
	
}

void SkyBackgroundPluginForm::on_loadPushButton_clicked()
{
	QFileDialog fileDlg(this);
	QDir dir = settings.readEntry("loadDir", skyDir.absPath());
	if (dir.exists())
		fileDlg.setDir(dir);
	fileDlg.setMode(QFileDialog::DirectoryOnly);
	fileDlg.setCaption("Select a sky background directory");

	if (fileDlg.exec())
	{
		const QDir* loadDir = fileDlg.dir();
		setupSkyBackground(*loadDir, zUpCheckBox->isChecked());
		directoryLineEdit->setText(fileDlg.dir()->absPath());
		enableCheckBox->setChecked(true);

		settings.writeEntry("loadDir", fileDlg.dir()->absPath());
	}
}

void SkyBackgroundPluginForm::on_presetDirPushButton_clicked()
{
	QFileDialog fileDlg(this);
	QDir dir = settings.readEntry("presetDir", skyDir.absPath());
	if (dir.exists())
		fileDlg.setDir(dir);
	fileDlg.setMode(QFileDialog::DirectoryOnly);
	fileDlg.setCaption("Select the presets directory");

	if (fileDlg.exec())
	{
		skyDir = QDir(*fileDlg.dir());
		presetDirLineEdit->setText(fileDlg.dir()->absPath());

		settings.writeEntry("presetDir", fileDlg.dir()->absPath());
	}
}

void SkyBackgroundPluginForm::on_enableCheckBox_toggled( bool checked )
{
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	ViewportPtr vredViewport = gl->getViewport();
	beginEditCP(vredViewport);
	if (checked && skyBackground != NullFC)
		vredViewport->setBackground(skyBackground);
	else if (oldBackground != NullFC)
		vredViewport->setBackground(oldBackground);
	endEditCP(vredViewport);

	enableCheckBox->setChecked(checked); // for scripting
}

void SkyBackgroundPluginForm::on_zUpCheckBox_toggled( bool checked )
{
	if (directoryLineEdit->text().length() > 0)
	{
		QDir dir = QDir(directoryLineEdit->text());
		setupSkyBackground(dir, checked );
	}

	zUpCheckBox->setChecked(checked);	// for scripting
}

void SkyBackgroundPluginForm::setupSkyBackground( const QDir dir, const bool zUp )
{	
	try
	{
		skyBackground = NullFC;
		
		bool imageLoaded[6] = {false, false, false, false, false, false};

		const QFileInfoList* fileList = dir.entryInfoList();
		QFileInfoListIterator it(*fileList);
		QFileInfo* fileInfo;

		while ((fileInfo = it.current()) != 0)
		{
			QString qstr = fileInfo->absFilePath();
			const char *filename = qstr.latin1();

			if (((qstr.find("north") >= 0) || (qstr.find("front") >= 0)) && !imageLoaded[0])
			{
				beginEditCP(images[5]);
				if (images[5]->read(filename))
				{
					beginEditCP(textures[5]);
					textures[5]->setImage(images[5]);
					endEditCP(textures[5]);
					vrLog::info("Sky Background: Front/North image loaded.");
					imageLoaded[0] = true;
				}
				endEditCP(images[5]);
			}
			if (((qstr.find("south") >= 0) || (qstr.find("back") >= 0)) && !imageLoaded[1])
			{
				beginEditCP(images[4]);
				if (images[4]->read(filename))
				{
					if (zUp)
						vrImage::rotateImage180Degrees(images[4]);
					beginEditCP(textures[4]);
					textures[4]->setImage(images[4]);
					endEditCP(textures[4]);
					vrLog::info("Sky Background: Back/South image loaded.");
					imageLoaded[1] = true;
				}
				endEditCP(images[4]);
			}
			if (((qstr.find("down") >= 0) || (qstr.find("bottom") >= 0)) && !imageLoaded[2])
			{
				beginEditCP(images[3]);
				if (images[3]->read(filename))
				{
					beginEditCP(textures[3]);
					textures[3]->setImage(images[3]);
					endEditCP(textures[3]);
					vrLog::info("Sky Background: Bottom/Down image loaded.");
					imageLoaded[2] = true;
				}
				endEditCP(images[3]);
			}
			if (((qstr.find("up") >= 0) || (qstr.find("top") >= 0)) && !imageLoaded[3])
			{
				beginEditCP(images[2]);
				if (images[2]->read(filename))
				{
					if (zUp)
						vrImage::rotateImage180Degrees(images[2]);
					beginEditCP(textures[2]);
					textures[2]->setImage(images[2]);
					endEditCP(textures[2]);
					vrLog::info("Sky Background: Top/Up image loaded.");
					imageLoaded[3] = true;
				}
				endEditCP(images[2]);
			}
			if (((qstr.find("east") >= 0) || (qstr.find("right") >= 0)) && !imageLoaded[4])
			{
				beginEditCP(images[1]);
				if (images[1]->read(filename))
				{
					if (zUp)
						vrImage::rotate90Left(images[1]);
					beginEditCP(textures[1]);
					textures[1]->setImage(images[1]);
					endEditCP(textures[1]);
					vrLog::info("Sky Background: Right/East image loaded.");
					imageLoaded[4] = true;
				}
				endEditCP(images[1]);
			}
			if (((qstr.find("west") >= 0) || (qstr.find("left") >= 0)) && !imageLoaded[5])
			{
				beginEditCP(images[0]);
				if (images[0]->read(filename))
				{
					if (zUp)
						vrImage::rotate90Right(images[0]);
					beginEditCP(textures[0]);
					textures[0]->setImage(images[0]);
					endEditCP(textures[0]);
					vrLog::info("Sky Background: Left/West image loaded.");
					imageLoaded[5] = true;
				}
				endEditCP(images[0]);
			}

			++it;
		}

		skyBackground = SkyBackground::create();
		beginEditCP(skyBackground);
		if (!zUp)
		{
			skyBackground->setFrontTexture(textures[5]);
			skyBackground->setBackTexture(textures[4]);
			skyBackground->setBottomTexture(textures[3]);
			skyBackground->setTopTexture(textures[2]);
			skyBackground->setRightTexture(textures[1]);
			skyBackground->setLeftTexture(textures[0]);
		}
		else
		{
			skyBackground->setFrontTexture(textures[3]);
			skyBackground->setBackTexture(textures[2]);
			skyBackground->setBottomTexture(textures[4]);
			skyBackground->setTopTexture(textures[5]);
			skyBackground->setRightTexture(textures[1]);
			skyBackground->setLeftTexture(textures[0]);
		}
		endEditCP(skyBackground);

		vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
		ViewportPtr vredViewport = gl->getViewport();
		//oldBackground = vredViewport->getBackground();
		beginEditCP(vredViewport);
		vredViewport->setBackground(skyBackground);
		endEditCP(vredViewport);

		directoryLineEdit->setText(dir.absPath());

		// read light settings
		if (SetLightingCheckBox->isChecked())
		{
			string lightName = LightNameLineEdit->text().ascii();
			if (!QFile::exists(dir.absPath() + "/LightSettings.xml"))
				vrLog::warning("Light Settings not found.");
			else
			{
				QFile* file = new QFile(dir.absPath() + "/LightSettings.xml");
				if (file->open(IO_ReadOnly))
				{
					LightSettingsHandler handler;
					QXmlSimpleReader reader;
					reader.setContentHandler(&handler);
					reader.setErrorHandler(&handler);
					QXmlInputSource source(file);
					reader.parse(source);
					file->close();
					handler.direction.normalize();

					vector<NodePtr> lights;
					vrLights::getLights(lights);
					bool lightSet = false;
					for (vector<NodePtr>::const_iterator it = lights.begin();
						it != lights.end(); ++it)
					{
						LightPtr light = LightPtr::dcast((*it)->getCore());
						if (light != NullFC)
						{
							NodePtr node = *it;
							string name = getName(node);
							if (name.find(lightName) != string::npos)
							{
								setLightSettings(light, handler);
								lightSet = true;
							}
						}
					}

					if (!lightSet)
					{
						NodePtr rootNode = vrScenegraph::getRoot();

						TransformPtr beaconTransform = Transform::create();
						NodePtr beaconNode = Node::create();
						beginEditCP(beaconNode);
						beaconNode->setCore(beaconTransform);
						endEditCP(beaconNode);

						PointLightPtr light = PointLight::create();
						beginEditCP(light);
						light->setAttenuation(1,0,0);
						light->setBeacon(beaconNode);
						endEditCP(light);
						NodePtr lightNode = Node::create();
						beginEditCP(lightNode);
						lightNode->setCore(light);
						endEditCP(lightNode);
						OSG::setName(lightNode, lightName);

						beginEditCP(rootNode);
						rootNode->addChild(lightNode);
						rootNode->addChild(beaconNode);
						endEditCP(rootNode);

						setLightSettings(light, handler);

						vrScenegraph::update(true);
					}

				}
			}
		}
	}
	catch (std::exception const& e)
	{
		cout << "type: " << typeid(e).name() << endl;
		cout << "message: " << e.what() << endl << endl;
	}
}

void SkyBackgroundPluginForm::setLightSettings( LightPtr light, LightSettingsHandler &handler )
{
	beginEditCP(light);
	light->setAmbient(handler.ambient);
	light->setDiffuse(handler.diffuse);
	light->setSpecular(handler.specular);
	endEditCP(light);

	NodePtr beacon = light->getBeacon();
	if (beacon == NullFC)
	{
		TransformPtr beaconTransform = Transform::create();
		beacon = Node::create();
		beginEditCP(beacon);
		beacon->setCore(beaconTransform);
		endEditCP(beacon);

	}
	TransformPtr transform = TransformPtr::dcast(beacon->getCore());
	if (transform == NullFC)
	{
		TransformPtr beaconTransform = Transform::create();
		beginEditCP(beacon);
		beacon->setCore(beaconTransform);
		endEditCP(beacon);
		transform = beaconTransform;
	}
	Matrix m;
	float transFactor = LightDistanceExSlider->value();
	Vec3f translate = Vec3f(handler.direction.x() * transFactor, handler.direction.y() * transFactor,
		handler.direction.z() * transFactor);
	if (zUpCheckBox->isChecked())
		m.setTranslate(translate.x(), translate.y(), translate.z());
	else
		m.setTranslate(translate.x(), translate.z(), -translate.y());
	beginEditCP(transform);
	transform->setMatrix(m);
	endEditCP(transform);
}
void SkyBackgroundPluginForm::on_Preset1_1PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[0].path());
	dir.cd("05");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset1_2PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[0].path());
	dir.cd("12");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset1_3PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[0].path());
	dir.cd("16");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset1_4PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[0].path());
	dir.cd("19");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset2_1PushButton_clicked()
{
	QDir dir = skyDir;
	//dir.cd(presetDirs[1].path());
	dir.cd("Blury_256");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset2_2PushButton_clicked()
{
	QDir dir = skyDir;
	//dir.cd(presetDirs[1].path());
	dir.cd("CloudyEvening_1024");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset2_3PushButton_clicked()
{
	QDir dir = skyDir;
	//dir.cd(presetDirs[1].path());
	dir.cd("Dusty_512");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset2_4PushButton_clicked()
{
	QDir dir = skyDir;
	//dir.cd(presetDirs[1].path());
	dir.cd("SunnyBright_512");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset3_1PushButton_clicked()
{
	QDir dir = skyDir;
	//dir.cd(presetDirs[2].path());
	dir.cd("SunnyClouds_1024");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset3_2PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[2].path());
	dir.cd("08");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset3_3PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[2].path());
	dir.cd("12");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::on_Preset3_4PushButton_clicked()
{
	QDir dir = skyDir;
	dir.cd(presetDirs[2].path());
	dir.cd("19");
	setupSkyBackground(dir, zUpCheckBox->isChecked());
	directoryLineEdit->setText(dir.absPath());
	enableCheckBox->setChecked(true);
}

void SkyBackgroundPluginForm::setupSkyBackgroundStr( const string dir, const bool zUp )
{
	setupSkyBackground(QDir(QString::fromAscii(dir.c_str())), zUp);
}
