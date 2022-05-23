
#include "CommonFunc.h"
#include "BaseObject.h"
#include "gameMap.h"
#include "mainObject.h"
#include "ImpTimer.h"
#include "TextObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
//#include "mainGame.h"


BaseObject g_background;
BaseObject g_back_menu;
BaseObject play;
BaseObject quit;
BaseObject over;
TTF_Font* font_time = NULL;
Mix_Music* music = NULL;


//int loop = 0;

bool InitData(){              //môi trường sdl
    bool succes = true;
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if(ret < 0){
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Super Dino",
                                 SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);

    if(g_window == NULL){
        printf("Could not init window %s\n", SDL_GetError());
        succes = false;
    }
    else{
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
            succes = false;
        else{
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags))
                succes == false;
        }
        if(TTF_Init() == -1)
        {
            cout << "Fail";
            succes = false;
        }
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        {
            printf("%s", Mix_GetError());

        }


        font_time = TTF_OpenFont("font/Brockers.ttf", 25);
        if(font_time == NULL)
        {
            succes = false;
        }
    }
    return succes;
}


bool LoadMedia(){
    bool ret = g_background.LoadImg("./image/background.png", g_screen);                  // load background
    bool retz = g_back_menu.LoadImg("./image/backmenu.png", g_screen);
    bool play_ret = play.LoadImg("./image/start_button.png", g_screen);
    bool quit_ret = quit.LoadImg("./image/quit_button.png", g_screen);
    bool gameover = over.LoadImg("./image/gameover.jpg", g_screen);
    music = Mix_LoadMUS("./audio/mortals.mp3");
    if(ret == false || retz == false || play_ret == false || quit_ret == false)
        return false;

    return true;
}



void close(){              //giải phóng đối tượng
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

std::vector<ThreatsObject*> MakeThreatList()
{
    std::vector<ThreatsObject*> list_threats;


    ThreatsObject* threats_objs = new ThreatsObject[30];
    for(int i = 0; i < 30; i++)
    {
        ThreatsObject* p_threat = (threats_objs + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("./image/threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(1500 + i*2000);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(0);

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);
        }
    }
    ThreatsObject* cactus_objs = new ThreatsObject[100];
    for(int i = 0; i < 100; i++)
    {
        ThreatsObject* p_threat = (cactus_objs + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("./image/cactus.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(600 + i*700);
            p_threat->set_y_pos(400);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(0);

            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{

    ImpTimer fps_timer;

    if(InitData() == false)
        return -1;
    if(LoadMedia() == false)
        {return -1;}

        //Load Music



    int mouseX, mouseY;

    bool game_loop = false;
    bool running_menu = true;
    bool is_quit = false;


    std::vector<ThreatsObject*> threats_list = MakeThreatList();
    Mix_PlayMusic(music, -1);

    while(!game_loop)
    {
        while(running_menu)
        {
            while(SDL_PollEvent(&g_event) != 0)
            {
                if(g_event.type == SDL_QUIT)
                {
                    running_menu = false;
                    game_loop = false;
                    SDL_Quit();
                }
                SDL_GetMouseState(&mouseX, &mouseY);
                if(g_event.type == SDL_MOUSEBUTTONDOWN)
            {
                if(mouseX > play.GetRect().x && mouseX < play.GetRect().x+100 && mouseY > play.GetRect().y && mouseY < play.GetRect().y + 64)
                {
                    is_quit = false;
                    running_menu = false;
                    break;
                }
                if(mouseX > quit.GetRect().x && mouseX < quit.GetRect().x+100 && mouseY > quit.GetRect().y && mouseY < quit.GetRect().y + 64)
                {
                    running_menu = false;
                    is_quit = true;
                    game_loop = true;
                    break;

                }

            }

            }

            SDL_RenderClear(g_screen);
            g_back_menu.SetRect(0, 0);
            g_back_menu.Render(g_screen, nullptr);                                                       //menu


            play.SetRect(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+100);
            play.Render(g_screen, nullptr);


            quit.SetRect(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2 +200);
            quit.Render(g_screen, nullptr);

            SDL_RenderPresent(g_screen);
        }




    GameMap game_map;

    game_map.LoadMap();
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("./image/cheems2.png", g_screen);
    p_player.set_clip();





    ExplosionObject exp_threat;

    bool tRet = exp_threat.LoadImg("./image/exp3.png", g_screen);
    if(!tRet) return -1;
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
    gold.SetColor(TextObject::BLACK_TEXT);

    int count_bullet = 0;

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
                game_loop = true;
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
                    if(num_die <= 3)
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);
                        continue;
                    }
                    else
                    {
                        p_player.Free();
                        p_threat->Free();
                        over.SetRect(SCREEN_WIDTH/2 - 200, 200);
                        over.Render(g_screen, nullptr);
                        SDL_RenderPresent(g_screen);
                        SDL_Delay(2000);

                        running_menu = true;
                        is_quit = true;
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
                        if(t == 0){
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
        }
        //Show game time



        //std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        /*Uint32 val_time = 300 - time_val;
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



        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND; //ms        // DINO SPEED


        if(time_val >= 20 && time_val <= 35)                        // tăng dần tốc độ sau mỗi 25s
        {
            time_one_frame = 30;
        }
        if(time_val >= 50 && time_val <= 65)
        {
            time_one_frame = 25;
        }
        if(time_val >= 65 && time_val <= 80)
        {
            time_one_frame = 20;
        }
        if(time_one_frame >= 80)
        {
            time_one_frame = 15;
        }
        if(real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0)
                SDL_Delay(delay_time);
        }

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

    close();
    return 0;

}
