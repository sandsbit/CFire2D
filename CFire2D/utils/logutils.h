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

#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <boost/log/sources/severity_logger.hpp>

namespace c2d {

    enum LoggingSeverity {
        FATAL = 4,
        ERROR = 3,
        WARNING = 2,
        INFO = 1,
        DEBUG = 0
    };

    typedef boost::log::sources::severity_logger<LoggingSeverity> logger_t;

}

#endif //LOGUTILS_H
