/*
 * This file is part of IIC-JKU QFR library which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum/ for more information.
 */

#include "QuantumComputation.hpp"
#include "algorithms/QFT.hpp"
#include "algorithms/Grover.hpp"
#include "algorithms/GoogleRandomCircuitSampling.hpp"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {

	//读取real文件
	std::string fileName = "./circuits/test.real";
	if(argc==1)
    {
		std::cout<<"没有输入参数,使用默认电路"<<std::endl;

 	}
	else fileName = argv[1];

	qc::QuantumComputation qc(fileName);
	//qc::QFT qc(9);

	auto dd = make_unique<dd::Package>(); // create an instance of the DD package
	auto functionality = qc.buildFunctionality(dd);
	//qc.printMatrix(dd, functionality, std::cout); //输出矩阵
	dd::export2Dot(functionality, "test-real.dot"); //输出QMDD表示文件
	std::cout << dd->size(functionality) << std::endl;

	qc::permutationMap varMap = qc.initialLayout;
	
	qc.printPermutationMap(varMap);
	auto reorderdd = dd->dynamicReorder(functionality, varMap, dd::DynamicReorderingStrategy::Sifting);
	//qc.printMatrix(dd, reorderdd, std::cout); //输出矩阵
	dd::export2Dot(reorderdd, "reorderdd.dot"); //输出QMDD表示文件
	std::cout << dd->size(reorderdd) << std::endl;
	qc.printPermutationMap(varMap);

/*
	auto initial_state = dd->makeZeroState(n+1); // create initial state |0...0>
	auto state_vector = grover.simulate(initial_state, dd);
	grover.printVector(dd, state_vector, std::cout);
	dd::export2Dot(state_vector, "state_vector.dot", true);
	std::cout << std::endl << grover << std::endl;
*/
	return 0;
}
