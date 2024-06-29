import numpy as np
from fractions import Fraction as fr

def solve():
    n=0
    m=0
    maxim = False
    status="optimal"
    ans={}
    with open("testcase.txt","r") as f:
        inp = f.read()

    inp=inp.split('\n')
    pi = 4
    A=[]
    while True:
        row = inp[pi].replace(' ','')
        if(row==''):
            break
        pi=pi+1
        row = list(map(fr,row.split(',')))
        m=m+1
        A.append(row)
    n=len(A[0])
    ns=0
    A=np.array(A)
    A = np.c_[A,-A]

    pi=pi+2
    b=[]
    while True:
        bi = inp[pi].replace(' ','')
        if(bi==''):
            break
        pi=pi+1
        b.append(fr(bi))
    b= np.array([b]).T


    pi=pi+2
    j=0
    while True:
        con = inp[pi]
        if(con==''):
            break
        if(con=='<='):
            A=np.c_[A,fr(1)*np.zeros((m,1),dtype=fr)]
            A[j,2*n+ns]=fr(1)
            ns=ns+1
        if(con=='>='):
            A=np.c_[A,fr(1)*np.zeros((m,1),dtype=fr)]
            A[j,2*n+ns]=fr(-1)
            ns=ns+1
        pi=pi+1
        j=j+1



    pi=pi+2
    c = list(map(fr,inp[pi].replace(' ','').split(',')))
    c = np.array([c])
    c = np.c_[c,-c,fr(1)*np.zeros((1,ns),dtype=fr)]
    if(inp[1]=='maximize'):
        c=-c
        maxim=True


    for i in range(m):
        if(b[i]<0):
            b[i]=-b[i]
            A[i]=-A[i]

    A=np.c_[A,fr(1)*np.identity(m,dtype=fr)]
    # x = np.r_[fr(1)*np.zeros((n,1),dtype=fr),b]
    B = list(range(2*n+ns,2*n+ns+m))
    r_0 = np.c_[-b.sum(),np.c_[fr(1)*np.zeros((1,2*n+ns),dtype=fr),fr(1)*np.ones((1,m),dtype=fr)] - np.matmul(fr(1)*np.ones((1,m),dtype=fr), A)]
    M = np.c_[b,A]

    # print(np.r_[r_0,M])
    # print("----------------")
    ans["initial_tableau"]=np.copy(A)
    while True:
        p_c=1
        while r_0[0,p_c]>=0:
            p_c=p_c+1
            if(p_c>2*n+ns+m):
                break
        if(p_c>2*n+ns+m):
            break
        p_r=0
        theta=fr(-1)
        for i in range(0,m):
            if(M[i,p_c]>0):
                if theta==-1 or theta*M[i,p_c] > M[i,0]:
                    theta = M[i,0]/M[i,p_c]
                    p_r=i
                elif theta * M[i,p_c] == M[i,0]:
                    if(B[i]<B[p_r]):
                        p_r=i
        B[p_r]=p_c-1
        M[p_r]=M[p_r]/M[p_r,p_c]
        M[p_r,p_c]=fr(0)
        M = M - np.matmul(np.array([M[:,p_c]]).T,np.array([M[p_r]]))
        M[:,p_c] = fr(1)*np.zeros(m,dtype=fr)
        M[p_r,p_c]=fr(1)
        r_0 = r_0 - r_0[0,p_c]*M[p_r]

    if(r_0[0,0]!=0):
        # print("infeasible")
        status = "infeasible"
    else:
        M = M[0:m,0:2*n+ns+1]
        tbd =[]
        for j in range(m):
            if(not M[j].any()):
                tbd.append(j)
            elif(B[j]>=2*n+ns):
                p_r = j
                for i in range(1,2*n+ns+1):
                    if(M[p_r,i]!=0):
                        p_c=i
                        break
                B[p_r]=p_c-1
                M[p_r]=M[p_r]/M[p_r,p_c]
                M[p_r,p_c]=fr(0)
                M = M - np.matmul(np.array([M[:,p_c]]).T,np.array([M[p_r]]))
                M[:,p_c] = fr(1)*np.zeros(m,dtype=fr)
                M[p_r,p_c] = fr(1)

        M=np.delete(M,tbd,axis=0)
        for j in tbd:
            B.pop(j)


        m=len(M)
        cB = fr(1)*np.zeros((1,m),dtype=fr)
        for i in range(m):
            cB[0,i]=c[0,B[i]]
        r_0 = np.c_[fr(0),c]-np.matmul(cB,M)


        while True:
            p_c=1
            while r_0[0,p_c]>=0:
                p_c=p_c+1
                if(p_c>2*n+ns):
                    break
            if(p_c>2*n+ns):
                break
            p_r=-1
            theta=fr(-1)
            for i in range(0,m):
                if(M[i,p_c]>0):
                    if theta==-1 or theta*M[i,p_c] > M[i,0]:
                        theta = M[i,0]/M[i,p_c]
                        p_r=i
                    elif theta * M[i,p_c] == M[i,0]:
                        if(B[i]<B[p_r]):
                            p_r=i
            if(p_r==-1):
                # print('unbounded')
                status='unbounded'
                break
            B[p_r]=p_c-1
            M[p_r]=M[p_r]/M[p_r,p_c]
            M[p_r,p_c]=fr(0)
            M = M - np.matmul(np.array([M[:,p_c]]).T,np.array([M[p_r]]))
            M[:,p_c] = fr(1)*np.zeros(m,dtype=fr)
            M[p_r,p_c]=fr(1)
            r_0 = r_0 - r_0[0,p_c]*M[p_r]

        # print(np.r_[r_0,M])
        # print("----------------")

        # print(r_0[0,0])
    ans["final_tableau"] = np.copy(A)
    ans["solution_status"] = status
    x = np.zeros(n,dtype=fr)
    for i in range(m):
        if(B[i]<n):
            x[B[i]]+=A[i,0]
        elif(B[i]<2*n):
            x[B[i]-n]-=A[i,0]
    ans["optimal_solution"] = x
    if(maxim): r_0[0,0] = -r_0[0,0]
    ans["optimal_value"] = r_0[0,0]
    return ans

ans= solve()

print(np.array(ans['initial_tableau'],dtype=str))
print(np.array(ans['final_tableau'],dtype=str))
print(np.array(ans['optimal_solution'],dtype=str))
print(float(ans['optimal_value']))
print(ans['solution_status'])