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
#include <al.h>
#include <alc.h>
#include <boost/log/common.hpp>
#include <boost/exception/all.hpp>

#include "openal.h"

namespace c2d::audio {

    OpenALAudioSystem::OpenALAudioSystem(const boost::log::sources::severity_logger<LoggingSeverity> &logger) : AudioSystem(logger) {
        this->logger = logger;
        this->device = alcOpenDevice(nullptr);
        this->checkAlcErrors(this->device);
    }

    OpenALAudioSystem::~OpenALAudioSystem() {
        alcCloseDevice(this->device);
        try {
            checkAlcErrors(this->device);
        } catch (std::exception &e) {
            BOOST_LOG_SEV(this->logger, FATAL) << "Exception while closing OpenAL device: " << e;
            exit(1);
        }
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
        checkAlcErrors(this->device);

        if (device != "")
            this->device = alcOpenDevice(device.c_str());
        else
            this->device = alcOpenDevice(nullptr);
        checkAlcErrors(this->device);
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

}
