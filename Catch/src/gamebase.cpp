#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include "catchapp.h"

int main(int argc, char **argv)
{
	// Temporary Hack
    #ifdef MACOSX
	String ExeDir = Mezzanine::ResourceManager::GetExecutableDirFromArg(argc,argv);
	Mezzanine::ResourceManager::ChangeDirectory(ExeDir);
    #endif
	
    CatchApp* Catch = new CatchApp();
    Catch->GetCatchin();
    delete Catch;
	return 0;
}

#endif
