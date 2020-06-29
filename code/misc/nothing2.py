f=open("system.h","r")
flist = f.read().split("\n")
f.close()
flist = flist[25:49]
elist = []
for i in flist:
	x=i.split(",")
	for n in x:
		if len(n) > 0:
			elist.append(n.replace("\t","").replace("    ",""))
for i in elist:
	print("        case "+i+": return s<<\""+i.lower().replace("_"," ")+"\";")
