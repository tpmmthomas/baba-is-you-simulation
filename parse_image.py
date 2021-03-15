from PIL import Image
import sys

if len(sys.argv) < 3:
    print("Usage: find_image.py image_path save_location")
    exit(-1)

x = 20
y = 20
path = sys.argv[1]
savefile = sys.argv[2]

im = Image.open(path)
x_int = im.size[0]/x
y_int = im.size[1]/y
print(x,y)

px = im.load()
a,b,c = px[0,0] #bg color
ctr = 0
cur_character = ""
final_str="{"
t = 0
for i in range(0,x):
    for j in range(0,y):
        x_co = i*x_int+(x_int/2)
        y_co = j*y_int+(y_int/2)
        d,e,f = px[x_co,y_co]
        px[x_co,y_co] = (200,200,200)
        if abs(d-a)+abs(e-b)+abs(c-f) > 50:
            cur_character = cur_character+"1"
        else:
            cur_character = cur_character+"0"
        ctr = ctr + 1
        if ctr == 8:
            #px[x_co+1,y_co] = (255,140,0)
            #px[x_co-1,y_co] = (255,140,0)
            ctr = 0;
            temp = int(cur_character,2)
            temp = hex(temp)
            final_str = final_str + temp + ","
            cur_character = ""
            t = t + 1
final_str = final_str[:-1] + "}\n"

with open(savefile,"a") as f:
    f.write(final_str)
im.save("test.jpg")
print(t)    
        
        


