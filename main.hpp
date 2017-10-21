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

namespace Project 
{

    void printArgs(int argc, char* argv[]) {
        DEBUG_NEW_MESSAGE("Start of args print: ")
            for (auto _index = 0; _index < argc; _index++)
                DEBUG_WRITE4("\t", _index, ") Argument: ", argv[_index]);
        DEBUG_END_MESSAGE
    }
    
    namespace Arguments 
    {

        static const char *_optionPrefixes = "-/";

        enum class ArgTypes : char {
            Null,
            Flag,
            Value//,
            //MultiValue
        };

        struct Option {
            const char* shortCommand;
            const char* longCommand;
            ArgTypes type;
            int data;
        };

        char* isOption(char* arg) {
            auto size = std::strspn(arg, _optionPrefixes);
            return (size == 0) ? nullptr : arg + size;
        }

        void findFlagArg(int argc, char* argv[], Option& opt, int start = 0) {
            char* _opt = nullptr;
            for (auto _index = start; _index < argc; _index++) {
                _opt = isOption(argv[_index]);
                if (_opt) {
                    bool result = std::strstr(_opt, opt.shortCommand) != NULL || 
                        std::strstr(_opt, opt.longCommand) != NULL;
                    if (result) {
                        opt.data = true;
                        return;
                    }
                }
            }
            opt.data = false;
        }

        void findValueArg(int argc, char* argv[], Option& opt, int start = 0) {
            char* _opt = nullptr;
            for (auto _index = start; _index < argc; _index++) {
                _opt = isOption(argv[_index]);
                if (_opt) {
                    bool result = !std::strcmp(_opt, opt.shortCommand)  || 
                        !std::strcmp(_opt, opt.longCommand);
                    if (result) {
                        opt.data = _index + 1;
                        return;
                    }
                }
            }
            opt.data = -1;
        }

        void parseArgs(int argc, char* argv[], int optc, Option optv[]) {
            for (int _index = 0; _index < optc; _index++) {
                switch (optv[_index].type) {
                    case ArgTypes::Flag:
                        findFlagArg(argc, argv, optv[_index]);
                        break;
                    case ArgTypes::Value:
                        findValueArg(argc, argv, optv[_index]);
                    default:
                        break;
                }
            }
        }
    }
}
#endif // !MAIN_HPP__