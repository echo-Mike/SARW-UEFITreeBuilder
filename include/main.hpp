#pragma once
#ifndef MAIN_HPP__
#define	MAIN_HPP__ "0.0.0@main.hpp"

/// STD
#include <iostream>
#include <fstream>
#include <memory>

/// SNIPPETS
#include <ArgumentsLib/fArgumentLib.hpp>

/// PROJECT
#include "General.hpp"

namespace Project 
{

}

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

struct {
	std::string inputFilePath;
} arguments;
#endif // !MAIN_HPP__