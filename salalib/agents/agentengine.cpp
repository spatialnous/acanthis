// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2000-2010, University College London, Alasdair Turner
// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2019, Petros Koutsolampros

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

#include "agentengine.h"
#include "agenthelpers.h"

// run one agent engine only

AgentEngine::AgentEngine() {
    m_timesteps = 5000;
    m_gatelayer = -1;
    m_record_trails = false;
    m_trail_count = MAX_TRAILS;
}

void AgentEngine::run(Communicator *comm, PointMap *pointmap) {
    for (auto &agentSet : agentSets) {
        if (agentSet.m_sel_type == AgentProgram::SEL_LOS_OCC) {
            pointmap->requireIsovistAnalysis();
        }
    }
    time_t atime = 0;
    if (comm) {
        qtimer(atime, 0);
        comm->CommPostMessage(Communicator::NUM_RECORDS, m_timesteps);
    }

    AttributeTable &table = pointmap->getAttributeTable();
    int displaycol = table.insertColumn(g_col_total_counts);

    int output_mode = Agent::OUTPUT_COUNTS;
    if (m_gatelayer != -1) {
        output_mode |= Agent::OUTPUT_GATE_COUNTS;
    }

    // remove any agent trails that are left from a previous run
    for (auto &agentSet : agentSets) {
        for (int k = 0; k < MAX_TRAILS; k++) {
            agentSet.g_trails[k].clear();
        }
    }

    int trail_num = -1;
    if (m_record_trails) {
        if (m_trail_count < 1) {
            m_trail_count = 1;
        }
        if (m_trail_count > MAX_TRAILS) {
            m_trail_count = MAX_TRAILS;
        }
        trail_num = 0;
    }

    // remove any agents that are left from a previous run
    for (auto &agentSet : agentSets) {
        agentSet.agents.clear();
    }

    for (int i = 0; i < m_timesteps; i++) {

        size_t j;
        for (auto &agentSet : agentSets) {
            int q = invcumpoisson(prandomr(), agentSet.m_release_rate);
            int length = agentSet.agents.size();
            int k;
            for (k = 0; k < q; k++) {
                agentSet.agents.push_back(Agent(&(agentSet), pointmap, output_mode));
            }
            for (k = 0; k < q; k++) {
                agentSet.init(length + k, trail_num);
                if (trail_num != -1) {
                    trail_num++;
                    // after trail count, stop recording:
                    if (trail_num == m_trail_count) {
                        trail_num = -1;
                    }
                }
            }
        }

        for (auto &agentSet : agentSets) {
            agentSet.move();
        }

        if (comm) {
            if (qtimer(atime, 500)) {
                if (comm->IsCancelled()) {
                    throw Communicator::CancelledException();
                }
                comm->CommPostMessage(Communicator::CURRENT_RECORD, i);
            }
        }
    }

    // output agent trails to file:
    if (m_record_trails) {
        // just dump in local file...
        std::ofstream trails("trails.cat");
        outputTrails(trails);
    }

    // actually, no, do this from the
    pointmap->overrideDisplayedAttribute(-2);
    pointmap->setDisplayedAttribute(displaycol);
}

void AgentEngine::outputTrails(std::ostream &trailsFile) {
    trailsFile << "CAT" << std::endl;
    for (auto &agentSet : agentSets) {
        for (int i = 0; i < m_trail_count; i++) {
            trailsFile << "Begin Polyline" << std::endl;
            for (size_t j = 0; j < agentSet.g_trails[i].size(); j++) {
                trailsFile << agentSet.g_trails[i][j].x << " " << agentSet.g_trails[i][j].y << std::endl;
            }
            trailsFile << "End Polyline" << std::endl;
        }
    }
}
