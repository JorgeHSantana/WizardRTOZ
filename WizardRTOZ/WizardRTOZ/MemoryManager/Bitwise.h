#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../System/Status.h"

/**
 * @namespace MemoryManager
 *
 * @brief Namespace containing memory management utilities.
 */
namespace MemoryManager{

    /**
     * @class Bitwise
     *
     * @brief Class providing bitwise memory management operations.
     *
     * @tparam DATA_TYPE The data type to operate on.
     */
    template <typename DATA_TYPE> class Bitwise {
        public:
            static constexpr size_t size_in_bytes = sizeof(DATA_TYPE);   ///< Size of the data type in bytes
            static constexpr size_t size_in_bits = (size_in_bytes << 3); ///< Size of the data type in bits

            /**
             * @class Bit
             *
             * @brief Class providing bitwise operations on individual bits.
             */
            class Bit {
                public:

                    /**
                     * @brief Set bits in the data type at the specified position.
                     *
                     * @param data The data type to operate on.
                     * @param position The starting position of the bits to set.
                     * @param size_in_bits The number of bits to set.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status set(DATA_TYPE& data, uint8_t position, size_t size_in_bits = 1){
                        if((position + size_in_bits - 1) >= Bitwise<DATA_TYPE>::size_in_bits){
                            return System::Status::length_error;
                        }
                        data |= ((DATA_TYPE(1) << size_in_bits) - 1) << position;
                        return System::Status::ok;
                    }

                    /**
                     * @brief Clear bits in the data type at the specified position.
                     *
                     * @param data The data type to operate on.
                     * @param position The starting position of the bits to clear.
                     * @param size_in_bits The number of bits to clear.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status clear(DATA_TYPE& data, uint8_t position, size_t size_in_bits = 1){
                        if((position + size_in_bits - 1) >= Bitwise<DATA_TYPE>::size_in_bits){
                            return System::Status::length_error;
                        }
                        data &= ~(((DATA_TYPE(1) << size_in_bits) - 1) << position);
                        return System::Status::ok;
                    }

                    /**
                     * @brief Toggle bits in the data type at the specified position.
                     *
                     * @param data The data type to operate on.
                     * @param position The starting position of the bits to toggle.
                     * @param size_in_bits The number of bits to toggle.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status toggle(DATA_TYPE& data, uint8_t position, size_t size_in_bits = 1){
                        if((position + size_in_bits - 1) >= Bitwise<DATA_TYPE>::size_in_bits){
                            return System::Status::length_error;
                        }
                        data ^= ((DATA_TYPE(1) << size_in_bits) - 1) << position;
                        return System::Status::ok;
                    }

                    /**
                     * @brief Read bits from the source and store them in the destiny variable.
                     *
                     * @tparam DESTINY_TYPE The type of the destiny variable.
                     * @param destiny The variable to store the read bits.
                     * @param source The source data to read from.
                     * @param position The starting position of the bits to read.
                     * @param size_in_bits The number of bits to read.
                     *
                     * @return The status of the operation.
                     */
                    template<typename DESTINY_TYPE> inline static System::Status read(DESTINY_TYPE& destiny, DATA_TYPE source, uint8_t position, size_t size_in_bits = 1){
                        if((position + size_in_bits - 1) >= Bitwise<DATA_TYPE>::size_in_bits && sizeof(DESTINY_TYPE) >= Bitwise<DATA_TYPE>::size_in_bytes){
                            return System::Status::length_error;
                        }
                        destiny = static_cast<DESTINY_TYPE>((source >> position) & ((DESTINY_TYPE(1) << size_in_bits) - 1));
                        return System::Status::ok;
                    }

                    /**
                     * @brief Copy bits from the source to the destiny.
                     *
                     * @param data_destiny The destination data type to copy bits into.
                     * @param destiny_position The starting position in the destination to copy bits to.
                     * @param data_source The source data type to copy bits from.
                     * @param source_position The starting position in the source to copy bits from.
                     * @param size_in_bits The number of bits to copy.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status copy(DATA_TYPE& data_destiny, size_t destiny_position, const DATA_TYPE& data_source, size_t source_position, size_t size_in_bits = 1){
                        if(((destiny_position + size_in_bits - 1) >= Bitwise<DATA_TYPE>::size_in_bits) || ((source_position + size_in_bits - 1) >= Bitwise<DATA_TYPE>::size_in_bits)){
                            return System::Status::length_error;
                        }
                        data_destiny = (data_destiny & ~(((DATA_TYPE(1) << size_in_bits) - 1) << destiny_position)) | ((data_source >> source_position) & ((DATA_TYPE(1) << size_in_bits) - 1)) << destiny_position;
                        return System::Status::ok;
                    }
            };

            /**
             * @class Byte
             *
             * @brief Class providing bitwise operations on individual bytes.
             */
            class Byte {
                public:

                    /**
                     * @brief Set bytes in the data type at the specified position.
                     *
                     * @param data The data type to operate on.
                     * @param position The starting position of the bytes to set.
                     * @param amount_of_bytes The number of bytes to set.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status set(DATA_TYPE& data, uint8_t position, size_t amount_of_bytes = 1){
                        return Bit::set(data, (position << 3), (amount_of_bytes << 3));
                    }

                    /**
                     * @brief Clear bytes in the data type at the specified position.
                     *
                     * @param data The data type to operate on.
                     * @param position The starting position of the bytes to clear.
                     * @param amount_of_bytes The number of bytes to clear.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status clear(DATA_TYPE& data, uint8_t position, size_t amount_of_bytes = 1){
                        return Bit::clear(data, (position << 3), (amount_of_bytes << 3));
                    }

                    /**
                     * @brief Toggle bytes in the data type at the specified position.
                     *
                     * @param data The data type to operate on.
                     * @param position The starting position of the bytes to toggle.
                     * @param amount_of_bytes The number of bytes to toggle.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status toggle(DATA_TYPE& data, uint8_t position, size_t amount_of_bytes = 1){
                        return Bit::toggle(data, (position << 3), (amount_of_bytes << 3));
                    }

                    /**
                     * @brief Read bytes from the source and store them in the destiny.
                     *
                     * @tparam DESTINY_TYPE The type of the destiny variable.
                     * @param destiny The variable to store the read bytes.
                     * @param source The source data to read from.
                     * @param position The starting position of the bytes to read.
                     * @param amount_of_bytes The number of bytes to read.
                     *
                     * @return The status of the operation.
                     */
                    template<typename DESTINY_TYPE> inline static System::Status read(DESTINY_TYPE& destiny, DATA_TYPE source, uint8_t position, size_t amount_of_bytes = 1){
                        return Bit::read(destiny, source, (position << 3), (amount_of_bytes << 3));
                    }

                    /**
                     * @brief Copy bytes from the source to the destiny.
                     *
                     * @param data_destiny The destination data type to copy bytes into.
                     * @param destiny_position The starting position in the destination to copy bytes to.
                     * @param data_source The source data type to copy bytes from.
                     * @param source_position The starting position in the source to copy bytes from.
                     * @param amount_of_bytes The number of bytes to copy.
                     *
                     * @return The status of the operation.
                     */
                    inline static System::Status copy(DATA_TYPE& data_destiny, size_t destiny_position, const DATA_TYPE& data_source, size_t source_position, size_t amount_of_bytes = 1){
                        return Bit::copy(data_destiny, (destiny_position << 3), data_source, (source_position << 3), (amount_of_bytes << 3));
                    }
                };

            /**
             * @brief Swap the endianess of the data.
             *
             * This method swaps the endianess of the given data. It converts the data from
             * little endian to big endian or vice versa.
             *
             * @param data The data to swap the endianess.
             */
            void static swapEndian(DATA_TYPE& data) {
                uint8_t* bytes = reinterpret_cast<uint8_t*>(&data);

                for (size_t counter = 0; counter < (Bitwise<DATA_TYPE>::size_in_bytes >> 1); ++counter) {
                    uint8_t buffer = bytes[counter];
                    bytes[counter] = bytes[Bitwise<DATA_TYPE>::size_in_bytes - counter - 1];
                    bytes[Bitwise<DATA_TYPE>::size_in_bytes - counter - 1] = buffer;
                }
            }

    };

}
