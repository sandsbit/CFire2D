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

#ifndef AUDIO_OBJECTS_H
#define AUDIO_OBJECTS_H

#include <filesystem>
#include <boost/log/sources/severity_logger.hpp>

#include "utils/2dutils.h"
#include "utils/logutils.h"
#include "openal/loaders/audiofile.h"

namespace c2d::audio {

 enum AudioFileType {
  WAVE,
  OGG,
  MP3,
  FLAC
 };

 class Sound {
  public:
  Sound(std::filesystem::path audioFIle, AudioFileType type, boost::log::sources::severity_logger<LoggingSeverity> &logger) noexcept(false);

  void setPosition(double x, double y);
  void setPosition(Point point);

  [[nodiscard]] const AudioFile getAudioFile() const noexcept;

 private:
  Point position{};
  AudioFile audioFile;
 };

 class Music {
 public:
  Music(std::filesystem::path audioFIle, AudioFileType type) noexcept(false);
 };

}

#endif //AUDIO_OBJECTS_H
