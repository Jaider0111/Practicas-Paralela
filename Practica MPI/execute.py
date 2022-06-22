import os
import time

images = ["perrito720", "perrito1080", "perrito4K"]

os.system("mpic++ image-filter-mpi2.cpp -o image-filter-mpi2 `pkg-config --cflags --libs opencv4` 2> datos.csv")


for img in images:
    for k in range(0, 5):
        for i in range(3):
            com = "mpirun -np " + \
                str(2**k) + " --hostfile mpi_hosts ./image-filter-mpi2 " + \
                img + ".jpg " + img + "F.jpg 2>> datos.csv"
            time.sleep(1)
            print(com)
            os.system(com)
