#include "mxstreamchunk.h"

#include "mxdsbuffer.h"
#include "mxdssubscriber.h"
#include "mxutilities.h"

// FUNCTION: LEGO1 0x100c2fe0
MxStreamChunk::~MxStreamChunk()
{
	if (m_buffer) {
		m_buffer->ReleaseRef(this);
	}
}

// FUNCTION: LEGO1 0x100c3050
MxResult MxStreamChunk::ReadChunk(MxDSBuffer* p_buffer, MxU8* p_chunkData)
{
	MxResult result = FAILURE;

	if (p_chunkData != NULL && *(MxU32*) p_chunkData == FOURCC('M', 'x', 'C', 'h')) {
		if (ReadChunkHeader(p_chunkData + 8)) {
			if (p_buffer) {
				SetBuffer(p_buffer);
				p_buffer->AddRef(this);
			}
			result = SUCCESS;
		}
	}

	return result;
}

// FUNCTION: LEGO1 0x100c30a0
MxU32 MxStreamChunk::ReadChunkHeader(MxU8* p_chunkData)
{
	MxU32 headersize = 0;
	if (p_chunkData) {
		MxU8* chunkData = p_chunkData;

		memcpy(&m_flags, p_chunkData, sizeof(m_flags));
		p_chunkData += sizeof(MxU16);

		memcpy(&m_objectId, p_chunkData, sizeof(m_objectId));
		p_chunkData += sizeof(MxU32);

		memcpy(&m_time, p_chunkData, sizeof(m_time));
		p_chunkData += sizeof(MxU32);

		memcpy(&m_length, p_chunkData, sizeof(m_length));
		p_chunkData += sizeof(MxU32);

		m_data = p_chunkData;
		headersize = p_chunkData - chunkData;
	}

	return headersize;
}

// FUNCTION: LEGO1 0x100c30e0
// FUNCTION: BETA10 0x10151517
MxResult MxStreamChunk::SendChunk(MxDSSubscriberList& p_subscriberList, MxBool p_append, MxS16 p_obj24val)
{
	for (MxDSSubscriberList::iterator it = p_subscriberList.begin(); it != p_subscriberList.end(); it++) {
		if ((*it)->GetObjectId() == m_objectId && (*it)->GetUnknown48() == p_obj24val) {
			if (m_flags & DS_CHUNK_END_OF_STREAM && m_buffer) {
				m_buffer->ReleaseRef(this);
				m_buffer = NULL;
			}

			(*it)->AddData(this, p_append);

			return SUCCESS;
		}
	}

	return FAILURE;
}

// FUNCTION: LEGO1 0x100c3170
void MxStreamChunk::SetBuffer(MxDSBuffer* p_buffer)
{
	m_buffer = p_buffer;
}

// FUNCTION: LEGO1 0x100c3180
// FUNCTION: BETA10 0x101515f1
MxU16* MxStreamChunk::IntoFlags(MxU8* p_buffer)
{
	return (MxU16*) (p_buffer + 0x08);
}

// FUNCTION: LEGO1 0x100c3190
MxU32* MxStreamChunk::IntoObjectId(MxU8* p_buffer)
{
	return (MxU32*) (p_buffer + 0x0a);
}

// FUNCTION: LEGO1 0x100c31a0
// FUNCTION: BETA10 0x10151626
MxLong* MxStreamChunk::IntoTime(MxU8* p_buffer)
{
	return (MxLong*) (p_buffer + 0x0e);
}

// FUNCTION: LEGO1 0x100c31b0
MxU32* MxStreamChunk::IntoLength(MxU8* p_buffer)
{
	return (MxU32*) (p_buffer + 0x12);
}
