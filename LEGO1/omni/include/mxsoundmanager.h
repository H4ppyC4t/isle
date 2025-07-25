#ifndef MXSOUNDMANAGER_H
#define MXSOUNDMANAGER_H

#include "decomp.h"
#include "mxatom.h"
#include "mxaudiomanager.h"

#include <dsound.h>

// VTABLE: LEGO1 0x100dc128
// VTABLE: BETA10 0x101c1ce8
// SIZE 0x3c
class MxSoundManager : public MxAudioManager {
public:
	MxSoundManager();
	~MxSoundManager() override; // vtable+0x00

	void Destroy() override;                                             // vtable+0x18
	void SetVolume(MxS32 p_volume) override;                             // vtable+0x2c
	virtual MxResult Create(MxU32 p_frequencyMS, MxBool p_createThread); // vtable+0x30
	virtual void Pause();                                                // vtable+0x34
	virtual void Resume();                                               // vtable+0x38

	LPDIRECTSOUND GetDirectSound() { return m_directSound; }

	MxS32 GetAttenuation(MxU32 p_volume);

	MxPresenter* FindPresenter(const MxAtomId& p_atomId, MxU32 p_objectId);

	// SYNTHETIC: LEGO1 0x100ae7b0
	// SYNTHETIC: BETA10 0x10133460
	// MxSoundManager::`scalar deleting destructor'

protected:
	void Init();
	void Destroy(MxBool p_fromDestructor);

	LPDIRECTSOUND m_directSound;    // 0x30
	LPDIRECTSOUNDBUFFER m_dsBuffer; // 0x34
	undefined m_unk0x38[4];
};

#endif // MXSOUNDMANAGER_H
