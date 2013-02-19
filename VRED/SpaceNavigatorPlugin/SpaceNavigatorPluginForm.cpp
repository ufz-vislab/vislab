/********************************************************************
	created:	6.5.2009   10:26
	filename: 	e:\bilke\apps\vred-2.5\dev\CityGeneratorPlugin\PluginForm.cpp
	author:		Lars Bilke
	
	purpose:	Implementation of the plugin form
*********************************************************************/

// ** INCLUDES **
#include "qstring.h"
#include "qregexp.h"
#include "qpushbutton.h"
#include "qlineedit.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qfiledialog.h"
#include "qsettings.h"

#include "SpaceNavigatorPluginForm.h"
#include "SpaceNavigatorOSGClient.h"

#include "vrScenegraph.h"
#include "vrNodePtr.h"
//#include "vrController.h"
#include "vrOSGWidget.h"
//#include "vrNavigator.h"
#include "vrLog.h"
#include "exSlider.h"


SpaceNavigatorPluginForm::SpaceNavigatorPluginForm( QWidget* parent /*= 0*/, const char* name /*= 0*/,
	WFlags fl /*= 0*/ )
	: SpaceNavigatorPluginFormBase( parent, name, fl )
{
	translationFactorSlider->setRange(0.01, 50.0);
	translationFactorSlider->setValue(10.0f);
	rotationFactorSlider->setRange(0.1, 2.0);
	rotationFactorSlider->setValue(1.0f);

	QSettings settings;
	settings.setPath("UFZ", "VRED-SpaceNavigatorPlugin");
	QString connectString = settings.readEntry("connectionString",
		QString("spacenav@141.65.34.28"));
	vrpnDeviceLineEdit->setText(connectString);
}

SpaceNavigatorPluginForm::~SpaceNavigatorPluginForm() 
{
}

void SpaceNavigatorPluginForm::CheckVRPNDeviceTextEditInput( const QString& text )
{
	initButton->setEnabled(false);
	QRegExp regex("^([0-9a-zA-Z]+[-._+&amp;])*[0-9a-zA-Z]+@([-0-9a-zA-Z]+[.])*", false);
	if (regex.search(text) >= 0)
		initButton->setEnabled(true);
}

void SpaceNavigatorPluginForm::SetConnectString( const char* connectString )
{
	vrpnDeviceLineEdit->setText(QString(connectString));
	QSettings settings;
	settings.setPath("UFZ", "VRED-SpaceNavigatorPlugin");
	settings.writeEntry("connectionString", QString(connectString));
}
void SpaceNavigatorPluginForm::ConnectToDevice()
{
	if (vrOSGWidget::getMGLW(-1)->getNavigator().getCameraZUp())
		SpaceNavigatorOSGClient::Instance()->init(vrpnDeviceLineEdit->text().ascii(), SpaceNavigatorOSGClient::Z);
	else
		SpaceNavigatorOSGClient::Instance()->init(vrpnDeviceLineEdit->text().ascii());
	SpaceNavigatorOSGClient::Instance()->setTranslationFactorToSceneSize(vrOSGWidget::getMGLW(-1)->getRoot());
	EnableCheckBox->setEnabled(true);
	SetTranslationFactor(translationFactorSlider->value());
	SetRotationFactor(rotationFactorSlider->value());
	SetEnableOrbitMode(OrbitModeCheckBox->isChecked());
	SetEnableHeightControl(HeightControlCheckBox->isChecked());

}
void SpaceNavigatorPluginForm::SetEnableSpaceNavigator( bool enable )
{
	vrOSGWidget* osgWidget = vrOSGWidget::getMGLW(-1);

	if(enable)
	{		
		// add this class to the VRED loop
		addLoop();

		osgWidget->getNavigator().setMode(vrNavigator::WALK);

		if(osgWidget->getNavigator().getCameraZUp())
			_heightOverZero = osgWidget->getNavigator().getFrom().z();
		else
			_heightOverZero = osgWidget->getNavigator().getFrom().y();
	}
	else
	{
		// sub this class to the VRED loop
		subLoop();

		osgWidget->getNavigator().setMode(vrNavigator::TRACKBALL);
	}

	EnableCheckBox->setChecked(enable);	// for command line
}

void SpaceNavigatorPluginForm::SetRotationFactor( float value )
{
	SpaceNavigatorOSGClient::Instance()->setRotationFactor(value);
	rotationFactorSlider->setValue(value);	// for command line
}

void SpaceNavigatorPluginForm::SetTranslationFactor( float value )
{
	SpaceNavigatorOSGClient::Instance()->setTranslationFactor(value);
	translationFactorSlider->setValue(value);	// for command line
}

void SpaceNavigatorPluginForm::SetDominationMode( bool enable )
{
	SpaceNavigatorOSGClient::Instance()->setDomination(enable);
	DominationModeCheckBox->setChecked(enable);	// for command line
}

void SpaceNavigatorPluginForm::SetNavigationMode( int mode )
{
	SpaceNavigatorOSGClient::Instance()->setMode((SpaceNavigatorClient::SpaceNavigatorMode)mode);
	NavigationModeComboBox->setCurrentItem(mode);	// for command line
}

void SpaceNavigatorPluginForm::SetEnableOrbitMode( bool enable )
{
	_isOrbitMode = enable;
	OrbitModeCheckBox->setChecked(enable);	// for command line
}
void SpaceNavigatorPluginForm::SetEnableHeightControl( bool enable )
{
	SpaceNavigatorOSGClient::Instance()->setHeightControl(enable);
	HeightControlCheckBox->setChecked(enable);	// for command line
}
void SpaceNavigatorPluginForm::WalkNavigatorForward( float step, Pnt3f& oldFrom, Pnt3f& oldAt, Vec3f& oldUp )
{
	Vec3f lv = oldFrom - oldAt;
	lv.normalize();

	Vec3f upn = oldUp;
	upn.normalize();

	Vec3f mv = lv - upn.dot(lv)*upn;
	mv.normalize();

	//side vector symbolizes shoulders
	Vec3f sv = mv;
	sv.crossThis(upn);
	sv.normalize();

	oldFrom = oldFrom + step*mv;
	if (!_isOrbitMode)
		oldAt = oldAt + step*mv;
	
	// The following does not work!!
	//osgWidget->getNavigator().setFrom(rFrom);
	//osgWidget->getNavigator().setAt(rAt);

}

void SpaceNavigatorPluginForm::WalkNavigatorRight( float step, Pnt3f& oldFrom, Pnt3f& oldAt, Vec3f& oldUp )
{
	Vec3f lv = oldFrom - oldAt;
	lv.normalize();

	Vec3f upn = oldUp;
	upn.normalize();

	Vec3f mv = lv - upn.dot(lv)*upn;
	mv.normalize();

	Vec3f sv = mv;
	sv.crossThis(upn);
	sv.normalize();

	oldFrom = oldFrom + step*sv;
	if (!_isOrbitMode)
		oldAt = oldAt + step*sv;

}

void SpaceNavigatorPluginForm::WalkNavigatorRotate( float deltaX, float deltaY, Pnt3f& oldFrom, Pnt3f& oldAt, Vec3f& oldUp )
{
	// rotate around the up vector
	Matrix final,temp;
	Quaternion q;

	q.setValueAsAxisRad(oldUp,-deltaX);
	q.getValue(temp);
	final.setIdentity();
	final.setTranslate(oldFrom);
	final.mult(temp);

	temp.setIdentity();
	temp.setTranslate(-oldFrom[0],-oldFrom[1],-oldFrom[2]);

	final.mult(temp);
	final.multMatrixPnt(oldAt);

	// rotate around the side vector
	Vec3f lv = oldAt-oldFrom;
	lv.normalize();

	Vec3f sv = lv;
	sv.crossThis(oldUp);
	sv.normalize();
	q.setValueAsAxisRad(sv,-deltaY);
	q.getValue(temp);

	final.setIdentity();
	final.setTranslate(oldFrom);
	final.mult(temp);

	temp.setIdentity();
	temp.setTranslate(-oldFrom[0],-oldFrom[1],-oldFrom[2]);

	final.mult(temp);
	final.multMatrixPnt(oldAt);
}

void SpaceNavigatorPluginForm::loop()
{
	SpaceNavigatorOSGClient* spaceNavigator = SpaceNavigatorOSGClient::Instance();
	spaceNavigator->update();

	vrOSGWidget* osgWidget = vrOSGWidget::getMGLW(-1);

	if(osgWidget->getNavigator().getCameraZUp())
		spaceNavigator->setZUpAxis(true);
	else
		spaceNavigator->setZUpAxis(false);

	if(!osgWidget)
		return;

	Vec3f trans = spaceNavigator->getTranslation();
	Vec3f rot = spaceNavigator->getRotation();

	Pnt3f rFrom = osgWidget->getNavigator().getFrom();
	Pnt3f rAt = osgWidget->getNavigator().getAt();
	Vec3f vUp;
	if(osgWidget->getNavigator().getCameraZUp())
		vUp = Vec3f(0,0,1);
	else
		vUp = Vec3f(0,1,0);

	if(osgWidget->getNavigator().getCameraZUp())
	{
		if (!_isOrbitMode)
			WalkNavigatorRotate(-rot.z(), -rot.x(), rFrom, rAt, vUp);
		WalkNavigatorForward(trans.y(), rFrom, rAt, vUp);
	}
	else
	{
		if (!_isOrbitMode)
			WalkNavigatorRotate(-rot.y(), -rot.x(), rFrom, rAt, vUp);
		WalkNavigatorForward(trans.z(), rFrom, rAt, vUp);
	}
	WalkNavigatorRight(-trans.x(), rFrom, rAt, vUp);

	// update the distance to the ground
	Vec3f fromToAt = rAt - rFrom;
	if(osgWidget->getNavigator().getCameraZUp())
	{
		if(spaceNavigator->getHeightControl())
			_heightOverZero += trans.z();

		if (_isElevGridEnabled)
		{
			float elevGridZ = _elevGrid.GetZ(rFrom.x(), rFrom.y());
			rFrom.setValues(rFrom.x(), rFrom.y(), _heightOverZero + elevGridZ);
		}
		else
			rFrom.setValues(rFrom.x(), rFrom.y(), _heightOverZero);
	}
	else
	{
		if(spaceNavigator->getHeightControl())
			_heightOverZero += trans.y();
		rFrom.setValues(rFrom.x(), _heightOverZero, rFrom.z());
	}
	rAt = rFrom + fromToAt;
	
	osgWidget->setFromAtUp(-1, rFrom, rAt, vUp);
}

void SpaceNavigatorPluginForm::ElevGridLoadFile()
{
	QFileDialog fileDlg(this);

	if (fileDlg.exec())
		ElevGridLoadFileCL(fileDlg.selectedFile().ascii());
}

void SpaceNavigatorPluginForm::ElevGridLoadFileCL( const char* file )
{
	if(_elevGrid.LoadFile(file))
	{
		_elevGrid.SortIntoGrid(ElevGridCellXLineEdit->text().toInt(), ElevGridCellXLineEdit->text().toInt());
		EnableElevGridCheckBox->setEnabled(true);
		ElevGridLineEdit->setText(file);
	}
	else
		vrLog::warning("File could not load!");
}
void SpaceNavigatorPluginForm::ElevGridLoadSelected()
{
	vrNodePtr node = vrScenegraph::getSelectedNode();
	if (node == NullFC)
	{
		vrLog::warning("No node selected!");
		return;
	}
	_elevGrid.LoadNode(node);
	_elevGrid.SortIntoGrid(ElevGridCellXLineEdit->text().toInt(), ElevGridCellXLineEdit->text().toInt());
	EnableElevGridCheckBox->setEnabled(true);
	ElevGridLineEdit->setText(getName(node));
}

void SpaceNavigatorPluginForm::SetEnableElevGrid( bool enable )
{
	_isElevGridEnabled = enable;
	EnableElevGridCheckBox->setChecked(enable); // for command line
}
