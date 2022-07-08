import numpy as np

num_vert = 3
num_arest = np.random.randint(num_vert**2)+1

print(num_vert, num_arest)
arestas = set()
i = 0
while i<num_arest:
    a = np.random.randint(num_vert)
    b = np.random.randint(num_vert)+num_vert
    if (a,b) not in arestas:
        arestas.add((a,b))
        i += 1


m = []

for a in arestas:
    for i in range(num_vert*2):
        if i in a:
            m.append(1)
        else:
            m.append(0)

m = np.array(m)
m = m.reshape((num_arest, num_vert*2))
m = m.T

for i in range(num_vert*2):
    for j in range(num_arest):
        if j == num_arest-1:
            print(m[i, j])
        else:
            print(m[i, j], end=' ')

for i in range(num_arest):
        if i == num_arest-1:
            print(np.random.randint(9)+1)
        else:
            print(np.random.randint(9)+1, end=' ')
