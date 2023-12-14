# bit（位）和byte(字节)关系
  - 1 byte = 8 bit
  - int --> 4 byte --> 32 bit
# 32-bit Decode
  - 位运算 ： buff[0] & 0x0000000f （提取前4bit的数）
  - [struct结构体中的“位域”或“位段](https://blog.csdn.net/qq_34018840/article/details/100891191):
```cpp
struct buff0Data
{ 
    int channel :4; 
    int SlotID :4; 
    int CrateID :4;
    int HeaderLength :5;
    int EventLength :14;
    int Pileup: 1;
}; 
```
