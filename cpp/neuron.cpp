#include "../headers/define.h"
#include "../headers/neuron.h"

neuron::neuron(int x, int y) : x(x), y(y), value(0), error(0), destination(0){

}

int neuron::getX() const {
    return x;
}

int neuron::getY() const {
    return y;
}

float neuron::getValue() const {
    return value;
}

void neuron::setValue(float val) {
    value = val;
}

float neuron::getError() const {
    return error;
}

void neuron::setError(float e) {
    error = e;
}

float neuron::getDest() const {
    return destination;
}

void neuron::setDest(float dest) {
    destination = dest;
}

void neuron::setBias() {
    if(bias){
        bias = false;
    } else {
        bias = true;
    }
}

bool neuron::isBias() const {
    return bias;
}
