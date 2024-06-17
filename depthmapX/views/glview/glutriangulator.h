// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "genlib/p2dpoly.h"
#include <vector>

// This is required to silence the warnings for OpenGL deprecation in macOS
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION true
#endif

#ifdef __linux__
#include "GL/glu.h"
#elif _WIN32
#include "GL/glu.h"
#include "windows.h"
#else
#include "glu.h"
#endif

class GLUTriangulator {
  public:
    static std::vector<Point2f> triangulate(const std::vector<Point2f> &polygon);
};
