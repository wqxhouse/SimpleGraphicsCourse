#pragma once

namespace House
{
	// This data structure defines a simple house

	extern int nVerts;    // your vertex array needs to have this many entries

	// These are the x,y,z coordinates of the vertices of the triangles
	extern float vertices[];

		// These are the RGB colors corresponding to the vertices, in the same order
		extern float colors[];

			// The index data stores the connectivity of the triangles; 
			// index 0 refers to the first triangle defined above
			extern unsigned int indices[];
}