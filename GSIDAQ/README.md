# Data ana
  - LMD file Decode/Online code
    - [CERNRoot](https://github.com/root-project/root)
      - [DecodeAndOnline/NearLine](https://github.com/IMP-HHuang/Data-analysis/tree/main/GSIDAQ/CERNRoot/code)
        - [Detector configuration](https://github.com/IMP-HHuang/Data-analysis/blob/main/GSIDAQ/CERNRoot/code/Par/DetectorInf.txt)
        - [Parameters](https://github.com/IMP-HHuang/Data-analysis/blob/main/GSIDAQ/CERNRoot/code/Par/UserDefine.txt)
    - [Go4](https://github.com/go4org/go4)
      - [Analysis code](Go4/AnaCode/) 
        - Get subevent in  Proc.cxx/h
        - Decode Subsystem are in Decode*.h/cxx
        - Data format for Tamex, WhitRabbit are in [Data.h](Go4/AnaCode/DecodeSubSystem1.1/Data.h)
        - The informations of detectors are in [Par/DetectorInf.txt](Go4/AnaCode/DecodeSubSystem1.1/Par/DetectorInf.txt)
    - [FairRoot](https://github.com/FairRootGroup/FairRoot)
    - Decode : [ucesb](https://github.com/op3/ucesb)
      - [c4root](https://github.com/cej25/c4Root)
        - Environment
            ```js
            export SIMPATH=/path/to/fairsoft
            export FAIRROOTPATH=/path/to/fairroot
            export UCESB_DIR=/path/to/ucesb
            git clone https://github.com/cej25/c4Root.git
            mkdir build
            ## produce ucesb code
            cd ../c4Root/unpack/exps/
            make s100 -j 10
            cd build
            ## environment
            cmake ../c4Root
            .   ./config.sh
            make 
            ## run
            cd ../../macros/despec/
            root -l -b s100_online.C
            make -j
          ```
        - [c4data/special subsystem/](https://github.com/cej25/c4Root/tree/main/c4data) : Raw data, calibartion and map parameters
        - [c4source/special subsystem/](https://github.com/cej25/c4Root/tree/main/c4source) : Data ana code
        - [/unpake/exp](https://github.com/cej25/c4Root/tree/main/unpack) : Dataformat for subsystem
        - [/macros/run.cpp](https://github.com/cej25/c4Root/tree/main/macros) : example fot using all class to ana data  
  - Tmaex [Fine time calibration](https://ieeexplore.ieee.org/document/571882)
    $$\frac{\Delta t}{T} = \frac{N(\Delta t)}{N_{sum}}$$
    where, $T$ = 5 ns is the coarse time, $\Delta t$ is the time span of one bin of fine time, $N(\Delta t)$ is the count in this bin  and $N_{sum}$ is count in all bins of fine time.
