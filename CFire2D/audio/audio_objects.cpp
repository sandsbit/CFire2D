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

#include "audio_objects.h"

#include "exceptions/unsupported_audio_format.h"
#include "loaders/wave.h"

c2d::audio::Sound::Sound(std::filesystem::path audioFIle, AudioFileType type, boost::log::sources::severity_logger<LoggingSeverity> &logger) noexcept(false) {
    switch (type) {
        case WAVE:
            this->audioFile = loadWav(audioFIle, logger);
            break;
        default:
            throw exceptions::unsupported_audio_format{};
    }
    this->position.x = 0;
    this->position.y = 0;
}

void c2d::audio::Sound::setPosition(double x, double y) {
    this->position.x = x;
    this->position.y = y;
}

void c2d::audio::Sound::setPosition(Point point) {
    this->position = point;
}

c2d::Point c2d::audio::Sound::getPosition() const {
    return position;
}

double c2d::audio::Sound::getPositionX() const {
    return position.x;
}

double c2d::audio::Sound::getPositionY() const {
    return position.y;
}

const c2d::audio::AudioFile c2d::audio::Sound::getAudioFile() const noexcept {
    return this->audioFile;
}
