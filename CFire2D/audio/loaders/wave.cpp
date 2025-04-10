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

#include "wave.h"

#include <filesystem>
#include <fstream>
#include <variant>
#include <cstring>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/common.hpp>
#include <boost/exception/all.hpp>

#include "audio/exceptions/invalid_audio_file_format.h"
#include "utils/memutils.h"
#include "utils/logutils.h"

static std::variant<std::string, c2d::audio::AudioFile> loadWavFileHeader(std::ifstream &file) {
    char buffer[4];
    c2d::audio::AudioFile audioFile;

    // the RIFF
    if (!file.read(buffer, 4))
        return "cound find read RIFF";
    if (std::strncmp(buffer, "RIFF", 4) != 0)
        return "file is not a valid WAVE file (header doesn't begin with RIFF)";


    // the size of the file
    if (!file.read(buffer, 4))
        return "could not read size of file";

    // the WAVE
    if (!file.read(buffer, 4))
        return "could not read WAVE";
    if (std::strncmp(buffer, "WAVE", 4) != 0)
        return "file is not a valid WAVE file (header doesn't contain WAVE)";

    // "fmt/0"
    if (!file.read(buffer, 4))
        return "could not read fmt/0";

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4))
        return "could not read the 16";

    // PCM should be 1?
    if (!file.read(buffer, 2))
        return "could not read PCM";

    // the number of channels
    if (!file.read(buffer, 2))
        return "could not read number of channels";
    audioFile.channels = c2d::memutils::convertMemoryToInt(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4))
        return "could not read sample rate";
    audioFile.sampleRate = c2d::memutils::convertMemoryToInt(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if (!file.read(buffer, 4))
        return "could not read (sampleRate * bitsPerSample * channels) / 8";

    // ?? dafaq
    if (!file.read(buffer, 2))
        return "could not read dafaq";

    // bitsPerSample
    if (!file.read(buffer, 2))
        return "could not read bits per sample";
    audioFile.bitsPerSample = c2d::memutils::convertMemoryToInt(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4))
        return "could not read data chunk header";
    if (std::strncmp(buffer, "data", 4) != 0)
        return "file is not a valid WAVE file (doesn't have 'data' tag)";

    // size of data
    if (!file.read(buffer, 4))
        return "could not read data size";
    audioFile.size = c2d::memutils::convertMemoryToInt(buffer, 4);

    /* cannot be at the end of file */
    if (file.eof())
        return "reached EOF on the file";
    if(file.fail())
        return "fail state set on the file";

    // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
    return audioFile;
}

static void processWaveLoadException(const std::string &message, c2d::logger_t &logger) {
    c2d::exceptions::invalid_audio_file_format exc{};
    BOOST_LOG_SEV(logger, c2d::ERROR) << message;
    exc << boost::error_info<struct tag_errmsg, std::string>{message};
    throw exc;
}

// https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/
// ReSharper disable once CppNotAllPathsReturnValue
c2d::audio::AudioFile c2d::audio::loadWav(std::filesystem::path file, logger_t &logger) {
    std::ifstream fileStream(file, std::ios::binary);
    if (!fileStream.is_open())
        processWaveLoadException("Could not open file " + file.string(), logger);
    auto audioFileOrErrorMsg = loadWavFileHeader(fileStream);

    try {
        auto audioFile = std::get<AudioFile>(audioFileOrErrorMsg);
        audioFile.data = new char[audioFile.size];
        fileStream.read(audioFile.data, audioFile.size);
        fileStream.close();
        return audioFile;
    } catch (const std::bad_variant_access &e) {
        processWaveLoadException(std::get<std::string>(audioFileOrErrorMsg), logger);
    }
}
