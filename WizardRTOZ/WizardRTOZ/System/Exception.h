#pragma once

#include <functional>
#include <stdlib.h>
#include <stdint.h>
#include "./IOStream.h"

namespace System{

    class Exception{
    private:
        const uint64_t id;
        const char* name;
    public:
        inline Exception(const char* name) : id(reinterpret_cast<uint64_t>(this)), name(name) {}
        inline void warning(const char* message = nullptr) const {
            if (message == nullptr){
                IOStream::printf("[!] Exception %s\r\n", this->name);
            } else {
                IOStream::printf("[!] Exception %s: %s\r\n", this->name, message);
            }
        }
        inline void error(const char* message = nullptr) const {
            this->warning(message);
            exit(-1);
        }
        inline bool test(bool condition, const char* message = nullptr, std::function<bool(Exception)> treatment_callback = [](Exception){ return false; }) const {
            if (condition == false){
                return true;
            }
            this->warning(message);
            return treatment_callback(*this);
        }
        inline bool operator==(const Exception& exception) const {
            return (this->id == exception.id);
        }
        inline bool operator!=(const Exception& exception) const {
            return (this->id != exception.id);
        }
        inline operator uint64_t() const{
            return this->id;
        }
    };

    namespace Exceptions {
        extern const Exception logic_error;
        extern const Exception domain_error;
        extern const Exception invalid_argument;
        extern const Exception length_error;
        extern const Exception out_of_range;
        extern const Exception runtime_error;
        extern const Exception range_error;
        extern const Exception overflow_error;
        extern const Exception underflow_error;
    };
}
