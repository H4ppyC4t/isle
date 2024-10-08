#ifndef LEGOCARBUILD_H
#define LEGOCARBUILD_H

#include "legostate.h"
#include "legoworld.h"

// VTABLE: LEGO1 0x100d66e0
// SIZE 0x50
class LegoVehicleBuildState : public LegoState {
public:
	LegoVehicleBuildState(const char* p_classType);

	// FUNCTION: LEGO1 0x10025ff0
	const char* ClassName() const override // vtable+0x0c
	{
		return this->m_className.GetData();
	}

	// FUNCTION: LEGO1 0x10026000
	MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, this->m_className.GetData()) || LegoState::IsA(p_name);
	}

	MxResult Serialize(LegoFile* p_file) override; // vtable+0x1c

	// SYNTHETIC: LEGO1 0x100260a0
	// LegoVehicleBuildState::`scalar deleting destructor'

	// TODO: Most likely getters/setters are not used according to BETA.

	Playlist m_unk0x08[4]; // 0x08

	// This can be one of the following:
	// * LegoRaceCarBuildState
	// * LegoCopterBuildState
	// * LegoDuneCarBuildState
	// * LegoJetskiBuildState
	MxString m_className; // 0x38

	// Known States:
	// * 1 == enter(ing) build screen
	// * 3 == cutscene/dialogue
	// * 6 == exit(ing) build screen
	MxU32 m_animationState; // 0x48
	undefined m_unk0x4c;    // 0x4c
	MxBool m_unk0x4d;       // 0x4d
	MxBool m_unk0x4e;       // 0x4e
	MxU8 m_placedPartCount; // 0x4f
};

typedef LegoVehicleBuildState LegoRaceCarBuildState;
typedef LegoVehicleBuildState LegoCopterBuildState;
typedef LegoVehicleBuildState LegoDuneCarBuildState;
typedef LegoVehicleBuildState LegoJetskiBuildState;

// VTABLE: LEGO1 0x100d6658
// SIZE 0x34c
class LegoCarBuild : public LegoWorld {
public:
	LegoCarBuild();
	~LegoCarBuild() override;

	MxLong Notify(MxParam& p_param) override; // vtable+0x04
	MxResult Tickle() override;               // vtable+0x08

	// FUNCTION: LEGO1 0x10022940
	const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f0504
		return "LegoCarBuild";
	}

	// FUNCTION: LEGO1 0x10022950
	MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, LegoCarBuild::ClassName()) || LegoWorld::IsA(p_name);
	}

	MxResult Create(MxDSAction& p_dsAction) override; // vtable+0x18
	void ReadyWorld() override;                       // vtable+0x50
	MxBool VTable0x5c() override;                     // vtable+0x5c
	MxBool Escape() override;                         // vtable+0x64
	void Enable(MxBool p_enable) override;            // vtable+0x68
	virtual void VTable0x6c();                        // vtable+0x6c
	virtual void VTable0x70();                        // vtable+0x70
	virtual void VTable0x74();                        // vtable+0x74
	virtual void VTable0x78();                        // vtable+0x78
	virtual void VTable0x7c();                        // vtable+0x7c
	virtual void VTable0x80();                        // vtable+0x80

	// SYNTHETIC: LEGO1 0x10022a60
	// LegoCarBuild::`scalar deleting destructor'

private:
	undefined4 m_unk0xf8;       // 0xf8
	undefined m_unk0xfc[0x8];   // 0xfc
	undefined4 m_unk0x104;      // 0x104
	undefined m_unk0x108;       // 0x108
	undefined m_unk0x109;       // 0x109
	undefined4 m_unk0x10c;      // 0x10c
	undefined4 m_unk0x110;      // 0x110
	Mx3DPointFloat m_unk0x114;  // 0x114
	undefined4 m_unk0x128;      // 0x128
	MxMatrix m_unk0x12c;        // 0x12c
	undefined m_unk0x174;       // 0x174
	MxMatrix m_unk0x178;        // 0x178
	MxMatrix m_unk0x1c0;        // 0x1c0
	MxMatrix m_unk0x208;        // 0x208
	undefined m_unk0x250[0x08]; // 0x250
	undefined4 m_unk0x258;      // 0x258
	Mx4DPointFloat m_unk0x25c;  // 0x25c
	Mx4DPointFloat m_unk0x274;  // 0x274
	undefined m_unk0x28c[0x18]; // 0x28c
	Mx4DPointFloat m_unk0x2a4;  // 0x2a4
	Mx4DPointFloat m_unk0x2bc;  // 0x2bc
	undefined m_unk0x2d4;       // 0x2d4
	undefined4 m_unk0x2dc;      // 0x2dc
	undefined4 m_unk0x2e0;      // 0x2e0
	undefined4 m_unk0x2e4;      // 0x2e4
	undefined4 m_unk0x2e8;      // 0x2e8
	undefined4 m_unk0x2ec;      // 0x2ec
	undefined4 m_unk0x2f0;      // 0x2f0
	undefined4 m_unk0x2f4;      // 0x2f4
	undefined4 m_unk0x2f8;      // 0x2f8
	undefined4 m_unk0x2fc;      // 0x2fc
	undefined4 m_unk0x300;      // 0x300
	undefined4 m_unk0x304;      // 0x304
	undefined4 m_unk0x308;      // 0x308
	undefined4 m_unk0x30c;      // 0x30c
	undefined4 m_unk0x310;      // 0x310
	undefined4 m_unk0x314;      // 0x314
	undefined4 m_unk0x318;      // 0x318
	undefined4 m_unk0x31c;      // 0x31c
	undefined4 m_unk0x320;      // 0x320
	undefined4 m_unk0x324;      // 0x324
	undefined4 m_unk0x328;      // 0x328
	undefined4 m_unk0x2d8;      // 0x2d8
	undefined4 m_unk0x32c;      // 0x32c
	undefined4 m_unk0x330;      // 0x330
	undefined4 m_unk0x334;      // 0x334
	undefined4 m_unk0x338;      // 0x338
	undefined4 m_unk0x33c;      // 0x33c
	undefined4 m_unk0x340;      // 0x340
	undefined4 m_unk0x344;      // 0x344
	undefined4 m_unk0x348;      // 0x348
};

#endif // LEGOCARBUILD_H
