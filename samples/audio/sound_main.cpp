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

#include <filesystem>
#include <memory>
#include <chrono>
#include <thread>
#include <audio/audio_objects.h>
#include <audio/openal/openal.h>
#include <utils/logutils.h>

int main() {
    c2d::logger_t logger;
    auto alarm = std::make_shared<c2d::audio::Sound>(std::filesystem::path("audio/alarm.wav"),
        c2d::audio::WAVE, logger);

    c2d::audio::OpenALAudioSystem audioSystem(logger);
    audioSystem.playSound(alarm);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}