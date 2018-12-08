#pragma once

namespace qc
{

namespace controller
{

class Controller
{
public:
    Controller() {};
    ~Controller() {};

    virtual void update() = 0;
};

};  // ! namespace controller

};  // ! namespace qc