from PIL import Image
import sys

if len(sys.argv) < 2:
    print("Usage: color_find.py color(RGB)")
    exit(-1)


color = sys.argv[1]

R = color[0:2]
G = color[2:4]
B = color[4:6]

print(R,G,B)

R = int(R,16)
G = int(G,16)
B = int(B,16)

print(R,G,B)

R = int(round(R/255*32))
G = int(round(G/255*64))
B = int(round(B/255*32))

print(R,G,B)

R = "{0:b}".format(R).zfill(5)
G = "{0:b}".format(G).zfill(6)
B = "{0:b}".format(B).zfill(5)

final = R+G+B
final = hex(int(final,2))
print(final)



        
        


