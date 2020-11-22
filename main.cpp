#include<cassert>
#include<cmath>
#include<cstdlib>
#include<iostream>
#include<random>
#include<vector>

#define EULER 2.718281828

using namespace std;

class Neuron;
typedef vector<Neuron> Layer;
typedef unsigned uint;

template <typename T>
void printVector(vector<T> vect)
{
	for(uint i = 0; i < vect.size(); i++)
		cout << vect[i] << ' ';
	cout << '\n';
}



class Neuron
{
public:
	Neuron();
	static double sigmoid(double x) { return 1/(1 + pow(EULER, -x)); }
	double getOutput();
	vector<double> inputWeights;
	vector<double> inputs;
private:
};

Neuron::Neuron()
{
	inputs.push_back(1.0);
	inputWeights.push_back(1.0);
	cout << "Neuron created" << '\n';
}

double Neuron::getOutput()
{
	double weightedSum = 0;
	for(uint i = 0; i < inputs.size(); i++)
	{
//		cout << inputs[i] << ' ' << inputWeights[i] << '\n';
		weightedSum += inputWeights[i] * inputs[i];
	}

	return sigmoid((weightedSum)/inputs.size());
}



class Net
{
public:
	Net(vector<uint> topology);
	static void randomizeWeights(Net& net);
	void setInputs(const vector<double> &inputs);
	void getOutputs(vector<double> &outputs);
	void train(const vector<double> inputs, const vector<double> targets);
private:
	void setWeights(const uint &layerIndex, const uint &neuronIndex, const vector<double> &weights);
	vector<double> calculateError();
	vector<Layer> layers;
};

Net::Net(vector<uint> topology)
{
	//Neuron and layer creation
	for(uint l = 0; l < topology.size(); l++)
	{
		layers.push_back(Layer());
		for(uint n = 0; n < topology[l]; n++)
		{
			layers.back().push_back(Neuron());
		}
	}

//	for(uint i = 0 ; i < layers[0].size(); i++)
//		cout << layers[0][i].inputs[0] << ' ' << layers[0][i].inputWeights[0] << '\n';
}

void Net::randomizeWeights(Net &net)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(-1.0, 1.0);
	for(uint l = 1; l < net.layers.size(); l++)
	{
		for(uint n = 0; n < net.layers[l].size(); n++)
		{
			net.layers[l][n].inputWeights.clear();
			for(uint w = 0; w < net.layers[l-1].size(); w++)
				net.layers[l][n].inputWeights.push_back(dist(mt));
		}
	}
}

void Net::setInputs(const vector<double> &inputs)
{
	for(uint n = 0; n < layers[0].size(); n++)
	{
		layers[0][n].inputs.clear();
		layers[0][n].inputs.push_back(inputs[n]);
	}
	for(uint i = 0 ; i < layers[0].size(); i++)
		cout << layers[0][i].inputs[0] << ' ';
	cout << '\n';
}

void Net::getOutputs(vector<double> &outputs)
{
	// For each layer and for each neuron in that layer,
	// assess the output of each neuron for the next layer's input
	// until we get to the output neurons.
	// The output neurons will give us the output
	for(uint l = 1; l < layers.size(); l++)
	{
		Layer& leftLayer = layers[l-1];
		Layer& thisLayer = layers[l];
		for(uint thisN = 0; thisN < thisLayer.size(); thisN++)
		{
			Neuron& thisNeuron = thisLayer[thisN];
			thisNeuron.inputs.clear();
			for(uint leftN = 0; leftN < leftLayer.size(); leftN++)
			{
				thisNeuron.inputs.push_back(leftLayer[leftN].getOutput());
//				cout << leftLayer[leftN].getOutput() << ' ';
			}
		}
	}

	outputs.clear();
	for(uint i = 0; i < layers.back().size(); i++)
	{
		outputs.push_back(layers.back()[i].getOutput());
//		cout << layers.back()[i].getOutput();
	}
}

vector<double> Net::calculateOutputError(const vector<double> &target)
{
	assert(target.size() == layers.back().size());
	vector<double> errors;
	for(uint n = 0; n < layers.back().size(); n++)
	{
		errors.push_back(layers.back()[n] );
	}
}

void Net::train(const vector<double> inputs, const vector<double> targets)
{

}


int main()
{
	vector<uint> topology = {2, 2};
	Net net(topology);

	Net::randomizeWeights(net);

	vector<double> inputs = {1.0, 0.5};
	net.setInputs(inputs);

	vector<double> outputs;
	net.getOutputs(outputs);

	for(uint i = 0; i < outputs.size(); i++)
	{
		cout << outputs[i] << '\n';
	}
}
