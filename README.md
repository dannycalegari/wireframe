**wireframe** version 1.0; February 19, 2013
============================================

wireframe is a simple program for creating 3d surfaces, and outputting
them in a variety of formats. It is designed especially to create
.eps figures for inclusion in mathematics papers.

![red hand](https://raw.github.com/dannycalegari/wireframe/master/red_hand.png)


file formats
------------

The program reads files in two different formats, wire format, and
graph format. To read a file in wire format, run the command

    ./wireframe -w filename
	
To read a file in graph format, run the command

    ./wireframe -g filename

A file in wire format specifies precisely a triangular
mesh in space, in vertex-vertex format. The program reads this mesh
in, and displays it in a graphical window, where it can be further
manipulated. A file in graph format specifies precisely a polyhedral
graph in space, also in vertex-vertex format. The program reads this
graph in, and generates a triangular mesh surface obtained as the
boundary of a thickened neighborhood of the graph. Thus the advantage
of this format is that a complicated surface topology can be entered
with relative ease.


interactive commands
--------------------

After the program is launched, a graphical window opens and displays
the mesh. The following commands are available:

* [f] toggles between solid and wireframe
* [v] toggles between verbose and silent
* [i] toggles between shaded and isobar rendering
* [1/2] makes isobars sparser/denser
* [s] subdivides and smooths mesh
* [r] retriangulates to shorten edges
* [c] flows the surface by curvature (really, energy)
* [t] trims pyramids (3-valent vertices)
* [arrow keys] to rotate mesh
* [+/-] to make mesh bigger/smaller
* [e] for output to file in .eps format
* [w] for output to file in wire format
* [p] for output to file in povray format
* [q] to quit


manual
------

The manual (i.e. the file manual.pdf) contains a much more detailed
description of file formats.


copyright
---------

This program is released under the terms of the GNU GPL version 3.0.
See the attached license file.
