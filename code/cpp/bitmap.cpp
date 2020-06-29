#include "bitmap.h"

Bitmap::Bitmap(int rows,int columns){
    //number of bytes per row
    int c = columns%64==0 ? columns/64 : columns/64+1;
    for(int r=0;r<rows;++r){
        std::vector<std::bitset<64>> v(c);
        map.push_back(v);
    }
    rowCount = rows;
    columnCount = columns;
}

bool Bitmap::areaIsClear(const Polygon* r)const{
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(getBit(row,col)&&r->contains(Position(row,col))) return false;
        }
    }
    return true;
}

bool Bitmap::areaIsSet(const Polygon* r)const{
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(!getBit(row,col)&&r->contains(Position(row,col))) return false;
        }
    }
    return true;
}
bool Bitmap::areaIsClear(const Rectangle* r)const{
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if((col < (topLeft.y-64)) && (col > (bottomRight.y+64))){
                if(map[row][col/64].any()) return false;
            }else{
                if(getBit(row,col)) return false;
            }
        }
    }
    return true;
}
bool Bitmap::areaIsSet(const Rectangle* r)const{
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if((col < (topLeft.y-64)) && (col > (bottomRight.y+64))){
                if(!map[row][col/64].all()) return false;
            }else{
                if(!getBit(row,col)) return false;
            }
        }
    }
    return true;
}
bool Bitmap::areaIsClear(const Circle* r)const{
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(getBit(row,col)&&r->contains(Position(row,col))) return false;
            if(r->contains(Point(row,col+64)) && r->contains(Point(row,col))){
                if(map[row][col/64].any()) return false;
                col += 63;
            }
        }
    }
    return true;
}

bool Bitmap::areaIsSet(const Circle* r)const{
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(!getBit(row,col)&&r->contains(Position(row,col))) return false;
            if(r->contains(Point(row,col+64)) && r->contains(Point(row,col))){
                if(!map[row][col/64].all()) return false;
                col += 63;
            }
        }
    }
    return true;
}
void Bitmap::setRegion(const Polygon* r){
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(r->contains(Position(row,col))){
                setBit(row,col,true);
            }
        }
    }
}
void Bitmap::clearRegion(const Polygon* r){
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(r->contains(Position(row,col))) setBit(row,col,false);
        }
    }
}
void Bitmap::setRegion(const Rectangle* r){
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if((col < (topLeft.y-64)) && (col > (bottomRight.y+64))){
                map[row][col/64].set();
            }else{
                setBit(row,col,true);
            }
        }
    }
}

void Bitmap::clearRegion(const Rectangle* r){
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if((col < (topLeft.y-64)) && (col > (bottomRight.y+64))){
                map[row][col/64].reset();
            }else{
                setBit(row,col,false);
            }
        }
    }
}
void Bitmap::setRegion(const Circle* r){
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(r->contains(Position(row,col))){
                if(r->contains(Point(row,col+64))){
                    map[row][col/64].set();
                    col += 63;
                }else{
                    setBit(row,col,true);
                }
            }
        }
    }
}
void Bitmap::clearRegion(const Circle* r){
    Position topLeft = r->getTopLeft();
    Position bottomRight = r->getBottomRight();
    if(topLeft.x < 0) topLeft.x = 0;
    if(topLeft.y >= columnCount) topLeft.y = columnCount;
    if(bottomRight.x > rowCount) bottomRight.x = rowCount;
    if(bottomRight.y < 0) bottomRight.y = 0;
    for(int row=topLeft.x;row<bottomRight.x;++row){
        for(int col=bottomRight.y;col<topLeft.y;++col){
            if(r->contains(Position(row,col))){
                if(r->contains(Position(row,col+64))){
                    map[row][col/64].reset();
                    col += 63;
                }else{
                    setBit(row,col,false);
                }
            }
        }
    }
}
void Bitmap::setAll(){
    for(auto x=map.begin();x!=map.end();++x){
        for(auto y=x->begin();y!=x->end();++y){
            y->set();
        }
    }
}
void Bitmap::clearAll(){
    for(auto x=map.begin();x!=map.end();++x){
        for(auto y=x->begin();y!=x->end();++y){
            y->reset();
        }
    }
}
Bitmap Bitmap::operator&(const Bitmap& bm)const{
    if(!(rowCount==bm.getRowCount() && columnCount==bm.getColCount()))
        throw 1;
    Bitmap out(rowCount,columnCount);
    for(int row=0;row<rowCount;++row){
        for(int col=0;col<map[0].size();++col){
            std::bitset<64> tmp = bm.map[row][col];
            tmp &= map[row][col];
            out.map[row][col] = tmp;
        }
    }
    return out;
}
Bitmap Bitmap::operator|(const Bitmap& bm)const{
    if(!(rowCount==bm.getRowCount() && columnCount==bm.getColCount()))
        throw 1;
    Bitmap out(rowCount,columnCount);
    for(int row=0;row<rowCount;++row){
        for(int col=0;col<map[0].size();++col){
            std::bitset<64> tmp = bm.map[row][col];
            tmp |= map[row][col];
            out.map[row][col] = tmp;
        }
    }
    return out;
}

std::optional<Position> Bitmap::closestSetBit(const Line& line)const{
    double g = line.getGradient();
    double a = line.getAngle();
    double y = line.getStart().y;
    double x = line.getStart().x;
    double y0 = y;
    double x0 = x;
    Rectangle boundries(Point(0,3000),Point(2000,0));

    if(a >= 315 || a < 45){
        double yPrev = y;
        for(;x<=line.getEnd().x;x+=1){
            Position p(x,y);
            if(!boundries.contains(p))return std::nullopt;
            if(getBit(p)) return std::optional<Position>(p);
            y = y0 + (x-x0)*g;
            if(static_cast<int>(y) != static_cast<int>(yPrev)){
                p = Position(x,y);
                if(!boundries.contains(p))return std::nullopt;
                if(getBit(p)) return std::optional<Position>(p);
            }
            yPrev = y;
        }
    }else if(a >= 45 && a < 135){
        double xPrev = x;
        for(;y<=line.getEnd().y;y+=1){
            Position p(x,y);
            if(!boundries.contains(p))return std::nullopt;
            if(getBit(p)) return std::optional<Position>(p);
            x = x0 + (y-y0)/g;
            if(static_cast<int>(x) != static_cast<int>(xPrev)){
                p = Position(x,y);
                if(!boundries.contains(p))return std::nullopt;
                if(getBit(p)) return std::optional<Position>(p);
            }
            xPrev = x;
        }
    }else if(a >= 135 && a < 225){
        double yPrev = y;
        for(;x>=line.getEnd().x;x-=1){
            Position p(x,y);
            if(!boundries.contains(p))return std::nullopt;
            if(getBit(p)) return std::optional<Position>(p);
            y = y0 + (x-x0)*g;
            if(static_cast<int>(y) != static_cast<int>(yPrev)){
                p = Position(x,y);
                if(!boundries.contains(p))return std::nullopt;
                if(getBit(p)) return std::optional<Position>(p);
            }
            yPrev = y;
        }
    }
    else if(a >= 225 && a < 315){
        double xPrev = x;
        for(;y>=line.getEnd().y;y-=1){
            Position p(x,y);
            if(!boundries.contains(p))return std::nullopt;
            if(getBit(p)) return std::optional<Position>(p);
            x = x0 + (y-y0)/g;
            if(static_cast<int>(x) != static_cast<int>(xPrev)){
                p = Position(x,y);
                if(!boundries.contains(p))return std::nullopt;
                if(getBit(p)) return std::optional<Position>(p);
            }
            xPrev = x;
        }
    }
    return std::optional<Position>();
}

bool Bitmap::lineClear(const Line& line)const{
    std::optional<Position> p = closestSetBit(line);
    return !p.has_value();
}

void Bitmap::print()const{
    int sf = 64;
    //int rat = columnCount/maxr;
    for(auto r=map.begin();r<map.end();r+=sf){
        for(auto c=r->begin();c<r->end();c+=1){
            std::cout<<c->any()?"1":"0";
        }
        std::cout<<"\n";
    }
}






//
