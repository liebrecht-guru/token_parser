# simple parser

Basic functionality for token analyzer taken from [Fundamentals of Parsing](https://www.dreamincode.net/forums/topic/153718-fundamentals-of-parsing/).
Reworked for use with smart pointers.

Added grammer for following hierarchical data structure:
```
shape = {

type = "tetrahedron"

vertices = {

point = { x = "1" y = "0" z = "0" 
 }

point= { x = "0" y = "1" z = "0" }

point ={ x = "0" y = "0" z = "1" }

point = { x = "1" y = "1" z = "1" }

}

color = { r = "0xFF"g = "0x00" b = "0x80" alpha = "0x80"} a="25"

}
```

Resulting output:
```
(1, 0, shape, )
(2, 1, type, tetrahedron)
(3, 1, vertices, )
(4, 3, point, )
(5, 4, x, 1)
(6, 4, y, 0)
(7, 4, z, 0)
(8, 3, point, )
(9, 8, x, 0)
(10, 8, y, 1)
(11, 8, z, 0)
(12, 3, point, )
(13, 12, x, 0)
(14, 12, y, 0)
(15, 12, z, 1)
(16, 3, point, )
(17, 16, x, 1)
(18, 16, z, 1)
(19, 1, color, )
(20, 19, r, 0xFF)
(21, 19, g, 0x00)
(22, 19, b, 0x80)
(23, 19, alpha, 0x80)
(24, 1, a, 25)
```

Whitespaces are ignored. 

Usage: `parser input_file [output_file]`
If no output file is specified, parsing result is printed to standard output.
