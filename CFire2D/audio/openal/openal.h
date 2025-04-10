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
#include <boost/log/sources/severity_logger.hpp>
#include <alc.h>

#include "audio/audio_abstracts.h"
#include "audio/exceptions/openal_error.h"
#include "utils/logutils.h"

namespace c2d::audio {
    class OpenALAudioSystem : public AudioSystem {

    public:

        explicit OpenALAudioSystem(logger_t &logger);
        ~OpenALAudioSystem() override;

        std::vector<std::string> listDevices() override;
        void setDevice(const std::string &device) noexcept(false) override;

        int playSound(std::unique_ptr<Sound> sound) noexcept(false) override;
        int playMusic(std::unique_ptr<Music> music) noexcept(false) override;

        void stop(int id) noexcept(false) override;
        void setListenerPosition(double x, double y) noexcept(false) override;

    private:

        boost::log::sources::severity_logger<LoggingSeverity> logger;

        ALCdevice *device;
        ALCcontext *context;

        struct BufferAndSource {
            ALuint buffer;
            ALuint source;
        };
        std::vector<BufferAndSource> buffersAndSources;
        std::mutex buffersAndSourcesMutex;
        std::thread clearingThread;
        std::atomic<bool> quit = false;

        std::optional<exceptions::openal_error> logMessageAndCreateError(std::string message);

        std::optional<exceptions::openal_error> checkAlErrors();
        std::optional<exceptions::openal_error> checkAlcErrors(ALCdevice *device);

        void cleanOpenALBuffersAndSourcesLoopIgnoreExceptions();
    };
}

#endif //CFIRE2D_LIBRARY_H