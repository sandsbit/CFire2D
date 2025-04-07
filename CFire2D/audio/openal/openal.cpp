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

#include <optional>
#include <string>
#include <thread>
#include <al.h>
#include <alc.h>
#include <boost/log/common.hpp>
#include <boost/exception/all.hpp>

#include "openal.h"

namespace c2d::audio {

    OpenALAudioSystem::OpenALAudioSystem(const boost::log::sources::severity_logger<LoggingSeverity> &logger) {
        this->logger = logger;
        this->device = alcOpenDevice(nullptr);
        if (this->device == nullptr)
            logMessageAndCreateError("Could not crete ALC device");
        this->checkAlcErrors(this->device);

        this->context = alcCreateContext(this->device, nullptr);
        this->checkAlcErrors(this->device);
        if (this->context == nullptr)
            logMessageAndCreateError("Could not crete ALC context");

        if (!alcMakeContextCurrent(this->context))
            logMessageAndCreateError("Could not make ALC context current");
        this->checkAlcErrors(this->device);

        std::thread clearingThread([this]() {
           this->cleanOpenALBuffersAndSourcesLoopIgnoreExceptions();
        });
    }

    OpenALAudioSystem::~OpenALAudioSystem() {
        quit = true;

        alcCloseDevice(this->device);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(this->context);
        try {
            checkAlcErrors(this->device);
        } catch (std::exception &e) {
            BOOST_LOG_SEV(this->logger, FATAL) << "Exception while closing OpenAL device: " << e.what();
            exit(1);
        }

        clearingThread.join();
    }

    std::vector<std::string> OpenALAudioSystem::listDevices() {
        const ALCchar *devices = alcGetString(this->device, ALC_DEVICE_SPECIFIER);
        checkAlcErrors(this->device);

        std::vector<std::string> devicesVector;
        const char *ptr = devices;
        do {
            devicesVector.push_back(std::string(ptr));
            ptr += devicesVector.back().size() + 1;
        } while (*(ptr + 1) != '\0');

        return devicesVector;
    }

    void OpenALAudioSystem::setDevice(const std::string &device) noexcept(false) {
        alcCloseDevice(this->device);
        if (this->device == nullptr)
            logMessageAndCreateError("Could not crete ALC device");
        checkAlcErrors(this->device);

        if (device != "")
            this->device = alcOpenDevice(device.c_str());
        else
            this->device = alcOpenDevice(nullptr);
        checkAlcErrors(this->device);
    }

    int OpenALAudioSystem::playSound(std::unique_ptr<Sound> sound) noexcept(false) {
        ALuint buffer;
        alGenBuffers(1, &buffer);
        checkAlErrors();

        ALenum format;
        AudioFile audioFile = sound->getAudioFile();
        if (audioFile.channels == 1 && audioFile.bitsPerSample == 8)
            format = AL_FORMAT_MONO8;
        else if (audioFile.channels == 1 && audioFile.bitsPerSample == 16)
            format = AL_FORMAT_MONO16;
        else if (audioFile.channels == 2 && audioFile.bitsPerSample == 8)
            format = AL_FORMAT_STEREO8;
        else if (audioFile.channels == 2 && audioFile.bitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
        else
            logMessageAndCreateError("Invalid WAVE file parameters: channels: " + std::to_string(audioFile.channels)
                + ", bits per sample: " + std::to_string(audioFile.bitsPerSample));

        alBufferData(buffer, format, audioFile.data, audioFile.size, audioFile.sampleRate);

        ALuint source;
        alGenSources(1, &source);
        checkAlErrors();
        alSourcef(source, AL_PITCH, 1);
        checkAlErrors();
        alSourcef(source, AL_GAIN, 1.0f);
        checkAlErrors();
        alSource3f(source, AL_POSITION, sound->getPositionX(), sound->getPositionY(), 0);
        checkAlErrors();
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        checkAlErrors();
        alSourcei(source, AL_LOOPING, AL_FALSE);
        checkAlErrors();
        alSourcei(source, AL_BUFFER, buffer);

        buffersAndSourcesMutex.lock();
        buffersAndSources.push_back({buffer, source});
        buffersAndSourcesMutex.unlock();

        alSourcePlay(source);
        checkAlErrors();

        return source;
    }

    int OpenALAudioSystem::playMusic(std::unique_ptr<Music> music) noexcept(false) {
    }

    void OpenALAudioSystem::stop(int id) noexcept(false) {
    }

    void OpenALAudioSystem::setListenerPosition(double x, double y) noexcept(false) {
    }

    std::optional<exceptions::openal_error> OpenALAudioSystem::logMessageAndCreateError(std::string message) {
        BOOST_LOG_SEV(this->logger, ERROR) << message;
        exceptions::openal_error openalError{};
        openalError << boost::error_info<struct tag_errmsg, std::string>{message};
        return std::make_optional(openalError);
    }

    inline const char * getAlErrorDescription(ALenum error) {
        switch (error) {
            case AL_INVALID_NAME:
                return "a bad name (ID) was passed to an OpenAL function";
            case AL_INVALID_ENUM:
                return "an invalid enum value was passed to an OpenAL function";
            case AL_INVALID_VALUE:
                return "an invalid value was passed to an OpenAL function";
            case AL_INVALID_OPERATION:
                return "the requested operation is not valid";
            case AL_OUT_OF_MEMORY:
                return "the requested operation resulted in OpenAL running out of memory";
            default:
                return "unknown OpenAL error";
        }
    }

    std::optional<exceptions::openal_error> OpenALAudioSystem::checkAlErrors() {
        ALenum error = alGetError();
        if (error == AL_NO_ERROR)
            return std::nullopt;
        return logMessageAndCreateError(getAlErrorDescription(error));
    }

    inline const char * getAlcErrorDescription(ALCenum error) {
        switch (error) {
            case ALC_INVALID_DEVICE:
                return "a bad device was passed to an OpenAL function";
            case ALC_INVALID_CONTEXT:
                return "a bad context was passed to an OpenAL function";
            case ALC_INVALID_ENUM:
                return "an unknown enum value was passed to an OpenAL function";
            case ALC_INVALID_VALUE:
                return "an invalid value was passed to an OpenAL function";
            case ALC_OUT_OF_MEMORY:
                return "the requested operation resulted in OpenAL running out of memory";
            default:
                return "unknown OpenAL ALC error";
        }
    }

    std::optional<exceptions::openal_error> OpenALAudioSystem::checkAlcErrors(ALCdevice *device) {
        ALenum error = alcGetError(device);
        if (error == ALC_NO_ERROR)
            return std::nullopt;
        return logMessageAndCreateError(getAlcErrorDescription(error));
    }

    void OpenALAudioSystem::cleanOpenALBuffersAndSourcesLoopIgnoreExceptions() {
        while (true) {
            if (quit)
                break;
            buffersAndSourcesMutex.lock();
            try {
                for (std::size_t i = 0; i < buffersAndSources.size(); ++i) {
                    auto bufferAndSource = buffersAndSources[i];
                    ALint state;
                    alGetSourcei(bufferAndSource.source, AL_SOURCE_STATE, &state);
                    if (state != AL_PLAYING and state != AL_PAUSED) {
                        alDeleteBuffers(1, &bufferAndSource.buffer);
                        alDeleteSources(1, &bufferAndSource.source);
                        checkAlErrors();
                    }
                }
            } catch (std::exception _) {}
            buffersAndSourcesMutex.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
}
