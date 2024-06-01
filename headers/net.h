#include "../headers/define.h"
#include "neuron.h"

#ifndef NEURAL_NET_NET_H
#define NEURAL_NET_NET_H


struct input {
    float x,y;
    bool mono;
    std::vector<int> rgb;
};

struct state {
    float yVelocity, yPosition, xPosition;
};

struct learnQueue {
    state oldState, newState, s;
    bool action;
    float reward;
    double probability;
};

class net {
private:
    std::vector<std::vector<neuron>> neurons;
    std::vector<std::vector<std::vector<float>>> weights;
    std::vector<input> inputs;
    std::vector<bool> outputs;
public:
    explicit net(std::vector<int> l);
    static float activationFuction(float x);
    static float activationFuctionDerivative(float x);
    std::vector<std::vector<neuron>> getNet();
    std::vector<std::vector<std::vector<float>>> &getWeights();
    void setNeuronValue(int layer, int neuron, float value);
    void setNeuronDestination(int layer, int neuron, float dest);
    input randomInput();
    //this is for training network
    double forwardTrain(float yVelocity, float yPosition, float xPosition, bool& a);
    double getQ(float yVelocity, float yPosition, float xPosition, bool a);
    //this is feeding data and getting result to draw output
    std::vector<double> forwardShow(std::vector<bool> spikeGrid, float yVelocity, float yPosition, float xPosition);
    void backProp(double e, bool a);
    std::vector<double> forwardTrainValue(float yVelocity, float yPosition, float xPosition, bool action);
};


#endif //NEURAL_NET_NET_H
