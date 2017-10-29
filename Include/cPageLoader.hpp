#pragma once
#ifndef PAGE_LOADER_HPP__
#define PAGE_LOADER_HPP__ "0.0.0@cPageLoader.hpp"
/**
*	DESCRIPTION:
*		Module contains implementation of page file loader.
*	AUTHOR:
*		Mikhail Demchenko
*		mailto:dev.echo.mike@gmail.com
*		https://github.com/echo-Mike
**/

//STD
#include <fstream>
#include <string>
#include <memory>
#include <utility>
#include <cstring>
//SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

namespace Project {

    struct PageLoader : public std::ifstream {
        //Path to file
        std::string filePath;
        //One page storage
        std::unique_ptr<char[]> page;
        //Full page size
        std::size_t pageSize;
        //Current ammount of owned binary data
        //Normaly : pageSize == currentPayload
        std::size_t currentPayload;

        PageLoader(std::size_t _pageSize = 0x10000) : 
            std::ifstream(),
            pageSize(_pageSize), currentPayload(0)
        {
            page = std::make_unique<char[]>(pageSize);
        }

        PageLoader(std::string&& _filePath, std::size_t _pageSize = 0x10000) : 
            std::ifstream(_filePath, std::ifstream::binary),
            filePath(std::move(_filePath)), pageSize(_pageSize), currentPayload(0)
        {
            page = std::make_unique<char[]>(pageSize);
        }

        PageLoader(const char* _filePath, std::size_t _pageSize = 0x10000) : 
            PageLoader(std::move(std::string(_filePath)), _pageSize) {}

        ~PageLoader() = default;

        DefaultMovableNotCopyable(PageLoader)

        bool nextPage() {
            //If state of stream is valid
            if (*this) {
                //Read at most pageSize bytes
                read(page.get(), pageSize);
                //Setup current payload size as count of readed chars
                currentPayload = gcount();
                return true;
            }
            return false;
        }

        using PageBuffer = std::pair<std::size_t, std::unique_ptr<char[]>>;

        PageBuffer mergeWithNextPage() {
            PageBuffer result{0, nullptr};
            if (*this) {
                //Construct buffer capable of holding 2 pages
                result.second = std::make_unique<char[]>(pageSize * 2);
                //Register currently stored page in buffer
                result.first = currentPayload;
                std::memcpy(result.second.get(), page.get(), currentPayload);
                //Read next page
                if (nextPage()) {
                    //Copy data of next page in buffer directly after last page
                    std::memcpy(result.second.get() + result.first, page.get(), currentPayload);
                    //Count total size of pages
                    result.first += currentPayload;
                }
            }
            return result;
        }

        PageBuffer mergeWithNextPage(PageBuffer&& _buffer) {
            PageBuffer result{0, nullptr};
            if (*this) {
                //Construct buffer capable of holding previous buffer size + page size
                result.second = std::make_unique<char[]>(_buffer.first + pageSize);
                //Register currently stored data in new buffer
                result.first = _buffer.first;
                std::memcpy(result.second.get(), _buffer.second.get(), result.first);
                //Read next page
                if (nextPage()) {
                    //Copy data of next page in buffer directly after last page
                    std::memcpy(result.second.get() + result.first, page.get(), currentPayload);
                    //Count total size of pages
                    result.first += currentPayload;
                }
            }
            return result;
        }
    };

}
#endif