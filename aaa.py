#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
from collections import deque
import copy as cp


# In[2]:


A = [[2,1,0],[3,4,5],[6,7,8]]
T = [[0,1,2],[3,4,5],[6,7,8]]
vis = []


# In[3]:


def where(arr, val):
    for i in range(len(arr)):
        for j in range(len(arr)):
            if arr[i][j]==val:
                return (i,j)
    return -1

def toNum(arr):
    num = 0
    k = 0
    for i in range(len(arr)):
        for j in range(len(arr)):
            num += arr[i][j]*(10**k)
            k += 1
    return num


# In[4]:


def Up(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    if i!= 0:
        arr[i][j], arr[i-1][j] = arr[i-1][j], arr[i][j]
    return arr

def Down(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    if i!= 0:
        arr[i][j], arr[i+1][j] = arr[i+1][j], arr[i][j]
    return arr

def Left(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    if j!= 0:
        arr[i][j], arr[i][j-1] = arr[i][j-1], arr[i][j]
    return arr

def Right(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    if i!= 0:
        arr[i][j], arr[i][j+1] = arr[i][j+1], arr[i][j]
    return arr


# In[5]:


def PossibleUp(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    return i!= 0
        #arr[i][j], arr[i-1][j] = arr[i-1][j], arr[i][j]
    #return arr

def PossibleDown(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    return i!= 0
        #arr[i][j], arr[i+1][j] = arr[i+1][j], arr[i][j]
    #return arr

def PossibleLeft(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    return j!= 0
        #arr[i][j], arr[i][j-1] = arr[i][j-1], arr[i][j]
    return arr

def PossibleRight(arr):
    loc = where(arr, 0)
    i, j = loc[0], loc[1]
    return i!= 0
        #arr[i][j], arr[i][j+1] = arr[i][j+1], arr[i][j]
    #return arr 


# In[9]:


def BFS(arr):
    queue = deque([arr])
    while True:
        if len(queue) == 0:
            return -1
        X = queue.popleft()
        vis.append(toNum(X))
        if X==T:
            return X
        print(np.array(X))
        if PossibleUp(X):
            Y = Up(X)
            if toNum(Y) not in vis:
                vis.append(toNum(Y))
                queue.append(Y)
                print('up', np.array(Y))
                
        if PossibleDown(X):
            Y = Down(X)
            if toNum(Y) not in vis:
                vis.append(toNum(Y))
                queue.append(Y)
                print('down', np.array(Y))
        
        if PossibleLeft(X):
            Y = Left(X)
            if toNum(Y) not in vis:
                vis.append(toNum(Y))
                queue.append(Y)
                print('lt', np.array(Y))
                
        if PossibleRight(X):
            Y = Right(X)
            if toNum(Y) not in vis:
                vis.append(toNum(Y))
                queue.append(Y)
                print('rt', np.array(Y))


# In[10]:



BFS(A)


# In[8]:


vis


# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:



