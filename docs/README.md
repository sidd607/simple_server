# simple_server
This is a Simple HTTP Server created in C for serving http 1.1 requests.

### To Compile
```
gcc src/server.c src/handle_request.c -o server
```
or 
```
make server
```

### To Run
```
./server [PORT]
```
### Features Implemented

- This server is a multi process server where each request is handled in a different process.
- To address the c10k problem, I was lookng at a even based architecture but that required external libraries, so I gave up that idea.
- Methods it supports:
- - ` GET /filename ` returns the contents of the file if exists
- - ` HEAD /file ` returns the just headers of the bove request.
- - ` POST ` Displays the data of the request. 
- - ` OPTIONS `  Displays the allowed methods.
-  Logging. All requests are logged according to the Apache web logs format.
### Benchmarking

- Apache benchmark was used to stress test the server for 10000 concurrent users.
#### Test 1 (1 user 1000 requests)
```
ab -n 100 -c 1 localhost:10000/
```

```
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1
Time taken for tests:   0.247 seconds
Complete requests:      1000
Failed requests:        0
Total transferred:      23000 bytes
HTML transferred:       0 bytes
Requests per second:    4041.73 [#/sec] (mean)
Time per request:       0.247 [ms] (mean)
Time per request:       0.247 [ms] (mean, across all concurrent requests)
Transfer rate:          90.78 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       0
Processing:     0    0   0.2      0       6
Waiting:        0    0   0.2      0       5
Total:          0    0   0.2      0       6

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      1
  99%      1
 100%      6 (longest request)
```

#### Test 2 (10 Users 1000 Requests)
```
ab -n 1000 -c 10 localhost:10000/
```

```
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      10
Time taken for tests:   0.139 seconds
Complete requests:      1000
Failed requests:        0
Total transferred:      23000 bytes
HTML transferred:       0 bytes
Requests per second:    7194.04 [#/sec] (mean)
Time per request:       1.390 [ms] (mean)
Time per request:       0.139 [ms] (mean, across all concurrent requests)
Transfer rate:          161.58 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       2
Processing:     0    1   1.2      1      18
Waiting:        0    1   1.2      1      18
Total:          0    1   1.2      1      18

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      2
  80%      2
  90%      3
  95%      4
  98%      5
  99%      5
 100%     18 (longest request)
```

#### Test 3 (100 Users 10000 requests)

```
 ab -n 10000 -c 100 localhost:10000/
```

```
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      100
Time taken for tests:   1.930 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      230000 bytes
HTML transferred:       0 bytes
Requests per second:    5182.51 [#/sec] (mean)
Time per request:       19.296 [ms] (mean)
Time per request:       0.193 [ms] (mean, across all concurrent requests)
Transfer rate:          116.40 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.7      0       6
Processing:     0   19  16.9     15      89
Waiting:        0   19  16.9     15      89
Total:          0   19  16.8     16      89

Percentage of the requests served within a certain time (ms)
  50%     16
  66%     25
  75%     30
  80%     31
  90%     42
  95%     55
  98%     63
  99%     69
 100%     89 (longest request)
```

#### Test 4 (1000 Users 10000 Requess)
```
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1000
Time taken for tests:   2.061 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      230000 bytes
HTML transferred:       0 bytes
Requests per second:    4851.07 [#/sec] (mean)
Time per request:       206.140 [ms] (mean)
Time per request:       0.206 [ms] (mean, across all concurrent requests)
Transfer rate:          108.96 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   25 150.0      0    1002
Processing:     0  171 130.9    166     479
Waiting:        0  170 130.9    166     479
Total:          0  196 208.1    168    1473

Percentage of the requests served within a certain time (ms)
  50%    168
  66%    215
  75%    250
  80%    302
  90%    397
  95%    450
  98%   1195
  99%   1228
 100%   1473 (longest request)
```