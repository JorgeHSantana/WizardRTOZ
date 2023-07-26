#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../System/Exception.h"

namespace MemoryManager{

    template <typename DATA_TYPE>
    class StaticList{
    public:
        class Element{
            friend class StaticList;
        private:
            DATA_TYPE& data;
            uint8_t priority;
            Element* previous_item {nullptr};
            Element* next_item {nullptr};
            StaticList* storing_list {nullptr};
        public:
            inline Element(DATA_TYPE& data, uint8_t priority = 0) : data(data), priority(priority) {}
            inline DATA_TYPE& getData(void){
                return this->data;
            }
            inline void setData(DATA_TYPE& data){
                this->data = data;
            }
            inline void setData(const DATA_TYPE data){
                this->data = data;
            }
            inline operator DATA_TYPE&() const {
                return this->data;
            }
            inline Element& operator=(DATA_TYPE& data){
                this->data = data;
                return *this;
            }
        };
        StaticList(){};
        inline size_t append(Element& element, std::function<bool(const System::Exception&)> error_callback = [](const System::Exception&){ return false; })
        {
            System::Exceptions::domain_error.test(element.storing_list != nullptr, "The argument element is not contained in this list object.", error_callback);

            element.storing_list = this;
            this->lenght++;

            /*
             * First insertion
             */
            if (this->first_item == nullptr){
                this->first_item = &element;
                element.next_item = nullptr;
                element.previous_item = nullptr;
                return 0;
            }

            /*
             * Insertion in first element position.
             */
            else if (this->first_item->priority > element.priority){
                this->first_item->previous_item = &element;
                element.next_item = this->first_item;
                element.previous_item = nullptr;
                this->first_item = &element;
                return 1;
            }

            /*
             * Insertion in other positions.
             */
            else {
                size_t position = 2;
                Element* buffer = this->first_item;
                for (position = 2 ; buffer->next_item != nullptr && buffer->next_item->priority <= element.priority ; position++){
                    buffer = buffer->next_item;
                }
                element.next_item = buffer->next_item;
                element.previous_item = buffer;
                buffer->next_item = &element;
                if (element.next_item != nullptr){
                    element.next_item->previous_item = &element;
                }
                else{
                    this->last_item = &element;
                }
                return position;
            }
        }
        inline size_t remove(Element& element, std::function<bool(const System::Exception&)> error_callback = [](const System::Exception&){ return false; }){
            System::Exceptions::domain_error.test(element.storing_list != this, "The argument element must be contained in this list object.", error_callback);

            Element* previous_item = element.previous_item;

            if (element.previous_item != nullptr){
                element.previous_item->next_item = element.next_item;
            }

            if (element.next_item != nullptr){
                element.next_item->previous_item = element.previous_item;
            }

            element.next_item = nullptr;
            element.previous_item = nullptr;
            element.storing_list = nullptr;
            return --this->lenght;
        }
        inline size_t remove(size_t position, std::function<bool(const System::Exception&)> error_callback = [](const System::Exception&){ return false; }){
            System::Exceptions::out_of_range.test(position >= this->lenght, "Invalid position.", error_callback);
            Element* buffer = this->first_item;

            if (position == 0){
                this->first_item->storing_list = nullptr;
                this->first_item = buffer->next_item;
            }
            else
            {
                for (size_t counter = 0 ; counter < position - 1 ; counter ++){
                    buffer = buffer->next_item;
                }
                buffer->next_item->storing_list = nullptr;
                buffer->next_item = buffer->next_item->next_item;
            }

            return --this->lenght;
        }
        inline Element& get(size_t position, std::function<bool(const System::Exception&)> error_callback = [](const System::Exception&){ return false; }){
            System::Exceptions::out_of_range.test(position >= this->lenght, "Invalid position.", error_callback);

            Element* buffer = this->first_item;

            for (size_t counter = 0 ; counter < position ; counter ++){
                buffer = buffer->next_item;
            }

            return *buffer;
        }
        inline Element& operator[](size_t position){
            return this->get(position);
        }
        inline Element& begin(void){
            return this->first_item;
        }
        inline Element& end(void){
            return this->last_item;
        }
    private:
        Element* first_item {nullptr};
        Element* last_item {nullptr};
        size_t lenght {0};
    };

}
