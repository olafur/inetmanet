// 
// Copyright (C) 2008 Olafur Ragnar Helgason, Laboratory for Communication 
//                    Networks, KTH, Stockholm
//           (C)      Kristjan Valur Jonsson, Reykjavik University, Reykjavik
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not see <http://www.gnu.org/licenses/>.
//

#include "TraceMobility.h"
#include <FWMath.h>


Define_Module(TraceMobility);

void TraceMobility::initialize(int stage)
{
    BasicMobility::initialize(stage);

    if (stage == 0)
    {
        hostPtr = getParentModule();
        //hostId = hostPtr->getId();
        _interpolate = par("interpolate").boolValue();
        if(_interpolate) {
            _updateInterval = par("updateInterval");
        }
        _updateEvent = new cMessage("updateEvent");
        // Whenever a move-update message and setdest are scheduled at the same time
        // we want to process the move-update first (setdest messages have priority 0)
        // message with smaller priority value is processed first
        _updateEvent->setSchedulingPriority(-1);
    } 
}

void TraceMobility::finish()
{
    cancelAndDelete(_updateEvent);
}

void TraceMobility::handleSelfMsg(cMessage *msg)
{
    if ( msg == _updateEvent )
    {
        move();
        updatePosition();
    }
    else 
    {
        BasicMobility::handleMessage(msg);
    }        
}

void TraceMobility::move()
{
    _step++;
    if( _step == _numSteps )
    {
        // Destination reached    
	      pos = _targetPos;
        scheduleNextWaypointEvent();
    } 
    else if( _step < _numSteps )
    {
	      // step forward
	      pos = _stepTarget;
	      _stepTarget += _stepSize;
        if ( _updateEvent->isScheduled() )
            cancelEvent( _updateEvent );
        if (_numSteps - _step == 1)
            scheduleAt( _timeAtTarget, _updateEvent );
        else
            scheduleAt( simTime() + _updateInterval, _updateEvent );
    }
    else
    {
        throw cRuntimeError("_step > _numSteps in TraceMobility");
    }
}

void TraceMobility::scheduleNextWaypointEvent()
{
    if ( !m_eventList.empty() )
    {
        WAYPOINT_EVENT waypoint = m_eventList.front();
        m_eventList.pop_front();
        setTarget( waypoint.x, waypoint.y, waypoint.timeAtDest );
    }  
}

void TraceMobility::setTarget(double x, double y, simtime_t timeAtTarget)
{
    if(simTime() > timeAtTarget)
        throw cRuntimeError("timeAtDest is in the past");
  
    _targetPos = Coord(x,y);
    _timeAtTarget = timeAtTarget;
    _step = 0;
    simtime_t nextEventTime;
    if (_targetPos == pos)
    {
        _numSteps = 1;
        _speed = 0;
        // No need to schedule mobility updates when we are stationary
        nextEventTime = _timeAtTarget;
    }
    else
    {
        if(simTime() == timeAtTarget) 
            throw cRuntimeError("simTime() == timeAtDest and destination is not same as current position");
    
        _speed = pos.distance(_targetPos)/(simTime() - _timeAtTarget);
        // calculate steps for interpolate or non-interpolate
        if(!_interpolate)
        {
            _numSteps = 1;
            nextEventTime = _timeAtTarget;
        }
        else  
        {
            // Get the number of steps needed to be covered.
            simtime_t travelTime = _timeAtTarget - simTime();
            _numSteps = static_cast<int>(ceil(travelTime.dbl()/_updateInterval));
            if(_numSteps > 1)
            {
                _stepSize = (_targetPos - pos)/_numSteps;
                _stepTarget = pos + _stepSize;
                nextEventTime = simTime() + _updateInterval;
            }
            else if (_numSteps == 1)
            {
                nextEventTime = _timeAtTarget;
            }
            else
            {
                throw cRuntimeError("Number of steps less than 0");
            }
        }
    }
    if ( _updateEvent->isScheduled() )
        cancelEvent( _updateEvent );
    scheduleAt( nextEventTime, _updateEvent );
}

void TraceMobility::initializeTrace( const waypointEventsList *eventList )
{
    Enter_Method_Silent();
  
    if ( eventList == NULL )
        return;
    
    // Copy the given event list to the internal list.
    m_eventList.resize(eventList->size());
    std::copy(eventList->begin(),eventList->end(),m_eventList.begin());
    scheduleNextWaypointEvent();
}
