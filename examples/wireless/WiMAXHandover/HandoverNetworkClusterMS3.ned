package inet.examples.wireless.WiMAXHandover;
//
// Roland Siedlaczek
//


import inet.underTest.nodes.WiMAX.WiMAXMS;


network HandoverNetworkClusterMS3 extends HandoverNetworkClusterBS
{
    submodules:
        ms3: WiMAXMS {
            parameters:
                @display("p=300,300;i=device/cellphone_s;r=,,#707070");
        }
}
