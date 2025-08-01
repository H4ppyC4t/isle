#include "mxvideopresenter.h"

#include "mxautolock.h"
#include "mxdisplaysurface.h"
#include "mxdsmediaaction.h"
#include "mxdssubscriber.h"
#include "mxmisc.h"
#include "mxregion.h"
#include "mxvideomanager.h"

DECOMP_SIZE_ASSERT(MxVideoPresenter, 0x64);
DECOMP_SIZE_ASSERT(MxVideoPresenter::AlphaMask, 0x0c);

// FUNCTION: LEGO1 0x100b24f0
MxVideoPresenter::AlphaMask::AlphaMask(const MxBitmap& p_bitmap)
{
	m_width = p_bitmap.GetBmiWidth();
	m_height = p_bitmap.GetBmiHeightAbs();

	MxS32 size = ((m_width * m_height) / 8) + 1;
	m_bitmask = new MxU8[size];
	memset(m_bitmask, 0, size);

	// The goal here is to enable us to walk through the bitmap's rows
	// in order, regardless of the orientation. We want to end up at the
	// start of the first row, which is either at position 0, or at
	// (image_stride * biHeight) - 1.

	// Reminder: Negative biHeight means this is a top-down DIB.
	// Otherwise it is bottom-up.

	MxU8* bitmapSrcPtr = p_bitmap.GetStart(0, 0);

	// How many bytes are there for each row of the bitmap?
	// (i.e. the image stride)
	// If this is a bottom-up DIB, we will walk it in reverse.
	MxS32 rowSeek = p_bitmap.AlignToFourByte(m_width);
	if (p_bitmap.GetBmiHeader()->biCompression != BI_RGB_TOPDOWN && p_bitmap.GetBmiHeight() >= 0) {
		rowSeek = -rowSeek;
	}

	// The actual offset into the m_bitmask array. The two for-loops
	// are just for counting the pixels.
	MxS32 offset = 0;

	for (MxS32 j = 0; j < m_height; j++) {
		MxU8* tPtr = bitmapSrcPtr;
		for (MxS32 i = 0; i < m_width; i++) {
			if (*tPtr) {
				m_bitmask[offset / 8] |= (1 << (offset % 8));
			}
			tPtr++;
			offset++;
		}
		// Seek to the start of the next row
		bitmapSrcPtr += rowSeek;
		tPtr = bitmapSrcPtr;
	}
}

// FUNCTION: LEGO1 0x100b2670
MxVideoPresenter::AlphaMask::AlphaMask(const MxVideoPresenter::AlphaMask& p_alpha)
{
	m_width = p_alpha.m_width;
	m_height = p_alpha.m_height;

	MxS32 size = ((m_width * m_height) / 8) + 1;
	m_bitmask = new MxU8[size];
	memcpy(m_bitmask, p_alpha.m_bitmask, size);
}

// FUNCTION: LEGO1 0x100b26d0
MxVideoPresenter::AlphaMask::~AlphaMask()
{
	if (m_bitmask) {
		delete[] m_bitmask;
	}
}

// FUNCTION: LEGO1 0x100b26f0
MxS32 MxVideoPresenter::AlphaMask::IsHit(MxU32 p_x, MxU32 p_y)
{
	if (p_x >= m_width || p_y >= m_height) {
		return 0;
	}

	MxS32 pos = p_y * m_width + p_x;
	return m_bitmask[pos / 8] & (1 << (pos % 8)) ? 1 : 0;
}

// FUNCTION: LEGO1 0x100b2760
void MxVideoPresenter::Init()
{
	m_frameBitmap = NULL;
	m_alpha = NULL;
	m_frameLoadTickleCount = 1;
	m_surface = NULL;
	m_frozenTime = -1;
	SetLoadedFirstFrame(FALSE);

	if (MVideoManager() != NULL) {
		MVideoManager();
		SetUseSurface(TRUE);
		SetUseVideoMemory(FALSE);
	}

	SetDoNotWriteToSurface(FALSE);
	SetBitmapIsMap(FALSE);
}

// FUNCTION: LEGO1 0x100b27b0
void MxVideoPresenter::Destroy(MxBool p_fromDestructor)
{
	if (MVideoManager() != NULL) {
		MVideoManager()->UnregisterPresenter(*this);
	}

	if (m_surface) {
		m_surface->Release();
		m_surface = NULL;
		SetUseSurface(FALSE);
		SetUseVideoMemory(FALSE);
	}

	if (MVideoManager() && (m_alpha || m_frameBitmap)) {
		// MxRect32 rect(m_location, MxSize32(GetWidth(), GetHeight()));
		MxS32 height = GetHeight();
		MxS32 width = GetWidth();
		MxS32 x = m_location.GetX();
		MxS32 y = m_location.GetY();

		MxRect32 rect(x, y, x + width, y + height);
		MVideoManager()->InvalidateRect(rect);
		MVideoManager()->UpdateView(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
	}

	delete m_frameBitmap;
	delete m_alpha;

	Init();

	if (!p_fromDestructor) {
		MxMediaPresenter::Destroy(FALSE);
	}
}

// FUNCTION: LEGO1 0x100b28b0
// FUNCTION: BETA10 0x101389c1
void MxVideoPresenter::NextFrame()
{
	MxStreamChunk* chunk = NextChunk();

	if (chunk->GetChunkFlags() & DS_CHUNK_END_OF_STREAM) {
		m_subscriber->FreeDataChunk(chunk);
		ProgressTickleState(e_repeating);
	}
	else {
		LoadFrame(chunk);
		m_subscriber->FreeDataChunk(chunk);
	}
}

// FUNCTION: LEGO1 0x100b2900
// FUNCTION: BETA10 0x10138a3a
MxBool MxVideoPresenter::IsHit(MxS32 p_x, MxS32 p_y)
{
	MxDSAction* action = GetAction();
	if ((action == NULL) || (((action->GetFlags() & MxDSAction::c_bit11) == 0) && !IsEnabled()) ||
		(!m_frameBitmap && !m_alpha)) {
		return FALSE;
	}

	if (!m_frameBitmap) {
		return m_alpha->IsHit(p_x - m_location.GetX(), p_y - m_location.GetY());
	}

	MxRect32 rect(0, 0, m_frameBitmap->GetBmiWidth(), m_frameBitmap->GetBmiHeightAbs());
	rect += GetLocation();

	if (p_x < rect.GetLeft() || p_x >= rect.GetRight() || p_y < rect.GetTop() || p_y >= rect.GetBottom()) {
		return FALSE;
	}

	MxU8* pixel = m_frameBitmap->GetStart(p_x - rect.GetLeft(), p_y - rect.GetTop());

	if (BitmapIsMap()) {
		return (MxBool) *pixel;
	}

	if ((GetAction()->GetFlags() & MxDSAction::c_bit4) && *pixel == 0) {
		return FALSE;
	}

	return TRUE;
}

inline MxS32 MxVideoPresenter::PrepareRects(RECT& p_rectDest, RECT& p_rectSrc)
{
	if (p_rectDest.top > 480 || p_rectDest.left > 640 || p_rectSrc.top > 480 || p_rectSrc.left > 640) {
		return -1;
	}

	if (p_rectDest.bottom > 480) {
		p_rectDest.bottom = 480;
	}

	if (p_rectDest.right > 640) {
		p_rectDest.right = 640;
	}

	if (p_rectSrc.bottom > 480) {
		p_rectSrc.bottom = 480;
	}

	if (p_rectSrc.right > 640) {
		p_rectSrc.right = 640;
	}

	LONG height, width;
	if ((height = (p_rectDest.bottom - p_rectDest.top) + 1) <= 1 ||
		(width = (p_rectDest.right - p_rectDest.left) + 1) <= 1) {
		return -1;
	}
	else if ((p_rectSrc.right - p_rectSrc.left + 1) == width && (p_rectSrc.bottom - p_rectSrc.top + 1) == height) {
		return 1;
	}
	else {
		p_rectSrc.right = (p_rectSrc.left + width) - 1;
		p_rectSrc.bottom = (p_rectSrc.top + height) - 1;
		return 0;
	}
}

// FUNCTION: LEGO1 0x100b2a70
void MxVideoPresenter::PutFrame()
{
	MxDisplaySurface* displaySurface = MVideoManager()->GetDisplaySurface();
	MxRegion* region = MVideoManager()->GetRegion();
	MxRect32 rect(MxPoint32(0, 0), MxSize32(GetWidth(), GetHeight()));
	rect += GetLocation();
	LPDIRECTDRAWSURFACE ddSurface = displaySurface->GetDirectDrawSurface2();

	if (m_action->GetFlags() & MxDSAction::c_bit5) {
		if (m_surface) {
			RECT src, dest;
			src.top = 0;
			src.left = 0;
			src.right = GetWidth();
			src.bottom = GetHeight();

			dest.left = GetX();
			dest.top = GetY();
			dest.right = dest.left + GetWidth();
			dest.bottom = dest.top + GetHeight();

			switch (PrepareRects(src, dest)) {
			case 0:
				ddSurface->Blt(&dest, m_surface, &src, DDBLT_KEYSRC, NULL);
				break;
			case 1:
				ddSurface->BltFast(dest.left, dest.top, m_surface, &src, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
			}
		}
		else {
			displaySurface->VTable0x30(
				m_frameBitmap,
				0,
				0,
				rect.GetLeft(),
				rect.GetTop(),
				m_frameBitmap->GetBmiWidth(),
				m_frameBitmap->GetBmiHeightAbs(),
				TRUE
			);
		}
	}
	else {
		MxRegionCursor cursor(region);
		MxRect32* regionRect;

		while ((regionRect = cursor.Next(rect))) {
			if (regionRect->GetWidth() >= 1 && regionRect->GetHeight() >= 1) {
				RECT src, dest;

				if (m_surface) {
					src.left = regionRect->GetLeft() - GetX();
					src.top = regionRect->GetTop() - GetY();
					src.right = src.left + regionRect->GetWidth();
					src.bottom = src.top + regionRect->GetHeight();

					dest.left = regionRect->GetLeft();
					dest.top = regionRect->GetTop();
					dest.right = dest.left + regionRect->GetWidth();
					dest.bottom = dest.top + regionRect->GetHeight();
				}

				if (m_action->GetFlags() & MxDSAction::c_bit4) {
					if (m_surface) {
						if (PrepareRects(src, dest) >= 0) {
							ddSurface->Blt(&dest, m_surface, &src, DDBLT_KEYSRC, NULL);
						}
					}
					else {
						displaySurface->VTable0x30(
							m_frameBitmap,
							regionRect->GetLeft() - GetX(),
							regionRect->GetTop() - GetY(),
							regionRect->GetLeft(),
							regionRect->GetTop(),
							regionRect->GetWidth(),
							regionRect->GetHeight(),
							FALSE
						);
					}
				}
				else if (m_surface) {
					if (PrepareRects(src, dest) >= 0) {
						ddSurface->Blt(&dest, m_surface, &src, 0, NULL);
					}
				}
				else {
					displaySurface->VTable0x28(
						m_frameBitmap,
						regionRect->GetLeft() - GetX(),
						regionRect->GetTop() - GetY(),
						regionRect->GetLeft(),
						regionRect->GetTop(),
						regionRect->GetWidth(),
						regionRect->GetHeight()
					);
				}
			}
		}
	}
}

// FUNCTION: LEGO1 0x100b2f60
void MxVideoPresenter::ReadyTickle()
{
	MxStreamChunk* chunk = NextChunk();

	if (chunk) {
		LoadHeader(chunk);
		m_subscriber->FreeDataChunk(chunk);
		ParseExtra();
		ProgressTickleState(e_starting);
	}
}

// FUNCTION: LEGO1 0x100b2fa0
void MxVideoPresenter::StartingTickle()
{
	MxStreamChunk* chunk = CurrentChunk();

	if (chunk && m_action->GetElapsedTime() >= chunk->GetTime()) {
		CreateBitmap();
		ProgressTickleState(e_streaming);
	}
}

// FUNCTION: LEGO1 0x100b2fe0
void MxVideoPresenter::StreamingTickle()
{
	if (m_action->GetFlags() & MxDSAction::c_bit10) {
		if (!m_currentChunk) {
			MxMediaPresenter::StreamingTickle();
		}

		if (m_currentChunk) {
			LoadFrame(m_currentChunk);
			m_currentChunk = NULL;
		}
	}
	else {
		for (MxS16 i = 0; i < m_frameLoadTickleCount; i++) {
			if (!m_currentChunk) {
				MxMediaPresenter::StreamingTickle();

				if (!m_currentChunk) {
					break;
				}
			}

			if (m_action->GetElapsedTime() < m_currentChunk->GetTime()) {
				break;
			}

			LoadFrame(m_currentChunk);
			m_subscriber->FreeDataChunk(m_currentChunk);
			m_currentChunk = NULL;
			SetLoadedFirstFrame(TRUE);

			if (m_currentTickleState != e_streaming) {
				break;
			}
		}

		if (LoadedFirstFrame()) {
			m_frameLoadTickleCount = 5;
		}
	}
}

// FUNCTION: LEGO1 0x100b3080
void MxVideoPresenter::RepeatingTickle()
{
	if (IsEnabled()) {
		if (m_action->GetFlags() & MxDSAction::c_bit10) {
			if (!m_currentChunk) {
				MxMediaPresenter::RepeatingTickle();
			}

			if (m_currentChunk) {
				LoadFrame(m_currentChunk);
				m_currentChunk = NULL;
			}
		}
		else {
			for (MxS16 i = 0; i < m_frameLoadTickleCount; i++) {
				if (!m_currentChunk) {
					MxMediaPresenter::RepeatingTickle();

					if (!m_currentChunk) {
						break;
					}
				}

				if (m_action->GetElapsedTime() % m_action->GetLoopCount() < m_currentChunk->GetTime()) {
					break;
				}

				LoadFrame(m_currentChunk);
				m_currentChunk = NULL;
				SetLoadedFirstFrame(TRUE);

				if (m_currentTickleState != e_repeating) {
					break;
				}
			}

			if (LoadedFirstFrame()) {
				m_frameLoadTickleCount = 5;
			}
		}
	}
}

// FUNCTION: LEGO1 0x100b3130
void MxVideoPresenter::FreezingTickle()
{
	MxLong sustainTime = ((MxDSMediaAction*) m_action)->GetSustainTime();

	if (sustainTime != -1) {
		if (sustainTime) {
			if (m_frozenTime == -1) {
				m_frozenTime = m_action->GetElapsedTime();
			}

			if (m_action->GetElapsedTime() >= m_frozenTime + ((MxDSMediaAction*) m_action)->GetSustainTime()) {
				ProgressTickleState(e_done);
			}
		}
		else {
			ProgressTickleState(e_done);
		}
	}
}

// FUNCTION: LEGO1 0x100b31a0
MxResult MxVideoPresenter::AddToManager()
{
	MxResult result = FAILURE;

	if (MVideoManager()) {
		result = SUCCESS;
		MVideoManager()->RegisterPresenter(*this);
	}

	return result;
}

// FUNCTION: LEGO1 0x100b31d0
// FUNCTION: BETA10 0x101396d9
void MxVideoPresenter::EndAction()
{
	if (m_action) {
		MxMediaPresenter::EndAction();
		AUTOLOCK(m_criticalSection);

		if (m_frameBitmap) {
			MxLong height = m_frameBitmap->GetBmiHeightAbs();
			MxLong width = m_frameBitmap->GetBmiWidth();
			MxS32 x = m_location.GetX();
			MxS32 y = m_location.GetY();

			MxRect32 rect(x, y, x + width, y + height);

			MVideoManager()->InvalidateRect(rect);
		}
	}
}

// FUNCTION: LEGO1 0x100b3280
// FUNCTION: BETA10 0x101397c0
MxResult MxVideoPresenter::PutData()
{
	AUTOLOCK(m_criticalSection);

	if (IsEnabled() && m_currentTickleState >= e_streaming && m_currentTickleState <= e_freezing) {
		PutFrame();
	}

	return SUCCESS;
}

// FUNCTION: LEGO1 0x100b3300
undefined MxVideoPresenter::VTable0x74()
{
	return 0;
}
