/*
 Copyright (c) 2012, The Cinder Project, All rights reserved.
 Copyright (c) Microsoft Open Technologies, Inc. All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/App.h"
#include "cinder/Display.h"
#include "cinder/Function.h"

#include "cinder/app/TouchEvent.h"

namespace cinder { namespace app {

typedef	signals::signal<bool (),BooleanAndEventCombiner>				EventSignalShouldQuit;

//! Abstract class that defines the interface for a 'basic' app (ex. desktop, installation, 'normal').
class AppBasic : public App {
  public:
	class Settings : public App::Settings {
  	  public:
		Settings();

		void	setShouldQuit ( bool aShouldQuit = true );
		bool	isFullScreen() { return mDefaultWindowFormat.isFullScreen(); }

		//! Returns whether the default window is resizable
		bool	isResizable() const { return mDefaultWindowFormat.isResizable(); }
		//! Sets the default window to be resizable or not
		void	setResizable( bool resizable = true ) { mDefaultWindowFormat.setResizable( resizable ); }
		//! Returns whether the default window will be created without a border (chrome/frame)
		bool	isBorderless() const { return mDefaultWindowFormat.isBorderless(); }
		//! Sets the default window to be created without a border (chrome/frame)
		void	setBorderless( bool borderless = true ) { mDefaultWindowFormat.setBorderless( borderless ); }
		//! Returns whether the default  window always remains above all other windows
		bool	isAlwaysOnTop() const { return mDefaultWindowFormat.isAlwaysOnTop(); }
		//! Sets whether the default window always remains above all other windows
		void	setAlwaysOnTop( bool alwaysOnTop = true ) { mDefaultWindowFormat.setAlwaysOnTop( alwaysOnTop ); }

		//! Returns the display for the default window
		DisplayRef	getDisplay() const { return mDefaultWindowFormat.getDisplay(); }
		//! Sets the display for the default window
		void		setDisplay( DisplayRef display ) { mDefaultWindowFormat.setDisplay( display ); }

#if defined( CINDER_MSW )
		//! If enabled MSW apps will display a secondary window which captures all cout, cerr, cin and App::console() output. Default is \c false.
		void	enableConsoleWindow( bool enable = true ) { mEnableMswConsole = enable; }
		//! Returns whether MSW apps will display a secondary window which captures all cout, cerr, cin and App::console() output. Default is \c false.
		bool	isConsoleWindowEnabled() const { return mEnableMswConsole; }
#endif

		//! Registers the app to receive multiTouch events from the operating system. Disabled by default. Only supported on WinRT, Windows 7/8 and Mac OS X trackpad.
		void		enableMultiTouch( bool enable = true ) { mEnableMultiTouch = enable; }
		//! Returns whether the app is registered to receive multiTouch events from the operating system. Disabled by default. Only supported on Windows 7 and Mac OS X trackpad.
		bool		isMultiTouchEnabled() const { return mEnableMultiTouch; }

		//! Sets whether the app quits automatically when its last window is closed. Enabled by default.
		void		enableQuitOnLastWindowClose( bool enable = true ) { mQuitOnLastWindowClose = enable; }
		//! Returns whether the app quits automatically when its last window is closed. Enabled by default.
		bool		isQuitOnLastWindowCloseEnabled() const { return mQuitOnLastWindowClose; }

	 private:
		bool		mQuitOnLastWindowClose;
#if defined( CINDER_MSW )
		bool		mEnableMswConsole;
#endif
	};

	// This is really just here to disambiguate app::WindowRef from the WindowRef found in QuickDraw (so that client code doesn't have to invoke cinder::app::WindowRef explicitly)	
	typedef std::shared_ptr<Window>		WindowRef;

 public:
	AppBasic();
	virtual ~AppBasic();

	virtual void		prepareSettings( Settings *settings ) {}

	//! This is fired before the app is quit. If any slots return false then the app quitting is canceled.
	EventSignalShouldQuit&	getSignalShouldQuit() { return mSignalShouldQuit; }

	//! Creates and returns a reference to a new Window, adhering to \a format.
	virtual WindowRef		createWindow( const Window::Format &format = Window::Format() ) = 0;

	void		restoreWindowContext() override;

	//! Disables frameRate limiting.
	virtual void		disableFrameRate() = 0;
	//! Returns whether frameRate limiting is enabled.
	virtual bool		isFrameRateEnabled() const = 0;

	//! Hides the mouse cursor
	virtual void		hideCursor() = 0;
	//! Shows the mouse cursor
	virtual void		showCursor() = 0;
	//! Returns AppBasic::Settings that were set during prepareSettings() (or the defaults)
	const Settings&		getSettings() const { return mSettings; }

	//! Returns a vector of the command line arguments passed to the app
	const std::vector<std::string>&		getArgs() const { return mCommandLineArgs; }

	//! Gets the foreground Window, which has keyboard and mouse focus
	virtual WindowRef	getForegroundWindow() const = 0;

	// DO NOT CALL - should be private but aren't for esoteric reasons
	//! \cond
	// Internal handlers - these are called into by AppImpl's. If you are calling one of these, you have likely strayed far off the path.
	bool		privateShouldQuit();
	
	//! \endcond
	
	//! Returns a pointer to the current global AppBasic
	static AppBasic*	get()	{ return sBasicInstance; }

	//! \cond
	// These are called by application instantiation macros and are only used in the launch process
	static void		prepareLaunch() { App::prepareLaunch(); }
	static void		cleanupLaunch() { App::cleanupLaunch(); }
	//! \endcond

  protected:
	static AppBasic*			sBasicInstance;
	EventSignalShouldQuit		mSignalShouldQuit;
	std::vector<std::string>	mCommandLineArgs;
	Settings					mSettings;
};

} } // namespace cinder::app
