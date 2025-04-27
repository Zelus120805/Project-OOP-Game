#ifndef _MAP_H_
#define _MAP_H_

#include <string>
#include <vector>

inline float offsetX = 0;
inline float offsetY = 0;

class Map {
private:
    const int _height = 17;
    const int _width = 150;
    std::vector<std::vector<std::string>> maps;
public:
    Map() {
        // maps.push_back({
        //     "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        //     "0                                                                                                                                                   0",
        //     "0                                                                                    w                                                              0",
        //     "0                   w                                  w                   w                                                                        0",
        //     "0                                      w                                       kk                                                                   0",
        //     "0                                                                             k  k    k    k                                                        0",
        //     "0                      c                                                      k      kkk  kkk  w                                                    0",
        //     "0                                                                       r     k       k    k                                                        0",
        //     "0                                                                      rr     k  k                                                                  0",
        //     "0                                                                     rrr      kk                                                                   0",
        //     "0               c    kckck                                           rrrr                                                                           0",
        //     "0                                      t0                           rrrrr                                                                           0",
        //     "0G                                     00              t0          rrrrrr            G                                                              0",
        //     "0           d    g       d             00              00         rrrrrrr                                                                           0",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        // });
        
        // Sau khi đã có kỹ năng phá tường mới đi map này được
        // maps.push_back({
        //     "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        //     "0                                                                                                                                                   0",
        //     "0                                                      w          w                w                                                               0",
        //     "0               rrr            kkk            ccc          w                   rrr                                                                 0",
        //     "0      w          r      w                                  kk            w         r                                                              0",
        //     "0                k k                   t0      k k k     c   c                 k k                                                                 0",
        //     "0      c   w      k                     00       k  k   k     k       c     t0   k   w                                                             0",
        //     "0         k k     k             r       00       kk k     w w w       k k   00   k k                                                                0",
        //     "0         kkk     k    rrr              00   w         rrr          kk k   00    kk                                                                0",
        //     "0c c c    k       k            G                  rrrr       rrr           00      G                                                              0",
        //     "0       t0      kkkk      c        ccc      t0       ccc      ccc         t0                                                                       0",
        //     "0       00        rrrr              00       00                                                                                                    0",
        //     "0       00      rr          G        00       00               G                                                                                   0",
        //     "0g  d    00  rr      d      g        00       00      g     d         g                                                                              0",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        // });        

        // maps.push_back({
        //     "000000000000000000000000000000",
        //     "0       rrrrr               G0",
        //     "0     c     c                0",
        //     "0         tttt              0",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        // });
        // Bạn có thể push_back thêm nhiều map khác
    }

    ~Map() { }

    int getHeight() {return _height;}

    int getWidth() {return _width;}

    const std::vector<std::string>& getMap(int level) const {
        return maps[level];
    }
public:
    // void loadMap() {
    //     for (int i = 0; i < _height; i++) {
    //         for (int j = 0; j < _width; j++) {
    //             char tileChar = currentMap[i][j];
    //             if (tileChar == ' ' || tileChar == '0') continue;

    //             if (tileChar == 'P') tile.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));
    //             else if (tileChar == 'k') tile.setTextureRect(IntRect(143, 112, 16, 16));
    //             else if (tileChar == 'c') tile.setTextureRect(IntRect(143 - 16, 112, 16, 16));
    //             else if (tileChar == 't') tile.setTextureRect(IntRect(0, 47, 32, 48));
    //             else if (tileChar == 'g') tile.setTextureRect(IntRect(0, 139, 48, 37));
    //             else if (tileChar == 'G') tile.setTextureRect(IntRect(145, 222, 77, 33));
    //             else if (tileChar == 'd') tile.setTextureRect(IntRect(0, 106, 74, 21));
    //             else if (tileChar == 'w') tile.setTextureRect(IntRect(99, 224, 41, 31));
    //             else if (tileChar == 'r') tile.setTextureRect(IntRect(143 - 32, 112, 16, 16));

    //             tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
    //             window.draw(tile);
    //         }
    //     }
    // }
};

#endif
