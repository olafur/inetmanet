//
// Copyright (C) 2006 Andras Varga
// Copyright (C) 2009 Lukáš Hlůže   lukas@hluze.cz
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//


package inet.linklayer.ieee80211;

import inet.linklayer.ieee80211.mac.Ieee80211eMac;
import inet.linklayer.ieee80211.mgmt.Ieee80211MgmtAPSimplified;
import inet.linklayer.radio.Ieee80211Radio;


//
// This NIC implements an infrastructure mode 802.11 network interface card
// for a wireless Access Point (AP), but using a simplified mgmt module
// that does not support handovers.
//
module Ieee80211eNicAPSimplified
{
    parameters:
        @display("i=block/ifcard");
    gates:
        input uppergateIn; // to upper layers
        output uppergateOut; // from upper layers
        input radioIn @labels(AirFrame); // to receive AirFrames
    submodules:
        mgmt: Ieee80211MgmtAPSimplified {
            parameters:
                @display("p=96,69;");
         }
        mac: Ieee80211eMac {
            parameters:
                queueModule = "mgmt";                
                @display("p=96,155");
        }
        radio: Ieee80211Radio {
            parameters:
                @display("p=96,240");
        }
    connections:
        radioIn --> radio.radioIn;
        radio.uppergateIn <-- mac.lowergateOut;
        radio.uppergateOut --> mac.lowergateIn;

        mac.uppergateOut --> mgmt.macIn;
        mac.uppergateIn <-- mgmt.macOut;

        mgmt.uppergateOut --> uppergateOut;
        mgmt.uppergateIn <-- uppergateIn;
}
