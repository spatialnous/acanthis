// Copyright (C) 2022, Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Interface meant

#pragma once

#include <QMatrix4x4>

class AGLObject {
  public:
    virtual void initializeGL(bool m_core) = 0;
    virtual void updateGL(bool m_core) = 0;
    virtual void cleanup() = 0;
    virtual void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                         const QMatrix4x4 &m_mModel) = 0;
    virtual ~AGLObject() {};
};