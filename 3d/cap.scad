// Cap-nut style cylinder with a blind M16 x 2 internal thread, 12 mm deep.
// Requires: threads.scad (Dan Kirshner). Put it next to this file.
//   https://github.com/adriankirchner/threads-scad  (or search "OpenSCAD threads.scad")
//
// Adjust the parameters below to taste.

use <threads.scad>;

//////////////////// Parameters ////////////////////
thread_d        = 16;       // nominal ISO metric thread diameter (M16)
pitch           = 2;        // thread pitch (mm)
thread_depth    = 12;       // length of threaded portion (mm), i.e. blind depth

// Body (outer cylinder) dimensions — tweak as you like:
body_d          = 28;       // outer diameter of the cap (mm)
body_h          = 18;       // total height of the cap (mm)

// 3D-printing/practical clearances:
major_clearance = 0.10;     // expands the "major" diameter of the void (helps fitting)
lead_in_extra   = 1.5;      // extra unthreaded clearance at the bottom of the blind hole (mm)
tap_core_extra  = 0.5;      // extra through “air” below threads for a crisp flat bottom (mm)

// Resolution
$fn             = 150;

//////////////////// Build ////////////////////
difference() {
  // The solid cap body (a plain cylinder)
  cylinder(d = body_d, h = body_h);

  // The blind threaded hole: made by subtracting a "tap" (male external thread)
  // plus a short core to make a flat blind bottom.
  // We slightly oversize the thread "major" so the mating bolt will fit more easily.
  translate([0,0, body_h - (thread_depth + lead_in_extra)])  // place top of the hole where you want it
  union() {
    // Subtract the male thread to create the internal thread.
    metric_thread(
      diameter      = thread_d + major_clearance,  // slight oversize for real-world fit
      pitch         = pitch,
      length        = thread_depth,
      internal      = false,  // produce an external thread solid to subtract
      n_starts      = 1,
      thread_size   = "coarse",
      // quality controls:
      angle         = 60,
      taper         = 0
    );

    // Add a small core cylinder continuation so the blind bottom is clean & flat.
    // Use the typical external-thread minor diameter approximation:
    // minor_d_ext ≈ major_d - 2 * 0.61343 * P
    minor_d_ext = (thread_d + major_clearance) - 2 * 0.61343 * pitch;
    translate([0,0, -tap_core_extra])
      cylinder(d = minor_d_ext, h = tap_core_extra + 0.01);
  }
}
