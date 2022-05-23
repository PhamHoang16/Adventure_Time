#include "mainGame.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "gameMap.h"
#include "mainObject.h"
#include "ImpTimer.h"
#include "TextObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
//#include "PlayerPower.h"


//TTF_Font* font_time = NULL;

bool MainGame::LoadBackground(){
    bool ret = g_background.LoadImg("./image/background.png", g_screen);                  // load background
    if(ret == false)
        return false;

    return true;
}


std::vector<ThreatsObject*> MainGame::MakeThreatList()
{
    std::vector<ThreatsObject*> list_threats;

    /*ThreatsObject* dynamic_threats= new ThreatsObject[20];

    for(int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("./image/threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i*500);
            p_threat->set_y_pos(200);

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;

            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }*/

    ThreatsObject* threats_objs = new ThreatsObject[40];
    for(int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threats_objs + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("./image/cheems.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(1500 + i*1200);    //vị trí của cac threat nằm trên bản đồ
            p_threat->set_y_pos(300);            //threat rơi từ vị trí này


            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);



            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject* threats_doge = new ThreatsObject[40];
    for(int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threats_doge + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg2("./image/doge.png", g_screen);
            //p_threat->set_clips();
            p_threat->set_x_pos(1000 + i*1000);    //vị trí của cac threat nằm trên bản đồ
            p_threat->set_y_pos(300);            //threat rơi từ vị trí này


            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);



            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

void MainGame::mainGame()
{
    ImpTimer fps_timer;

    //if(InitData() == false)
        //return;
    if(LoadBackground() == false)
        return;

//    GameMap game_map;

    game_map.LoadMap();
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("./image/cheems2.png", g_screen);
    p_player.set_clip();

    //PlayerPower player_power;
    //PlayerPower->Init(g_screen);

    //player_power.Show(g_screen);

    std::vector<ThreatsObject*> threats_list = MakeThreatList();

    ExplosionObject exp_threat;

    bool tRet = exp_threat.LoadImg("./image/exp3.png", g_screen);
    if(!tRet) return;
    exp_threat.set_clip();

    int num_die = 0;

    // khai báo text

    TextObject time_game;
    time_game.SetColor(TextObject::BLACK_TEXT);

    TextObject score;
    score.SetColor(TextObject::BROWN_TEXT);

    TextObject heart;
    heart.SetColor(TextObject::WHITE_TEXT);

    TextObject gold;
    gold.SetColor(TextObject::WHITE_TEXT);

    int count_bullet = 0;
    bool is_quit = false;
    int xpos = 0;
    int count_score = 0;
    while(!is_quit)
    {

        fps_timer.start();
        while(SDL_PollEvent(&g_event) != 0)
        {
            if(g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            p_player.HandleInputAction(g_event, g_screen);
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);     //set lại màn hình

        g_background.Render(g_screen, NULL);

        game_map.DrawMap(g_screen);
        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);

        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);



        xpos = p_player.GetXpos();
        if(xpos % 8 == 0 && xpos > 0)
        {
            count_score ++;
        }
        //cout << xpos << endl;

        for(int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {
                float threat_pos = p_threat->GetXpos();
                //cout << "]][][" << threat_pos << endl;

                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->DoPlayer(map_data);
                //if(threat_pos - xpos <= 900 && threat_pos - xpos > 0)

                    p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

                //}

                p_threat->Show(g_screen);

                SDL_Rect rect_player = p_player.GetRectFrame();
                SDL_Rect rect_threat = p_threat->GetRectFrame();

                int frame_exp_width = exp_threat.get_frame_width();
                int frame_exp_height = exp_threat.get_frame_height();

                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                for(int jj = 0; jj < tBullet_list.size(); ++jj)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if(pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if(bCol1)
                        {
                            p_threat->RemoveBullet(jj);
                            break;

                        }
                    }
                }
                //SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);

                if(bCol1 || bCol2)
                {
                    for(int ex = 0; ex < NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = p_player.GetRect().x - frame_exp_width*0.2;
                                int y_pos = p_player.GetRect().y - frame_exp_height*0.5;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                                SDL_RenderPresent(g_screen);

                            }
                    num_die++;
                    //if(num_die <= 3)
                    /*{
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);
                        continue;
                    }*/
                    //else
                    {
                        p_player.Free();
                        p_threat->Free();
                        SDL_Quit();
                        return;
                 }

                }
            }
        }
        //cout << "--------------" << endl;

        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        //main kill threat
        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
        for(int r = 0; r < bullet_arr.size(); ++r)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if(p_bullet != NULL)
            {
                for(int t = 0; t < threats_list.size(); ++t)
                {
                    ThreatsObject* obj_threat = threats_list.at(t);
                    if(obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        tRect.h = obj_threat->get_width_frame();

                        SDL_Rect bRect = p_bullet->GetRect();

                        //bool bCol = false;

                        bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

                        if(bCol)
                        {
                            bool bCol2 = SDLCommonFunc::CheckCollision(bRect, tRect);
                            if(bCol2)
                            {

                            for(int ex = 0; ex < NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                                SDL_RenderPresent(g_screen);
                            }

                            count_score += 100;
                            p_player.RemoveBullet(r);
                            obj_threat->Free();
                            threats_list.erase(threats_list.begin() + t);
                            count_bullet = 0;
                            }
                        }

                    }
                }
            }
        }
        //Show game time

        /*cout << xpos << endl;

        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 300 - time_val;
        {
                std::string str_val = std::to_string(val_time);
                str_time += str_val;

                time_game.SetText(str_time);
                time_game.LoadFromRenderText(font_time, g_screen);
                time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }*/

        //show score



        std::string val_str_score = std::to_string(count_score);
        std::string strScore = "Score: ";

        strScore += val_str_score;

        score.SetText(strScore);
        score.LoadFromRenderText(font_time, g_screen);
        score.RenderText(g_screen, SCREEN_WIDTH - 200, 15);

        //show gold


        int count_gold = p_player.GetGoldCount();
        std::string gold_str = std::to_string(count_gold);

        gold.SetText(gold_str);
        gold.LoadFromRenderText(font_time, g_screen);
        gold.RenderText(g_screen, 64, 15);


        //show heart

        int count_heart = p_player.GetHeartCount();
        std::string heart_str = std::to_string(count_heart);

        gold.SetText(heart_str);
        gold.LoadFromRenderText(font_time, g_screen);
        gold.RenderText(g_screen, 64*3, 15);



        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND; //ms        // DINO SPEED

        if(count_score % 800 != 0 && count_score > 0 && count_score <= 3000 && time_one_frame > 10)
        {
            time_one_frame -= (count_score/800) * 100/FRAME_PER_SECOND;              //TĂNG DẦN TỐC ĐỘ SAU MỖI 500 ĐIỂM
        }
        else if(count_score > 3000)
        {
            time_one_frame = 15;
        }
        else
        {
            time_one_frame = 1000/FRAME_PER_SECOND;
        }
        //cout << time_one_frame << endl;
        //cout << xpos/64 << endl;
        if(real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0)
                SDL_Delay(delay_time);
        }

    }
    for(int i = 0; i < threats_list.size(); i++)
    {
        ThreatsObject* p_threat = threats_list.at(i);
        if(p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }

    threats_list.clear();

}
