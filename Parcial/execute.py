import os

for j in range(3, 11):
    com = "echo " + str(2**j) + " | python3 mat-gen.py"
    print(com)
    os.system(com)
    for k in range(0, 5):
        for i in range(3):
            com = "./multiplicacionMatrices " + str(2**k) + " 2>> datos.csv"
            print(com)
            os.system(com)
