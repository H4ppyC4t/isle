#if !defined(AFX_MAINDLG_H)
#define AFX_MAINDLG_H

#include "StdAfx.h"
#include "compat.h"
#include "decomp.h"
#include "res/resource.h"

// VTABLE: CONFIG 0x004063e0
// VTABLE: CONFIGD 0x0040c500
// SIZE 0x70
class CMainDialog : public CDialog {
public:
	CMainDialog(CWnd* pParent);
	enum {
		IDD = IDD_MAIN
	};

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	void UpdateInterface();
	void SwitchToAdvanced(BOOL p_advanced);

	undefined m_unk0x60[4]; // 0x60
	HCURSOR m_icon;         // 0x64
	BOOL m_modified;        // 0x68
	BOOL m_advanced;        // 0x6c
							// Implementation

protected:
	BOOL OnInitDialog() override;
	void OnSysCommand(UINT nID, LPARAM lParam);
	void OnPaint();
	HCURSOR OnQueryDragIcon();
	void OnList3DevicesSelectionChanged();
	void OnCancel();
	void OnDestroy();
	void OnButtonCancel();
	void OnCheckboxWideAngle();
	void OnCheckbox3DSound();
	void OnCheckbox3DVideoMemory();
	void OnRadiobuttonPalette16bit();
	void OnRadiobuttonPalette256();
	void OnCheckboxFlipVideoMemPages();
	void OnCheckboxFullScreen();
	void OnRadiobuttonModelLowestQuality();
	void OnRadiobuttonModelLowQuality();
	void OnRadiobuttonModelHighQuality();
	void OnRadiobuttonTextureLowQuality();
	void OnRadiobuttonTextureHighQuality();
	void OnCheckboxJoystick();
	void OnButtonAdvanced();
	void OnCheckboxDrawCursor();
	void OnCheckboxMusic();

	DECLARE_MESSAGE_MAP()
};

// SYNTHETIC: CONFIG 0x00403160
// SYNTHETIC: CONFIGD 0x00408490
// CMainDialog::~CMainDialog

// SYNTHETIC: CONFIG 0x00403de0
// SYNTHETIC: CONFIGD 0x00409910
// CMainDialog::`scalar deleting destructor'

// FUNCTION: CONFIG 0x00403e60
// FUNCTION: CONFIGD 0x004087a9
// CMainDialog::_GetBaseMessageMap

// FUNCTION: CONFIG 0x00403e70
// FUNCTION: CONFIGD 0x004087be
// CMainDialog::GetMessageMap

// GLOBAL: CONFIG 0x00406120
// GLOBAL: CONFIGD 0x0040c1a8
// CMainDialog::messageMap

// GLOBAL: CONFIG 0x00406128
// GLOBAL: CONFIGD 0x0040c1b0
// CMainDialog::_messageEntries

#endif // !defined(AFX_MAINDLG_H)
