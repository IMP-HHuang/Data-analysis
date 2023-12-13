# Data-analysis

  - [Decode]
  - PKUXIADAQ
    - [Correlation](./PKUXIADAQ/correlation)
    - [Draw](./PKUXIADAQ/Draw)
  - Root
    - Hitogram的bin太多会报错：
      - Error in <TBufferFile::WriteByteCount>: bytecount too large (more than 1073741822)
    - TTree::Draw()：设置string的cut
      - tree->Draw("ye:yzid>>yze(60, 0, 60, 510, 0.0, 510.0)", "p==\\"gamma\\"");
