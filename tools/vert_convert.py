
filename = "verts.txt"
outfilename = "outverts.txt"
linelength = 12

vertices = []
indices = []
normals = []

def parse_verts(file, outfile):
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
			vertices.append(splitline[i])
			i+=1
			
	# Write out verts
	print("Vertices %s {" % len(vertices), file=outfile)
	i = 0	
	while i < len(vertices):
		print("\t{ %s, %s, %s }," % (vertices[i], vertices[i+1], vertices[i+2]), file=outfile)
		i+=3
	print("}", file=outfile)

def parse_indices(file, outfile):
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
	
	# Write out indices
	print("Faces %s %s {" % (int(len(indices)/3), len(indices)), file=outfile)	
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

# Convert normals
def parse_normals(file, outfile):
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
	normals = list(range(len(norm)))
	for k,v in norm.items():
		normals[k] = (v[1][0] / v[0], v[1][1] / v[0], v[1][2] / v[0])
		
	# Write out normals
	print("Normals %s {" % len(normals), file=outfile)	
	for normal in normals:
		print("\t{ %.6f, %.6f, %.6f }," % (normal[0], normal[1], normal[2]), file=outfile)
	print("}", file=outfile)
	
	
def main():
	file = open(filename)
	outfile = open(outfilename, "w")
	
	
	while 1:
		line = file.readline().strip()
		
		if not line:
			break
			
		if line == "vertices":
			parse_verts(file, outfile)
			
		if line == "indices":
			parse_indices(file, outfile)
		
		if line == "normals":
			parse_normals(file, outfile)
		
	
	
	

if __name__ == "__main__":
	main()