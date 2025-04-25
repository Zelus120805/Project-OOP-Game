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
       maps.push_back({
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
            "0                                                                                                                                                   0",
            "0                                                                                    w                                                              0",
            "0                   w                                  w                   w                                                                        0",
            "0                                      w                                       kk                                                                   0",
            "0                                                                             k  k    k    k                                                        0",
            "0                      c                                                      k      kkk  kkk  w                                                    0",
            "0                                                                       r     k       k    k                                                        0",
            "0                                                                      rr     k  k                                                                  0",
            "0                                                                     rrr      kk                                                                   0",
            "0               c    kckck                                           rrrr                                                                           0",
            "0                                      t0                           rrrrr                                                                           0",
            "0G                                     00              t0          rrrrrr            G                                                              0",
            "0           d    g       d             00              00         rrrrrrr                                                                           0",
            "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
            "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
            "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
            });
        
        // maps.push_back({
        //     "000000000000000000000000000000",
        //     "0                            0",
        //     "0         kkk                0",
        //     "0    w                      G0",
        //     "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
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
};

#endif
