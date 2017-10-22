#pragma onece
#ifndef ARGUMENT_LIB_HPP__
#define ARGUMENT_LIB_HPP__ "0.0.0@fArgumentLib.hpp"
/**
*	DESCRIPTION:
*		Module contains implementation of argument parser and helprer structures.
*	AUTHOR:
*		Mikhail Demchenko
*		mailto:dev.echo.mike@gmail.com
*		https://github.com/echo-Mike
**/

//STD
#include <cstring>

namespace Arguments 
{
    //Set of possible option prefixes
    static const char *_optionPrefixes = "-/";

    //Set of types of otions
    enum class ArgTypes : char {
        Null = 0,   //Type of last option in option list
        Flag,   //The flag option type
        Value   //Option with leading value type
        //MultiValue
    };

    struct Option {
        const char* shortCommand;
        const char* longCommand;
        ArgTypes type;
        int data;
    };

    /*
    *   @brief Identifies null-terminated string 'arg' as option/
    *   @param[in]  arg Pointer to first char of examined null-terminated string.
    *   @return Pointer to first option char in 'arg' or nullptr if not an option/
    */
    char* isOption(char* arg) {
        auto size = std::strspn(arg, _optionPrefixes);
        return (size == 0) ? nullptr : arg + size;
    }

    /*
    *   @brief Attempts to find flag argument in argument list
    *   @param[in]  argc    Argument list length.
    *   @param[in]  argv    Argument list.
    *   @param[in:out]  opt Option to find.
    *   @param[in]  start   Position in argument list to start with.
    *   @return Noreturn
    */
    void findFlagArg(int argc, char* argv[], Option& opt, int start = 0) {
        opt.data = false;
        if (start >= argc) return;
        char* _opt = nullptr;
        for (auto _index = start; _index < argc; _index++) {
            _opt = isOption(argv[_index]);
            if (_opt) {
                if (std::strstr(_opt, opt.shortCommand) != NULL || 
                    !std::strcmp(_opt, opt.longCommand)) 
                {
                    opt.data = true;
                    return;
                }
            }
        }
    }

    /*
    *   @brief Attempts to find value argument in argument list
    *   @param[in]  argc    Argument list length.
    *   @param[in]  argv    Argument list.
    *   @param[in:out]  opt Option to find.
    *   @param[in]  start   Position in argument list to start with.
    *   @return Noreturn
    */
    void findValueArg(int argc, char* argv[], Option& opt, int start = 0) {
        opt.data = -1;
        if (start >= argc - 1) return;
        char* _opt = nullptr;
        for (auto _index = start; _index < argc - 1; _index++) {
            _opt = isOption(argv[_index]);
            if (_opt) {
                if (!std::strcmp(_opt, opt.shortCommand) || 
                    !std::strcmp(_opt, opt.longCommand)) 
                {
                    opt.data = _index + 1;
                    return;
                }
            }
        }
    }

    /*
    *   @brief Main argumant parsing function.
    *   @param[in]  argc    Argument list length.
    *   @param[in]  argv    Argument list.
    *   @param[in:out]  optv Option list.
    *   @retunr Noreturn
    */
    void parseArgs(int argc, char* argv[], Option optv[]) {
        int _index = 0;
        while (optv[_index].type != ArgTypes::Null) 
        {
            switch (optv[_index].type) {
                case ArgTypes::Flag:
                    findFlagArg(argc, argv, optv[_index]);
                    break;
                case ArgTypes::Value:
                    findValueArg(argc, argv, optv[_index]);
                    break;
                default:
                    return;
            }
            ++_index;
        }
    }
}
#endif