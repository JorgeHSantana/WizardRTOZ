#pragma once

#include "../System/Exception.h"
#include "./BitArray.h"

namespace MemoryManager{

    template <typename DATA_TYPE = uint8_t, size_t POOL_SIZE = 1>
    class MemoryPool{
    private:
        DATA_TYPE data[POOL_SIZE] {};
        BitArray<POOL_SIZE> in_use_tag;
        size_t free_space {POOL_SIZE};
        size_t allocation_position {0};
        template <typename POINTER_TYPE_CAST = DATA_TYPE*> POINTER_TYPE_CAST getDataBegin(void){
            return static_cast<POINTER_TYPE_CAST>(&this->data[0] < &this->data[POOL_SIZE - 1] ? &this->data[0] : &this->data[POOL_SIZE - 1]);
        }
        template <typename POINTER_TYPE_CAST = DATA_TYPE*> POINTER_TYPE_CAST getDataEnd(void){
            return static_cast<POINTER_TYPE_CAST>(&this->data[0] < &this->data[POOL_SIZE - 1] ? &this->data[POOL_SIZE - 1] : &this->data[0]);
        }
    public:
        class Reference{
            friend class MemoryPool<DATA_TYPE, POOL_SIZE>;
        private:
            MemoryPool& memory_pool;
            DATA_TYPE* data {nullptr};
            size_t size_allocation {0};
            inline Reference(MemoryPool& memory_pool) : memory_pool(memory_pool){}
        public:
            inline ~Reference(){
                this->memory_pool.free(*this);
            }
            inline size_t getTypeSize(void){
                return sizeof(DATA_TYPE);
            }
            inline size_t getDataSize(void){
                return this->size_allocation * this->getTypeSize();
            }
            inline size_t getLenght(void){
                return this->size_allocation;
            }
            inline void setData(DATA_TYPE data, size_t position = 0){
                System::Exceptions::length_error.test(position >= this->size_allocation, "Invalid position.");
                this->data[position] = data;
            }
            inline DATA_TYPE& getData(size_t position = 0){
                System::Exceptions::length_error.test(position >= this->size_allocation, "Invalid position.");
                return this->data[position];
            }
            inline DATA_TYPE* begin(void){
                return &this->data[0];
            }
            inline DATA_TYPE* end(void){
                return &this->data[this->size_allocation];
            }
            inline operator DATA_TYPE&() const {
                return *this->data;
            }
            inline DATA_TYPE& operator[] (size_t position){
                return this->getData(position);
            }
            inline Reference& operator=(DATA_TYPE data){
                this->setData(data);
                return *this;
            }
        };
        inline MemoryPool(void) {}
        Reference allocate(size_t size_allocation = 1){
            System::Exceptions::length_error.test(size_allocation == 0, "Invalid allocation size.");
            while(this->in_use_tag.get(this->allocation_position, size_allocation) != 0 && this->allocation_position < POOL_SIZE){
                this->allocation_position++;
            }
            System::Exceptions::out_of_range.test(this->allocation_position >= POOL_SIZE, "This memory pool is full!");
            DATA_TYPE* data = &this->data[this->allocation_position];
            this->in_use_tag.set(this->allocation_position, size_allocation);
            this->allocation_position += size_allocation;
            this->free_space -= size_allocation;
            Reference reference(*this);
            reference.size_allocation = size_allocation;
            reference.data = data;
            return reference;
        }
        void free(Reference& reference){
            System::Exceptions::out_of_range.test(
                (static_cast<void*>(reference.data) < this->getDataBegin<void*>() || static_cast<void*>(reference.data) > this->getDataEnd<void*>()),
                "This data pointer is not stored in this memory pool object."
            );
            size_t free_position = static_cast<DATA_TYPE*>(reference.data) - this->getDataBegin<DATA_TYPE*>();
            this->allocation_position = free_position < this->allocation_position ? free_position : this->allocation_position;
            this->free_space += reference.size_allocation;
            this->in_use_tag.write(free_position, 0, reference.size_allocation);
            reference.data = nullptr;
            reference.size_allocation = 0;
        }
        inline void setData(DATA_TYPE data, size_t position = 0){
            System::Exceptions::length_error.test(position >= this->size_allocation, "Invalid position.");
            this->data[position] = data;
        }
        inline DATA_TYPE& getData(size_t position = 0){
            System::Exceptions::length_error.test(position >= this->size_allocation, "Invalid position.");
            return this->data[position];
        }
        size_t getFreeSpace(void){
            return this->free_space;
        }
        inline size_t getTypeSize(void){
            return sizeof(DATA_TYPE);
        }
        inline size_t getDataSize(void){
            static const size_t data_size = POOL_SIZE * this->getTypeSize();
            return data_size;
        }
        inline size_t getLenght(void){
            return POOL_SIZE;
        }
        inline DATA_TYPE* begin(void){
            return &this->data[0];
        }
        inline DATA_TYPE* end(void){
            return &this->data[POOL_SIZE - 1];
        }
        inline operator DATA_TYPE&() const {
            return *this->data;
        }
        inline DATA_TYPE& operator[] (size_t position){
            return this->getData(position);
        }
        inline Reference& operator=(DATA_TYPE data){
            this->setData(data);
            return *this;
        }
    };
}
