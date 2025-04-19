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

#ifndef CFIRE2D_LIBRARY_H
#define CFIRE2D_LIBRARY_H

#include <optional>
#include <thread>
#include <mutex>
#include <atomic>
#include <alc.h>

#include "audio/audio_abstracts.h"
#include "audio/exceptions/openal_error.h"
#include "utils/logutils.h"

namespace c2d::audio {
    class OpenALAudioSystem : public AudioSystem {

    public:

        explicit OpenALAudioSystem(logger_t &logger);
        ~OpenALAudioSystem() override;

        [[nodiscard]] std::vector<std::string> listDevices() const override;
        void setDevice(const std::string &device) noexcept(false) override;

        int playSound(std::shared_ptr<Sound> sound) noexcept(false) override;
        int playMusic(std::shared_ptr<Music> music) noexcept(false) override;

        void stop(int id) noexcept(false) override;
        void setListenerPosition(double x, double y) noexcept(false) override;

        void setNumberOfBuffers(std::size_t n) noexcept override;
        void setSizeBufferSize(std::size_t size) noexcept override;

    private:

        mutable logger_t logger;

        ALCdevice *device;
        ALCcontext *context;

        std::size_t numberOfBuffers = 4;
        std::size_t bufferSize = 65536;  // 32 kb

        struct BufferAndSource {
            ALuint buffer;
            ALuint source;
        };
        std::vector<BufferAndSource> soundBuffersAndSources;
        std::mutex soundBuffersAndSourcesMutex;
        struct MusicBuffersAndSource {
            std::vector<ALuint> buffers;
            ALuint source;
            ALenum format;
            std::int32_t sampleRate;
            char *data;
            std::size_t cursor;
            std::size_t size;
        };
        std::vector<MusicBuffersAndSource> musicBuffersAndSources;
        std::mutex musicBuffersAndSourcesMutex;
        std::thread clearingThread;
        std::atomic<bool> quit = false;

        ALint setupSource(const Point &location) noexcept(false);

        [[nodiscard]] exceptions::openal_error logMessageAndCreateError(std::string message) const noexcept(false);

        void checkAlErrors() const noexcept(false);
        void checkAlcErrors(ALCdevice *device) const noexcept(false);

        void cleanUpSoundBuffers();
        void cleanUpMusicBuffers();
        void updateMusicBuffers();

        void cleanupAndBuffersUpdate();
    };
}

#endif //CFIRE2D_LIBRARY_H