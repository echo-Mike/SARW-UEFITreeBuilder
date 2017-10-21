#pragma once
#ifndef MAIN_HPP__
#define	MAIN_HPP__ "0.0.0@main.hpp"
#include <iostream>
#include "stdafx.h"
#include "mDebuglib.hpp"

namespace Project {
    
    void printArgs(int argc, char* argv[]) {
        DEBUG_NEW_MESSAGE("Start of args print: ")
            for (auto _index = 0; _index < argc; _index++)
                DEBUG_WRITE4("\t", _index, ") : ", argv[_index]);
        DEBUG_END_MESSAGE
    }
}
#endif // !MAIN_H__