Update: 
The 6.in runs with 4 threads twice as fast as that with 1 thread. I got 0.027 and 0.056 respectively. 


Usage (PLEASE NOTE IT'S `pzip` NOT `wzip`)

```shell
make pzip
cp pzip wzip
./test-wzip.sh
export NTHREADS=4 && time ./test-wzip.sh
```


I used one thread for reading file info, one thread for writing output to stdout, and NTHREADS threads to write compression. The speed up is somewhat significant. In my test (see the screenshot below), the ./test-wzip.sh runs from 0.140 to 0.123 with 1 thread and 4 threads respectively. I tried to compile and run the same program on my linux machine, but it gives segmentation fault for some reason. Valgrind doesn't give me any useful information so I don't know how to fix it. In my limited testing, I am not sure if there is any race conditions. 

![Screen Shot 2020-10-12 at 01.37.23](./Screen Shot 2020-10-12 at 01.37.23.png)