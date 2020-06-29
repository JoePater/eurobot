file = open("arena.cpp","r+")

fileString = file.read()

fileList = fileString.split("\n")

newList = []

for i in range(16,38):
	print(fileList[i])
	x = fileList[i]
	n = x.find("Position")
	s = x.find(",",n)
	if(s != -1):
		e = x.find(")",s)
		a = int(x[s+1:e])
		x = x[0:s+1]+str(3000-a)+x[e:]
	n = x.find("Position",s)
	s = x.find(",",n)
	if(s != -1):
		e = x.find(")",s)
		a = int(x[s+1:e])
		newStr = x[0:s+1]+str(3000-a)+x[e:]
		newList.append(newStr)
total = ""
for i in newList:
	total += i+"\n"

print(total)
