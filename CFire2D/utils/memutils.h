/**
* Copyright (C) 2025 Nikita Serba. All rights reserved.
 *
 * This file is part of CFire 2D.
 *
 * CFire 2D is free software: you can redistribute it and/or modify it under the terms of
 * the GNU Lesser General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * CFire 2D is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License and GNU Lesser General
 * Public License along with CFire 2D. If not, see https://www.gnu.org/licenses/.
 */

#ifndef MEMUTILS_H
#define MEMUTILS_H

#include <cstdint>
#include <bit>

namespace c2d::memutils {

    // https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/
    inline std::int32_t convertMemoryToInt(char *buffer, std::size_t length) {
        std::int32_t a = 0;
        if(std::endian::native == std::endian::little)
            std::memcpy(&a, buffer, length);
        else
            for(std::size_t i = 0; i < length; ++i)
                reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
        return a;
    }
}

#endif //MEMUTILS_H
