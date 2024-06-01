#include "../headers/define.h"

#ifndef NEURAL_NET_NEURON_H
#define NEURAL_NET_NEURON_H


class neuron {
private:
    int x,y;
    float value;
    float error;
    float destination;
    bool bias;
public:
    neuron(int x, int y);
    int getX() const;
    int getY() const;
    float getValue() const;
    void setValue(float val);
    float getDest() const;
    void setDest(float dest);
    float getError() const;
    void setError(float e);
    void setBias();
    bool isBias() const;
};


#endif //NEURAL_NET_NEURON_H
