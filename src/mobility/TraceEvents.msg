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

//
// Base trace event message
//
// Trace event messages are utilized by the node factory object to signal
// the various scripted events. This is the base message. See the derived
// messages for further details.
//
message TraceEvent
{
  fields:
    simtime_t time;
    int nodeID;  
}

//
// Create event signals a node creation. Used by NodeFactory.
//
message CreateEvent extends TraceEvent
{
  fields:
    double x;
    double y;
    double z;
    string type;
    string name;
}

//
// Destroy event signals a node destruction. Used by NodeFactory.
//
message DestroyEvent extends TraceEvent
{
}

