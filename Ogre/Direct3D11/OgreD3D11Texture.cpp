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
#include "OgreD3D11Texture.h"
#include "OgreD3D11HardwarePixelBuffer.h"
#include "OgreD3D11Mappings.h"
#include "OgreD3D11Device.h"
#include "OgreException.h"

namespace Ogre 
{
	//---------------------------------------------------------------------
	D3D11Texture::D3D11Texture(ResourceManager* creator, const String& name, 
		ResourceHandle handle, const String& group, bool isManual, 
		ManualResourceLoader* loader, D3D11Device & device)
		:Texture(creator, name, handle, group, isManual, loader),
		mDevice(device), 
		mpTex(NULL),
		mpShaderResourceView(NULL),
		mp1DTex(NULL),
		mp2DTex(NULL),
		mp3DTex(NULL),
		mDynamicTextures(false)
	{
	}
	//---------------------------------------------------------------------
	D3D11Texture::~D3D11Texture()
	{
		// have to call this here reather than in Resource destructor
		// since calling virtual methods in base destructors causes crash
		if (isLoaded())
		{
			unload(); 
		}
		else
		{
			freeInternalResources();
		}
	}
	//---------------------------------------------------------------------
	void D3D11Texture::copyToTexture(TexturePtr& target)
	{
		// check if this & target are the same format and type
		// blitting from or to cube textures is not supported yet
		if (target->getUsage() != this->getUsage() ||
			target->getTextureType() != this->getTextureType())
		{
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
				"Src. and dest. textures must be of same type and must have the same usage !!!", 
				"D3D11Texture::copyToTexture" );
		}


		D3D11Texture *other;
		// get the target
		other = reinterpret_cast< D3D11Texture * >( target.get() );

		mDevice.GetImmediateContext()->CopyResource(other->getTextureResource(), mpTex);
		if (mDevice.isError())
		{
			String errorDescription = mDevice.getErrorDescription();
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"D3D11 device cannot copy resource\nError Description:" + errorDescription,
				"D3D11Texture::copyToTexture");
		}

	}
	//---------------------------------------------------------------------
	void D3D11Texture::loadImage( const Image &img )
	{
		// Use OGRE its own codecs
		vector<const Image*>::type imagePtrs;
		imagePtrs.push_back(&img);
		_loadImages( imagePtrs );
	}
	//---------------------------------------------------------------------
	void D3D11Texture::loadImpl()
	{
		if (mUsage & TU_RENDERTARGET)
		{
			createInternalResources();
			return;
		}

		// Make sure streams prepared.
		if (mLoadedStreams.isNull())
		{
			prepareImpl();
		}

		// Set reading positions of loaded streams to the beginning.
		for (uint i = 0; i < mLoadedStreams->size(); ++i)
		{
			MemoryDataStreamPtr curDataStream = (*mLoadedStreams)[i];

			curDataStream->seek(0);
		}

		// only copy is on the stack so well-behaved if exception thrown
		LoadedStreams loadedStreams = mLoadedStreams;

		this->_loadTex(loadedStreams);

	}
	//---------------------------------------------------------------------
	void D3D11Texture::freeInternalResources(void)
	{
		freeInternalResourcesImpl();
	}
	//---------------------------------------------------------------------
	void D3D11Texture::freeInternalResourcesImpl()
	{
		SAFE_RELEASE(mpTex);
        SAFE_RELEASE(mpShaderResourceView);
		SAFE_RELEASE(mp1DTex);
		SAFE_RELEASE(mp2DTex);
		SAFE_RELEASE(mp3DTex);
	}
	//---------------------------------------------------------------------
	void D3D11Texture::_loadTex(LoadedStreams & loadedStreams)
	{
		size_t pos = mName.find_last_of(".");
		String ext = mName.substr(pos+1);
		String baseName = mName.substr(0, pos);
		if((getSourceFileType() != "dds") && (this->getTextureType() == TEX_TYPE_CUBE_MAP))
		{
			// Load from 6 separate files
			// Use OGRE its own codecs
			//	String baseName;
			//	size_t pos = mName.find_last_of(".");
			
			//	if ( pos != String::npos )
			//		ext = mName.substr(pos+1);
			vector<Image>::type images(6);
			ConstImagePtrList imagePtrs;
			static const String suffixes[6] = {"_rt", "_lf", "_up", "_dn", "_fr", "_bk"};

            assert(loadedStreams->size()==6);
			for(size_t i = 0; i < 6; i++)
			{
				String fullName = baseName + suffixes[i];
				if (!ext.empty())
					fullName = fullName + "." + ext;

				// find & load resource data intro stream to allow resource
				// group changes if required
				DataStreamPtr stream((*loadedStreams)[i]);

				images[i].load(stream, ext);

				size_t imageMips = images[i].getNumMipmaps();

				if(imageMips < mNumMipmaps) {
					mNumMipmaps = imageMips;
				}


				imagePtrs.push_back(&images[i]);
			}

			_loadImages( imagePtrs );

		}
		else
		{
            assert(loadedStreams->size()==1);

			Image img;
			DataStreamPtr dstream((*loadedStreams)[0]);
		
			if(ext=="dds")
			{
				HRESULT hr;

				MemoryDataStreamPtr memoryptr=MemoryDataStreamPtr(new MemoryDataStream(dstream));
				// Load the Texture
				hr = D3DX11CreateShaderResourceViewFromMemory( mDevice.get(),
										memoryptr->getPtr(),
										memoryptr->size(),
										NULL,
										NULL, 
										&mpShaderResourceView, 
										NULL );
				if( FAILED( hr ) )
				{
					LogManager::getSingleton().logMessage("D3D11 : "+mName+" Could not be loaded");
				}
				D3D11_SHADER_RESOURCE_VIEW_DESC pDesc;
				mpShaderResourceView->GetDesc(&pDesc);
				int a=0;
			}
			else
			{
				img.load(dstream, ext);
				loadImage(img);
			}
		}
	}

	void D3D11Texture::createInternalResources(void)
	{
		createInternalResourcesImpl();
	}

	//---------------------------------------------------------------------
	void D3D11Texture::createInternalResourcesImpl(void)
	{
		// If mSrcWidth and mSrcHeight are zero, the requested extents have probably been set
		// through setWidth and setHeight, which set mWidth and mHeight. Take those values.
		if(mSrcWidth == 0 || mSrcHeight == 0) {
			mSrcWidth = mWidth;
			mSrcHeight = mHeight;
		}

		// Determine D3D pool to use
		// Use managed unless we're a render target or user has asked for 
		// a dynamic texture
		if (//(mUsage & TU_RENDERTARGET) ||
			(mUsage & TU_DYNAMIC))
		{
			mIsDynamic = true;
		}
		else
		{
			mIsDynamic = false;
		}
		// load based on tex.type
		switch (this->getTextureType())
		{
		case TEX_TYPE_1D:
			this->_create1DTex();
			break;
		case TEX_TYPE_2D:
		case TEX_TYPE_CUBE_MAP:
		case TEX_TYPE_2D_ARRAY:
			this->_create2DTex();
			break;
		case TEX_TYPE_3D:
			this->_create3DTex();
			break;
		default:
			this->freeInternalResources();
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D11Texture::createInternalResources" );
		}
	}
	//---------------------------------------------------------------------
	void D3D11Texture::_create1DTex()
	{
		// we must have those defined here
		assert(mSrcWidth > 0 || mSrcHeight > 0);

		// determine which D3D11 pixel format we'll use
		HRESULT hr;
		DXGI_FORMAT d3dPF = this->_chooseD3DFormat();

		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		UINT numMips = static_cast<UINT>(mNumRequestedMipmaps );

		D3D11_TEXTURE1D_DESC desc;
		desc.Width			= static_cast<UINT>(mSrcWidth);
		desc.MipLevels		= numMips;
		desc.ArraySize		= 1;
		desc.Format			= d3dPF;
		desc.Usage			= D3D11Mappings::_getUsage(mUsage);
		desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11Mappings::_getAccessFlags(mUsage);
		desc.MiscFlags		= D3D11_RESOURCE_MISC_GENERATE_MIPS;

		if (mIsDynamic)
		{
			desc.Usage			= D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE ;
		}

		// create the texture
		hr = mDevice->CreateTexture1D(	
			&desc,
			NULL,
			&mp1DTex);						// data pointer
		// check result and except if failed
		if (FAILED(hr) || mDevice.isError())
		{
			this->freeInternalResources();
			String errorDescription = mDevice.getErrorDescription();
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Error creating texture\nError Description:" + errorDescription, 
				"D3D11Texture::_create1DTex" );
		}

		// set the base texture we'll use in the render system
		hr = mp1DTex->QueryInterface(__uuidof(ID3D11Resource), (void **)&mpTex);
		if (FAILED(hr) || mDevice.isError())
		{
			this->freeInternalResources();
			String errorDescription = mDevice.getErrorDescription();
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Can't get base texture\nError Description:" + errorDescription, 
				"D3D11Texture::_create1DTex" );
		}

		// set final tex. attributes from tex. description
		// they may differ from the source image !!!
		mp1DTex->GetDesc(&desc);

		ZeroMemory( &mSRVDesc, sizeof(mSRVDesc) );
		mSRVDesc.Format = desc.Format;
		mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		mSRVDesc.Texture1D.MipLevels = desc.MipLevels;
		hr = mDevice->CreateShaderResourceView( mp1DTex, &mSRVDesc, &mpShaderResourceView );
		if (FAILED(hr) || mDevice.isError())
		{
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"D3D11 device can't create shader resource view.\nError Description:" + errorDescription,
				"D3D11Texture::_create1DTex");

		}

		this->_setFinalAttributes(desc.Width, 1, 1, D3D11Mappings::_getPF(desc.Format));

	}
	//---------------------------------------------------------------------
	void D3D11Texture::_create2DTex()
	{
		// we must have those defined here
		assert(mSrcWidth > 0 || mSrcHeight > 0);

		// determine which D3D11 pixel format we'll use
		HRESULT hr;
		DXGI_FORMAT d3dPF = this->_chooseD3DFormat();

		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		UINT numMips = 0;

		if (mNumRequestedMipmaps == MIP_UNLIMITED)
		{
			numMips = 0;
			mNumMipmaps = 0; // Will get this value from the created texture
		}
		//else
		//{
		//	numMips = static_cast<UINT>(mNumRequestedMipmaps + 1);
		//}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width			= static_cast<UINT>(mSrcWidth);
		desc.Height			= static_cast<UINT>(mSrcHeight);
		desc.MipLevels		= numMips;
		desc.ArraySize		= mDepth == 0 ? 1 : mDepth;
		desc.Format			= d3dPF;

		// Handle multisampled render target
		if (mUsage & TU_RENDERTARGET && (mFSAA > 1 || atoi(mFSAAHint.c_str()) > 0))
		{
			desc.SampleDesc.Count = mFSAA;
			desc.SampleDesc.Quality = atoi(mFSAAHint.c_str());
		}
		else
		{
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}

		desc.Usage			= D3D11Mappings::_getUsage(mUsage);
		desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11Mappings::_getAccessFlags(mUsage);
		desc.MiscFlags		= 0;//D3D11_RESOURCE_MISC_GENERATE_MIPS;
		//if (mMipmapsHardwareGenerated)
		{
			desc.MiscFlags		|= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		if (mIsDynamic)
		{
			desc.Usage			= D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}

		if (this->getTextureType() == TEX_TYPE_CUBE_MAP)
		{
			desc.MiscFlags		|= D3D11_RESOURCE_MISC_TEXTURECUBE;
			desc.ArraySize		= 6;
		}

		if( desc.Format == DXGI_FORMAT_BC1_UNORM || 
			desc.Format == DXGI_FORMAT_BC2_UNORM ||
			desc.Format == DXGI_FORMAT_BC3_UNORM ||
			desc.Format == DXGI_FORMAT_BC4_UNORM ||
			desc.Format == DXGI_FORMAT_BC5_UNORM 
			)
		{
			desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = 0;
			mNumRequestedMipmaps -= 2;
			desc.MipLevels -= 2;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}

		// create the texture
		hr = mDevice->CreateTexture2D(	
			&desc,
			NULL,// data pointer
			&mp2DTex);						
		// check result and except if failed
		if (FAILED(hr) || mDevice.isError())
		{
			this->freeInternalResources();
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
				"Error creating texture\nError Description:" + errorDescription, 
				"D3D11Texture::_create2DTex" );
		}

		if(mNumMipmaps == 0)
		{
			D3D11_TEXTURE2D_DESC texDesc;
			mp2DTex->GetDesc(&texDesc);
			mNumMipmaps = texDesc.MipLevels; 		
		}

		// set the base texture we'll use in the render system
		hr = mp2DTex->QueryInterface(__uuidof(ID3D11Resource), (void **)&mpTex);
		if (FAILED(hr) || mDevice.isError())
		{
			this->freeInternalResources();
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"Can't get base texture\nError Description:" + errorDescription, 
				"D3D11Texture::_create2DTex" );
		}

		// set final tex. attributes from tex. description
		// they may differ from the source image !!!
		mp2DTex->GetDesc(&desc);
		
		ZeroMemory( &mSRVDesc, sizeof(mSRVDesc) );
		mSRVDesc.Format = desc.Format;
		
		switch(this->getTextureType())
		{
		case TEX_TYPE_CUBE_MAP:
			mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			mSRVDesc.TextureCube.MipLevels = desc.MipLevels;
			mSRVDesc.TextureCube.MostDetailedMip = 0;
			break;

		case TEX_TYPE_2D_ARRAY:
			if (mUsage & TU_RENDERTARGET && (mFSAA > 1 || atoi(mFSAAHint.c_str()) > 0))
			{
				mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
				mSRVDesc.Texture2DMSArray.FirstArraySlice = 0;
				mSRVDesc.Texture2DMSArray.ArraySize = mDepth;
			}
			else
			{
				mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				mSRVDesc.Texture2DArray.FirstArraySlice = 0;
				mSRVDesc.Texture2DArray.ArraySize = mDepth;
				mSRVDesc.Texture2DArray.MostDetailedMip = 0;
				mSRVDesc.Texture2DArray.MipLevels = mNumMipmaps;
			}
			break;

		case TEX_TYPE_2D:
			if (mUsage & TU_RENDERTARGET && (mFSAA > 1 || atoi(mFSAAHint.c_str()) > 0))
			{
				mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				mSRVDesc.Texture2D.MostDetailedMip = 0;
				mSRVDesc.Texture2D.MipLevels = mNumMipmaps;
			}
			break;
		}

		if (this->getTextureType() == TEX_TYPE_2D_ARRAY)
		{
			mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			mSRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
			mSRVDesc.Texture2DArray.MostDetailedMip = 0;
			mSRVDesc.Texture2DArray.ArraySize = desc.ArraySize;
			mSRVDesc.Texture2DArray.FirstArraySlice = 0;

		}
		
		hr = mDevice->CreateShaderResourceView( mp2DTex, &mSRVDesc, &mpShaderResourceView );
		if (FAILED(hr) || mDevice.isError())
		{
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"D3D11 device can't create shader resource view.\nError Description:" + errorDescription,
				"D3D11Texture::_create2DTex");
		}

		this->_setFinalAttributes(desc.Width, desc.Height, desc.ArraySize, D3D11Mappings::_getPF(desc.Format));
	}
	//---------------------------------------------------------------------
	void D3D11Texture::_create3DTex()
	{
		// we must have those defined here
		assert(mWidth > 0 && mHeight > 0 && mDepth>0);

		// determine which D3D11 pixel format we'll use
		HRESULT hr;
		DXGI_FORMAT d3dPF = this->_chooseD3DFormat();

		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		UINT numMips = static_cast<UINT>(mNumRequestedMipmaps + 1);

		D3D11_TEXTURE3D_DESC desc;
		desc.Width			= static_cast<UINT>(mSrcWidth);
		desc.Height			= static_cast<UINT>(mSrcHeight);
		desc.Depth			= static_cast<UINT>(mDepth);
		desc.MipLevels		= numMips;
		desc.Format			= d3dPF;
		desc.Usage			= D3D11Mappings::_getUsage(mUsage);
		desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11Mappings::_getAccessFlags(mUsage);
		desc.MiscFlags		= 0;

		if (mIsDynamic)
		{
			desc.Usage			= D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE ;
		}
		// create the texture
		hr = mDevice->CreateTexture3D(	
			&desc,
			NULL,
			&mp3DTex);						// data pointer
		// check result and except if failed
		if (FAILED(hr) || mDevice.isError())
		{
			this->freeInternalResources();
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"Error creating texture\nError Description:" + errorDescription, 
				"D3D11Texture::_create3DTex" );
		}

		// set the base texture we'll use in the render system
		hr = mp3DTex->QueryInterface(__uuidof(ID3D11Resource), (void **)&mpTex);
		if (FAILED(hr) || mDevice.isError())
		{
			this->freeInternalResources();
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"Can't get base texture\nError Description:" + errorDescription, 
				"D3D11Texture::_create3DTex" );
		}

		// set final tex. attributes from tex. description
		// they may differ from the source image !!!
		mp3DTex->GetDesc(&desc);

	
		ZeroMemory( &mSRVDesc, sizeof(mSRVDesc) );
		mSRVDesc.Format = desc.Format;
		mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		mSRVDesc.Texture3D.MostDetailedMip = 0;
		mSRVDesc.Texture3D.MipLevels = desc.MipLevels;
		hr = mDevice->CreateShaderResourceView( mp3DTex, &mSRVDesc, &mpShaderResourceView );
		if (FAILED(hr) || mDevice.isError())
		{
			String errorDescription = mDevice.getErrorDescription(hr);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"D3D11 device can't create shader resource view.\nError Description:" + errorDescription,
				"D3D11Texture::_create3DTex");
		}

		this->_setFinalAttributes(desc.Width, desc.Height, desc.Depth, D3D11Mappings::_getPF(desc.Format));
	}
	//---------------------------------------------------------------------
	void D3D11Texture::_setFinalAttributes(unsigned long width, unsigned long height, 
		unsigned long depth, PixelFormat format)
	{ 
		// set target texture attributes
		mHeight = height; 
		mWidth = width; 
		mDepth = depth;
		mFormat = format; 

		// Update size (the final size, including temp space because in consumed memory)
		// this is needed in Resource class
		mSize = calculateSize();

		// say to the world what we are doing
		if (mWidth != mSrcWidth ||
			mHeight != mSrcHeight)
		{
			LogManager::getSingleton().logMessage("D3D11 : ***** Dimensions altered by the render system");
			LogManager::getSingleton().logMessage("D3D11 : ***** Source image dimensions : " + StringConverter::toString(mSrcWidth) + "x" + StringConverter::toString(mSrcHeight));
			LogManager::getSingleton().logMessage("D3D11 : ***** Texture dimensions : " + StringConverter::toString(mWidth) + "x" + StringConverter::toString(mHeight));
		}

		// Create list of subsurfaces for getBuffer()
		_createSurfaceList();
	}
	//---------------------------------------------------------------------
	void D3D11Texture::_setSrcAttributes(unsigned long width, unsigned long height, 
		unsigned long depth, PixelFormat format)
	{ 
		// set source image attributes
		mSrcWidth = width; 
		mSrcHeight = height; 
		mSrcDepth = depth;
		mSrcFormat = format;
		// say to the world what we are doing
		switch (this->getTextureType())
		{
		case TEX_TYPE_1D:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D11 : Creating 1D RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D11 : Loading 1D Texture, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_2D:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D11 : Creating 2D RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D11 : Loading 2D Texture, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_2D_ARRAY:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D11 : Creating 2D array RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D11 : Loading 2D Texture array, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_3D:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D11 : Creating 3D RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D11 : Loading 3D Texture, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_CUBE_MAP:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D11 : Creating Cube map RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D11 : Loading Cube Texture, base image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		default:
			this->freeInternalResources();
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D11Texture::_setSrcAttributes" );
		}
	}
	//---------------------------------------------------------------------
	DXGI_FORMAT D3D11Texture::_chooseD3DFormat()
	{
		// Choose frame buffer pixel format in case PF_UNKNOWN was requested
		if(mFormat == PF_UNKNOWN)
			return mBBPixelFormat;
		// Choose closest supported D3D format as a D3D format
		return D3D11Mappings::_getPF(D3D11Mappings::_getClosestSupportedPF(mFormat));

	}
	//---------------------------------------------------------------------
	void D3D11Texture::_createSurfaceList(void)
	{
		unsigned int bufusage;
		if ((mUsage & TU_DYNAMIC))
		{
			bufusage = HardwareBuffer::HBU_DYNAMIC;
		}
		else
		{
			bufusage = HardwareBuffer::HBU_STATIC;
		}
		if (mUsage & TU_RENDERTARGET)
		{
			bufusage |= TU_RENDERTARGET;
		}

		bool updateOldList = mSurfaceList.size() == (getNumFaces() * (mNumMipmaps + 1));
		if(!updateOldList)
		{	
			// Create new list of surfaces
			mSurfaceList.clear();
			PixelFormat format = D3D11Mappings::_getClosestSupportedPF(mFormat);
			size_t depth = mDepth;

			for(size_t face=0; face<getNumFaces(); ++face)
			{
				size_t width = mWidth;
				size_t height = mHeight;
				for(size_t mip=0; mip<=mNumMipmaps; ++mip)
				{ 

					D3D11HardwarePixelBuffer *buffer;
					size_t subresourceIndex = mip + face * mNumMipmaps;
					if (getNumFaces() > 0)
					{
						subresourceIndex = mip;

					}
					buffer = new D3D11HardwarePixelBuffer(
						this, // parentTexture
						mDevice, // device
						subresourceIndex, // subresourceIndex
						width, 
						height, 
						depth,
						face,
						format,
						(HardwareBuffer::Usage)bufusage // usage
						); 

					mSurfaceList.push_back(
						HardwarePixelBufferSharedPtr(buffer)
						);
					width /= 2;
					height /= 2;
				}
			}
		}

		// do we need to bind?

	}
	//---------------------------------------------------------------------
	HardwarePixelBufferSharedPtr D3D11Texture::getBuffer(size_t face, size_t mipmap) 
	{
		if(face >= getNumFaces())
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "A three dimensional cube has six faces",
			"D3D11Texture::getBuffer");
		if(mipmap > mNumMipmaps)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Mipmap index out of range",
			"D3D11Texture::getBuffer");
		size_t idx = face*(mNumMipmaps+1) + mipmap;
		assert(idx < mSurfaceList.size());
		return mSurfaceList[idx];
	}
	//---------------------------------------------------------------------
	bool D3D11Texture::releaseIfDefaultPool(void)
	{
		if(mIsDynamic)
		{
			LogManager::getSingleton().logMessage(
				"Releasing D3D11 default pool texture: " + mName);
			// Just free any internal resources, don't call unload() here
			// because we want the un-touched resource to keep its unloaded status
			// after device reset.
			freeInternalResources();
			LogManager::getSingleton().logMessage(
				"Released D3D11 default pool texture: " + mName);
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------
	bool D3D11Texture::recreateIfDefaultPool(D3D11Device &  device)
	{
		bool ret = false;
		if(mIsDynamic)
		{
			ret = true;
			LogManager::getSingleton().logMessage(
				"Recreating D3D11 default pool texture: " + mName);
			// We just want to create the texture resources if:
			// 1. This is a render texture, or
			// 2. This is a manual texture with no loader, or
			// 3. This was an unloaded regular texture (preserve unloaded state)
			if ((mIsManual && !mLoader) || (mUsage & TU_RENDERTARGET) || !isLoaded())
			{
				// just recreate any internal resources
				createInternalResources();
			}
			// Otherwise, this is a regular loaded texture, or a manual texture with a loader
			else
			{
				// The internal resources already freed, need unload/load here:
				// 1. Make sure resource memory usage statistic correction.
				// 2. Don't call unload() in releaseIfDefaultPool() because we want
				//    the un-touched resource keep unload status after device reset.
				unload();
				// if manual, we need to recreate internal resources since load() won't do that
				if (mIsManual)
					createInternalResources();
				load();
			}
			LogManager::getSingleton().logMessage(
				"Recreated D3D11 default pool texture: " + mName);
		}

		return ret;

	}
	//---------------------------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC D3D11Texture::getShaderResourceViewDesc() const
	{
		return mSRVDesc;
	}
	//---------------------------------------------------------------------
	void D3D11Texture::prepareImpl( void )
	{
		if (mUsage & TU_RENDERTARGET || isManuallyLoaded())
		{
			return;
		}

		//D3D11_DEVICE_ACCESS_CRITICAL_SECTION
		
        LoadedStreams loadedStreams;

		// prepare load based on tex.type
		switch (getTextureType())
		{
		case TEX_TYPE_1D:
		case TEX_TYPE_2D:
		case TEX_TYPE_2D_ARRAY:
			loadedStreams = _prepareNormTex();
			break;
		case TEX_TYPE_3D:
			loadedStreams = _prepareVolumeTex();
			break;
		case TEX_TYPE_CUBE_MAP:
			loadedStreams = _prepareCubeTex();
			break;
		default:
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D11Texture::prepareImpl" );
		}

		mLoadedStreams = loadedStreams;		
	}
	//---------------------------------------------------------------------
	D3D11Texture::LoadedStreams D3D11Texture::_prepareCubeTex()
	{
		assert(getTextureType() == TEX_TYPE_CUBE_MAP);

        LoadedStreams loadedStreams = LoadedStreams(OGRE_NEW_T (vector<MemoryDataStreamPtr>::type, MEMCATEGORY_GENERAL), SPFM_DELETE_T );
        // DDS load?
		if (getSourceFileType() == "dds")
		{
            // find & load resource data
			DataStreamPtr dstream = 
				ResourceGroupManager::getSingleton().openResource(
					mName, mGroup, true, this);
            loadedStreams->push_back(MemoryDataStreamPtr(OGRE_NEW MemoryDataStream(dstream)));
        }
        else
        {
			// Load from 6 separate files
			// Use OGRE its own codecs
			String baseName, ext;
			size_t pos = mName.find_last_of(".");
			baseName = mName.substr(0, pos);
			if ( pos != String::npos )
				ext = mName.substr(pos+1);
			static const String suffixes[6] = {"_rt", "_lf", "_up", "_dn", "_fr", "_bk"};

			for(size_t i = 0; i < 6; i++)
			{
				String fullName = baseName + suffixes[i];
				if (!ext.empty())
					fullName = fullName + "." + ext;

            	// find & load resource data intro stream to allow resource
				// group changes if required
				DataStreamPtr dstream = 
					ResourceGroupManager::getSingleton().openResource(
						fullName, mGroup, true, this);

                loadedStreams->push_back(MemoryDataStreamPtr(OGRE_NEW MemoryDataStream(dstream)));
			}
        }

        return loadedStreams;
	}
	//---------------------------------------------------------------------
	D3D11Texture::LoadedStreams D3D11Texture::_prepareVolumeTex()
	{
		assert(getTextureType() == TEX_TYPE_3D);

		// find & load resource data
		DataStreamPtr dstream = 
			ResourceGroupManager::getSingleton().openResource(
				mName, mGroup, true, this);

        LoadedStreams loadedStreams = LoadedStreams(OGRE_NEW_T (vector<MemoryDataStreamPtr>::type, MEMCATEGORY_GENERAL), SPFM_DELETE_T);
        loadedStreams->push_back(MemoryDataStreamPtr(OGRE_NEW MemoryDataStream(dstream)));
        return loadedStreams;
    }
	//---------------------------------------------------------------------
	D3D11Texture::LoadedStreams D3D11Texture::_prepareNormTex()
	{
		assert(getTextureType() == TEX_TYPE_1D || getTextureType() == TEX_TYPE_2D || getTextureType() == TEX_TYPE_2D_ARRAY);

		// find & load resource data
		DataStreamPtr dstream = 
			ResourceGroupManager::getSingleton().openResource(
				mName, mGroup, true, this);

        LoadedStreams loadedStreams = LoadedStreams(OGRE_NEW_T (vector<MemoryDataStreamPtr>::type, MEMCATEGORY_GENERAL), SPFM_DELETE_T);
        loadedStreams->push_back(MemoryDataStreamPtr(OGRE_NEW MemoryDataStream(dstream)));
        return loadedStreams;
	}
	//---------------------------------------------------------------------
	void D3D11Texture::unprepareImpl( void )
	{
		if (mUsage & TU_RENDERTARGET || isManuallyLoaded())
		{
			return;
		}   
	}
	//---------------------------------------------------------------------
	void D3D11Texture::postLoadImpl()
	{
		//D3D11_DEVICE_ACCESS_CRITICAL_SECTION
		mLoadedStreams.setNull();	
	}
	//---------------------------------------------------------------------
	// D3D11RenderTexture
	//---------------------------------------------------------------------
	void D3D11RenderTexture::rebind( D3D11HardwarePixelBuffer *buffer )
	{
		mBuffer = buffer;
		mWidth = (unsigned int) mBuffer->getWidth();
		mHeight = (unsigned int) mBuffer->getHeight();
		mColourDepth = (unsigned int) PixelUtil::getNumElemBits(mBuffer->getFormat());
		
		ID3D11Resource * pBackBuffer = buffer->getParentTexture()->getTextureResource();

		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory( &RTVDesc, sizeof(RTVDesc) );

		RTVDesc.Format = buffer->getParentTexture()->getShaderResourceViewDesc().Format;
		switch(buffer->getParentTexture()->getShaderResourceViewDesc().ViewDimension)
		{
		case D3D11_SRV_DIMENSION_BUFFER:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE1D:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
			break;
		case D3D11_SRV_DIMENSION_TEXTURECUBE:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			RTVDesc.Texture2DArray.FirstArraySlice = buffer->getFace();
			RTVDesc.Texture2DArray.ArraySize = 1;
			RTVDesc.Texture2DArray.MipSlice = 0;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2D:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2DMS:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
			break;
		case D3D11_SRV_DIMENSION_TEXTURE3D:
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			break;
		default:
			assert(false);
		}
		HRESULT hr = mDevice->CreateRenderTargetView( pBackBuffer, &RTVDesc, &mRenderTargetView );

		if (FAILED(hr) || mDevice.isError())
		{
			String errorDescription = mDevice.getErrorDescription();
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Error creating Render Target View\nError Description:" + errorDescription, 
				"D3D11RenderTexture::rebind" );
		}
	}
	//---------------------------------------------------------------------
	void D3D11RenderTexture::getCustomAttribute( const String& name, void *pData )
	{
		if(name == "DDBACKBUFFER")
		{
			// IDXGISurface ** pSurf = (IDXGISurface **)pData;
			//*pSurf = static_cast<D3D11HardwarePixelBuffer*>(mBuffer)->getSurface();
			return;
		}
		else if(name == "HWND")
		{
			HWND *pHwnd = (HWND*)pData;
			*pHwnd = NULL;
			return;
		}
		else if(name == "BUFFER")
		{
			*static_cast<HardwarePixelBuffer**>(pData) = mBuffer;
			return;
		}
		else if(name == "ID3D11RenderTargetView")
		{
			//*static_cast<ID3D11RenderTargetView**>(pData) = mRenderTargetView;
			*static_cast<ID3D11RenderTargetView***>(pData) = &mRenderTargetView;
			return;
		}
		else if( name == "numberOfViews" )
		{
			unsigned int* n = reinterpret_cast<unsigned int*>(pData);
			*n = 1;
		}
	}
	//---------------------------------------------------------------------
	D3D11RenderTexture::D3D11RenderTexture( const String &name, D3D11HardwarePixelBuffer *buffer,  D3D11Device & device ) : mDevice(device),
	RenderTexture(buffer, 0)
	{
		mName = name;

		rebind(buffer);
	}

	//---------------------------------------------------------------------

	D3D11RenderTexture::~D3D11RenderTexture()
	{

	}
}
