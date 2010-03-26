//
// TODO Place comment here
//
package inet.examples.wireless.lan80211e;

import inet.networklayer.autorouting.FlatNetworkConfigurator;
import inet.world.ChannelControl;
import inet.nodes.wireless.WirelessAPWithEthe;
import inet.nodes.wireless.WirelessHoste;
import inet.nodes.inet.StandardHost;

network Ecko
{
    parameters:
        int numwirelessClients;
        @display("bgb=514,354");
    submodules:
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=375,24");
        }
        channelControl: ChannelControl {
            @display("p=101,19");
        }
        wirelessAPWithEthe: WirelessAPWithEthe {
            @display("p=337,119;i=device/accesspoint");
        }
        wirelessClient[numwirelessClients]: WirelessHoste {
            //@display("p=315,235");

        }

        server: StandardHost {
            @display("p=117,122");
        }
    connections allowunconnected:
        wirelessAPWithEthe.ethg++ <--> server.ethg++;
}
