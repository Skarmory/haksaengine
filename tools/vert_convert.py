# Author: Alastair Paterson

# This tool is designed to take in blocks of data ripped from .fbx files and convert it into my own custom .mdl file format
# Data must first be extracted from a .fbx for each mesh and placed into a format looking something like this:
#
# vertices
# 0.0,1.0,2.0,3.0,4.0,...
#
# indices
# 0,1,2,3,4,5,6,...
#
# and so on.
#
# It currently works with the data copied directly from raw ascii .fbx file values from Blender's export, with a gap between each section.
# (This means some things such as amount of values per line currently matter)
# In future I will try to update this to just work directly on an .fbx file automagically

from math import sqrt
from copy import copy

filename = "verts.txt"
outfilename = "outverts.txt"

vertices = []
indices = []
uv_vals = []
uv_idx = []

class Vertex:
	position = ()
	normal   = ()
	uv       = ()

# Reads groups of 3 from the file as the vertex positions
def parse_verts(file):
	print("Parsing vertices")
	
	while 1:
		line = file.readline().strip()
		if not line:
			break
		
		splitline = line.strip()
		if splitline[-1] == ",":
			splitline = splitline[:-1]
			
		splitline = splitline.split(",")
		
		i = 0
		while i < len(splitline):
			v = Vertex()
			v.position = tuple(splitline[i:i+3])
			vertices.append(v)
			i+=3

# Reads in and reformats index data into the correct form
def parse_indices(file):
	print("Parsing indices")
	
	while 1:
		line = file.readline().strip()
		if not line:
			break
			
		splitline = line.strip()
		if splitline[-1] == ",":
			splitline = splitline[:-1]
			
		splitline = splitline.split(',')
		
		i = 0
		while i < len(splitline):			
			value = int(splitline[i])
			if value < 0:
				value = ((value * -1) - 1)
			indices.append(value)
			i+=1

# Reads in and formats normal data into the correct form
def parse_normals(file):
	print("Parsing normals")

	norm = {}
	norms = []
	
	# Read normal data in as tuples of 3
	while 1:
		line = file.readline().strip()
		if not line:
			break
		
		splitline = line
		if splitline[-1] == ",":
			splitline = splitline[:-1]
		splitline = splitline.split(',')
		
		i = 0
		while i < len(splitline):
			n = [float(splitline[i]), float(splitline[i+1]), float(splitline[i+2])]
			norms.append(n)
			i+=3
	
	# Normals not in per-vertex format
	# Add normals of the same vertex together and keep count of how many there are
	i = 0
	while i < len(indices):
		
		index = indices[i]
		normal = norms[i]
		
		if index not in norm:
			norm[index] = [1, normal]
		else:
			norm[index][0] += 1
			norm[index][1][0] += normal[0]
			norm[index][1][1] += normal[1]
			norm[index][1][2] += normal[2]
		
		i+=1
	
	# Go through and average the normals out now
	for k,v in norm.items():
		normal = (v[1][0] / v[0], v[1][1] / v[0], v[1][2] / v[0])
		
		# Make sure they're actually still normalised
		magnitude = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2])
		normal = (normal[0] / magnitude, normal[1] / magnitude, normal[2] / magnitude)
		
		vertices[k].normal = normal

# Adds UV values into a list of 2-tuples
def parse_uvs(file):
	print("Parsing UVs")
	
	while 1:
		line = file.readline().strip()
		if not line:
			break
		
		splitline = line.strip()
		if splitline[-1] == ",":
			splitline = splitline[:-1]
			
		splitline = splitline.split(",")
		
		i = 0
		while i < len(splitline):
			uv_vals.append(tuple([float(x) for x in splitline[i:i+2]]))
			i+=2

# Reads UV indices into a list
def parse_uvindex(file):
	print("Parsing UV indices")
	
	while 1:
		line = file.readline().strip()
		if not line:
			break
		
		splitline = line.strip()
		if splitline[-1] == ",":
			splitline = splitline[:-1]
			
		splitline = splitline.split(",")
		
		i = 0
		while i < len(splitline):
			uv_idx.append(int(splitline[i]))
			i+=1

# UVs are stored per face, but we need per vertex. Only way is to duplicate vertices.
# This method goes through and duplicates the vertices it needs to and updates the vertex indices
def reconcile_uvs():
	global vertices
	global indices
	global uv_vals
	global uv_idx

	added = 0
	cont = 0
	mod = 0
	
	i = 0
	while i < len(uv_idx):
		vidx = indices[i]
		uvidx = uv_idx[i]

		vert = vertices[vidx]
		uv = uv_vals[uvidx]
		
		if vert.uv == uv:
			i += 1
			cont += 1
			continue
		elif len(vert.uv) == 0:
			mod += 1
			vert.uv = uv
		else:
			added += 1
			vertices.append(copy(vert))
			vertices[-1].uv = uv
			indices[i] = len(vertices)-1
	
		i += 1

	print("Added %s vertices" % added)
	print("Modified %s vertices" % mod)
	print("Continued %s times" % cont)

# Final step, rewrite data into our .mdl format
def write_data(outfile):
	# Vertices
	print("Vertices %d {" % len(vertices), file=outfile)
	for vertex in vertices:
		print("\t{ %s, %s, %s }," % vertex.position, file=outfile)
	print("}", file=outfile)
	
	# Indices
	print("Indices %s {" % (len(indices)), file=outfile)	
	print("\t%s, " % indices[0], file=outfile, end="")
	i = 1
	while i < len(indices):
		if i % 32 == 0:
			print("", file=outfile)
			print("\t", file=outfile, end="")
		print("%s, " % indices[i], file=outfile, end="")
		i+=1
	
	print("", file=outfile)
	print("}", file=outfile)
	
	# Normals
	print("Normals %s {" % len(vertices), file=outfile)	
	for vertex in vertices:
		print("\t{ %.6f, %.6f, %.6f }," % vertex.normal, file=outfile)
	print("}", file=outfile)
	
	# UVs
	print("UVs %s {" % len(vertices), file=outfile)	
	for vertex in vertices:
		print("\t{ %.6f, %.6f }," % vertex.uv, file=outfile)
	print("}", file=outfile)
	
def main():
	file = open(filename)
	outfile = open(outfilename, "w")
	
	
	while 1:
		line = file.readline().strip()
		
		if not line:
			break
			
		if line == "vertices":
			parse_verts(file)
			print("%s vertices" % int(len(vertices)))
			
		if line == "indices":
			parse_indices(file)
			print("%s indices" % len(indices))
		
		if line == "normals":
			parse_normals(file)
			
		if line == "uvs":
			parse_uvs(file)
			print("%s uv values" % int(len(uv_vals)))
			
		if line == "uvindex":
			parse_uvindex(file)
			print("%s uv indices" % len(uv_idx))
	
	
	reconcile_uvs()
	
	write_data(outfile)

if __name__ == "__main__":
	main()