#pragma once
#ifndef MAIN_HPP__
#define	MAIN_HPP__ "0.0.0@main.hpp"

/// STD
#include <iostream>
#include <fstream>

/// SNIPPETS
#include <ArgumentsLib/fArgumentLib.hpp>

/// PROJECT
#include "General.hpp"

namespace Project 
{

    using FVHeaderPtr = EFI_FIRMWARE_VOLUME_HEADER*;

    using FVHeaderData = std::pair<
        std::size_t, //Header offset
        PiFirmwareVolumeProxy //Pointer to header structure
    >;

    using FVHeaderStorage = std::list<FVHeaderData>;
}

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

struct {
	std::string inputFilePath;
} arguments;
#endif // !MAIN_HPP__