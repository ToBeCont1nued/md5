# md5
md5的c++实现  
  
usage:  
``` cpp
const char* message = "hello world";  
uint8_t digest[32];  
md5(message, strlen(message), digest);  
```
结果存储在digest中，以byte的形式存储。  
如果message中间存在'\0'，那么要自己在md5的第二个参数指定长度  
  
显示十六进制结果  
``` cpp
for (int i = 0; i != 16; ++i)  
    cout << hex << setw(2) << setfill('0') << (uint32_t)digest[i];  
cout << endl;  
```
