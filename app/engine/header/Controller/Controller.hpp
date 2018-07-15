#pragma once

namespace app
{

namespace controller
{

class Controller
{
public:
    Controller()
    {};

    virtual void updateController(float elaspedTime) = 0;
};

}// ! controller

}// ! app
