/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd

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
#include "OgreD3D11HardwarePixelBuffer.h"
#include "OgreD3D11Texture.h"
#include "OgreD3D11Mappings.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreBitwise.h"

#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreD3D11Texture.h"
#include "OgreD3D11Device.h"

namespace Ogre {

	//-----------------------------------------------------------------------------  

	D3D11HardwarePixelBuffer::D3D11HardwarePixelBuffer(D3D11Texture * parentTexture, D3D11Device & device, size_t subresourceIndex,
		size_t width, size_t height, size_t depth, size_t face, PixelFormat format, HardwareBuffer::Usage usage):
	HardwarePixelBuffer(width, height, depth, format, usage, false, false),
		mParentTexture(parentTexture),
		mDevice(device),
		mSubresourceIndex(subresourceIndex),
		mFace(face),
		mDataForStaticUsageLock(0)
	{
		if(mUsage & TU_RENDERTARGET)
		{
			// Create render target for each slice
			mSliceTRT.reserve(mDepth);
			assert(mDepth==1);
			for(size_t zoffset=0; zoffset<mDepth; ++zoffset)
			{
				String name;
				name = "rtt/"+StringConverter::toString((size_t)mParentTexture) + "/" + StringConverter::toString(mSubresourceIndex) + "/" + parentTexture->getName();

				RenderTexture *trt = new D3D11RenderTexture(name, this, mDevice);
				mSliceTRT.push_back(trt);
				Root::getSingleton().getRenderSystem()->attachRenderTarget(*trt);
			}
		}
	}
	D3D11HardwarePixelBuffer::~D3D11HardwarePixelBuffer()
	{
		if(mSliceTRT.empty())
			return;
		// Delete all render targets that are not yet deleted via _clearSliceRTT
		for(size_t zoffset=0; zoffset<mDepth; ++zoffset)
		{
			if(mSliceTRT[zoffset])
				Root::getSingleton().getRenderSystem()->destroyRenderTarget(mSliceTRT[zoffset]->getName());
		}

		if (mDataForStaticUsageLock != NULL)
		{
			SAFE_DELETE_ARRAY(mDataForStaticUsageLock) ;
		}
	}
	//-----------------------------------------------------------------------------  
	// Util functions to convert a D3D locked box to a pixel box
	void D3D11HardwarePixelBuffer::fromD3DLock(PixelBox &rval, const DXGI_MAPPED_RECT &lrect)
	{
		rval.rowPitch = lrect.Pitch / PixelUtil::getNumElemBytes(rval.format);
		rval.slicePitch = rval.rowPitch * rval.getHeight();
		assert((lrect.Pitch % PixelUtil::getNumElemBytes(rval.format))==0);
		rval.data = lrect.pBits;
	}
	//-----------------------------------------------------------------------------  
	PixelBox D3D11HardwarePixelBuffer::lockImpl(const Image::Box lockBox,  LockOptions options)
	{
		// Check for misuse
		if(mUsage & TU_RENDERTARGET)
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "DirectX does not allow locking of or directly writing to RenderTargets. Use blitFromMemory if you need the contents.",
			"D3D11HardwarePixelBuffer::lockImpl");	

		mLockBox = lockBox;

		// Set extents and format
		// Note that we do not carry over the left/top/front here, since the returned
		// PixelBox will be re-based from the locking point onwards
		PixelBox rval(lockBox.getWidth(), lockBox.getHeight(), lockBox.getDepth(), mFormat);
		// Set locking flags according to options
		D3D11_MAP  flags = D3D11_MAP_WRITE_DISCARD ;
		switch(options)
		{
		case HBL_NO_OVERWRITE:
			flags = D3D11_MAP_WRITE_NO_OVERWRITE;
			break;
		case HBL_NORMAL:
			flags = D3D11_MAP_WRITE_DISCARD;
			break;
		case HBL_DISCARD:
			flags = D3D11_MAP_WRITE_DISCARD;
			break;
		case HBL_READ_ONLY:
			flags = D3D11_MAP_READ;
			break;
		default: 
			break;
		};

		if (mUsage & HBU_DYNAMIC)
		{
			mDevice.clearStoredErrorMessages();

			D3D11_MAPPED_SUBRESOURCE pMappedResource;
			pMappedResource.pData = NULL;

			// TODO - check return values here
			switch(mParentTexture->getTextureType()) {
			case TEX_TYPE_1D:
				{

					mDevice.GetImmediateContext()->Map(mParentTexture->GetTex1D(), static_cast<UINT>(mSubresourceIndex), flags, 0, &pMappedResource);
					rval.data = pMappedResource.pData;
					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 1D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			case TEX_TYPE_CUBE_MAP:
			case TEX_TYPE_2D:
				{
					mDevice.GetImmediateContext()->Map(
							mParentTexture->GetTex2D(), 
							D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mFace, mParentTexture->getNumMipmaps()), 
							flags, 
							0, 
							&pMappedResource);
					rval.data = pMappedResource.pData;
					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 1D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			case TEX_TYPE_2D_ARRAY:
				{
					mDevice.GetImmediateContext()->Map(
						mParentTexture->GetTex2D(), 
						D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), lockBox.front, mParentTexture->getNumMipmaps()), 
						flags, 
						0, 
						&pMappedResource);
					rval.data = pMappedResource.pData;
					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 1D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			case TEX_TYPE_3D:
				{
					mDevice.GetImmediateContext()->Map(mParentTexture->GetTex3D(), static_cast<UINT>(mSubresourceIndex), flags, 0, &pMappedResource);
					rval.data = pMappedResource.pData;
					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 1D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			}

		}
		else
		{
			// for static usage just alloc
			size_t sizeOfImage = rval.getConsecutiveSize();
			mDataForStaticUsageLock = new int8[sizeOfImage];
			rval.data = mDataForStaticUsageLock;
		}

		mCurrentLock = rval;

		return rval;
	}
	//-----------------------------------------------------------------------------  
	void D3D11HardwarePixelBuffer::unlockImpl(void)
	{
		if (mUsage & HBU_DYNAMIC)
		{
			switch(mParentTexture->getTextureType()) {
			case TEX_TYPE_1D:
				{
					mDevice.GetImmediateContext()->Unmap(mParentTexture->GetTex1D(), mSubresourceIndex);
				}
				break;
			case TEX_TYPE_CUBE_MAP:
			case TEX_TYPE_2D:
				{							  
					mDevice.GetImmediateContext()->Unmap(mParentTexture->GetTex2D(), 
					D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mFace, mParentTexture->getNumMipmaps()));
				}
				break;
			case TEX_TYPE_2D_ARRAY:
				{
					mDevice.GetImmediateContext()->Unmap(mParentTexture->GetTex2D(), 
						D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mLockBox.front, mParentTexture->getNumMipmaps()));
				}
				break;
			case TEX_TYPE_3D:
				{
					mDevice.GetImmediateContext()->Unmap(mParentTexture->GetTex3D(), mSubresourceIndex);
				}
				break;
			}
		}
		else
		{
			D3D11_BOX dstBoxDx11 = OgreImageBoxToDx11Box(mLockBox);
			dstBoxDx11.front = 0;
			dstBoxDx11.back = mLockBox.getDepth();

			// copied from dx9
			size_t rowWidth;
			if (PixelUtil::isCompressed(mCurrentLock.format))
			{
				// D3D wants the width of one row of cells in bytes
				if (mCurrentLock.format == PF_DXT1)
				{
					// 64 bits (8 bytes) per 4x4 block
					rowWidth = (mCurrentLock.rowPitch / 4) * 8;
				}
				else
				{
					// 128 bits (16 bytes) per 4x4 block
					rowWidth = (mCurrentLock.rowPitch / 4) * 16;
				}

			}
			else
			{
				rowWidth = mCurrentLock.rowPitch * PixelUtil::getNumElemBytes(mCurrentLock.format);
			}

			switch(mParentTexture->getTextureType()) {
			case TEX_TYPE_1D:
				{

					mDevice.GetImmediateContext()->UpdateSubresource(
						mParentTexture->GetTex1D(), 
						static_cast<UINT>(mSubresourceIndex),
						&dstBoxDx11, 
						mDataForStaticUsageLock, rowWidth, 0);
					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 1D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			case TEX_TYPE_CUBE_MAP:
			case TEX_TYPE_2D:
				{
					mDevice.GetImmediateContext()->UpdateSubresource(
						mParentTexture->GetTex2D(), 
						D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mFace, mParentTexture->getNumMipmaps()),
						&dstBoxDx11, 
						mDataForStaticUsageLock, rowWidth, 0);

					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 2D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			case TEX_TYPE_2D_ARRAY:
				{
					mDevice.GetImmediateContext()->UpdateSubresource(
						mParentTexture->GetTex2D(), 
						D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mLockBox.front, mParentTexture->getNumMipmaps()),
						&dstBoxDx11, 
						mDataForStaticUsageLock, rowWidth, 0);

					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 2D texture array\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			case TEX_TYPE_3D:
				{
					size_t sliceWidth;
					if (PixelUtil::isCompressed(mCurrentLock.format))
					{
						// D3D wants the width of one slice of cells in bytes
						if (mCurrentLock.format == PF_DXT1)
						{
							// 64 bits (8 bytes) per 4x4 block
							sliceWidth = (mCurrentLock.slicePitch / 16) * 8;
						}
						else
						{
							// 128 bits (16 bytes) per 4x4 block
							sliceWidth = (mCurrentLock.slicePitch / 16) * 16;
						}

					}
					else
					{
						sliceWidth = mCurrentLock.slicePitch * PixelUtil::getNumElemBytes(mCurrentLock.format);
					}

					mDevice.GetImmediateContext()->UpdateSubresource(
						mParentTexture->GetTex3D(), 
						static_cast<UINT>(mSubresourceIndex),
						&dstBoxDx11, 
						mDataForStaticUsageLock, rowWidth, sliceWidth);
					if (mDevice.isError())
					{
						String errorDescription = mDevice.getErrorDescription();
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
							"D3D11 device cannot map 3D texture\nError Description:" + errorDescription,
							"D3D11HardwarePixelBuffer::lockImpl");
					}
				}
				break;
			}

			SAFE_DELETE_ARRAY(mDataForStaticUsageLock) ;
		}
	}

	//-----------------------------------------------------------------------------  

	D3D11_BOX D3D11HardwarePixelBuffer::OgreImageBoxToDx11Box(const Image::Box &inBox) const
	{
		D3D11_BOX res;
		res.left	= static_cast<UINT>(inBox.left);
		res.top		= static_cast<UINT>(inBox.top);
		res.front	= static_cast<UINT>(inBox.front);
		res.right	= static_cast<UINT>(inBox.right);
		res.bottom	= static_cast<UINT>(inBox.bottom);
		res.back	= static_cast<UINT>(inBox.back);

		return res;
	}

	//-----------------------------------------------------------------------------  

	void D3D11HardwarePixelBuffer::blit(const HardwarePixelBufferSharedPtr &rsrc, const Image::Box &srcBox, const Image::Box &dstBox)
	{
		if (
			(srcBox.getWidth() != dstBox.getWidth())
			|| (srcBox.getHeight() != dstBox.getHeight())
			|| (srcBox.getDepth() != dstBox.getDepth())
			)
		{
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"D3D11 device cannot copy a subresource - source and dest size are not the same and they have to be the same in DX11.",
				"D3D11HardwarePixelBuffer::blit");
		}

		D3D11_BOX srcBoxDx11 = OgreImageBoxToDx11Box(srcBox);


		D3D11HardwarePixelBuffer * rsrcDx11 = static_cast<D3D11HardwarePixelBuffer *>(rsrc.get());

		switch(mParentTexture->getTextureType()) {
		case TEX_TYPE_1D:
			{

				mDevice.GetImmediateContext()->CopySubresourceRegion(
					mParentTexture->GetTex1D(), 
					static_cast<UINT>(mSubresourceIndex),
					static_cast<UINT>(dstBox.left),
					0,
					0,
					rsrcDx11->mParentTexture->GetTex1D(),
					static_cast<UINT>(rsrcDx11->mSubresourceIndex),
					&srcBoxDx11);
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot copy 1d subresource Region\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blit");
				}			
			}
			break;
		case TEX_TYPE_CUBE_MAP:
		case TEX_TYPE_2D:
			{
				mDevice.GetImmediateContext()->CopySubresourceRegion(
					mParentTexture->GetTex2D(), 
					D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mFace, mParentTexture->getNumMipmaps()),
					static_cast<UINT>(dstBox.left),
					static_cast<UINT>(dstBox.top),
					mFace,
					rsrcDx11->mParentTexture->GetTex2D(),
					static_cast<UINT>(rsrcDx11->mSubresourceIndex),
					&srcBoxDx11);
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot copy 2d subresource Region\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blit");
				}
			}
			break;
		case TEX_TYPE_2D_ARRAY:
			{
				mDevice.GetImmediateContext()->CopySubresourceRegion(
					mParentTexture->GetTex2D(), 
					D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), srcBox.front, mParentTexture->getNumMipmaps()),
					static_cast<UINT>(dstBox.left),
					static_cast<UINT>(dstBox.top),
					srcBox.front,
					rsrcDx11->mParentTexture->GetTex2D(),
					static_cast<UINT>(rsrcDx11->mSubresourceIndex),
					&srcBoxDx11);
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot copy 2d subresource Region\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blit");
				}
			}
			break;
		case TEX_TYPE_3D:
			{
				mDevice.GetImmediateContext()->CopySubresourceRegion(
					mParentTexture->GetTex3D(), 
					static_cast<UINT>(mSubresourceIndex),
					static_cast<UINT>(dstBox.left),
					static_cast<UINT>(dstBox.top),
					static_cast<UINT>(dstBox.front),
					rsrcDx11->mParentTexture->GetTex3D(),
					static_cast<UINT>(rsrcDx11->mSubresourceIndex),
					&srcBoxDx11);
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot copy 3d subresource Region\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blit");
				}
			}
			break;
		}


		_genMipmaps();

	}
	//-----------------------------------------------------------------------------  
	void D3D11HardwarePixelBuffer::blitFromMemory(const PixelBox &src, const Image::Box &dstBox)
	{
		bool isDds = false;
		switch(mFormat)
		{
		case PF_DXT1:
		case PF_DXT2:
		case PF_DXT3:
		case PF_DXT4:
		case PF_DXT5:
			isDds = true;
			break;
		default:

			break;
		}

		if (isDds && (dstBox.getWidth() % 4 != 0 || dstBox.getHeight() % 4 != 0 ))
		{
			return;
		}


		// for scoped deletion of conversion buffer
		MemoryDataStreamPtr buf;
		PixelBox converted = src;

		D3D11_BOX dstBoxDx11 = OgreImageBoxToDx11Box(dstBox);
		dstBoxDx11.front = 0;
		dstBoxDx11.back = converted.getDepth();

		// convert to pixelbuffer's native format if necessary
		if (src.format != mFormat)
		{
			buf.bind(new MemoryDataStream(
				PixelUtil::getMemorySize(src.getWidth(), src.getHeight(), src.getDepth(),
				mFormat)));
			converted = PixelBox(src.getWidth(), src.getHeight(), src.getDepth(), mFormat, buf->getPtr());
			PixelUtil::bulkPixelConversion(src, converted);
		}

		// copied from dx9
		size_t rowWidth;
		if (PixelUtil::isCompressed(converted.format))
		{
			// D3D wants the width of one row of cells in bytes
			if (converted.format == PF_DXT1)
			{
				// 64 bits (8 bytes) per 4x4 block
				rowWidth = (converted.rowPitch / 4) * 8;
			}
			else
			{
				// 128 bits (16 bytes) per 4x4 block
				rowWidth = (converted.rowPitch / 4) * 16;
			}

		}
		else
		{
			rowWidth = converted.rowPitch * PixelUtil::getNumElemBytes(converted.format);
		}


		switch(mParentTexture->getTextureType()) {
		case TEX_TYPE_1D:
			{

				mDevice.GetImmediateContext()->UpdateSubresource( 
					mParentTexture->GetTex1D(), 
					0,
					&dstBoxDx11,
					converted.data,
					rowWidth,
					0 );
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot update 1d subresource\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blitFromMemory");
				}
			}
			break;
		case TEX_TYPE_CUBE_MAP:
		case TEX_TYPE_2D:
			{
				mDevice.GetImmediateContext()->UpdateSubresource( 
					mParentTexture->GetTex2D(), 
					D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), mFace, mParentTexture->getNumMipmaps()),
					&dstBoxDx11,
					converted.data,
					rowWidth,
					0 );
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot update 2d subresource\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blitFromMemory");
				}
			}
			break;
		case TEX_TYPE_2D_ARRAY:
			{
				mDevice.GetImmediateContext()->UpdateSubresource( 
					mParentTexture->GetTex2D(), 
					D3D11CalcSubresource(static_cast<UINT>(mSubresourceIndex), src.front, mParentTexture->getNumMipmaps()),
					&dstBoxDx11,
					converted.data,
					rowWidth,
					0 );
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot update 2d array subresource\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blitFromMemory");
				}
			}
			break;
		case TEX_TYPE_3D:
			{
				// copied from dx9
				size_t sliceWidth;
				if (PixelUtil::isCompressed(converted.format))
				{
					// D3D wants the width of one slice of cells in bytes
					if (converted.format == PF_DXT1)
					{
						// 64 bits (8 bytes) per 4x4 block
						sliceWidth = (converted.slicePitch / 16) * 8;
					}
					else
					{
						// 128 bits (16 bytes) per 4x4 block
						sliceWidth = (converted.slicePitch / 16) * 16;
					}

				}
				else
				{
					sliceWidth = converted.slicePitch * PixelUtil::getNumElemBytes(converted.format);
				}

				mDevice.GetImmediateContext()->UpdateSubresource( 
					mParentTexture->GetTex3D(), 
					static_cast<UINT>(mSubresourceIndex),
					&dstBoxDx11,
					converted.data,
					rowWidth,
					sliceWidth
					);
				if (mDevice.isError())
				{
					String errorDescription = mDevice.getErrorDescription();
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"D3D11 device cannot update 3d subresource\nError Description:" + errorDescription,
						"D3D11HardwarePixelBuffer::blitFromMemory");
				}
			}
			break;
		}


		if (!isDds)
		{
			_genMipmaps();
		}

	}
	//-----------------------------------------------------------------------------  
	void D3D11HardwarePixelBuffer::blitToMemory(const Image::Box &srcBox, const PixelBox &dst)
	{
		// Decide on pixel format of temp surface
		PixelFormat tmpFormat = mFormat; 
		if(D3D11Mappings::_getPF(dst.format) != DXGI_FORMAT_UNKNOWN)
		{
			tmpFormat = dst.format;
		}
		assert(srcBox.getDepth() == 1 && dst.getDepth() == 1);

		//This is a pointer to the texture we're trying to copy
		//Only implemented for 2D at the moment...
		ID3D11Texture2D *textureResource = mParentTexture->GetTex2D();

		// get the description of the texture
		D3D11_TEXTURE2D_DESC desc = {0};
		textureResource->GetDesc( &desc );
		//Alter the description to set up a staging texture
		desc.Usage = D3D11_USAGE_STAGING;
		//This texture is not bound to any part of the pipeline
		desc.BindFlags = 0;
		//Allow CPU Access
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		//No Misc Flags
		desc.MiscFlags = 0;
		//Create the staging texture
		ID3D11Texture2D* pStagingTexture = NULL;
		mDevice->CreateTexture2D( &desc, NULL, &pStagingTexture );
		//Copy our texture into the staging texture
		mDevice.GetImmediateContext()->CopyResource( pStagingTexture, textureResource );
		//Create a mapped resource and map the staging texture to the resource
		D3D11_MAPPED_SUBRESOURCE mapped = {0};
		mDevice.GetImmediateContext()->Map( pStagingTexture, 0, D3D11_MAP_READ , 0, &mapped );
		
		// read the data out of the texture.
		unsigned int rPitch = mapped.RowPitch;
		BYTE *data = ((BYTE *)mapped.pData);

		//Using existing OGRE functions
		DXGI_MAPPED_RECT lrect;	
		lrect.pBits = data;
		lrect.Pitch = rPitch;


		PixelBox locked(dst.getWidth(), dst.getHeight(), dst.getDepth(), tmpFormat);
		fromD3DLock(locked, lrect);
		PixelUtil::bulkPixelConversion(locked, dst);

		//Release the staging texture
		mDevice.GetImmediateContext()->Unmap( pStagingTexture, 0 );
        pStagingTexture->Release();
	}

	//-----------------------------------------------------------------------------  
	void D3D11HardwarePixelBuffer::_genMipmaps()
	{
		mDevice.GetImmediateContext()->GenerateMips(mParentTexture->getTexture());
		if (mDevice.isError())
		{
			String errorDescription = mDevice.getErrorDescription();
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"D3D11 device cannot generate mips\nError Description:" + errorDescription,
				"D3D11HardwarePixelBuffer::_genMipmaps");
		}	


	}
	//-----------------------------------------------------------------------------    
	RenderTexture *D3D11HardwarePixelBuffer::getRenderTarget(size_t zoffset)
	{
		assert(mUsage & TU_RENDERTARGET);
		assert(zoffset < mDepth);
		return mSliceTRT[zoffset];
	}
	//-----------------------------------------------------------------------------    
	D3D11Texture * D3D11HardwarePixelBuffer::getParentTexture() const
	{
		return mParentTexture;
	}
	//-----------------------------------------------------------------------------    
	size_t D3D11HardwarePixelBuffer::getSubresourceIndex() const
	{
		return mSubresourceIndex;
	}
	//-----------------------------------------------------------------------------    
	size_t D3D11HardwarePixelBuffer::getFace() const
	{
		return mFace;
	}
};
