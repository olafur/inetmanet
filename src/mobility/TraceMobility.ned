//
// Trace mobility module.
//
// Used in conjunction with the NodeFactory object to implement the
// trace mobility model.
//
// Trace mobility uses externally generated traces or measurement 
// results to control a nodes movement. The module is initialized by the 
// factory at a certain location upon creation. It then supplies the module
// with its waypoint list and an optional destroy event. 
//
// @author  Olafur R. Helgason
// @author  Kristjan V. Jonsson
// @version 1.0 
//

package inet.mobility;

simple TraceMobility like BasicMobility
{
  parameters:    
      double x = default(-1);
      double y = default(-1);
      bool interpolate = default(false);  // Interpolate between waypoints or not
      double updateInterval @unit("s") = default(0.1s);
}

