#include "../headers/define.h"
#include "../headers/net.h"

net::net(std::vector<int> l) {
    unsigned int size = l.size();
    int xpos = WIDTH/(size+1);

    for(int i=0;i<size;i++) {
        int ypos = HEIGHT/(l[i]+1);

        std::vector<neuron> tmp;
        for(int j=0;j<l[i];j++) {
            neuron n(xpos*(i+1)+WIDTH,ypos*(j+1));
            tmp.push_back(n);
        }
        neurons.push_back(tmp);
    }


    for(int i=0;i<size-1;i++) {
        std::vector<std::vector<float>> tmpNeuron;
        for(int j=0;j<l[i];j++) {
            std::vector<float> tmpFloat;
            for(int k=0;k<l[i+1];k++) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dis(-100, 100);
                int randomInt = dis(gen);
                tmpFloat.push_back(static_cast<float>(randomInt) / 100.0f);
                //std::cout << "layer: " << i << " from neuron " << j << " to neuron " << k << " weight " << static_cast<float>(randomInt) / 100.0f << std::endl;
            }
            tmpNeuron.push_back(tmpFloat);
        }
        weights.push_back(tmpNeuron);
    }
}

float net::activationFuction(float x) {
    return 1/(1+exp(-x));
}

float net::activationFuctionDerivative(float x) {
    double sig = activationFuction(x);
    return sig * (1-sig);
}

std::vector<std::vector<neuron>> net::getNet() {
    return neurons;
}

std::vector<std::vector<std::vector<float>>> &net::getWeights() {
    return weights;
}

void net::setNeuronValue(int layer, int neuron, float value) {
    neurons[layer][neuron].setValue(value);
}

void net::setNeuronDestination(int layer, int neuron, float dest) {
    neurons[layer][neuron].setDest(dest);
}

input net::randomInput() {
    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, inputs.size()-1);

    for(auto x : weights) {
        //std::cout << x.size() << std::endl;
    }
    return inputs[dist(mt)];

}

double net::forwardTrain(float yVelocity, float yPosition, float xPosition, bool& a) {
    //calculation of neuron weights
    for(int i=0;i<neurons.size();i++) {
        for(int j=0;j<neurons[i].size();j++) {
            if(i==0) {
                switch(j) {
                    case 0:
                        neurons[i][j].setValue(yVelocity);
                        break;
                    case 1:
                        neurons[i][j].setValue(yPosition);
                        break;
                    case 2:
                        neurons[i][j].setValue(xPosition);
                        break;
                }
            } else {
                float sum = 0;
                for(int k=0;k<neurons[i-1].size();k++) {
                    sum+=weights[i-1][k][j]*neurons[i-1][k].getValue();
                }
                neurons[i][j].setValue(activationFuction(sum));
            }
        }
    }
    neurons.back()[0].getValue() < neurons.back()[1].getValue() ? a = true : a = false;
    return std::max(neurons.back()[0].getValue(), neurons.back()[1].getValue());
}

double net::getQ(float yVelocity, float yPosition, float xPosition, bool a) {
    //calculation of neuron weights
    for(int i=0;i<neurons.size();i++) {
        for(int j=0;j<neurons[i].size();j++) {
            if(i==0) {
                switch(j) {
                    case 0:
                        neurons[i][j].setValue(yVelocity);
                        break;
                    case 1:
                        neurons[i][j].setValue(yPosition);
                        break;
                    case 2:
                        neurons[i][j].setValue(xPosition);
                        break;
                }
            } else {
                float sum = 0;
                for(int k=0;k<neurons[i-1].size();k++) {
                    sum+=weights[i-1][k][j]*neurons[i-1][k].getValue();
                }
                neurons[i][j].setValue(activationFuction(sum));
            }
        }
    }
    return neurons.back()[a].getValue();
}




std::vector<double> net::forwardTrainValue(float yVelocity, float yPosition, float xPosition, bool action) {
    //calculation of neuron weights
    for(int i=0;i<neurons.size();i++) {
        for(int j=0;j<neurons[i].size();j++) {
            if(i==0) {
                switch(j) {
                    case 0:
                        neurons[i][j].setValue(yVelocity);
                        break;
                    case 1:
                        neurons[i][j].setValue(yPosition);
                        break;
                    case 2:
                        neurons[i][j].setValue(xPosition);
                        break;
                    case 3:
                        neurons[i][j].setValue(action);
                        break;
                }
            } else {
                float sum = 0;
                for(int k=0;k<neurons[i-1].size();k++) {
                    sum+=weights[i-1][k][j]*neurons[i-1][k].getValue();
                }
                neurons[i][j].setValue(activationFuction(sum));
            }
        }
    }
    return {neurons.back()[0].getValue(), neurons.back()[1].getValue()};
}






std::vector<double> net::forwardShow(std::vector<bool> spikeGrid, float yVelocity, float yPosition, float xPosition) {
    std::vector<std::vector<neuron>> nCopy = neurons;
    nCopy[0][0].setValue(yVelocity);
    nCopy[0][1].setValue(yPosition);
    nCopy[0][2].setValue(xPosition);

//    for(int i=0;i<spikeGrid.size();i++) {
//        nCopy[0][i+4].setValue(spikeGrid[i]);
//    }

    for(int i=1;i<nCopy.size();i++) {
        for(int j=0;j<nCopy[i].size();j++) {
            float sum = 0;
            for(int k=0;k<nCopy[i-1].size();k++) {
                sum+=weights[i-1][k][j]*nCopy[i-1][k].getValue();
            }
            nCopy[i][j].setValue(activationFuction(sum));
        }
    }
    //std::cout << nCopy.back()[0].getValue() << " " << nCopy.back()[1].getValue() << " " << nCopy.back()[2].getValue() << std::endl;

    return {nCopy.back()[0].getValue(), nCopy.back()[1].getValue()};
}







void net::backProp(double e, bool a) {
    //error calculation for hidden layers
    for(unsigned int i=neurons.size()-1;i>0;i--) {
        for(int j=0;j<neurons[i].size();j++) {
            if(i==neurons.size()-1) {
                if(j==a) {
                    neurons[i][j].setError(e);
                } else {
                    neurons[i][j].setError(0);
                }
            } else {
                float eSum = 0;
                for(int k=0;k<neurons[i+1].size();k++) {
                    eSum+=neurons[i+1][k].getError()*weights[i][j][k];
                }
                neurons[i][j].setError(eSum);
            }
        }
    }

    //using learning rate and shit I have no idea why it doesn't work I swear to god, I spent waaaay too much time looking for mistake
    for(unsigned int i=neurons.size()-1;i>0;i--) {
        for(unsigned int j=0;j<neurons[i].size();j++) {
            for (unsigned int k = 0; k < neurons[i - 1].size(); k++) {
                //std::cout << neurons[i][j].getError() << " " << neurons[i-1][k].getValue() << " " << activationFuctionDerivative(neurons[i][j].getValue()) << std::endl;
                weights[i - 1][k][j] += LEARNING_RATE * neurons[i][j].getError() * neurons[i-1][k].getValue() *
                            activationFuctionDerivative(neurons[i][j].getValue());
            }
        }
    }
}



//void net::backProp(input rand) {
//    //error calculation for hidden layers
//    for(unsigned int i=neurons.size()-1;i>0;i--) {
//        for(int j=0;j<neurons[i].size();j++) {
//            if(i==neurons.size()-1) {
//                if(rand.rgb.empty()) {
//                    //std::cout << "error: " << neurons[i][j].getValue() << " " << float(rand.mono) << std::endl;
//                    neurons[i][j].setError(float(neurons[i][j].getValue()-float(rand.mono)));
//                } else {
//                    neurons[i][j].setError(float(neurons[i][j].getValue()-float(rand.rgb[j])));
//                }
//            } else {
//                float eSum = 0;
//                for(int k=0;k<neurons[i+1].size();k++) {
//                    eSum+=neurons[i+1][k].getError()*weights[i][j][k];
//                }
//
//                neurons[i][j].setError(eSum);
//                if(abs(eSum)>1) {
//                }
//            }
//            //std::cout << "layer: " << i << " neuron: " << j << " error: " << neurons[i][j].getError() << std::endl;
//        }
//    }
//
//    //using learning rate and shit i have no idea why it doesn't work i swear to god, i spent waaaay too much time looking for mistake
//    for(unsigned int i=neurons.size()-1;i>0;i--) {
//        //std::cout << "i: " <<  i << std::endl;
//        //std::cout << weights[i-1].size() << std::endl;
//        for(unsigned int j=0;j<neurons[i].size();j++) {
//            for(unsigned int k=0;k<neurons[i-1].size();k++) {
//                //std::cout << "error: " <<  neurons[i][k].getError() << " value: " << neurons[i][k].getValue() << " der: " << activationFuctionDerivative(neurons[i][k].getValue()) << std::endl;
//                weights[i-1][k][j] -= LEARNING_RATE * neurons[i][j].getError() * neurons[i-1][k].getValue() *
//                                      activationFuctionDerivative(neurons[i][j].getValue());
//            }
//        }
//    }
//}