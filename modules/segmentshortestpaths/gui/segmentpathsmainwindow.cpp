// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "segmentpathsmainwindow.h"

#include "salalib/segmmodules/segmmetricshortestpath.h"
#include "salalib/segmmodules/segmtopologicalshortestpath.h"
#include "salalib/segmmodules/segmtulipshortestpath.h"

#include "depthmapX/graphdoc.h"
#include "depthmapX/mainwindowhelpers.h"

#include <QMenuBar>
#include <QMessageBox>

bool SegmentPathsMainWindow::createMenus(MainWindow *mainWindow) {
    QMenu *toolsMenu = MainWindowHelpers::getOrAddRootMenu(mainWindow, tr("&Tools"));
    QMenu *segmentMenu = MainWindowHelpers::getOrAddMenu(toolsMenu, tr("&Segment"));
    QMenu *shortestPathsMenu = MainWindowHelpers::getOrAddMenu(segmentMenu, tr("Shortest Paths"));

    QAction *angularPathAct = new QAction(tr("Angular shortest path"), mainWindow);
    angularPathAct->setStatusTip(tr("Create an angular shortest path"));
    connect(angularPathAct, &QAction::triggered, this,
            [this, mainWindow] { OnShortestPath(mainWindow, PathType::ANGULAR); });
    shortestPathsMenu->addAction(angularPathAct);

    QAction *metricPathAct = new QAction(tr("Metric shortest path"), mainWindow);
    metricPathAct->setStatusTip(tr("Create a metric shortest path"));
    connect(metricPathAct, &QAction::triggered, this,
            [this, mainWindow] { OnShortestPath(mainWindow, PathType::METRIC); });
    shortestPathsMenu->addAction(metricPathAct);

    QAction *topoPathAct = new QAction(tr("Topological shortest path"), mainWindow);
    topoPathAct->setStatusTip(tr("Create a topological shortest path"));
    connect(topoPathAct, &QAction::triggered, this,
            [this, mainWindow] { OnShortestPath(mainWindow, PathType::TOPOLOGICAL); });
    shortestPathsMenu->addAction(topoPathAct);

    return true;
}

void SegmentPathsMainWindow::OnShortestPath(MainWindow *mainWindow, PathType pathType) {
    QGraphDoc *graphDoc = mainWindow->activeMapDoc();
    if (graphDoc == nullptr)
        return;

    if (graphDoc->m_communicator) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please wait, another process is running"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    if (graphDoc->m_meta_graph->getDisplayedMapType() != ShapeMap::SEGMENTMAP) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please make sure the displayed map is a segment map"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (graphDoc->m_meta_graph->getDisplayedShapeGraph().getSelSet().size() != 2) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please select two segments to create a path between"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    graphDoc->m_communicator = new CMSCommunicator();
    switch (pathType) {
    case PathType::ANGULAR:
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new SegmentTulipShortestPath(graphDoc->m_meta_graph->getDisplayedShapeGraph())));
        break;
    case PathType::METRIC:
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new SegmentMetricShortestPath(graphDoc->m_meta_graph->getDisplayedShapeGraph())));
        break;
    case PathType::TOPOLOGICAL:
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new SegmentTopologicalShortestPath(graphDoc->m_meta_graph->getDisplayedShapeGraph())));
        break;
    }
    graphDoc->m_communicator->SetFunction(CMSCommunicator::FROMCONNECTOR);
    graphDoc->m_communicator->setSuccessUpdateFlags(QGraphDoc::NEW_DATA);
    graphDoc->m_communicator->setSuccessRedrawFlags(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_POINTS,
                                                    QGraphDoc::NEW_DATA);

    graphDoc->CreateWaitDialog(tr("Calculating shortest path..."));
    graphDoc->m_thread.render(graphDoc);
}
