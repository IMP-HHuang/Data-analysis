# 32-bit Decode
  - 位运算 ： buff[0] & 0x0000000f （提取前4bit的数）
  - struct结构体中的“位域”或“位段:
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
