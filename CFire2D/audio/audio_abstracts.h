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

#ifndef AUDIO_ABSTRACTS_H
#define AUDIO_ABSTRACTS_H

#include <memory>
#include <string>
#include <vector>
#include <boost/log/sources/severity_logger.hpp>

#include "utils/2dutils.h"
#include "utils/logutils.h"
#include "audio_objects.h"

namespace c2d::audio {
 class AudioSystem {
 public:
  virtual ~AudioSystem() = default;

  virtual std::vector<std::string> listDevices() = 0;
  virtual void setDevice(const std::string &device) noexcept(false) = 0;

  virtual int playSound(std::unique_ptr<Sound> sound) noexcept(false) = 0;
  virtual int playMusic(std::unique_ptr<Music> music) noexcept(false) = 0;
  virtual void stop(int id) noexcept(false) = 0;

  virtual void setListenerPosition(double x, double y) noexcept(false) = 0;
  void setListenerPosition(Point point) noexcept(false) {
   setListenerPosition(point.x, point.y);
  }
 };
}


#endif //AUDIO_ABSTRACTS_H
