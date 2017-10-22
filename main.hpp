#pragma once
#ifndef MAIN_HPP__
#define	MAIN_HPP__ "0.0.0@main.hpp"
//STD
#include <iostream>
#include <fstream>
#include <cstring>
//Precompiled
#include "stdafx.h"
//Project
#include "mDebuglib.hpp"
#include "fArgumentLib.hpp"
#include "cPageLoader.hpp"

namespace Project 
{
    void printArgs(int argc, char* argv[]) {
        DEBUG_NEW_MESSAGE("Start of args print: ")
            for (auto _index = 0; _index < argc; _index++)
                DEBUG_PRINT4("\t", _index, ") Argument: ", argv[_index]);
        DEBUG_END_MESSAGE
    }
}
#endif // !MAIN_HPP__