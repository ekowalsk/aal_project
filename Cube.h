//
// Created by ewa on 27.12.17.
//

#ifndef AAL_BLOCK_H
#define AAL_BLOCK_H

#define MAX_HEIGHT 10;

class cube {

private:
    int type;

public:
    enum type
    {
        empty = 0,
        water = 1,
        block = 2
    };

    cube();
    void setType(int type);
    int getType() const;
};

class Field : public cube
{
private:
    int height;
    bool checked;

public:
    Field();

    int getHeight() const;

    void setHeight(int height);

    bool isChecked() const;

    void setChecked(bool checked);

};

class Block
{
private:
    int height;
    int x;
    int y;

public:
    Block (int x, int y, int height);

    int getHeight() const;

    void setHeight(int height);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

};
#endif //AAL_BLOCK_H
