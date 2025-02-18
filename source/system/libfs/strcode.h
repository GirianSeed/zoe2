#include "common.h"

//!
//! @brief      3rd-generation string-hashing function.
//!             Originally from "ZONE OF THE ENDERS" (2001).
//!
//! Returns a 32-bit hash of a given string, or "string code."
//! This algorithm is used for referencing various resoureces in place of
//! direct string comparisons, helping to optimize memory accesses.
//!
//! @param      str         NULL-terminated string.
//!
//! @retval     0           if @p string is empty.
//! @retval     non-zero    32-bit hash of @p string.
//!
inline uint32 FS_StrCode( const char *str )
{
    uint32 c;
    sint32 n = 0;
    uint32 id = 0;

    while (( c = *str++ ))
    {
        id += ((id << (c & 0x0f)) | ((id >> 3) + (c << (n & 0x0f)) + c));
        n++;
    }
    return id;
}
