// Copyright (C) 2020, Petros Koutsolampros

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

#pragma once

#include <QAction>
#include <QMenuBar>
#include <iostream>

class MainWindow;

class MainWindowPlugin : public QObject {
    Q_OBJECT

  public:
    MainWindowPlugin() {}
    virtual bool createMenus(MainWindow *m_mainWindow) = 0;
    virtual ~MainWindowPlugin() {}

  protected:
    QMenu *getOrAddRootMenu(MainWindow *mainWindow, QString menuTitle);
    QMenu *getOrAddMenu(QMenu *parent, QString menuTitle);
};
