#pragma once
#ifndef MAIN_HPP__
#define	MAIN_HPP__ "0.0.0@main.hpp"
//STD
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <list>
//Precompiled
#include "stdafx.h"
//Snippets
#include <DebugLib/mDebuglib.hpp>
#include <ClassUtilsLib/mClassUtils.hpp>
#include <ArgumentsLib/fArgumentLib.hpp>
//Project
#include "cPageLoader.hpp"

namespace Project 
{
    void printArgs(int argc, char* argv[]) {
        DEBUG_NEW_MESSAGE("Start of args print: ")
            for (auto _index = 0; _index < argc; _index++)
                DEBUG_PRINT4("\t", _index, ") Argument: ", argv[_index]);
        DEBUG_END_MESSAGE
    }

    enum ExitCodes : int {
        NORMAL = 0,
        NO_INPUT_FILE_PATH,
        CANT_OPEN_INPUT_FILE,
        INVALID_FILE_SIZE,
        FILE_SIZE_AND_READ_COUNT_DONT_MATCH
    };

    template <std::uint16_t Signature>
    union cast_signature_16 {
        unsigned char bytes[2];
        std::uint16_t signature = Signature;
    };

    template <std::uint32_t Signature>
    union cast_signature_32 {
        unsigned char bytes[4];
        std::uint32_t signature = Signature;
    };

    template <std::uint64_t Signature>
    union cast_signature_64 {
        unsigned char bytes[8];
        std::uint64_t signature = Signature;
    };

    using FVHeaderStorage = std::list<
        std::pair<
            std::size_t, //Header offset
            EFI_FIRMWARE_VOLUME_HEADER* //Pointer to header structure
        >
    >;
}

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

struct {
	std::string inputFilePath;
} arguments;
#endif // !MAIN_HPP__