
from collections import deque

moves = [(1, 2), (1, -2), (-1, 2), (-1, -2),(2, 1), (2, -1), (-2, 1), (-2, -1)]

def inside(x, y, n):
    if x < 0 or x >= n or y < 0 or y >= n:
        return False
    return True

def knight(n, start, end):
    queue = deque()
    queue.append(start)
    visited = [[False] * n for _ in range(n)]
    visited[start[0]][start[1]] = True
    steps = 0

    while queue:
        size = len(queue)

        for _ in range(size):
            x, y = queue.popleft()

            if (x, y) == end:
                return steps

            for dx, dy in moves:
                nx, ny = x + dx, y + dy

                if inside(nx, ny, n) and not visited[nx][ny]:
                    queue.append((nx, ny))
                    visited[nx][ny] = True

        steps += 1


n = 8 
f = open(input('檔名(ex. test_5)')+'.txt')
data = f.readline().split()
while data[0] != '0' and data[1] != '0':
    start = list(data[0])
    start = ( ord(start[0])-97, int(start[1])-1)
    end = list(data[1])
    end = ( ord(end[0])-97, int(end[1])-1)

    min_steps = knight(n, start, end)
    print(f"From {data[0]} to {data[1]} ,Knight Moves= {min_steps}")
    data = f.readline().split()
