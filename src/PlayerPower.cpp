/*#include "PlayerPower.h"

PlayerPower::PlayerPower()
{
    number_ = 0;
}

PlayerPower::~PlayerPower()
{

}

void AddPos(const int& xp)
{
    pos_list_.push_back(xp);
}

void PlayerPower::Init(SDL_Renderer* screen)
{
    LoadImg("./image/heart.png", screen);
    number_ = 3;
    if(pos_list_.size() > 0)
    {
        pos_list_.clear;
    }

    AddPos(20);
    AddPos(60);
    AddPos(100);
}

void PlayerPower::Show(SDL_Renderer* screen)
{
    for(int i = 0; i < pos_list_.size(); i++)
    {
        rect_.x = pos_list_.at(i);
        rect_.y = 0;
        Render(screen);
    }
}

void PlayerPower::Decrease()
{
    number_--;
    pos_list_.pop_back();
}

void PlayerPower::InitCrease()
{
    number_++;
    int last_pos = pos_list_.back();
    last_pos += 40;
    pos_list_.push_back(last_pos);
}
*/
