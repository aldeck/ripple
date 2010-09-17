/*
 * Copyright 2009-2010, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */


#include <Alert.h>

#include "App.h"
#include "MainWindow.h"

#include <stdio.h>


App::App()
	:
	BApplication("application/x-vnd.Haiku-Ripple"),
	fMainWindow(NULL)
{
}


App::~App()
{
}


void
App::ReadyToRun()
{
	BRect frame(50, 50, 512 + 50, 512 + 50);
	const char* title = "Ripple";
	fMainWindow = new MainWindow(frame, title);
	fMainWindow->Show();
}


void
App::AboutRequested()
{
	BAlert* alert;
	alert = new BAlert("About", "A little 2d rendering demo", "ok");
	alert->Go(NULL);
}


bool
App::QuitRequested()
{
	return true;
}


int
main(int argc, char** argv)
{
	App app;
	app.Run();
	return 0;
}
