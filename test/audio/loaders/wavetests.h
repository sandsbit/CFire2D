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

#ifndef WAVETESTS_H
#define WAVETESTS_H

#include <boost/test/included/unit_test.hpp>

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/common.hpp>

#include "audio/loaders/audiofile.h"
#include "audio/loaders/wave.h"
#include "utils/logutils.h"

BOOST_AUTO_TEST_SUITE(WaveLoadingTest)


BOOST_AUTO_TEST_CASE(WaveHeaderCheck) {
    boost::log::sources::severity_logger<c2d::LoggingSeverity> logger{};
    BOOST_LOG_SEV(logger, c2d::DEBUG) << "Starting wave loading unit test";
    c2d::audio::AudioFile audioFile = c2d::audio::loadWav("audio/alarm.wav", logger);

    BOOST_CHECK_EQUAL(audioFile.channels, 1);
    BOOST_CHECK_EQUAL(audioFile.bitsPerSample, 16);
    BOOST_CHECK_EQUAL(audioFile.sampleRate, 48000);
}


BOOST_AUTO_TEST_SUITE_END()

#endif //WAVETESTS_H
