#pragma once
#ifndef MAIN_HPP__
#define	MAIN_HPP__ "0.0.0@main.hpp"

/// STD
#include <iostream>
#include <fstream>
#include <memory>

/// SNIPPETS
#include <ArgumentsLib/fArgumentLib.hpp>

/// JSON
#include <nlohmann/json.hpp>

/// PROJECT
#include "General.hpp"
#include "MemoryView.hpp"
#include "PiParsers.hpp"
#include "JsonConverters.hpp"

namespace Project 
{

}

Arguments::Option opts[] = {
	{ "f",     "file",         Arguments::ArgTypes::Value, 0 },
	{ "o",     "output",       Arguments::ArgTypes::Value, 0 },
	{ "d",     "decompressed", Arguments::ArgTypes::Value, 0 },
	{ nullptr, nullptr,        Arguments::ArgTypes::Null,  0 }
};

struct {
	std::string inputFilePath;
	std::string outputFilePath;
	std::string decompDirPath;
} arguments;
#endif // !MAIN_HPP__