nodes = [1,8,12,16,20,24,28,32,36,40,44,48]
times = [36.418847,4.691736,3.122340,2.355710,1.933689,1.580074,1.357894,1.191409,1.534962,1.467548,1.405894,1.191885]

for n in range(len(nodes)):
    ratio = (times[n]/times[0]) / (nodes[0]/nodes[n])
    print(f"{nodes[n]}: {ratio}")

for n in range(len(nodes)):
    ratio = (times[0]/times[n]) / nodes[n]
    print(f"{nodes[n]}: {ratio}")