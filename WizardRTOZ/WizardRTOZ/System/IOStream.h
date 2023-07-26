#pragma once

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace System{

    class IOStream{
    public:
        static IOStream printer;

        static inline void put(char data){
            fputc(data, stdout);
        }
        static inline void write(const char* message, size_t message_size = 0){
            if (message_size == 0){
                for (size_t index = 0 ; message[index] != 0 ; index++){
                    IOStream::put(message[index]);
                }
            } else {
                for (size_t index = 0 ; index < message_size ; index++){
                    IOStream::put(message[index]);
                }
            }
        }
        static void printf(const char* message, ...) __attribute__((format(printf, 1, 2))){
            char buffer[100] = {0};
            va_list arguments;
            va_start(arguments, message);
            vsnprintf(buffer, 100, message, arguments);
            IOStream::write(buffer);
            va_end(arguments);
        }
        inline IOStream& operator<<(const char* message) {
            IOStream::write(message);
            return *this;
        }
        inline IOStream& operator<<(char data) {
            IOStream::put(data);
            return *this;
        }
        inline IOStream& operator<<(float data) {
            IOStream::printf("%.2f", data);
            return *this;
        }
        inline IOStream& operator<<(int data) {
            IOStream::printf("%d", data);
            return *this;
        }
        inline IOStream& operator<<(size_t data) {
            IOStream::printf("%d", static_cast<int>(data));
            return *this;
        }
    };

    extern IOStream printer;
}
