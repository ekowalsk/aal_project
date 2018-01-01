//
// Created by ewa on 27.12.17.
//

#include "Cube.h"

cube::cube() {
    type = waterfield;
}

cube::~cube() {

}

int cube::getType() const {
    return type;
}

void cube::setType(int type) {
    this->type = type;
}

int Field::getHeight() const {
    return height;
}

void Field::setHeight(int height) {
    Field::height = height;
}

bool Field::isChecked() const {
    return checked;
}

void Field::setChecked(bool checked) {
    Field::checked = checked;
}

Field::Field() {
    checked = false;
    height = 0;
    setType(waterfield);
}

Block::Block(int x, int y, int height) {
    this->height = height;
    this->x = x;
    this->y = y;
}


int Block::getHeight() const {
    return height;
}

void Block::setHeight(int height) {
    Block::height = height;
}

int Block::getX() const {
    return x;
}

void Block::setX(int x) {
    Block::x = x;
}

int Block::getY() const {
    return y;
}

void Block::setY(int y) {
    Block::y = y;
}
