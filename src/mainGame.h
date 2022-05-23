#ifndef MAINGAME_H_INCLUDED
#define MAINGAME_H_INCLUDED

#include "CommonFunc.h"
#include "BaseObject.h"
#include "gameMap.h"
#include "mainObject.h"
#include "ImpTimer.h"
#include "TextObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
//#include "PlayerPower.h"

class MainGame
{
    public:

        void mainGame();



    private:

        BaseObject g_background;
        GameMap game_map;

        bool LoadBackground();
        std::vector<ThreatsObject*> MakeThreatList();


};



#endif // MAINGAME_H_INCLUDED
