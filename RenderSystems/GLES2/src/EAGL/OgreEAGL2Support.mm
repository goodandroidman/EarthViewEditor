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

#include "OgreRoot.h"

#include "OgreGLES2RenderSystem.h"
#include "OgreEAGLES2Context.h"

#include "OgreEAGL2Support.h"
#include "OgreEAGL2Window.h"

#include "macUtils.h"

namespace Ogre {

    EAGL2Support::EAGL2Support()
    {
        mCurrentOSVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    }

    EAGL2Support::~EAGL2Support()
    {
    }

    void EAGL2Support::addConfig(void)
    {
        ConfigOption optFullScreen;
        ConfigOption optVideoMode;
        ConfigOption optDisplayFrequency;
        ConfigOption optContentScalingFactor;
        ConfigOption optFSAA;
        ConfigOption optRTTMode;

        optFullScreen.name = "Full Screen";
        optFullScreen.possibleValues.push_back("Yes");
        optFullScreen.possibleValues.push_back("No");
        optFullScreen.currentValue = "Yes";
        optFullScreen.immutable = false;
        
        // Get the screen mode.  On all iPhones and iPod Touches this will always be the mode when in portrait
        // because the dock does not change orientations. 
        CGSize modeSize = [[UIScreen mainScreen] currentMode].size;
        uint w = modeSize.width / [UIScreen mainScreen].scale;
        uint h = modeSize.height / [UIScreen mainScreen].scale;

        UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;

        if(UIInterfaceOrientationIsPortrait(orientation))
           std::swap(w, h);

        optVideoMode.name = "Video Mode";
        optVideoMode.possibleValues.push_back("320 x 480");     // iPhones/iPod Touches
        optVideoMode.possibleValues.push_back("640 x 960");     // iPhones/iPod Touches - retina 3.5"
        optVideoMode.possibleValues.push_back("640 x 1136");    // iPhones/iPod Touches - retina 4"
        optVideoMode.possibleValues.push_back("320 x 568");     // iPad Mini - portrait
        optVideoMode.possibleValues.push_back("568 x 320");     // iPad Mini - landscape
        optVideoMode.possibleValues.push_back("1024 x 768");    // iPad - landscape
        optVideoMode.possibleValues.push_back("768 x 1024");    // iPad - portrait
        optVideoMode.possibleValues.push_back("1536 x 2048");   // iPad - retina portrait
        optVideoMode.possibleValues.push_back("2048 x 1536");   // iPad - retina landscape
        optVideoMode.possibleValues.push_back("1280 x 720");    // Apple TV over AirPlay 720p
        optVideoMode.currentValue = StringConverter::toString(w) + " x " +
                                    StringConverter::toString(h);
        optVideoMode.immutable = false;

        optDisplayFrequency.name = "Display Frequency";
        optDisplayFrequency.possibleValues.push_back("0 Hz");
        optDisplayFrequency.currentValue = "0 Hz";
        optDisplayFrequency.immutable = false;

        optContentScalingFactor.name = "Content Scaling Factor";
        optContentScalingFactor.possibleValues.push_back( "1.0" );
        optContentScalingFactor.possibleValues.push_back( "1.33" );
        optContentScalingFactor.possibleValues.push_back( "1.5" );
        optContentScalingFactor.possibleValues.push_back( "2.0" );
        optContentScalingFactor.currentValue = StringConverter::toString([UIScreen mainScreen].scale);
        optContentScalingFactor.immutable = false;
        
        optFSAA.name = "FSAA";
        optFSAA.possibleValues.push_back( "0" );
        optFSAA.possibleValues.push_back( "2" );
        optFSAA.possibleValues.push_back( "4" );
        optFSAA.currentValue = "0";
        optFSAA.immutable = false;

        optRTTMode.name = "RTT Preferred Mode";
        optRTTMode.possibleValues.push_back("Copy");
        optRTTMode.possibleValues.push_back("FBO");
        optRTTMode.currentValue = "FBO";
        optRTTMode.immutable = false;

        mOptions[optFullScreen.name] = optFullScreen;
        mOptions[optVideoMode.name] = optVideoMode;
        mOptions[optDisplayFrequency.name] = optDisplayFrequency;
        mOptions[optContentScalingFactor.name] = optContentScalingFactor;
        mOptions[optFSAA.name] = optFSAA;
        mOptions[optRTTMode.name] = optRTTMode;
        
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
        ConfigOption optOrientation;
        optOrientation.name = "Orientation";
        optOrientation.possibleValues.push_back("Portrait");
        optOrientation.possibleValues.push_back("Landscape Left");
        optOrientation.possibleValues.push_back("Landscape Right");
        optOrientation.currentValue = "Portrait";
        optOrientation.immutable = false;
        mOptions[optOrientation.name] = optOrientation;
#endif
        
        // Set the shader cache path
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString *cachesDirectory = [paths objectAtIndex:0];

        setShaderCachePath([[cachesDirectory stringByAppendingString:@"/"] cStringUsingEncoding:NSASCIIStringEncoding]);
        
        // Set the shader library path
        setShaderLibraryPath(Ogre::macBundlePath() + "/Contents/Media/RTShaderLib");
    }

    String EAGL2Support::validateConfig(void)
    {
        // TODO - DJR
        return StringUtil::BLANK;
    }

    String EAGL2Support::getDisplayName(void)
    {
        return "todo";
	}

    CFDictionaryRef EAGL2Support::chooseGLConfig(const GLint *attribList, GLint *nElements)
    {
        // TODO: DJR - implement
        CFDictionaryRef configs = NULL;

        return configs;
    }

    GLint EAGL2Support::getGLConfigAttrib(CFDictionaryRef glConfig, GLint attribute, GLint *value)
    {
        // TODO: DJR - implement
        GLint status = 0;

        return status;
    }

    CFDictionaryRef EAGL2Support::getGLConfigFromContext(EAGLES2Context context)
    {
        // TODO: DJR - implement
        CFDictionaryRef glConfig = 0;

        return glConfig;
    }

    CFDictionaryRef EAGL2Support::getGLConfigFromDrawable(CAEAGLLayer *drawable,
                                                    unsigned int *w, unsigned int *h)
    {
        // TODO: DJR - implement
        CFDictionaryRef glConfig = 0;

        return glConfig;
    }

    //------------------------------------------------------------------------
    // A helper class for the implementation of selectFBConfig
    //------------------------------------------------------------------------
    class GLConfigAttribs
    {
        public:
            GLConfigAttribs(const int* attribs)
            {
                for (int i = 0; attribs[2*i] != 0; i++)
                {
                    fields[attribs[2*i]] = attribs[2*i+1];
                }
            }

            void load(EAGL2Support* const glSupport, CFDictionaryRef glConfig)
            {
                std::map<int,int>::iterator it;

                for (it = fields.begin(); it != fields.end(); it++)
                {
                    it->second = 0;

                    glSupport->getGLConfigAttrib(glConfig, it->first, &it->second);
                }
            }

            bool operator>(GLConfigAttribs& alternative)
            {
                return false;
            }

            std::map<int,int> fields;
    };

    CFDictionaryRef EAGL2Support::selectGLConfig(const int* minAttribs, const int *maxAttribs)
    {
        // TODO: DJR - implement
        CFDictionaryRef glConfig = 0;
        int nConfigs = 0;//, config;

        glConfig = chooseGLConfig(minAttribs, &nConfigs);

        if (!nConfigs)
        {
            return 0;
        }

        return glConfig;
    }
    
    RenderWindow * EAGL2Support::createWindow(bool autoCreateWindow,
                                           GLES2RenderSystem* renderSystem,
                                           const String& windowTitle)
    {
        RenderWindow *window = 0;

        if (autoCreateWindow)
        {
            ConfigOptionMap::iterator opt;
            ConfigOptionMap::iterator end = mOptions.end();
            NameValuePairList miscParams;

            CGSize modeSize = [[UIScreen mainScreen] currentMode].size;
            bool fullscreen = false;
            uint w = (uint)modeSize.width, h = (uint)modeSize.height;

            if ((opt = mOptions.find("Full Screen")) != end)
            {
                fullscreen = (opt->second.currentValue == "Yes");
            }

            if ((opt = mOptions.find("Display Frequency")) != end)
            {
                miscParams["displayFrequency"] = opt->second.currentValue;
            }

            if ((opt = mOptions.find("Content Scaling Factor")) != end)
            {
                miscParams["contentScalingFactor"] = opt->second.currentValue;
            }
            
            if ((opt = mOptions.find("Video Mode")) != end)
            {
                String val = opt->second.currentValue;
                String::size_type pos = val.find('x');

                if (pos != String::npos)
                {
                    w = StringConverter::parseUnsignedInt(val.substr(0, pos));
                    h = StringConverter::parseUnsignedInt(val.substr(pos + 1));
                }
            }

            if ((opt = mOptions.find("FSAA")) != end)
            {
                miscParams["FSAA"] = opt->second.currentValue;
            }

            window = renderSystem->_createRenderWindow(windowTitle, w, h, fullscreen, &miscParams);
        }

        return window;
    }

    RenderWindow * EAGL2Support::newWindow(const String &name,
                                        unsigned int width, unsigned int height,
                                        bool fullScreen,
                                        const NameValuePairList *miscParams)
    {
        EAGL2Window *window = OGRE_NEW EAGL2Window(this);
        window->create(name, width, height, fullScreen, miscParams);

        return window;
    }

    EAGLES2Context * EAGL2Support::createNewContext(CFDictionaryRef &glconfig, CAEAGLLayer *drawable, EAGLSharegroup *group) const
    {
        EAGLES2Context *context = new EAGLES2Context(drawable, group);
        if (context == NULL)
        {
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
                        "Fail to create new context",
                        __FUNCTION__);
            return context;
        }
        
        return context;
    }

    void * EAGL2Support::getProcAddress(const Ogre::String& name)
    {
        return NULL;
    }
    
    void EAGL2Support::start()
    {
    }
    
    void EAGL2Support::stop()
    {
    }

    bool EAGL2Support::interfaceOrientationIsSupported(NSString *orientation)
    {
        NSArray *supportedOrientations = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"UISupportedInterfaceOrientations"];

        return [supportedOrientations containsObject:orientation];
    }

    bool EAGL2Support::portraitIsSupported()
    {
        NSArray *supportedOrientations = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"UISupportedInterfaceOrientations"];

        return ([supportedOrientations containsObject:@"UIInterfaceOrientationPortrait"] || 
                [supportedOrientations containsObject:@"UIInterfaceOrientationPortraitUpsideDown"]);
    }
}
