#ifndef BITMAP_H
#define BITMAP_H

#include <bitset>
#include <vector>
#include <optional>

#include "region.h"

class Bitmap{
    int rowCount;
    int columnCount;
    std::vector<std::vector<std::bitset<64>>> map;
public:
    Bitmap(int rows,int columns);

    bool areaIsClear(const Polygon* r)const; //true if all bits contained by r are 0
    bool areaIsSet(const Polygon* r)const; //true if all bits contained by r are 1
    bool areaIsClear(const Rectangle* r)const; //true if all bits contained by r are 0
    bool areaIsSet(const Rectangle* r)const; //true if all bits contained by r are 1
    bool areaIsClear(const Circle* r)const; //true if all bits contained by r are 0
    bool areaIsSet(const Circle* r)const; //true if all bits contained by r are 1

    void setRegion(const Polygon* r);
    void clearRegion(const Polygon* r);
    void setRegion(const Circle* r);
    void clearRegion(const Circle* r);
    void setRegion(const Rectangle* r);
    void clearRegion(const Rectangle* r);

    void setAll();
    void clearAll();
    Bitmap operator&(const Bitmap& bm)const;
    Bitmap operator|(const Bitmap& bm)const;

    bool lineClear(const Line& line)const; //check that line doesn't touch set bits
    std::optional<Position> closestSetBit(const Line& line)const;

    bool getBit(int row,int column)const{
        if(row>=0 && row<rowCount && column>=0 && column<columnCount)
            return map[row][column/64][column%64];
        else return false;
    };
    bool getBit(Point p)const{return getBit(static_cast<int>(p.x),static_cast<int>(p.y));};
    void setBit(int row,int column,bool val){
        if(row>=0 && row<rowCount && column>=0 && column<columnCount)
            map[row][column/64][column%64] = val;
    };
    void setBit(Point p,bool val){setBit(static_cast<int>(p.x),static_cast<int>(p.y),val);};
    int getRowCount()const{return rowCount;};
    int getColCount()const{return columnCount;};

    void print()const;
};
/*
 ---------------------------
|01234567 01234567 01234567 |
|                           |
|                           | rows = x
|                           |
|                           |
|                           |
 ---------------------------
      <-columns = y ->
map[row][column]
*/
#endif
