from random import randint as ri

n = int(input())
with open('input.txt', 'w') as file:
    file.write(f'{n}\n')
    for i in range(2*n):
        for j in range(n):
            #file.write(f'{ri(0,1000)} ')
            file.write(f'{i%n+1} ')
        file.write(f'\n')
