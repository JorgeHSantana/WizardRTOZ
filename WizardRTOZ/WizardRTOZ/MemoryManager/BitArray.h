#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../System/Status.h"
#include "../System/Exception.h"

#include "./Bitwise.h"

namespace MemoryManager{
    template <size_t AMOUNT_OF_BITS = 8>
    class BitArray{
    private:
        class Binary{
        public:
            size_t mask {0};
            size_t byte {0};
            uint8_t bit {0};
            inline Binary(size_t bit_position, uint8_t amount_of_bits){
                System::Exceptions::out_of_range.test((bit_position + amount_of_bits) > AMOUNT_OF_BITS, "Position argument is not allowed by this object.");
                System::Exceptions::length_error.test(amount_of_bits == 0, "Invalid amount of bit to write number.");
                this->bit = (bit_position & 7);
                this->byte = (bit_position >> 3);
                this->mask = ((1 << (amount_of_bits)) - 1);
            }
        };
        uint8_t data[BitArray<AMOUNT_OF_BITS>::size_in_bytes] {0};
    public:
        class Reference{
        private:
            BitArray<AMOUNT_OF_BITS>& bit_array;
            size_t bit_position;
        public:
            inline Reference(BitArray<AMOUNT_OF_BITS>& bit_array, size_t bit_position) : bit_array(bit_array), bit_position(bit_position) {}
            template <typename DATA_TYPE> inline void write(DATA_TYPE value) const{
                this->bit_array.write(this->bit_position, value);
            }
            inline void set(void) const {
                this->write(true);
            }
            inline void clear(void) const {
                this->write(false);
            }
            inline void toggle(void) const {
                this->write(!this->get());
            }
            template <typename DATA_TYPE = bool> inline DATA_TYPE value(void) const {
                return this->bit_array.get(this->bit_position);
            }
            inline operator bool() const {
                return this->value<bool>();
            }
            template <typename DATA_TYPE> inline Reference operator=(DATA_TYPE value) const {
                this->write(value);
                return Reference(this->bit_array, this->bit_position);
            }
            inline Reference operator=(const Reference& value) const {
                this->write(value.value());
                return Reference(this->bit_array, this->bit_position);
            }
            inline Reference const& operator++(){
                this->bit_position++;
                return *this;
            }
            inline Reference const& operator*(){
                return *this;
            }
            inline bool operator==(Reference& reference){
                return (&this->bit_array == &reference.bit_array) && (this->bit_position == reference.bit_position);
            }
            inline bool operator!=(Reference& reference){
                return !(*this == reference);
            }
        };
        static constexpr size_t size_in_bytes = (((AMOUNT_OF_BITS) < 8) ? 1 : (((AMOUNT_OF_BITS - 1) >> 3) + 1));
        static constexpr size_t size_in_bits = AMOUNT_OF_BITS;

        inline BitArray(void) {
            memset(this->data, 0, BitArray<AMOUNT_OF_BITS>::size_in_bytes);
        }
        inline BitArray(const bool (&bool_array)[AMOUNT_OF_BITS]) {
            for (size_t bit = 0 ; bit < AMOUNT_OF_BITS ; bit++){
                this->write(bit, bool_array[bit]);
            }
        }
        inline ~BitArray(void) {
            memset(this->data, 0, BitArray<AMOUNT_OF_BITS>::size_in_bytes);
        }
        template <typename DATA_TYPE = bool> inline DATA_TYPE get(size_t bit_position, uint8_t amount_of_bits = 1){
            BitArray<AMOUNT_OF_BITS>::Binary binary(bit_position, amount_of_bits);
            return static_cast<DATA_TYPE>((this->data[binary.byte] >> binary.bit) & binary.mask);
        }
        template <typename DATA_TYPE> inline void write(size_t bit_position, DATA_TYPE value, uint8_t amount_of_bits = 1){
            BitArray<AMOUNT_OF_BITS>::Binary binary(bit_position, amount_of_bits);
            if (static_cast<bool>(value) == true){
                this->data[binary.byte] |= (binary.mask << binary.bit);
            } else {
                this->data[binary.byte] &= ~(binary.mask << binary.bit);
            }
        }
        inline void set(size_t bit_position, uint8_t amount_of_bits = 1){
            this->write(bit_position, true, amount_of_bits);
        }
        inline void clear(size_t bit_position, uint8_t amount_of_bits = 1){
            this->write(bit_position, false, amount_of_bits);
        }
        inline void toggle(size_t bit_position, uint8_t amount_of_bits = 1){
            BitArray<AMOUNT_OF_BITS>::Binary binary(bit_position, amount_of_bits);
            this->data[binary.byte] ^= (binary.mask << binary.bit);
        }
        inline void fill(bool value){
            memset(this->data, value ? 0xFF : 0x00, BitArray<AMOUNT_OF_BITS>::size_in_bytes);
        }
        inline void clear(void){
            this->fill(false);
        }
        template <typename ARRAY_TYPE> inline void toArray(ARRAY_TYPE (&array)[AMOUNT_OF_BITS]){
            for (size_t bit = 0 ; bit < AMOUNT_OF_BITS ; bit++){
                array[bit] = static_cast<ARRAY_TYPE>(this->get(bit));
            }
        }
        inline Reference begin(void){
            return Reference(*this, 0);
        }
        inline Reference end(void){
            return Reference(*this, AMOUNT_OF_BITS - 1);
        }
        inline Reference operator[](size_t bit_position){
            return Reference(*this, bit_position);
        }
        inline bool operator[](size_t bit_position) const {
            return this->get(bit_position);
        }
    };
}
