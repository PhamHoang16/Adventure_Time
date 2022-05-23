#include "BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = BULLET;
}

BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;

    if(bullet_type_ == LASER_BULLET)
    {
       ret = LoadImg("./image/lazer.png", des);
    }
    else if(bullet_type_ == RED_BULLET)
    {
        ret = LoadImg("./image/threat_bullet.png", des);
    }
    else if(bullet_type_ == GREEN_BULLET)
    {
        ret = LoadImg("./image/green_bullet.png", des);
    }
    else if(bullet_type_ == BONE_BULLET)
    {
        ret = LoadImg("./image/bone.png", des);
    }
    else
    {
        ret = LoadImg("./image/bullet.png", des);
    }
    return ret;
}
void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
    if(bullet_dir_ == DIR_RIGHT)
    {
        rect_.x += x_val_;
        if(rect_.x > x_border)
        {
            is_move_ = false;
        }
    }

    else if(bullet_dir_ == DIR_LEFT)
    {
        rect_.x -= x_val_;
        if(rect_.x < 0)
        {
            is_move_ = false;
        }
    }


}
