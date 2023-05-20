#include "../../headers/solutions/CanonicalExamples.h"
#include <math.h>

// const params for random network

Network randomNetwork(int seed, int NumFactories, int maxCoord){
    srand(seed);
    Network net;

    //Check if the area given can handle numFactories
    //The given number of factories should also be 2 or greater
    if(NumFactories > pow(maxCoord * 2, 2) || NumFactories < 2) return net;
    
    //Count is a failsafe just in case the random numbers constantly overlap
    int count = 0;
    while((net.getNumFactories() < NumFactories) && (count < NumFactories * 2)){
        //Generate a random x,y for the factory
        Coord randx = (rand() % (2*maxCoord)) - maxCoord;
        Coord randy = (rand() % (2*maxCoord)) - maxCoord;
        //Generate a random resource list for the factory
        std::map<Resource, Quant> temp;
        for(Resource j = Resource(0); j < Resource::COUNT; j++){
            temp.emplace(j, (rand() % (21)) - 10);
        }
        ResourceList randList(temp);
        //Add the factory to the network
        net.addFactory(randx, randy, randList);
        count++;
    }

    //Make sure the network is valid
    ResourceList deficit;
    for(auto i = net.factCBegin(); i != net.factCEnd(); ++i){
        deficit += i->second.getBaseQuants();
    }
    for(int i = 0; i < Resource::COUNT; i++) {
        if(deficit[i] < 0) deficit[i] = abs(deficit[i]);
        else if(deficit[i] > 0) deficit[i] = 0;
    }
    
    //If there is a deficit in some resources then create a new factory that produces those resources
    if (deficit != ResourceList())
    {
        int numFact = net.getNumFactories();
        while(net.getNumFactories() == numFact){
            Coord randx = (rand() % (2*maxCoord)) - maxCoord;
            Coord randy = (rand() % (2*maxCoord)) - maxCoord;
            net.addFactory(randx, randy, deficit);
        }
    }

    //Add some random number of junctions to the space
    /*for(int i = 0; i < rand() % (NumFactories / 2) + 3; i++){
        Coord randx = (rand() % (2*maxCoord)) - maxCoord;
        Coord randy = (rand() % (2*maxCoord)) - maxCoord;
        net.createJunct(randx, randy);
    }*/

    return net;
}