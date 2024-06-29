import re
with open("Tests/raw.txt","r") as f:
    inp=f.readlines()


i=0
l=len(inp)
def spl(a):
    return a.split("x")
def repl(a):
    if(a[0]==''): return ("1",a[1])
    elif(a[0]=='-'): return ("-1",a[1])
    else: return a

while i<l:
    line = inp[i]
    oup=[]
    if(line[0:3]=="min" or line[0:3]=="MIN"):
        oup.append("minimize")
    else:
        oup.append("maximize")
    line=line[8:].replace(" ","")
    line = map(repl,map(spl,re.findall("-?+?[0-9]*x[0-9]*")))

    
        





