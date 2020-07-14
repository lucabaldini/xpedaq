##

file = open ("trgmask.cfg" ,"w")

for col in range (2,336):
 for row in range (2,388):
  file.write ("%d %d\n" %(col,row))

file.close