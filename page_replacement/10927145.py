import copy

def valid(data, frame):
    for i in frame:
        if i == data:
            return True
    return False

def FIFO(data, size):
    frame = []
    fault, replaces = 0, 0
    for index, page in enumerate(data):
        if not valid(page, frame):
            fault += 1   # page fault
            if len(frame) < size:
                frame.insert(0, copy.deepcopy(page))
            else:
                replaces += 1
                frame.pop()
                frame.insert(0, copy.deepcopy(page))

            data[index].append(copy.deepcopy(frame))
            data[index].append('F')
        else:
            data[index].append(copy.deepcopy(frame))
    output(path, 1, (copy.deepcopy(data), fault, replaces), size)

def LRU(data, size):
    frame = []
    fault, replaces = 0, 0
    for index, page in enumerate(data):
        if not valid(page, frame):
            fault += 1   # page fault
            if len(frame) < size:
                frame.insert(0, copy.deepcopy(page))
            else:
                replaces += 1
                frame.pop()
                frame.insert(0, copy.deepcopy(page))

            data[index].append(copy.deepcopy(frame))
            data[index].append('F')
        else:
            for i in range(len(frame)):  # 将被ref的移到最前
                if frame[i] == page:
                    frame.insert(0, copy.deepcopy(page))
                    frame.pop(i+1)

            data[index].append(copy.deepcopy(frame))
    output(path, 2, (copy.deepcopy(data), fault, replaces), size)

def LFU_FIFO(data, size):
    frame = []
    counter_list = []
    fault, replaces = 0, 0
    for index, page in enumerate(data):
        if not valid(page, frame):
            fault += 1   # page fault
            if len(frame) < size:
                frame.insert(0, copy.deepcopy(page))
                counter_list.append( [0] )
            else:
                replaces += 1
                min_value = min(counter_list)
                min_index = None
                for i in range(len(counter_list)):
                    if counter_list[i] == min_value:
                        min_index = i
                frame.pop(min_index)
                counter_list.pop(min_index)
                frame.insert(0, copy.deepcopy(page))
                counter_list.insert(0, [0])

            data[index].append(copy.deepcopy(frame))
            data[index].append('F')
        else:
            for i in range( len(frame) ):
                if frame[i] == page:
                    counter_list[i][0] += 1
            data[index].append(copy.deepcopy(frame))
    output(path, 3, (copy.deepcopy(data), fault, replaces), size)

def MFU_FIFO(data, size):
    frame = []
    counter_list = []
    fault, replaces = 0, 0
    for index, page in enumerate(data):
        if not valid(page, frame):
            fault += 1   # page fault
            if len(frame) < size:
                frame.insert(0, copy.deepcopy(page))
                counter_list.append( [0] )
            else:
                replaces += 1
                max_value = max(counter_list)
                max_index = None
                for i in range(len(counter_list)):
                    if counter_list[i] == max_value:
                        max_index = i
                frame.pop(max_index)
                counter_list.pop(max_index)
                frame.insert(0, copy.deepcopy(page))
                counter_list.insert(0, [0])

            data[index].append(copy.deepcopy(frame))
            data[index].append('F')
        else:
            for i in range( len(frame) ):
                if frame[i] == page:
                    counter_list[i][0] += 1
            data[index].append(copy.deepcopy(frame))
    output(path, 4, (copy.deepcopy(data), fault, replaces), size)

def LFU_LRU(data, size):
    frame = []
    counter_list = []
    fault, replaces = 0, 0
    for index, page in enumerate(data):
        if not valid(page, frame):
            fault += 1   # page fault
            if len(frame) < size:
                frame.insert(0, copy.deepcopy(page))
                counter_list.append( [0] )
            else:
                replaces += 1
                min_value = min(counter_list)
                min_index = None
                for i in range(len(counter_list)):
                    if counter_list[i] == min_value:
                        min_index = i
                frame.pop(min_index)
                counter_list.pop(min_index)
                frame.insert(0, copy.deepcopy(page))
                counter_list.insert(0, [0])

            data[index].append(copy.deepcopy(frame))
            data[index].append('F')
        else:
            for i in range(len(frame)):  # 将被ref的移到最前
                if frame[i] == page:
                    frame.insert(0, copy.deepcopy(page))
                    frame.pop(i+1)
                    counter_list.insert(0, copy.deepcopy(counter_list[i]))
                    counter_list.pop(i+1)
                    counter_list[0][0] += 1

            data[index].append(copy.deepcopy(frame))
    output(path, 5, (copy.deepcopy(data), fault, replaces), size)

def output(path, method, result, size):  # gantt
    data = copy.copy(result[0])
    fault = result[1]
    replaces = result[2]
    if method == 1 :
        command = 'FIFO'
    elif method == 2:
        command = 'LRU'
    elif method == 3:
        command = 'Least Frequently Used Page Replacement'
    elif method == 4:
        command = 'Most Frequently Used Page Replacement '
    elif method == 5:
        command = 'Least Frequently Used LRU Page Replacement'
    with open('out_'+path+'.txt', 'a') as out:
        print('--------------', command, '-----------------------', file=out, sep='')
        for page in data:
            print(page[0], ''.join(str(sublist[0]) for sublist in page[1]), file=out, end='', sep='	')
            if len(page) == 3:
                print('	', page[2], file=out, sep='')
            else:
                print('', file=out)
        print("Page Fault =", fault, " Page Replaces =", replaces, " Page Frames =", size, file=out)


path = input('Please Enter File Name:(0:end)')
data = []
while path != '0':
    with open(path + '.txt', 'r') as file:
        line = file.readline().split()
        method = int(line[0])
        size = int(line[1])
        line = file.readline().split()
        data = [[int(char)] for word in line for char in word]
        if method == 1:
            FIFO(copy.deepcopy(data), size)
        elif method == 2:
            LRU(copy.copy(data), size)
        elif method == 3 :
            LFU_FIFO(copy.copy(data), size)
        elif method == 4 :
            MFU_FIFO(copy.copy(data), size)
        elif method == 5 :
            LFU_LRU(copy.copy(data), size)
        elif method == 6:
            method = 1
            FIFO(copy.deepcopy(data), size)
            with open('out_'+path+'.txt', 'a') as out:
                print("", file = out)

            method = 2
            LRU(copy.deepcopy(data), size)
            with open('out_'+path+'.txt', 'a') as out:
                print("", file = out)

            method = 3
            LFU_FIFO(copy.deepcopy(data), size)
            with open('out_'+path+'.txt', 'a') as out:
                print("", file = out)

            method = 4
            MFU_FIFO(copy.deepcopy(data), size)
            with open('out_'+path+'.txt', 'a') as out:
                print("", file = out)

            method = 5
            LFU_LRU(copy.deepcopy(data), size)

        print('end')
        path = input('Please Enter File Name:(0:end)')
        data = []
