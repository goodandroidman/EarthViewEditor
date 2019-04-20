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
#include "OgreGL3PlusDepthBuffer.h"
#include "OgreGL3PlusHardwarePixelBuffer.h"
#include "OgreGL3PlusRenderSystem.h"
#include "OgreGL3PlusFrameBufferObject.h"

namespace Ogre
{
	GL3PlusDepthBuffer::GL3PlusDepthBuffer( uint16 poolId, GL3PlusRenderSystem *renderSystem, GL3PlusContext *creatorContext,
									GL3PlusRenderBuffer *depth, GL3PlusRenderBuffer *stencil,
									uint32 width, uint32 height, uint32 fsaa, uint32 multiSampleQuality,
									bool isManual ) :
				DepthBuffer( poolId, 0, width, height, fsaa, "", isManual ),
				mMultiSampleQuality( multiSampleQuality ),
				mCreatorContext( creatorContext ),
				mDepthBuffer( depth ),
				mStencilBuffer( stencil ),
				mRenderSystem( renderSystem )
	{
		if( mDepthBuffer )
		{
			switch( mDepthBuffer->getGLFormat() )
			{
			case GL_DEPTH_COMPONENT16:
				mBitDepth = 16;
				break;
			case GL_DEPTH_COMPONENT24:
            case GL_DEPTH24_STENCIL8:  // Packed depth / stencil
                mBitDepth = 24;
            case GL_DEPTH_COMPONENT32:
            case GL_DEPTH32F_STENCIL8:
				mBitDepth = 32;
				break;
			}
		}
	}

	GL3PlusDepthBuffer::~GL3PlusDepthBuffer()
	{
		if( mStencilBuffer && mStencilBuffer != mDepthBuffer )
		{
			delete mStencilBuffer;
			mStencilBuffer = 0;
		}

		if( mDepthBuffer )
		{
			delete mDepthBuffer;
			mDepthBuffer = 0;
		}
	}
	//---------------------------------------------------------------------
	bool GL3PlusDepthBuffer::isCompatible( RenderTarget *renderTarget ) const
	{
		bool retVal = false;

		//Check standard stuff first.
		if( mRenderSystem->getCapabilities()->hasCapability( RSC_RTT_DEPTHBUFFER_RESOLUTION_LESSEQUAL ) )
		{
			if( !DepthBuffer::isCompatible( renderTarget ) )
				return false;
		}
		else
		{
			if( this->getWidth() != renderTarget->getWidth() ||
				this->getHeight() != renderTarget->getHeight() ||
				this->getFsaa() != renderTarget->getFSAA() )
					return false;
		}

		//Now check this is the appropriate format
		GL3PlusFrameBufferObject *fbo = 0;
        renderTarget->getCustomAttribute(GL3PlusRenderTexture::CustomAttributeString_FBO, &fbo);

		if( !fbo )
		{
			GL3PlusContext *windowContext;
			renderTarget->getCustomAttribute( GL3PlusRenderTexture::CustomAttributeString_GLCONTEXT, &windowContext );

			//Non-FBO targets and FBO depth surfaces don't play along, only dummies which match the same
			//context
			if( !mDepthBuffer && !mStencilBuffer && mCreatorContext == windowContext )
				retVal = true;
		}
		else
		{
			//Check this isn't a dummy non-FBO depth buffer with an FBO target, don't mix them.
			//If you don't want depth buffer, use a Null Depth Buffer, not a dummy one.
			if( mDepthBuffer || mStencilBuffer )
			{
				GLenum internalFormat = fbo->getFormat();
				GLenum depthFormat, stencilFormat;
				mRenderSystem->_getDepthStencilFormatFor( internalFormat, &depthFormat, &stencilFormat );

				bool bSameDepth = false;

				if( mDepthBuffer )
					bSameDepth |= mDepthBuffer->getGLFormat() == depthFormat;

				bool bSameStencil = false;

                if( !mStencilBuffer || mStencilBuffer == mDepthBuffer )
                   bSameStencil = stencilFormat == GL_NONE;
                else
                {
                    if( mStencilBuffer )
                        bSameStencil = stencilFormat == mStencilBuffer->getGLFormat();
                }

                if(internalFormat == PF_DEPTH)
                    retVal = bSameDepth;
                else
                    retVal = bSameDepth && bSameStencil;
			}
		}

		return retVal;
	}
}
