#ifndef LEGOCAMERACONTROLLER_H
#define LEGOCAMERACONTROLLER_H

#include "legopointofviewcontroller.h"
#include "mxgeometry.h"
#include "mxgeometry/mxgeometry3d.h"
#include "mxgeometry/mxmatrix.h"

// VTABLE: LEGO1 0x100d57b0
// VTABLE: BETA10 0x101bb748
// SIZE 0xc8
class LegoCameraController : public LegoPointOfViewController {
public:
	LegoCameraController();
	~LegoCameraController() override; // vtable+0x00

	MxLong Notify(MxParam& p_param) override; // vtable+04

	// FUNCTION: LEGO1 0x10011ec0
	// FUNCTION: BETA10 0x1006a950
	const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f0850
		return "LegoCameraController";
	}

	// FUNCTION: LEGO1 0x10011ed0
	MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, ClassName()) || MxCore::IsA(p_name);
	}

	virtual void OnLButtonDown(MxPoint32 p_point);                // vtable+0x30
	virtual void OnLButtonUp(MxPoint32 p_point);                  // vtable+0x34
	virtual void OnRButtonDown(MxPoint32 p_point);                // vtable+0x38
	virtual void OnRButtonUp(MxPoint32 p_point);                  // vtable+0x3c
	virtual void OnMouseMove(MxU8 p_modifier, MxPoint32 p_point); // vtable+0x40
	virtual MxResult Create();                                    // vtable+0x44

	void SetWorldTransform(const Vector3& p_at, const Vector3& p_dir, const Vector3& p_up);
	void FUN_10012290(float p_angle);
	void FUN_10012320(float p_angle);
	MxResult FUN_100123b0(Matrix4& p_matrix);
	void FUN_100123e0(const Matrix4& p_transform, MxU32 p_und);
	Mx3DPointFloat GetWorldUp();
	Mx3DPointFloat GetWorldLocation();
	Mx3DPointFloat GetWorldDirection();

private:
	MxMatrix m_matrix1; // 0x38
	MxMatrix m_matrix2; // 0x80
};

// SYNTHETIC: LEGO1 0x10011f50
// LegoCameraController::`scalar deleting destructor'

#endif // LEGOCAMERACONTROLLER_H
