#pragma once
#include <stdint.h>

/**
 * @namespace System
 *
 * @brief Namespace containing system-related enums and classes.
 */
namespace System {
    /**
     * @enum Status
     *
     * @brief Enum class representing different status codes.
     */
    enum class Status : int8_t {
        logic_error,        ///< Logic error status
        domain_error,       ///< Domain error status
        invalid_argument,   ///< Invalid argument error status
        length_error,       ///< Length error status
        out_of_range,       ///< Out of range error status
        runtime_error,      ///< Runtime error status
        range_error,        ///< Range error status
        overflow_error,     ///< Overflow error status
        underflow_error,    ///< Underflow error status
        ok = 0,             ///< Ok status
    };
}
