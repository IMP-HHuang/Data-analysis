# Data ana
  - LMD file Decode/Online code
    - [Go4](https://github.com/go4org/go4)
      - [Analysis code](Go4/AnaCode/) 
        - Get subevent in  Proc.cxx/h
        - Decode Subsystem are in Decode*.h/cxx
        - Data format for Tamex, WhitRabbit are in [Data.h](Go4/AnaCode/DecodeSubSystem1.1/Data.h)
        - The informations of detectors are in [Par/DetectorInf.txt](Go4/AnaCode/DecodeSubSystem1.1/Par/DetectorInf.txt)
    - [FairRoot](https://github.com/FairRootGroup/FairRoot)
      - [c4root](https://github.com/cej25/c4Root)
        - [c4data/special subsystem/](https://github.com/cej25/c4Root/tree/main/c4data) : Raw data, calibartion and map parameters
        - [c4source/special subsystem/](https://github.com/cej25/c4Root/tree/main/c4source) : Data ana code
        - [/unpake/exp](https://github.com/cej25/c4Root/tree/main/unpack) : Dataformat for subsystem
        - [/macros/run.cpp](https://github.com/cej25/c4Root/tree/main/macros) : example fot using all class to ana data  
  - Tmaex [Fine time calibration](https://ieeexplore.ieee.org/document/571882)
    $$\frac{\Delta t}{T} = \frac{N(\Delta t)}{N_{sum}}$$
    where, $T$ = 5 ns is the coarse time, $\Delta t$ is the time span of one bin of fine time, $N(\Delta t)$ is the count in this bin  and $N_{sum}$ is count in all bins of fine time.
