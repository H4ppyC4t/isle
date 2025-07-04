#ifndef LEGOLOD_H
#define LEGOLOD_H

#include "misc/legotypes.h"
#include "viewmanager/viewlod.h"

class LegoTextureContainer;
class LegoTextureInfo;
class LegoStorage;

// VTABLE: LEGO1 0x100dbf10
// SIZE 0x20
class LegoLOD : public ViewLOD {
public:
	// SIZE 0x08
	struct Mesh {
		Tgl::Mesh* m_tglMesh; // 0x00
		BOOL m_textured;      // 0x04
	};

	LegoLOD(Tgl::Renderer*);
	~LegoLOD() override;

	// FUNCTION: LEGO1 0x100aae70
	int NumPolys() const override { return m_numPolys; } // vtable+0x0c

	// FUNCTION: LEGO1 0x100aae80
	float VTable0x10() override { return 0.0; } // vtable+0x10

	LegoResult Read(Tgl::Renderer* p_renderer, LegoTextureContainer* p_textureContainer, LegoStorage* p_storage);
	LegoLOD* Clone(Tgl::Renderer* p_renderer);
	LegoResult SetColor(LegoFloat p_red, LegoFloat p_green, LegoFloat p_blue, LegoFloat p_alpha);
	LegoResult SetTextureInfo(LegoTextureInfo* p_textureInfo);
	LegoResult UpdateTextureInfo(LegoTextureInfo* p_textureInfo);
	void ClearMeshOffset();
	LegoResult GetTextureInfo(LegoTextureInfo*& p_textureInfo);

	static LegoBool HasInhPrefix(const LegoChar* p_name);

	// SYNTHETIC: LEGO1 0x100aa430
	// LegoLOD::`scalar deleting destructor'

protected:
	Mesh* m_melems;        // 0x0c
	LegoU32 m_numMeshes;   // 0x10
	LegoU32 m_numVertices; // 0x14
	LegoU32 m_numPolys;    // 0x18
	LegoU32 m_meshOffset;  // 0x1c
};

#endif // LEGOLOD_H
