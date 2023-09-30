import threading as th
import time
from queue import Queue
from datetime import datetime
from multiprocessing import Process, Pool
import os

def bubblesort(data):
    n = len(data)
    for i in range(n-1):
        for j in range(n-i-1):
            if data[j] > data[j+1]:
                data[j], data[j+1] = data[j+1], data[j]
    return data

def mission1(data,path):
    start = time.time()
    data = bubblesort(data)
    end = time.time()
    output(path,data,end-start,1)
########方法2#############################
def mergesort2(data):
    while len(data) > 1:
        i =0
        temp = []
        for i in range(0,len(data)-1,2):
            temp.append(merge2(data[i],data[i+1]))
            if i == len(data)-3 :
                temp.append(data[i+2])
        data = temp

    return data[0]
    
def merge2(left,right):
    i = j = k = 0
    ans = []
    while i < len(left) and j < len(right):
        if left[i] < right[j] :
            ans.append(left[i])
            i = i+1
        else:
            ans.append(right[j])
            j = j+1

    while i < len(left) :
        ans.append(left[i])
        i = i+1
        
    while j < len(right):
        ans.append(right[j])
        j = j+1
    return ans

def combine(data): #先bubble再merge
    sorted = []
    for i in data:
        sorted.append(bubblesort((i)))
    data = mergesort2(sorted)
    return data
    

def mission2(data,path):
    start = time.time()
    pool = Pool(1)
    data = pool.map(combine, [data])[0]
    end = time.time()
    output(path,data,end-start,2)
    
#########方法3############################
def mergesort3(data):
    temp =[]
    i =0
    two_data = []
    cpus = os.cpu_count()
    while len(data) > 1:
        i =0
        pool = Pool(cpus)
        temp = []
        for i in range(0,len(data)-1,2):
            two_data=[(data[i],data[i+1])]
            temp.append(pool.map(merge3,two_data)[0])
            if i == len(data)-3 :
                temp.append(data[i+2])
        data = temp

    return data[0]

def merge3(data):
    left = data[0]
    right = data[1]
    i = j = k = 0
    ans = []
    pid = os.getpid()
    print('pid',pid)
    while i < len(left) and j < len(right):
        if left[i] < right[j] :
            ans.append(left[i])
            i = i+1
        else:
            ans.append(right[j])
            j = j+1

    while i < len(left) :
        ans.append(left[i])
        i = i+1
        
    while j < len(right):
        ans.append(right[j])
        j = j+1

    return ans

def mission3(data,path):
    cpus = os.cpu_count()
    print(len(data))
    start = time.time()
    pool = Pool(cpus)
    data = pool.map(bubblesort, data)
    data = mergesort3(data)
    end = time.time()
    output( path, data,end-start,3)
#########方法4#############################
def mergesort4(data):
    t_list =[]
    q = Queue()
    temp =[]
    j = 0
    i =0
    k =0
    while len(data) > 1:
        t_list.clear()
        temp.clear()
        k =0
        j =0
        for i in range(0,len(data)-1,2):
            t = th.Thread(target=merge4, args=(data[i],data[i+1],q))
            t.start()
            t_list.append(t)
        for k in t_list:
            k.join()
        if int(len(data)/2) != int((len(data)+1)/2) :
            temp.append(data[len(data)-1])
            data.clear()
            data = temp.copy()
        else:
            data.clear()
        for j in range(q.qsize()):
            data.insert(0,q.get())
    return data[0]

def merge4(left,right,temp):
    global lock
    i = j = k = 0
    ls = []

    while i < len(left) and j < len(right):
        if left[i] < right[j] :
            ls.append(left[i])
            i = i+1
        else:
            ls.append(right[j])
            j = j+1

    while i < len(left) :
        ls.append(left[i])
        i = i+1
        
    while j < len(right):
        ls.append(right[j])
        j = j+1

    temp.put(ls)

def mission4(data,path,k):
    
    start = time.time()
    t_list = []
    for i in range(int(k)):
        t_list.append(th.Thread(target=bubblesort, args=(data[i],)))
        t_list[i].start()

    for j in t_list:
        j.join()
    data = mergesort4(data)
    end = time.time()
    output(path,data,end-start,4)
######################################
def output(path, data,cputime,method):
    with open(path+'_output'+str(method)+'.txt','w') as out :
        print('Sort : ', file = out)
        for i in data:
            print( i, file = out)
        print( 'CPU Time : ', cputime, file = out )
        print( 'Output Time : ', datetime.now(), '+08:00', file = out)

if __name__ == '__main__':
    path = input('請輸入檔案名稱:(0:quit)\n')
    temp =[]
    data = []
    inputdata = []
    while(path !="0"):
        data.clear()
        inputdata.clear()
        with open(path +'.txt', 'r') as file:
            inputdata = list(map(int, file.read().split()))

        method = input('請輸入方法編號:(方法1,方法2,方法3,方法4)\n')
        if (method == "1" ):
            data = inputdata.copy()
            mission1(data,path)
            path = input('請輸入檔案名稱:(0:quit)\n')
            continue
        k = input('請輸入要切成幾份:\n')
        while ( not k.isdigit() ):
            print( '請輸入數字' )
            k = input('請輸入要切成幾份:\n')
        
        chunksize = int(len(inputdata)/int(k))
        for j in range( 0, int(k)):
            if j != int(k)-1 : 
                data.append(inputdata[j*chunksize:chunksize*(j+1)])
            else :
                data.append(inputdata[j*chunksize:])
        
        if (method == "2" ):
            mission2(data,path)
        elif(method == "3"):
            mission3(data,path)
        elif(method == "4"):
            mission4(data,path,k)

        
        path = input('請輸入檔案名稱:(0:quit)\n')