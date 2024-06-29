import numpy as np
from math import *
from fractions import Fraction as fr

file = 'input_ilp7.txt'

def dual_simplex(A,B):
    m,n = A.shape
    m-=1
    n-=1
    status = True
    while True:
        p_r=-1
        p_c=-1
        for i in range(0,m):
            if(A[i+1,0]<0):
                p_r = i+1
                break
        if(p_r==-1):
            break
        min=[-1]
        for i in range(0,n):
            if(A[p_r,i+1]<0):
                if(min[0]==-1 or list(-A[:,i+1]/A[p_r,i+1])<min):
                    min = list(-A[:,i+1]/A[p_r,i+1])
                    p_c = i+1
        if(p_c==-1):
            status = False
            break
        B[p_r-1] = p_c
        A[p_r] = A[p_r] / A[p_r,p_c]
        A[p_r,p_c]=fr(0)
        A = A - np.matmul(np.array([A[:,p_c]],dtype=fr).T,np.array([A[p_r]],dtype=fr))
        A[:,p_c] = fr(1) * np.zeros(m+1,dtype=fr)
        A[p_r,p_c]=fr(1)
    return (A,B,status)

def simplex_std(A,b,c):
    m = len(b)
    n = len(c[0])
    for i in range(0,m):
        if(b[i]<0):
            b[i]=-b[i]
            A[i]=-A[i]
    A = np.r_[np.c_[-b.sum(),np.c_[fr(1)*np.zeros((1,n),dtype=fr),fr(1)*np.ones((1,m),dtype=fr)] - np.matmul(fr(1)*np.ones((1,m),dtype=fr), np.c_[A,fr(1)*np.identity(m,dtype=fr)])],np.c_[b,A,fr(1)*np.identity(m,dtype=fr)]]
    B = list(range(n+1,n+m+1))
    bdd = True
    while True:
        p_r=-1
        p_c=-1
        for i in range(1,n+m+1):
            if(A[0,i]<0):
                p_c=i
                break
        if(p_c==-1):
            break
        min = -1
        for j in range(1,m+1):
            if(A[j,p_c]>0):
                if(min==-1 or A[j,0]/A[j,p_c]<min):
                    p_r=j
                    min = A[j,0]/A[j,p_c]
                elif(A[j,0]/A[j,p_c]==min and B[j-1]<B[p_r-1]):
                    p_r=j
        if(p_r==-1):
            bdd = False
            break
        B[p_r-1] = p_c
        A[p_r] = A[p_r] / A[p_r,p_c]
        A[p_r,p_c]=fr(0)
        A = A - np.matmul(np.array([A[:,p_c]],dtype=fr).T,np.array([A[p_r]],dtype=fr))
        A[:,p_c] = fr(1) * np.zeros(m+1,dtype=fr)
        A[p_r,p_c]=fr(1)
    if(A[0,0]!=0):
        return (A,B,'infeasible')
    A = A[:,0:n+1]
    tbd = []
    for j in range(1,m+1):
        if(not A[j].any()):
            tbd.append(j)
        elif(B[j-1]>n):
            p_r = j
            for i in range(1,n+1):
                if(A[p_r,i]!=0):
                    p_c = i
                    break
            B[p_r-1] = p_c
            A[p_r] = A[p_r] / A[p_r,p_c]
            A[p_r,p_c]=fr(0)
            A = A - np.matmul(np.array([A[:,p_c]],dtype=fr).T,np.array([A[p_r]],dtype=fr))
            A[:,p_c] = fr(1) * np.zeros(m+1,dtype=fr)
            A[p_r,p_c]=fr(1)
    A=np.delete(A,tbd,axis=0)
    for j in tbd:
        B.pop(j-1)
    m = len(B)
    cB = fr(1)*np.zeros((1,m),dtype=fr)
    for i in range(m):
        cB[0,i]=c[0,B[i]-1]
    A[0] = np.c_[fr(0),c]-np.matmul(cB,A[1:])
    bdd = True
    while True:
        p_r=-1
        p_c=-1
        for i in range(1,n+1):
            if(A[0,i]<0):
                p_c=i
                break
        if(p_c==-1):
            break
        min = -1
        for j in range(1,m+1):
            if(A[j,p_c]>0):
                if(min==-1 or A[j,0]/A[j,p_c]<min):
                    p_r=j
                    min = A[j,0]/A[j,p_c]
                elif(A[j,0]/A[j,p_c]==min and B[j-1]<B[p_r-1]):
                    p_r=j
        if(p_r==-1):
            bdd = False
            break
        B[p_r-1] = p_c
        A[p_r] = A[p_r] / A[p_r,p_c]
        A[p_r,p_c]=fr(0)
        A = A - np.matmul(np.array([A[:,p_c]],dtype=fr).T,np.array([A[p_r]],dtype=fr))
        A[:,p_c] = fr(1) * np.zeros(m+1,dtype=fr)
        A[p_r,p_c]=fr(1)
    if(bdd):
        return (A,B,'optimal')
    else:
        return (A,B,'unbounded')
    
def gomory_cut_algo():
    n=0
    m=0
    maxim = False
    status='optimal'
    ans={}
    with open(file,'r') as f:
        inp = f.read().split('\n')
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
    n_or=n
    ns=0
    A=np.array(A)

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
        con = inp[pi].replace(' ','')
        if(con==''):
            break
        if(con=='<='):
            A=np.c_[A,fr(1)*np.zeros((m,1),dtype=fr)]
            A[j,n+ns]=fr(1)
            ns=ns+1
        if(con=='>='):
            A=np.c_[A,fr(1)*np.zeros((m,1),dtype=fr)]
            A[j,n+ns]=fr(-1)
            ns=ns+1
        pi=pi+1
        j=j+1

    pi=pi+2
    c = list(map(fr,inp[pi].replace(' ','').split(',')))
    c = np.array([c])
    c = np.c_[c,fr(1)*np.zeros((1,ns),dtype=fr)]
    if(inp[1].replace(' ','')=='maximize'):
        c=-c
        maxim=True

    A,B,status = simplex_std(A,b,c)
    x = np.zeros(n_or,dtype=float)
    for i in range(len(B)):
        if(B[i]<=n_or):
            x[B[i]-1]=A[i+1,0]
    print('initial_solution: ',end='')
    print(*x,sep=', ')
    n_cuts=0
    n_or2=len(A[0])-1

    if(status == 'infeasible'):
        pass
    elif(status == 'unbounded'):
        pass
    else:
        m,n = A.shape
        n-=1
        m-=1
        a = -1
        for i in range(0,m+1):
            if(A[i,0] != floor(A[i,0])): # and B[i-1]<=n_or):
                if(a==-1):
                    a = i
                elif(A[i,0] - floor(A[i,0]) > A[a,0] - floor(A[a,0])):
                    a = i
        # for i in range(0,m+1):
        #     if(A[i,0] != floor(A[i,0])): #and B[i-1]<=n_or):
        #         a=i
        #         break
        while a!=-1:
            n_cuts+=1
            # print(A[0,0])
            A = np.c_[np.r_[A,np.array([list(map(floor,A[a]))],dtype=fr) - A[a,:]],fr(0)*np.zeros((m+2,1),dtype=fr)]
            A[m+1,n+1]=fr(1)
            B.append(n+1)
            m+=1
            n+=1
            A,B,fes = dual_simplex(A,B)
            if(not fes):
                status ='infeasible'
                break
            tbdA = []
            tbdB = []
            for i in range(m):
                if(B[i]>n_or2):
                    tbdB.append(i+1)
                    tbdA.append(B[i])
                    m-=1
                    n-=1
            A = np.delete(A,tbdA,axis=1)
            A = np.delete(A,tbdB,axis=0)
            tbdB = sorted(tbdB,reverse=True)
            for a in tbdB:
                B.pop(a-1)
            a = -1
            # for i in range(0,m+1):
            #     if(A[i,0] != floor(A[i,0])): #and B[i-1]<=n_or):
            #         if(a==-1):
            #             a = i
            #         elif(A[i,0] - floor(A[i,0]) > A[a,0] - floor(A[a,0])):
            #             a = i
            for i in range(0,m+1):
                if(A[i,0] != floor(A[i,0])): #and B[i-1]<=n_or):
                    a=i
                    break
                    
    x = np.zeros(n_or,dtype=int)
    for i in range(len(B)):
        if(B[i]<=n_or):
            x[B[i]-1]=A[i+1,0]
    print('final_solution: ',end='')
    print(*x,sep=', ')
    print('solution_status:',status)
    print('number_of_cuts:',n_cuts)
    if(not maxim):
        A[0,0]*=-1
    print('optimal_value:',float(A[0,0]))
gomory_cut_algo()