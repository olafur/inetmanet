//
// Copyright (C) 2005 Georg Lutz, Institut fuer Telematik, University of Karlsruhe
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "RandomWPMobility.h"
#include <algorithm> // min
#include <math.h> // floor

Define_Module(RandomWPMobility);

void RandomWPMobility::initialize(int stage)
{
    BasicMobility::initialize(stage);
    if (stage == 1)
    {
        stationary = (par("speed").getType()=='L' || par("speed").getType()=='D') && (double)par("speed") == 0;
        if(!stationary)
        {
            updateInterval = par("updateInterval");
            moving = false;
            setTargetPosition();
            if(!stationary)
            {
                simtime_t nextUpdate = std::min(targetTime, simTime() + updateInterval);
                scheduleAt(nextUpdate, new cMessage("move"));
            }
        }
    }
}

void RandomWPMobility::setTargetPosition()
{
    if (moving)
    {
        simtime_t waitTime = par("waitTime");
        targetTime = simTime() + waitTime;
        moving = false;
    }
    else
    {
        targetPos = getRandomPosition();
        double speed = par("speed");
        if (speed != 0)
        {
            double distance = pos.distance(targetPos);
            simtime_t travelTime = distance / speed;
            targetTime = simTime() + travelTime;
            double numIntervals = floor(SIMTIME_DBL(travelTime) / updateInterval);
            if (numIntervals != 0) 
            {
                step = (targetPos - pos) / numIntervals;
            }
            else
            {
                step = targetPos - pos;
            }
            moving = true;
        }
        else
        {
            // Node has randomly come to a stop because speed is 0. Usually 
            // this is not desired. Perhaps we should report an error here.
            stationary = true;
        }
    }
}

void RandomWPMobility::handleSelfMsg(cMessage *msg)
{
    simtime_t now = simTime();
    if (moving) {
        if (now < targetTime) 
        {
            // Destination not yet reached - keep on moving
            pos += step;
            updatePosition();
            simtime_t nextUpdate = std::min(targetTime, now + updateInterval);
            scheduleAt(nextUpdate, msg);
        }
        else if (now == targetTime)
        {
            // Destination reached - pause
            pos = targetPos;
            updatePosition();
            setTargetPosition();
            scheduleAt(targetTime, msg);
        }
        else
        {
            delete msg;
            error("Error, mobility update scheduled after target was reached");
        }
    } else {
        // Pause finished - time to start moving
        if (targetTime != now)
        {
            delete msg;
            error("Received a mobility update while pausing.");
        }
        setTargetPosition();
        if(!stationary)
        {
            simtime_t nextUpdate = std::min(targetTime, now + updateInterval);
            scheduleAt(nextUpdate, msg);
        }
        else
        {
            delete msg;
        }
    }
}

void RandomWPMobility::fixIfHostGetsOutside()
{
    raiseErrorIfOutside();
}

