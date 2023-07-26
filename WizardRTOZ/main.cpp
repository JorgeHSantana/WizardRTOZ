#include "./WizardRTOZ/WizardRTOZ.h"
#include "./UnitTest/UnitTest.h"

#include <inttypes.h>

UNIT_TEST_BEGIN
{
    // Testing swapEndian
    uint64_t mock_variable = 0xAABBCCDDEEFF;
    MemoryManager::Bitwise<uint64_t>::swapEndian(mock_variable);
    UNIT_TEST_COMPARE(mock_variable, 0xFFEEDDCCBBAA0000);
}
UNIT_TEST_END

UNIT_TEST_BEGIN
{
    uint64_t mock_variable = 0;
    System::Status status;

    // Testing Bit::set
    mock_variable = 0;
    status = MemoryManager::Bitwise<uint64_t>::Bit::set(mock_variable, 6, 3);;
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_COMPARE(mock_variable, 0b0000000111000000);

    // Testing Bit::set (error)
    status = MemoryManager::Bitwise<uint64_t>::Bit::set(mock_variable, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Bit::clear
    mock_variable = 0xFFFF;
    status = MemoryManager::Bitwise<uint64_t>::Bit::clear(mock_variable, 6, 3);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_COMPARE(mock_variable, 0b1111111000111111);

    // Testing Bit::clear (error)
    status = MemoryManager::Bitwise<uint64_t>::Bit::clear(mock_variable, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Bit::toggle
    mock_variable = 0xAAAA;
    status = MemoryManager::Bitwise<uint64_t>::Bit::toggle(mock_variable, 4, 8);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_COMPARE(mock_variable, 0xA55A);

    // Testing Bit::toggle (error)
    status = MemoryManager::Bitwise<uint64_t>::Bit::toggle(mock_variable, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Bit::copy
    mock_variable = 0xAAAAAAAAAAAAAAAA;
    status = MemoryManager::Bitwise<uint64_t>::Bit::copy(mock_variable, 8, 0x5555555555555555, 0, 8);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_COMPARE(mock_variable, 0xAAAAAAAAAAAA55AA);

    // Testing Bit::copy (error)
    status = MemoryManager::Bitwise<uint64_t>::Bit::copy(mock_variable, 100, 0xFFFFFFFFFFFFFFFF, 0, 8);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Bit::read
    status = MemoryManager::Bitwise<uint64_t>::Bit::read(mock_variable, 0b1100101, 5, 2);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_COMPARE(mock_variable, 0b11);

    // Testing Bit::read (error)
    status = MemoryManager::Bitwise<uint64_t>::Bit::read(mock_variable, 0xA55A, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);
}
UNIT_TEST_END

UNIT_TEST_BEGIN
{
    uint64_t mock_variable = 0;
    System::Status status;

    // Testing Byte::set
    mock_variable = 0xFF00000000FF;
    status = MemoryManager::Bitwise<uint64_t>::Byte::set(mock_variable, 1, 4);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_ASSERT(mock_variable == 0xFFFFFFFFFFFF);

    // Testing Byte::set (error)
    status = MemoryManager::Bitwise<uint64_t>::Byte::set(mock_variable, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Byte::clear
    mock_variable = 0xFFFFFFFFFFFF;
    status = MemoryManager::Bitwise<uint64_t>::Byte::clear(mock_variable, 1, 4);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_ASSERT(mock_variable == 0xFF00000000FF);

    // Testing Byte::clear (error)
    status = MemoryManager::Bitwise<uint64_t>::Byte::clear(mock_variable, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Byte::toggle
    mock_variable = 0xAAAAAA;
    status = MemoryManager::Bitwise<uint64_t>::Byte::toggle(mock_variable, 1, 1);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_ASSERT(mock_variable == 0xAA55AA);

    // Testing Byte::toggle (error)
    status = MemoryManager::Bitwise<uint64_t>::Byte::toggle(mock_variable, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Byte::copy
    mock_variable = 0xAAAAAAAAAAAAAAAA;
    status = MemoryManager::Bitwise<uint64_t>::Byte::copy(mock_variable, 2, 0x5555555555555555, 0, 4);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_ASSERT(mock_variable == 0xAAAA55555555AAAA);

    // Testing Byte::copy (error)
    status = MemoryManager::Bitwise<uint64_t>::Byte::copy(mock_variable, 100, 0xFFFFFFFFFFFFFFFF, 0, 8);
    UNIT_TEST_ASSERT(status == System::Status::length_error);

    // Testing Byte::read
    status = MemoryManager::Bitwise<uint64_t>::Byte::read(mock_variable, 0xA55A, 1, 2);
    UNIT_TEST_ASSERT(status == System::Status::ok);
    UNIT_TEST_COMPARE(mock_variable, 0xA5);

    // Testing Byte::read (error)
    status = MemoryManager::Bitwise<uint64_t>::Byte::read(mock_variable, 0xA55A, 100, 5);
    UNIT_TEST_ASSERT(status == System::Status::length_error);
}
UNIT_TEST_END

int main()
{
    UnitTest::run(false);
    return 0;
}
