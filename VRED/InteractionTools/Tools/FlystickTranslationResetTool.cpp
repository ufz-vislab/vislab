#include "FlystickTranslationResetTool.h"

#include <OpenSG/OSGTransform.h>

CFlystickTranslationResetTool::CFlystickTranslationResetTool(void){
}

CFlystickTranslationResetTool::~CFlystickTranslationResetTool(void){
}

bool CFlystickTranslationResetTool::SetAttr(std::string attr, std::string value){
	return CFlystickPickTool::SetAttr(attr, value);
}

void CFlystickTranslationResetTool::OnTrackingButtonReleased(Quaternion q, Vec3f position){
	std::cout << "CFlystickTranslationResetTool::OnTrackingButtonReleased()" << std::endl;

	if (! m_bToolActive){
		std::cout << "tool is not active" << std::endl;
		return;
	}

	if (m_posgPickedObjectNode == NullFC){
		std::cout << "CFlystickTranslationResetTool: No object picked to reset translation" << std::endl;
		return;
	}

	NodePtr parentingTransformNode = GetParentTransformNode(m_posgPickedObjectNode);
	if (parentingTransformNode != NullFC){
		std::cout << "CFlystickTranslationResetTool: Resetting translation!" << std::endl;
		TransformPtr transform = TransformPtr::dcast(parentingTransformNode->getCore());
		Matrix m = transform->getMatrix();
		m.setTranslate(0.0, 0.0, 0.0);
		beginEditCP(transform, Transform::MatrixFieldMask);{
			transform->setMatrix(m);
		};endEditCP(transform, Transform::MatrixFieldMask);
	}else{
		std::cout << "CFlystickTranslationResetTool: Found no parenting transform to reset!" << std::endl;
	}

	m_bToolActive = false;
}