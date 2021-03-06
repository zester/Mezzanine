#ifndef _catchapp_h
#define _catchapp_h

#include <mezzanine.h>

#include "levelzones.h"
#include "levelloader.h"
#include "levelscorer.h"
#include "itemshop.h"
#include "profilemanager.h"
#include "workunits.h"

using namespace Mezzanine;

class CatchApp
{
    public:
        typedef std::vector<ActorBase*> ThrowableContainer;
        typedef std::vector<StartArea*> StartAreaContainer;
        typedef std::vector<ScoreArea*> ScoreAreaContainer;
        enum GameState
        {
            Catch_Init,
            Catch_Loading,
            Catch_MenuScreen,
            Catch_GameScreen,
            Catch_ScoreScreen
        };
    protected:
        friend class CatchPreInputWorkUnit;
        friend class CatchPostInputWorkUnit;
        friend class CatchPostUIWorkUnit;
        friend class CatchPostGraphicsWorkUnit;

        static CatchApp* TheRealCatchApp;

        CatchPreInputWorkUnit* PreInputWork;
        CatchPostInputWorkUnit* PostInputWork;
        CatchPostUIWorkUnit* PostUIWork;
        CatchPostGraphicsWorkUnit* PostGraphicsWork;

        Entresol* TheEntresol;
        ProfileManager* Profiles;
        LevelLoader* Loader;
        LevelScorer* Scorer;
        ItemShop* Shop;
        ActorBase* LastActorThrown;

        Timer* LevelTimer;
        StopWatchTimer* EndTimer;

        bool Paused;
        CatchApp::GameState CurrentState;

        const Plane PlaneOfPlay;

        ThrowableContainer ThrownItems;
        StartAreaContainer StartAreas;

        void MakeGUI();
        void CreateLoadingScreen();
        void InitMusic();
        void VerifySettings();

        void ChangeState(const CatchApp::GameState &StateToSet);

        bool CheckEndOfLevel();
        bool AllStartZonesEmpty();
        void UnloadLevel();
    public:
        CatchApp();
        ~CatchApp();

        static CatchApp* GetCatchAppPointer();
        int GetCatchin();
        void PauseGame(bool Pause);
        bool GameIsPaused() const;

        bool IsAThrowable(ActorBase* Actor) const;
        bool IsInsideAnyStartZone(ActorBase* Actor) const;
        void RegisterStartArea(StartArea* Start);
        void AddThrowable(ActorBase* Throwable);

        ThrowableContainer& GetThrowables();
        LevelLoader* GetLevelLoader() const;
        LevelScorer* GetLevelScorer() const;
        ProfileManager* GetProfiles() const;
        ItemShop* GetItemShop() const;
        Timer* GetLevelTimer() const;
        StopWatchTimer* GetEndTimer() const;
};//CatchApp

#endif
