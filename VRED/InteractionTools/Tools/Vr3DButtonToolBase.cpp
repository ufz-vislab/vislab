#include "Vr3DButtonToolBase.h"

CVr3DButtonToolBase::CVr3DButtonToolBase(void)
{
}

CVr3DButtonToolBase::~CVr3DButtonToolBase(void)
{
}

void CVr3DButtonToolBase::OnTrackingButtonPressed(Quaternion q, Vec3f position){
	std::cout << "button 0 pressed: " << std::endl;
	std::cout << "quaternion: " << q << std::endl;
	std::cout << "position: " << position << std::endl;

	Matrix m;
	m.setIdentity();
	m.setRotate(q);

	Vec3f ez(0.0, 0.0, -1.0);
	m.mult(ez);
	std::cout << "translation vector: " << ez << std::endl;
};

void CVr3DButtonToolBase::OnTrackingButtonReleased(Quaternion q, Vec3f position){
	std::cout << "button 0 released: " << q << " " << position << std::endl; 
};

void CVr3DButtonToolBase::OnTrackingMove(Quaternion q, Vec3f position){
}
