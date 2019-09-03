##

file = open ("trgmask.cfg" ,"w")
for col in range (7):
 for row in range (388):
  file.write ("%d %d\n" %(col,row))
file.close