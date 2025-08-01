#ifndef JETSKI_H
#define JETSKI_H

#include "decomp.h"
#include "islepathactor.h"

class LegoControlManagerNotificationParam;

// VTABLE: LEGO1 0x100d9ec8
// VTABLE: BETA10 0x101ba540
// SIZE 0x164
class Jetski : public IslePathActor {
public:
	Jetski();

	// FUNCTION: LEGO1 0x1007e430
	// FUNCTION: BETA10 0x10037910
	const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f03d8
		return "Jetski";
	}

	// FUNCTION: LEGO1 0x1007e440
	MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, Jetski::ClassName()) || IslePathActor::IsA(p_name);
	}

	MxResult Create(MxDSAction& p_dsAction) override;                    // vtable+0x18
	void Animate(float p_time) override;                                 // vtable+0x70
	MxLong HandleClick() override;                                       // vtable+0xcc
	MxLong HandleControl(LegoControlManagerNotificationParam&) override; // vtable+0xd4
	void Exit() override;                                                // vtable+0xe4

	void ActivateSceneActions();

	MxS16 GetJetskiDashboardStreamId() { return m_jetskiDashboardStreamId; }

	// SYNTHETIC: LEGO1 0x1007e5c0
	// Jetski::`scalar deleting destructor'

private:
	void RemoveFromWorld();

	MxS16 m_jetskiDashboardStreamId; // 0x160
};

#endif // JETSKI_H
