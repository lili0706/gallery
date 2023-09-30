from queue import Queue
def sort_arr( data ):
    n = len(data)
    for i in range(n-1):
        for j in range(n-i-1):
            if data[j][2] > data[j+1][2]:
                data[j], data[j+1] = data[j+1], data[j]
            elif data[j][2] == data[j+1][2] and data[j][0] > data[j+1][0]:
                data[j], data[j+1] = data[j+1], data[j]
    return data

def sort_pid( data ):
    n = len(data)
    for i in range(n-1):
        for j in range(n-i-1):
            if data[j][0] > data[j+1][0]:
                data[j], data[j+1] = data[j+1], data[j]
    return data

def sort_priority_q( data ):
    n = data.qsize()
    list_data = []
    for i in range(n):
        list_data.append(data.get())
    for i in range(n-1):
        for j in range(n-i-1):
            if list_data[j][3] > list_data[j+1][3]:
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]
    for i in list_data:
        data.put(i)
    return data

def sort_priority( data ):
    n = len(data)
    for i in range(n-1):
        for j in range(n-i-1):
            if data[j][2] > data[j+1][2]:
                data[j], data[j+1] = data[j+1], data[j]
            elif data[j][2] == data[j+1][2] and data[j][3] > data[j+1][3]:
                data[j], data[j+1] = data[j+1], data[j]
    return data

def sort_cpu( data ):
    n = len(data)
    for i in range(n-1):
        for j in range(n-i-1):
            if data[j][2] > data[j+1][2]:
                data[j], data[j+1] = data[j+1], data[j]
            elif data[j][2] == data[j+1][2] and data[j][1] > data[j+1][1]:
                data[j], data[j+1] = data[j+1], data[j]
    return data

def sort_cpu_q( data ):
    n = data.qsize()
    list_data = []
    for i in range(n):
        list_data.append(data.get())
    for i in range(n-1):
        for j in range(n-i-1):
            if list_data[j][4] > list_data[j+1][4]:
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]
            elif list_data[j][4] == list_data[j+1][4] and list_data[j][2] > list_data[j+1][2]:
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]
            elif list_data[j][4] == list_data[j+1][4] and list_data[j][2] == list_data[j+1][2] and list_data[j][0] > list_data[j+1][0]:
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]

    for i in list_data:
        data.put(i)
    return data

def ratio(time, data ):
    return ((time-data[2])+data[1])/data[1]

def sort_ratio_q( time, data ):
    n = data.qsize()
    list_data = []
    for i in range(n):
        list_data.append(data.get())
    for i in range(n-1):
        for j in range(n-i-1):
            if ratio(time, list_data[j]) < ratio(time, list_data[j+1]):
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]
            elif ratio(time, list_data[j]) == ratio(time, list_data[j+1]) and list_data[j][2] > list_data[j+1][2]:
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]
            elif ratio(time, list_data[j]) == ratio(time, list_data[j+1]) and list_data[j][2] == list_data[j+1][2] and list_data[j][0] > list_data[j+1][0]:
                list_data[j], list_data[j+1] = list_data[j+1], list_data[j]

    for i in list_data:
        data.put(i)
    return data

def FCFS( data ):
    Gantt = ''
    now = data[0]
    time = data[0][2]
    now[4] = now[1] 
    now.append( time - now[2] )
    for i in range(time):
        Gantt += '-'
    ready = Queue()
    i = 1
    while True:
        while ( i < len(data) and data[i][2] == time ): #arr後進ready queue
            ready.put(data[i])
            i += 1
        if now != [] and now[4] == 0 :#end
            now.append( time - now[2] ) #turnaround time
            now = []
        elif now != [] : #還沒end => cpuburst time--
            now[4] -= 1

        if now == [] and ready.qsize() > 0: #context switch
            now = ready.get()
            now[4] = now[1]
            now[4] -= 1
            now.append( time - now[2] ) #waiting time
        elif now == [] and ready.empty and i == len(data): break #沒人了
        time += 1
        if now == []:
            Gantt+='-'
        elif now[0] < 10 :
            Gantt+=str(now[0])
        else:
            Gantt+=chr( now[0] - 10 + ord('A'))
    return ( Gantt, data )

def RR( data, time_slice ):
    Gantt = ''
    now = data[0]
    now_time = 0
    time = data[0][2]
    now[4] = now[1]
    for i in range(time):
        Gantt += '-'
    ready = Queue()
    i = 1
    while True:
        while ( i < len(data) and data[i][2] == time ): #arr後進ready queue
            ready.put(data[i])
            i += 1
        if now != [] and now[4] == 0 :#end
            now.append( time - now[2] - now[1] )
            now.append( time - now[2] ) #turnaround time
            now = []
            now_time = 0
        elif now != [] and now_time == time_slice:#timeout
            now_time = 0
            ready.put(now)
            now = []
        elif now != [] : #還沒end => cpuburst time--
            now[4] -= 1
            now_time += 1 #現在執行的人執行了多久

        if now == [] and ready.qsize() > 0: #context switch
            now = ready.get()
            if now[4] == 0 : now[4] = now[1]
            now[4] -= 1
            now_time += 1
        elif now == [] and ready.empty and i == len(data): break #沒人了
        time += 1
        if now == []:
            Gantt+='-'
        elif now[0] < 10 :
            Gantt+=str(now[0])
        else:
            Gantt+=chr( now[0] - 10 + ord('A'))
    return ( Gantt, data )

def SJF( data):
    Gantt = ''
    data = sort_cpu(data)
    now = data[0]
    time = data[0][2]
    now[4] = now[1]
    for i in range(time):
        Gantt += '-'
    ready = Queue()
    i = 1
    while True:
        while ( i < len(data) and data[i][2] == time ): #arr後進ready queue
            data[i][4] = data[i][1]
            ready.put(data[i])
            i += 1
        ready = sort_cpu_q(ready)
        if now != [] and now[4] == 0 :#end
            now.append( time - now[2] - now[1] )
            now.append( time - now[2] ) #turnaround time
            now = []
        elif now != [] : #還沒end => cpuburst time--
            now[4] -= 1

        if now == [] and ready.qsize() > 0: #context switch
            now = ready.get()
            if now[4] == 0 : now[4] = now[1]
            now[4] -= 1
        elif now == [] and ready.empty and i == len(data): break #沒人了
        time += 1
        if now == []:
            Gantt+='-'
        elif now[0] < 10 :
            Gantt+=str(now[0])
        else:
            Gantt+=chr( now[0] - 10 + ord('A'))
    return ( Gantt, data )

def SRTF( data ):
    Gantt = ''
    data = sort_cpu(data)
    now = data[0]
    time = data[0][2]
    now[4] = now[1]
    for i in range(time):
        Gantt += '-'
    ready = Queue()
    i = 1
    while True:
        while ( i < len(data) and data[i][2] == time ): #arr後進ready queue
            data[i][4] = data[i][1]
            ready.put(data[i])
            i += 1
        ready = sort_cpu_q(ready)
        if now != [] and now[4] == 0 :#end
            now.append( time - now[2] - now[1] )
            now.append( time - now[2] ) #turnaround time
            now = []
        elif now != [] and ready.qsize() > 0 and ready.queue[0][4] < now[4]:
            ready.put(now)
            now = []
        elif now != [] : #還沒end => cpuburst time--
            now[4] -= 1

        if now == [] and ready.qsize() > 0: #context switch
            now = ready.get()
            if now[4] == 0 : now[4] = now[1]
            now[4] -= 1
        elif now == [] and ready.empty and i == len(data): break #沒人了
        time += 1
        if now == []:
            Gantt+='-'
        elif now[0] < 10 :
            Gantt+=str(now[0])
        else:
            Gantt+=chr( now[0] - 10 + ord('A'))
    return ( Gantt, data )

def HRRN( data ):
    Gantt = ''
    now = data[0]
    time = data[0][2]
    now[4] = now[1]
    for i in range(time):
        Gantt += '-'
    ready = Queue()
    i = 1
    while True:
        while ( i < len(data) and data[i][2] == time ): #arr後進ready queue
            ready.put(data[i])
            i += 1
        ready = sort_ratio_q(time, ready)
        if now != [] and now[4] == 0 :#end
            now.append( time - now[2] - now[1] )
            now.append( time - now[2] ) #turnaround time
            now = []
        elif now != [] : #還沒end => cpuburst time--
            now[4] -= 1

        if now == [] and ready.qsize() > 0: #context switch
            now = ready.get()
            if now[4] == 0 : now[4] = now[1]
            now[4] -= 1
        elif now == [] and ready.empty and i == len(data): break #沒人了
        time += 1
        if now == []:
            Gantt+='-'
        elif now[0] < 10 :
            Gantt+=str(now[0])
        else:
            Gantt+=chr( now[0] - 10 + ord('A'))
    return ( Gantt, data )

def PPRR( data, time_slice ):
    Gantt = ''
    data = sort_priority(data)
    now = data[0]
    now_time = 0
    time = data[0][2]
    now[4] = now[1]
    for i in range(time):
        Gantt += '-'
    ready = Queue()
    i = 1
    while True:
        while ( i < len(data) and data[i][2] == time ): #arr後進ready queue
            ready.put(data[i])
            i += 1
        ready = sort_priority_q( ready )
        if now != [] and now[4] == 0 :#end
            now.append( time - now[2] - now[1] ) #waiting
            now.append( time - now[2] ) #turnaround time
            now = []
            now_time = 0
        elif now != [] and ready.qsize() > 0 and ready.queue[0][3] < now[3] :#有priority更低的
            now_time = 0
            ready.put(now)
            now = []
        elif now != [] and ready.qsize() > 0 and ready.queue[0][3] == now[3] and now_time == time_slice:#timeout
            now_time = 0
            ready.put(now)
            now = []
        elif now != [] : #還沒end => cpuburst time--
            now[4] -= 1
            now_time += 1 #現在執行的人執行了多久

        if now == [] and ready.qsize() > 0: #context switch
            now = ready.get()
            if now[4] == 0 :now[4] = now[1] 
            now[4] -= 1
            now_time += 1
        elif now == [] and ready.empty and i == len(data): break #沒人了
        time += 1
        if now == []:
            Gantt+='-'
        elif now[0] < 10 :
            Gantt+=str(now[0])
        else:
            Gantt+=chr( now[0] - 10 + ord('A'))
    return ( Gantt, data )
def output(path, method, data): #gantt
    gantt = data[0]
    data = sort_pid( data[1] )
    if method == 1 : 
        command = 'FCFS'
    elif method ==2 :
        command = 'RR'
    elif method == 3 : 
        command = 'SJF'
    elif method == 4 : 
        command = 'SRTF'
    elif method == 5 : 
        command = 'HRRN'
    elif method == 6 : 
        command = 'PPRR'
    else : command = 'All'
    with open('out_'+path + '.txt','w') as out :
        if method == 6: print( 'Priority RR', file = out )
        else :print( command, file = out )
        if method == 7 :
            print( '=='+'%12s' %'FCFS'+'==', file = out )
            print( gantt[0], file = out )
            print( '=='+'%12s' %'RR'+'==', file = out )
            print( gantt[1], file = out )
            print( '=='+'%12s' %'SJF'+'==', file = out )
            print( gantt[2], file = out )
            print( '=='+'%12s' %'SRTF'+'==', file = out )
            print( gantt[3], file = out )
            print( '=='+'%12s' %'HRRN'+'==', file = out )
            print( gantt[4], file = out )
            print( '=='+'%12s' %'PPRR'+'==', file = out )
            print( gantt[5], file = out )
        else:
            print( '=='+'%12s' %command+'==', file = out )
            print( gantt[0], file = out )
        print( '===========================================================', file = out)
        print( '\nWaiting Time', file = out )
        if method == 7 :
            print( 'ID\t'+ 'FCFS\t'+'RR\t'+'SJF\t'+'SRTF\t'+'HRRN\t'+'PPRR', file = out )
            print( '===========================================================', file = out)
            for i in data:
                print( i[0], i[5], i[7], i[9], i[11], i[13], i[15],sep='\t', file = out )
        else:
            print( 'ID\t'+ command, file = out )
            print( '===========================================================', file = out)
            for i in data:
                print( i[0], i[5], sep='\t', file = out )
        print( '===========================================================', file = out)
        print( '\nTurnaround Time', file = out )
        if method == 7 :
            print( 'ID\t'+ 'FCFS\t'+'RR\t'+'SJF\t'+'SRTF\t'+'HRRN\t'+'PPRR', file = out )
            print( '===========================================================', file = out)
            for i in data:
                print( i[0], i[6], i[8], i[10], i[12], i[14], i[16], sep='\t', file = out )
        else:
            print( 'ID\t'+ command, file = out )
            print( '===========================================================', file = out)
            for i in data:
                print( i[0], i[6], sep='\t', file = out )
        print( '===========================================================\n', file = out)

        
path = input('Please Enter File Name:(0:end)')
data = []
while path != '0':
    with open( path + '.txt', 'r' ) as file : 
        line = file.readline().split()
        method = int(line[0])
        time_slice = int(line[1])
        file.readline()
        for line in file:
            if line.isspace() : continue
            data.append(list(map(int, line.split())))
        data = sort_arr( data )
        for i in data:
            i.append(i[1]) #剩餘時間
        gantt = []
        if method == 1 : 
            result = FCFS(data)
            gantt.append( result[0] )
        elif method == 2 : 
            result = RR( data, time_slice )
            gantt.append( result[0] )
        elif method == 3 : 
            result = SJF( data )
            gantt.append( result[0] )
        elif method == 4 : 
            result = SRTF( data )
            gantt.append( result[0] )
        elif method == 5 : 
            result = HRRN( data )
            gantt.append( result[0] )
        elif method == 6 : 
            result = PPRR( data, time_slice )
            gantt.append( result[0] )
        elif method == 7 : 
            result = FCFS(data)
            gantt.append( result[0] )
            result = sort_arr( result[1] )

            result = RR( result, time_slice )
            gantt.append( result[0] )
            result = sort_arr( result[1] )

            result = SJF( result )
            gantt.append( result[0] )
            result = sort_arr( result[1] )

            result = SRTF( result )
            gantt.append( result[0] )
            result = sort_arr( result[1] )

            result = HRRN( result )
            gantt.append( result[0] )
            result = sort_arr( result[1] )

            result = PPRR( result, time_slice )
            gantt.append( result[0] )
        output( path, method, (gantt, result[1]) )
        print('end')
        path = input('Please Enter File Name:(0:end)')
        data = []