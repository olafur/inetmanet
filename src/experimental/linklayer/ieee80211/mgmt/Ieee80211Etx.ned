//
// Copyright (C) 2008 Alfonso Ariza
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
//

package inet.experimental.linklayer.ieee80211.mgmt;

simple Ieee80211Etx
{
    parameters:
        double ETXInterval @unit(s)=default(1s);
		double ETTInterval @unit(s)=default(60s);
		double ETXMeasureInterval @unit(s)=default(10s);
		int ETXSize @unit(byte)=default(150byte);
		int ETTSize1 @unit(byte)=default(100byte);
		int ETTSize2 @unit(byte)=default(1300byte);
		double TimeToLive @unit(s)=default(5s);
		double powerWindow=default(1); // window size for measure the S/N and Power received
		double Index=default(0);
		volatile double jitter @unit(s) = uniform(0s,0.01s);
    gates:
        output  toMac;
        input fromMac;

}
