/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreGLES2HardwareBufferManager.h"
#include "OgreGLES2HardwareVertexBuffer.h"
#include "OgreGLES2HardwareIndexBuffer.h"
#include "OgreGLES2HardwareUniformBuffer.h"
#include "OgreGLES2VertexDeclaration.h"
#include "OgreGLES2RenderToVertexBuffer.h"
#include "OgreGLES2RenderSystem.h"
#include "OgreGLES2Support.h"
#include "OgreRoot.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    // Scratch pool management (32 bit structure)
    struct GLES2ScratchBufferAlloc
    {
        /// Size in bytes
        uint32 size: 31;
        /// Free? (pack with size)
        uint32 free: 1;
    };
    #define SCRATCH_POOL_SIZE 1 * 1024 * 1024
    #define SCRATCH_ALIGNMENT 32

    GLES2HardwareBufferManagerBase::GLES2HardwareBufferManagerBase()
    {
        // Init scratch pool
        // TODO make it a configurable size?
        // 32-bit aligned buffer
        mScratchBufferPool = static_cast<char*>(OGRE_MALLOC_ALIGN(SCRATCH_POOL_SIZE,
                                                                  MEMCATEGORY_GEOMETRY,
                                                                  SCRATCH_ALIGNMENT));
        GLES2ScratchBufferAlloc* ptrAlloc = (GLES2ScratchBufferAlloc*)mScratchBufferPool;
        ptrAlloc->size = SCRATCH_POOL_SIZE - sizeof(GLES2ScratchBufferAlloc);
        ptrAlloc->free = 1;

        // non-Win32 machines are having issues glBufferSubData, looks like buffer corruption
		// disable for now until we figure out where the problem lies			
#	if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
		mMapBufferThreshold = 0;
#	endif
		mStateCacheManager = dynamic_cast<GLES2RenderSystem*>(Root::getSingleton().getRenderSystem())->getGLES2Support()->getStateCacheManager();
}

    GLES2HardwareBufferManagerBase::~GLES2HardwareBufferManagerBase()
    {
        destroyAllDeclarations();
        destroyAllBindings();

        OGRE_FREE_ALIGN(mScratchBufferPool, MEMCATEGORY_GEOMETRY, SCRATCH_ALIGNMENT);
    }

    HardwareVertexBufferSharedPtr
        GLES2HardwareBufferManagerBase::createVertexBuffer(size_t vertexSize,
                                                      size_t numVerts,
                                                      HardwareBuffer::Usage usage,
                                                      bool useShadowBuffer)
    {
        // always use shadowBuffer
        GLES2HardwareVertexBuffer* buf =
            OGRE_NEW GLES2HardwareVertexBuffer(this, vertexSize, numVerts, usage, true);
        {
            OGRE_LOCK_MUTEX(mVertexBuffersMutex)
            mVertexBuffers.insert(buf);
        }
        return HardwareVertexBufferSharedPtr(buf);
    }

    HardwareIndexBufferSharedPtr GLES2HardwareBufferManagerBase::createIndexBuffer(HardwareIndexBuffer::IndexType itype,
                                                                              size_t numIndexes,
                                                                              HardwareBuffer::Usage usage,
                                                                              bool useShadowBuffer)
    {
        // always use shadowBuffer
        GLES2HardwareIndexBuffer* buf =
            OGRE_NEW GLES2HardwareIndexBuffer(this, itype, numIndexes, usage, true);
        {
            OGRE_LOCK_MUTEX(mIndexBuffersMutex)
            mIndexBuffers.insert(buf);
        }
        return HardwareIndexBufferSharedPtr(buf);
    }

	RenderToVertexBufferSharedPtr GLES2HardwareBufferManagerBase::createRenderToVertexBuffer()
	{
#if OGRE_NO_GLES3_SUPPORT == 0
		return RenderToVertexBufferSharedPtr(new GLES2RenderToVertexBuffer);
#else
		// not supported
		return RenderToVertexBufferSharedPtr();
#endif
	}

	VertexDeclaration* GLES2HardwareBufferManagerBase::createVertexDeclarationImpl(void)
	{
		return OGRE_NEW GLES2VertexDeclaration();
	}

    void GLES2HardwareBufferManagerBase::destroyVertexDeclarationImpl(VertexDeclaration* decl)
	{
        if(decl)
            OGRE_DELETE decl;
	}

    GLenum GLES2HardwareBufferManagerBase::getGLUsage(unsigned int usage)
    {
        switch(usage)
        {
            case HardwareBuffer::HBU_STATIC:
            case HardwareBuffer::HBU_STATIC_WRITE_ONLY:
                return GL_STATIC_DRAW;
            case HardwareBuffer::HBU_DYNAMIC:
            case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY:
                return GL_DYNAMIC_DRAW;
            case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
                return GL_STREAM_DRAW;
            default:
                return GL_DYNAMIC_DRAW;
        };
    }

    GLenum GLES2HardwareBufferManagerBase::getGLType(unsigned int type)
    {
        switch(type)
        {
            case VET_FLOAT1:
            case VET_FLOAT2:
            case VET_FLOAT3:
            case VET_FLOAT4:
                return GL_FLOAT;
            case VET_SHORT1:
            case VET_SHORT2:
            case VET_SHORT3:
            case VET_SHORT4:
                return GL_SHORT;
            case VET_COLOUR:
            case VET_COLOUR_ABGR:
            case VET_COLOUR_ARGB:
            case VET_UBYTE4:
                return GL_UNSIGNED_BYTE;
#if OGRE_NO_GLES3_SUPPORT == 0
            case VET_INT1:
            case VET_INT2:
            case VET_INT3:
            case VET_INT4:
                return GL_INT;
            case VET_UINT1:
            case VET_UINT2:
            case VET_UINT3:
            case VET_UINT4:
                return GL_UNSIGNED_INT;
            case VET_USHORT1:
            case VET_USHORT2:
            case VET_USHORT3:
            case VET_USHORT4:
                return GL_UNSIGNED_SHORT;
#endif
            default:
                return 0;
        };
    }

    void* GLES2HardwareBufferManagerBase::allocateScratch(uint32 size)
    {
        // simple forward link search based on alloc sizes
        // not that fast but the list should never get that long since not many
        // locks at once (hopefully)
        OGRE_LOCK_MUTEX(mScratchMutex)

        // Alignment - round up the size to 32 bits
        // control blocks are 32 bits too so this packs nicely
        if (size % 4 != 0)
        {
            size += 4 - (size % 4);
        }

        uint32 bufferPos = 0;
        while (bufferPos < SCRATCH_POOL_SIZE)
        {
            GLES2ScratchBufferAlloc* pNext = (GLES2ScratchBufferAlloc*)(mScratchBufferPool + bufferPos);
            // Big enough?
            if (pNext->free && pNext->size >= size)
            {
                // split? And enough space for control block
                if(pNext->size > size + sizeof(GLES2ScratchBufferAlloc))
                {
                    uint32 offset = sizeof(GLES2ScratchBufferAlloc) + size;

                    GLES2ScratchBufferAlloc* pSplitAlloc = (GLES2ScratchBufferAlloc*)
                        (mScratchBufferPool + bufferPos + offset);
                    pSplitAlloc->free = 1;
                    // split size is remainder minus new control block
                    pSplitAlloc->size = pNext->size - size - sizeof(GLES2ScratchBufferAlloc);

                    // New size of current
                    pNext->size = size;
                }
                // allocate and return
                pNext->free = 0;

                // return pointer just after this control block (++ will do that for us)
                return ++pNext;
            }

            bufferPos += sizeof(GLES2ScratchBufferAlloc) + pNext->size;
        }

        // no available alloc
        return 0;
    }

    void GLES2HardwareBufferManagerBase::deallocateScratch(void* ptr)
    {
        OGRE_LOCK_MUTEX(mScratchMutex)

        // Simple linear search dealloc
        uint32 bufferPos = 0;
        GLES2ScratchBufferAlloc* pLast = 0;
        while (bufferPos < SCRATCH_POOL_SIZE)
        {
            GLES2ScratchBufferAlloc* pCurrent = (GLES2ScratchBufferAlloc*)(mScratchBufferPool + bufferPos);

            // Pointers match?
            if ((mScratchBufferPool + bufferPos + sizeof(GLES2ScratchBufferAlloc)) == ptr)
            {
                // dealloc
                pCurrent->free = 1;
                // merge with previous
                if (pLast && pLast->free)
                {
                    // adjust buffer pos
                    bufferPos -= (pLast->size + sizeof(GLES2ScratchBufferAlloc));
                    // merge free space
                    pLast->size += pCurrent->size + sizeof(GLES2ScratchBufferAlloc);
                    pCurrent = pLast;
                }

                // merge with next
                uint32 offset = bufferPos + pCurrent->size + sizeof(GLES2ScratchBufferAlloc);
                if (offset < SCRATCH_POOL_SIZE)
                {
                    GLES2ScratchBufferAlloc* pNext = (GLES2ScratchBufferAlloc*)(
                        mScratchBufferPool + offset);
                    if (pNext->free)
                    {
                        pCurrent->size += pNext->size + sizeof(GLES2ScratchBufferAlloc);
                    }
                }

                // done
                return;
            }

            bufferPos += sizeof(GLES2ScratchBufferAlloc) + pCurrent->size;
            pLast = pCurrent;

        }

        // Should never get here unless there's a corruption
        assert(false && "Memory deallocation error");
    }
	//---------------------------------------------------------------------
	size_t GLES2HardwareBufferManagerBase::getGLMapBufferThreshold() const
	{
		return mMapBufferThreshold;
	}
	//---------------------------------------------------------------------
	void GLES2HardwareBufferManagerBase::setGLMapBufferThreshold( const size_t value )
	{
		mMapBufferThreshold = value;
	}
    //---------------------------------------------------------------------
    Ogre::HardwareUniformBufferSharedPtr GLES2HardwareBufferManagerBase::createUniformBuffer( size_t sizeBytes, HardwareBuffer::Usage usage, bool useShadowBuffer, const String& name )
    {
#if OGRE_NO_GLES3_SUPPORT == 0
        GLES2HardwareUniformBuffer* buf =
        new GLES2HardwareUniformBuffer(this, sizeBytes, usage, useShadowBuffer, name);
        {
            OGRE_LOCK_MUTEX(mUniformBuffersMutex)
            mUniformBuffers.insert(buf);
        }
        return HardwareUniformBufferSharedPtr(buf);
#else
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
            "GLES2 does not support uniform buffer objects", 
            "GLES2HardwareBufferManagerBase::createUniformBuffer");
#endif
    }
    //---------------------------------------------------------------------
    Ogre::HardwareCounterBufferSharedPtr GLES2HardwareBufferManagerBase::createCounterBuffer( size_t sizeBytes, HardwareBuffer::Usage usage, bool useShadowBuffer, const String& name )
    {
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
                    "GLES2 does not support atomic counter buffers",
                    "GLES2HardwareBufferManagerBase::createCounterBuffer");
    }

}
