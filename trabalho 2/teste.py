a = 18111/2
b = 90555
c = 633885/2
d = 452773
e = 217331
for i in range(1, 6):
    res = a*pow(i, 4) - b*pow(i, 3) + c*pow(i, 2) - d*i + e
    print(res)