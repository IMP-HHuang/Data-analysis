# Data-analysis

  - [Decode](./位运算/README.md)
  - [GUI](./code/GUI)
  - [PKUXIADAQ](https://github.com/wuhongyi/PKUXIADAQ)
    - [Correlation](./PKUXIADAQ/correlation)
    - [Draw](./PKUXIADAQ/Draw)
  - [GSIDAQ](./GSIDAQ)
  - Root
    - Hitogram的bin太多会报错：
      - Error in <TBufferFile::WriteByteCount>: bytecount too large (more than 1073741822)
    - TTree::Draw()：设置string的cut
      - tree->Draw("ye:yzid>>yze(60, 0, 60, 510, 0.0, 510.0)", "p==\\"gamma\\"");
    - TTree::Draw()：数组下标 Iteration$
      - tree->Draw("data:Iteration$", "", "*", 1, 7)
    - THttpServer::Register("Path", THD)
      - path（包括子path） 不能以数字开头 （不信你就试试！！）
    - Error in <TFile::WriteTObject>: Directory ***.root is not writable
      - 这不是权限问题，是你文件close早了！！！
    - TTree::Draw()：Get TGraph
      ```cpp
      n = tree->Draw("YSO0D.data:Iteration$>>tr", "", "", 167, 1);
      TGraph *g = new TGraph(n,tree->GetV2(),tree->GetV1());
      ```    
