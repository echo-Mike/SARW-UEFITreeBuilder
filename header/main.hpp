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
#include <PatternsLib/cPipeline.hpp>
//Project
#include "cPageLoader.hpp"
#include "vFFSGuids.hpp"
#include "cMemoryProxy.hpp"

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
        std::uint32_t signature;
        constexpr cast_signature_32() noexcept : signature(Signature) {}
    };

    template <std::uint64_t Signature>
    union cast_signature_64 {
        unsigned char bytes[8];
        std::uint64_t signature = Signature;
    };

    using FVHeaderPtr = EFI_FIRMWARE_VOLUME_HEADER*;

    using FVHeaderData = std::pair<
        std::size_t, //Header offset
        PiFirmwareVolumeProxy //Pointer to header structure
    >;

    using FVHeaderStorage = std::list<FVHeaderData>;

    std::uint16_t calculateChecksum16(const void* buffer, std::size_t length) {
        std::uint16_t result = 0;
        for (std::size_t index = 0; index < length / sizeof(std::uint16_t); ++index)
            result += reinterpret_cast<const std::uint16_t*>(buffer)[index];
        result = static_cast<std::uint16_t>(-result);
		return result;
    }
}

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

struct {
	std::string inputFilePath;
} arguments;
#endif // !MAIN_HPP__