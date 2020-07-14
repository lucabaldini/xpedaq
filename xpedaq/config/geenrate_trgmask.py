##

file = open ("trgmask.cfg" ,"w")

for col in range (24,33):
 for row in range (246, 255):
  file.write ("%d %d\n" %(row,col))
  
for col in range (242,257):
 for row in range (20, 35):
  file.write ("%d %d\n" %(row,col))

for col in range (16,37):
 for row in range (240, 259):
  file.write ("%d %d\n" %(row,col))
  
file.close